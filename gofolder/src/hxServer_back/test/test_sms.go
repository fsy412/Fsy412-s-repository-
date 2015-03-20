package test

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"strings"
	"sync"
	"time"
)

var (
	nbr_thread = 10 //模拟线程数
	waitgroup  sync.WaitGroup
)

//post模拟短信发送请求
func Send() {
	//随机产生号码
	var moblie string
	var info string
	var times int64

	post_str := fmt.Sprintf("mobile=%s&info=%s&times=%d", moblie, info, times)
	fmt.Println("post_str:", post_str)
	resp, err := http.Post("http://127.0.0.1:8080/test",
		"application/x-www-form-urlencoded",
		strings.NewReader(post_str))

	if err != nil {
		fmt.Println(err)
		return
	}

	defer resp.Body.Close()

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Println(err)
		return
	}

	fmt.Println(string(body))
	waitgroup.Done()
}

func Test() {
	start := time.Now()
	for i := 0; i < nbr_thread; i++ {
		waitgroup.Add(1)
		go Send()
	}
	waitgroup.Wait()

	end := time.Now()
	du := end.Sub(start)
	du.Seconds()

	s := fmt.Sprintf("time used:%d  ", du.Seconds())
	fmt.Println(s)
}
