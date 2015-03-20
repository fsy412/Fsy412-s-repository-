package mongo

import (
	"fmt"
	"hxServer/logs"
	"sync"
	"time"
)

var defaultpools *pools // MGO默认连接池

// 初始化默认数据库
func Initialize() error {
	var err error
	defaultpools, err = NewPools("mongodb")
	if err != nil {
		return err
	}
	return nil
}

func Get() *Client {
	return defaultpools.Get()
}

func Put(db *Client) {
	defaultpools.Put(db)
}

var wg sync.WaitGroup

//---------------------------------------------------------------------------
// 测试
// 采用Go测试框架
func Test() {
	start := time.Now()
	for i := 0; i < 1; i++ {
		test(40000)
	}
	fmt.Println("mgo test end:", time.Now().Sub(start).String())

}

func test(t int) {
	wg.Add(t)
	start := time.Now()
	for i := 0; i < t; i++ {
		go task(i)
	}
	wg.Wait()
	fmt.Println("mgo test:", time.Now().Sub(start).String())
	logger.Error("error:%s", "sdfsdf")
}

func task(i int) {
	data := &struct {
		Key     string
		Counter int
	}{"1231231232424234234", i}

	//	fmt.Println("AAA")
	ss := Get()
	if ss.Err != nil {

	}
	//	fmt.Println("BBB")
	defer Put(ss)
	cc := ss.Conn.DB("boai").C("mgotest")
	err := cc.Insert(data)
	if err != nil {
		fmt.Println("mgo test :%s", err.Error())
	}
	//	ss.Fsync(true)
	wg.Done()
}
