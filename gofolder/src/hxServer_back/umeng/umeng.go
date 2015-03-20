package umeng

import (
	"fmt"
	"github.com/bitly/go-simplejson"
	"gopkg.in/mgo.v2/bson"
	"hxServer_back/mongo"
	"hxServer_back/util"
	"net/http"
	//	"net/url"
	"github.com/astaxie/beego/config"
	"github.com/astaxie/beego/logs"
	"hxServer_back/logs"
	"io/ioutil"
	"strings"
	"time"
)

const (
	postPath        = "/api/send"
	host            = "http://msg.umeng.com"
	appkey          = "550253d7fd98c55fdd000bab"
	appMasterSecret = "exv7cjcxby5h7katpkjmgdtkzxbkkob6"

	android_appkey             = "550a99a2fd98c558bc000a90"
	android_appMasterSecret    = "tyn73lxbvh1zd1z69etknha1zgztiwh5"
	android_umengMessageSecret = "cb84f1e80def89fc113b8e24bf1f5c26"

	MAX_CHAN = 100000     // 最大的管道
	T        = "smscache" // 短信缓存表
	TLog     = "smslog"   // 短信日志表

)

// 数据库机制------------------------------------------------------------------
// 数据库:mongodb
// 表结构
type DB_pushinfo struct {
	Token       string // 手机设备号
	Info        string // 信息内容
	Device_Type string // 设备类型
	ValidTime   int64  // 有效时间
	OnTime      int64  // 发送时间
}

var (
	production_mode bool                                                  //生产环境
	push_queue      chan *DB_pushinfo = make(chan *DB_pushinfo, MAX_CHAN) //需要发送的消息队列
	log             *logs.BeeLogger
)

//初始化读配置
func Initialize() {
	ini, err := config.NewConfig("ini", "./conf/umeng.conf")
	if err != nil {
		logger.Error("umeng NewConfig fail.msg:%s", err.Error())
		return
	}

	if ini.DefaultBool("active", false) == false {
		fmt.Println("return  DefaultBool")
		return
	}

	if ini.DefaultBool("production_mode", false) == false {
		fmt.Println("return  DefaultBool")
		return
	}

	log = logger.NewLogger("./log/push.log", ini.DefaultBool("console", false))
	log.Info("Umeng server start...")
	log.SetLevel(ini.DefaultInt("loglevel", 7))

	go push()

}

//计算签名
func getSign(body string, secret string) string {
	url := host + postPath
	return util.GetMD5([]byte("POST" + url + body + secret))
}

//获得post消息体
//参数：设备号， 推送内容, 消息发送类型, 设备类型
//返回：json 消息体
func getBody(device_token, content, cast_type, device string) string {
	body := simplejson.New()
	body.Set("appkey", appkey)
	body.Set("timestamp", time.Now().Unix())
	body.Set("type", cast_type)
	body.Set("device_tokens", device_token)
	body.Set("production_mode", production_mode)
	body.Set("description", content)

	playlaod := simplejson.New()
	if device == "android" {
		fmt.Println("getBody:device android")
		body.Set("appkey", android_appkey)
		playlaod.Set("display_type", "notification")

		embody := simplejson.New()
		embody.Set("ticker", "一条推送")
		embody.Set("title", "test")
		embody.Set("text", "来自友盟推送")

		playlaod.Set("body", embody)
		body.Set("appkey", android_appkey)

	} else {
		aps := simplejson.New()
		aps.Set("alert", content)
		playlaod.Set("aps", aps)
	}
	body.Set("payload", playlaod)
	out, _ := body.Encode()

	return string(out)
}

//发送单播推送消息
//参数：设备号，推送内容, 设备类型(ios,android)
//返回：1 success, 0 fail
func SendUnicast(device_token, content, device string) int {
	body := getBody(device_token, content, "unicast", device)
	var secret string
	if device == "ios" {
		secret = appMasterSecret
	} else {
		secret = android_appMasterSecret
	}

	url := host + postPath + "?sign=" + getSign(body, secret)
	resp, err := http.Post(url,
		"application/x-www-form-urlencoded",
		strings.NewReader(body))
	if err != nil {
		logger.Error("SendUnicast:%s", err.Error())
		return 0
	}
	defer resp.Body.Close()
	response, err := ioutil.ReadAll(resp.Body)

	if err != nil {
		logger.Error("SendUnicast:%s", err.Error())
		return 0
	}

	jsret, err := simplejson.NewJson(response)
	if err != nil {
		logger.Error("SendUnicast:%s", err.Error())
	}

	if jsret.Get("ret").MustString("") == "SUCCESS" {
		return 1
	} else {
		logger.Error("SendUnicast:%s", string(response))
		return 0
	}
}

func PushMsg(token, info, device_type string, times int64) bool {
	//	if running == false {
	//		return false
	//	}
	if times == 0 {
		times = 3600 * 24 * 7 // 7天
	}
	msg := &DB_pushinfo{
		Token:       token,
		Info:        info,
		Device_Type: device_type,
		ValidTime:   time.Now().Unix() + times,
		OnTime:      0,
	}
	put_queue(msg)
	return true
}

func push() {
	//	ticker := time.NewTicker(60 * time.Second) // 60秒查询一次余额信息
	timer := time.NewTimer(3 * time.Second) // 轮空3秒后执行
	for {
		for {
			select {
			case msg := <-push_queue:
				if time.Now().Unix() > msg.ValidTime {
					log.Warn("Umeng Send time over! token:%s info:%s", msg.Token, msg.Info)
					break
				}

				ret := SendUnicast(msg.Token, msg.Info, msg.Device_Type)
				switch ret {
				case 1:
					writeLogTable(msg)
				case 0:
					// 网络错误，重新发送
					log.Warn("Umeng Send Fail network! token:%s info:%s", msg.Token, msg.Info)
					push_queue <- msg // // 重新写入队列
				}
			case <-timer.C:
				readCache()

			}
		}
	}
}

// 将发送内容写入队列
func put_queue(msg *DB_pushinfo) {
	select {
	case push_queue <- msg:
	default:
		//writeCache(msg)
	}
}

// 读取发送数据
func readCache() {
	ss := mongo.Get()
	if ss.Err != nil {
		log.Warn("Umeng ReadSendTable Fail! msg:%s", ss.Err.Error())
		return
	}
	defer mongo.Put(ss)
	cc := ss.Conn.DB("boai").C(T)

	data := &struct {
		Id   bson.ObjectId `bson:"_id"`
		Info *DB_pushinfo
	}{}

	lmt := int(MAX_CHAN / 3)
	if lmt > 1000 {
		lmt = 1000
	}
	iter := cc.Find(bson.M{}).Limit(lmt).Iter()
	for iter.Next(data) {
		put_queue(data.Info)
		ss.Err = cc.RemoveId(data.Id)
		if ss.Err != nil {
			return
		}
	}

}

// 写缓存
func writeCache(msg *DB_pushinfo) {
	ss := mongo.Get()
	if ss.Err != nil {
		log.Warn("Umeng ReadSendTable Fail! msg:%s", ss.Err.Error())
		return
	}
	defer mongo.Put(ss)
	cc := ss.Conn.DB("boai").C(T)
	ss.Err = cc.Insert(msg)
}

// 写日志数据库
func writeLogTable(msg *DB_pushinfo) {
	ss := mongo.Get()
	if ss.Err != nil {
		log.Warn("Umeng ReadSendTable Fail! msg:%s", ss.Err.Error())
		return
	}
	defer mongo.Put(ss)
	cc := ss.Conn.DB("boai").C(TLog)
	ss.Err = cc.Insert(msg)
}

func Test() {
	//getBody("test", "test", "unicast", "ios")
	fmt.Println(SendUnicast("20ea1c31d6234f9fd6412d1c4afecb5a7306e22c8ac5a0c646782ed3897d3f12", "test", "ios"))
	//AlSLQ8ab9TQeHJao2hF_RPIytS-oElKXJMmszfECptlo

	//fmt.Println(SendUnicast("AlSLQ8ab9TQeHJao2hF_RPIytS-oElKXJMmszfECptlo", "test", "android"))

}
