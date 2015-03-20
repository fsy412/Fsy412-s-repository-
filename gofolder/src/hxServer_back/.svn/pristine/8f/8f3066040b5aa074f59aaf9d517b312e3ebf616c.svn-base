/*
短信服务
	lmq 2015.1.27 C
	短信发送
*/

package sms

import (
	//	"encoding/binary"
	"fmt"
	"github.com/astaxie/beego/config"
	"github.com/astaxie/beego/logs"
	"gopkg.in/mgo.v2/bson"
	"hxServer/logs"
	"hxServer/mongo"
	"hxServer/util"
	"io/ioutil"
	"net/http"
	"net/url"
	"strconv"
	"strings"
	"time"
)

const (
	MAX_CHAN = 100000     // 最大的管道
	T        = "smscache" // 短信缓存表
	TLog     = "smslog"   // 短信日志表
)

// 数据库机制------------------------------------------------------------------
// 数据库:mongodb
// 表结构
type DB_smsinfo struct {
	Mobile    string // 接收号码
	Info      string // 信息内容
	ValidTime int64  // 有效时间
	OnTime    int64  // 发送时间
}

var (
	sms_queue chan *DB_smsinfo // 发送消息队列
	log       *logs.BeeLogger  // 日志

	// 短信服务相关配置
	cust_code  string // 用户账号
	sp_code    string // 用户长号码
	cust_id    string // 用户标识
	cust_psswd string // 用户密码
	uri        string // 服务地址

//	running    bool   // 运行标识
//	seq_number uint32 // 序列号
)

// --------------------------------------------------------------------------
// 发送短信
// 服务商:未来无线 协议版本:HTTP
// 返回值:
// 		-1 	网络错误
//		0	发送成功
//		13	目标号码在黑名单中
//		15	通道不支持该目标号码
//		8	流量超过限制
func send(mobile, info string) int {
	msg := fmt.Sprintf("cust_code=%s&sp_code=%s&content=%s&destMobiles=%s&sign=%s",
		cust_code,
		sp_code,
		info,
		mobile,
		util.GetMD5([]byte(url.QueryEscape(info)+cust_psswd)))

	resp, err := http.Post(uri,
		"application/x-www-form-urlencoded",
		strings.NewReader(msg))

	if err != nil {
		return -1
	}

	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return -1
	}

	ret := string(body)
	if ret == "" {
		return -1
	}

	// 解析发送结果
	var vals []string = strings.Split(ret, ":")
	status_code, err := strconv.Atoi(strings.TrimSpace(vals[len(vals)-1]))
	if err != nil {
		log.Error("strconv.Atoi err:%s", err.Error())
		return -1
	}

	return status_code
}

// Boai发送短信接口
// 1.0版本：
// 验证类短信，不保证送达
// times:有效时间(秒)，当为0的时候，表示没有时间限制(实际:7天有效)
func SendSMS(mobile string, info string, times int64) bool {
	//	if running == false {
	//		return false
	//	}
	if times == 0 {
		times = 3600 * 24 * 7 // 7天
	}
	msg := &DB_smsinfo{
		Mobile:    mobile,
		Info:      info,
		ValidTime: time.Now().Unix() + times,
		OnTime:    0,
	}
	put_queue(msg)
	return true
}

// --------------------------------------------------------------------------
// 短信网关服务器
func SmsServer() {
	ini, err := config.NewConfig("ini", "./conf/sms.conf")
	if err != nil {
		logger.Error("SmsServer NewConfig fail.msg:%s", err.Error())
		fmt.Println("return  NewConfig")
		return
	}
	if ini.DefaultBool("active", false) == false {
		fmt.Println("return  DefaultBool")
		return
	}

	//读取短信服务相关配置
	cust_code = ini.String("cust_code")
	sp_code = ini.String("sp_code")
	cust_id = ini.String("cust_id")
	cust_psswd = ini.String("cust_psswd")
	uri = ini.String("uri")

	log = logger.NewLogger("./log/sms.log", ini.DefaultBool("console", false))
	log.Info("SmsServer start...")
	log.SetLevel(ini.DefaultInt("loglevel", 7))
	sms_queue = make(chan *DB_smsinfo, MAX_CHAN) // 默认开启消息队列长度

	fmt.Println("uri:", uri)

	go sender()

}

func sender() {
	ticker := time.NewTicker(60 * time.Second) // 60秒查询一次余额信息
	timer := time.NewTimer(3 * time.Second)    // 轮空3秒后执行
	for {
		for {
			select {
			case msg := <-sms_queue:
				if time.Now().Unix() > msg.ValidTime {
					log.Warn("SMS Send time over! mobile:%s info:%s", msg.Mobile, msg.Info)
					break
				}

				ret := send(msg.Mobile, msg.Info)
				switch ret {
				case 0:
					writeLogTable(msg)
				case -1:
					// 网络错误，重新发送
					log.Warn("SMS Send Fail network! mobile:%s info:%s", msg.Mobile, msg.Info)
					sms_queue <- msg // // 重新写入队列
				case 13, 15:
					// 发送失败，只记录日志,不重发
					log.Warn("SMS Send Fail code:%d! mobile:%s info:%s", ret, msg.Mobile, msg.Info)
				case 8:
					// 流量告警
					// 系统告警 TODO :结合余额告警，将数据反馈到监控系统
					log.Warn("SMS Send Fail 流量告警! mobile:%s info:%s", msg.Mobile, msg.Info)
					sms_queue <- msg // // 重新写入队列
				}
			case <-timer.C:
				readCache()
			case <-ticker.C:
				// 查询余额
			}
		}
	}
}

// 将发送内容写入队列
func put_queue(msg *DB_smsinfo) {
	select {
	case sms_queue <- msg:
	default:
		//writeCache(msg)
	}
}

// 读取发送数据
func readCache() {
	ss := mongo.Get()
	if ss.Err != nil {
		log.Warn("SMS ReadSendTable Fail! msg:%s", ss.Err.Error())
		return
	}
	defer mongo.Put(ss)
	cc := ss.Conn.DB("boai").C(T)

	data := &struct {
		Id   bson.ObjectId `bson:"_id"`
		Info *DB_smsinfo
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
func writeCache(msg *DB_smsinfo) {
	ss := mongo.Get()
	if ss.Err != nil {
		log.Warn("SMS ReadSendTable Fail! msg:%s", ss.Err.Error())
		return
	}
	defer mongo.Put(ss)
	cc := ss.Conn.DB("boai").C(T)
	ss.Err = cc.Insert(msg)
}

// 写日志数据库
func writeLogTable(msg *DB_smsinfo) {
	ss := mongo.Get()
	if ss.Err != nil {
		log.Warn("SMS ReadSendTable Fail! msg:%s", ss.Err.Error())
		return
	}
	defer mongo.Put(ss)
	cc := ss.Conn.DB("boai").C(TLog)
	ss.Err = cc.Insert(msg)
}

//查询余额
//返回值 cust_code, status ,sms_balance
//status表示：
//  0 账号已停用
//  1 账号在用
//  2 账号处在测试状态
// -1 网络错误
func Query() (int, int, int) {
	client := &http.Client{}
	url := fmt.Sprintf(uri+"?action=GetToken&cust_code=%s", cust_code)
	//获取余额查询请求token
	req, err := http.NewRequest("GET", url, nil)

	// NOTE this !!
	req.Close = true

	resp, err := client.Do(req)
	if err != nil {
		return -1, -1, -1
	}
	defer resp.Body.Close()

	response, err_ := ioutil.ReadAll(resp.Body)
	if err_ != nil {
		return -1, -1, -1
	}

	token := strings.Split(string(response), ":")[1]
	//发送查询请求，md5计算签名
	url = fmt.Sprintf(uri+"?action=QueryAccount&cust_code=%s&sign=%s", cust_code, util.GetMD5([]byte(token+cust_psswd)))

	req, err = http.NewRequest("GET", url, nil)
	if err != nil {
		log.Error("Query:new request err:%s", err.Error())
		return -1, -1, -1
	}
	resp, err = client.Do(req)
	if err != nil {
		log.Error("Query:client.Do err:%s", err.Error())
		return -1, -1, -1
	}
	response, err_ = ioutil.ReadAll(resp.Body)
	if err != nil {
		log.Error("Query:ioutil.ReadAll err:%s", err.Error())
		return -1, -1, -1
	}
	//解析服务器返回结果
	ret := []string(strings.Split(string(response), ":"))
	if ret[0] == "ERROR" {
		log.Error("Query:ERROR")
		return -1, -1, -1
	}

	vals := []string(strings.Split(string(response), ","))
	code, _ := strconv.Atoi(strings.Split(vals[0], ":")[1])
	status, _ := strconv.Atoi(strings.Split(vals[1], ":")[1])
	sms_balance, _ := strconv.Atoi(strings.Split(vals[2], ":")[1])

	return code, status, sms_balance
}
