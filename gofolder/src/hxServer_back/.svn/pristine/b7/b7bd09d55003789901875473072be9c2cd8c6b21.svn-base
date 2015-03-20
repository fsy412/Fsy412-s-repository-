package mongo

import (
	"fmt"
	"github.com/astaxie/beego/config"
	"gopkg.in/mgo.v2"
	"hxServer/logs"
	"strings"
	"sync"
	"sync/atomic"
	"time"
)

type Client struct {
	Conn *mgo.Session // 数据连接对象
	Err  error        // 错误信息
	on   time.Time    // 出入队列时间
}

// 数据库连接池
type pools struct {
	pconn   *mgo.Session // 主连接连接对象
	ch      chan *Client // 连接池
	id      string       // 连接标识
	servers []string     // 服务器
	user    string       // 用户名
	pwd     string       // 密码
	maxlink int32        // 最大并发连接数
	active  int32        // 活动连接
	sync.Mutex
}

func (l *pools) gen() *Client {
	l.Lock()
	defer l.Unlock()
	// 限制连接数量
	act := atomic.LoadInt32(&l.active)
	if act >= l.maxlink {
		return nil
	}
	cc := &Client{}

	// MGO自带连接池
	if l.pconn == nil {
		di := &mgo.DialInfo{
			Addrs:     l.servers,
			Username:  l.user,
			Password:  l.pwd,
			PoolLimit: int(l.maxlink)}
		l.pconn, cc.Err = mgo.DialWithInfo(di)
		if cc.Err != nil {
			logger.Error("Mongodb[%s] DialWithInfo failure.", l.id)
			return cc
		}
	}
	cc.Conn = l.pconn.Clone()

	// 独立拨号连接池(每个连接占用两个Socket)
	//	cc.Conn, cc.Err = mgo.DialWithInfo(di)
	//	if cc.Err != nil {
	//		logger.Gerr("Mongodb[%s] DialWithInfo failure.", l.id)
	//		fmt.Printf("mgo link fail. %s \n", cc.Err.Error())
	//		l.mu.Unlock()
	//		return cc
	//	}

	cc.on = time.Now()
	cc.Conn.SetMode(mgo.Monotonic, true)
	atomic.AddInt32(&l.active, 1) // 创建成功后，活动计数器+1
	return cc
}

func (l *pools) Get() *Client {
	var cc *Client
	select {
	case cc = <-l.ch:
	default:
	}
	if cc == nil {
		cc = l.gen()
		if cc == nil {
			cc = <-l.ch
		}
	}
	return cc
}

func (l *pools) Put(cc *Client) {
	if cc.Err != nil {
		cc.Conn.Close()
		atomic.AddInt32(&l.active, -1)
		logger.Error("Mongodb[%s] put -%s", l.id, cc.Err.Error())
		return
	}
	cc.on = time.Now()
	l.ch <- cc
}

//// 根据配置标识获取配置后做连接池的初始化
func NewPools(id string) (*pools, error) {
	pool := &pools{}
	pool.id = id
	fn := fmt.Sprintf("./conf/%s.conf", id)
	fmt.Println(fn)
	ini, err := config.NewConfig("ini", fn)
	if err != nil {
		logger.Error("Mongodb[%s] NewConfig failure.", id)
		return nil, fmt.Errorf("mgo(%s): NewPools Fail.msg:%s", id, err.Error())
	}

	server := ini.DefaultString("server", "localhost")
	pool.servers = strings.Split(strings.TrimSpace(server), ";")
	pool.maxlink = int32(ini.DefaultInt("pools", 50))
	pool.user = ini.DefaultString("user", "")
	pool.pwd = ini.DefaultString("pwd", "")

	pool.ch = make(chan *Client, pool.maxlink+1)
	pool.active = 0

	return pool, nil
}
