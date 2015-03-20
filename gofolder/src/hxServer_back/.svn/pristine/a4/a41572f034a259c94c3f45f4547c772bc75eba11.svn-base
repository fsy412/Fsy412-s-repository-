package referer

import (
	"fmt"
	"log"
	"strconv"
	"time"
)

type Ref struct {
	val string
	//	active bool
}

var (
	refs    []Ref = make([]Ref, 3)
	MAX_LEN       = 3
	exit          = make(chan int)
)

func Start() {
	fmt.Print("referer start")

	refs[0].val = "test0"
	refs[1].val = "test1"
	refs[2].val = strconv.FormatInt(time.Now().Unix(), 10)
	log.Println(refs[0].val, refs[1].val, refs[2].val)
	//	fmt.Println(fmt.Printf("ref0:%s, ref1:%s, ref2:%s", refs[0].val, refs[1].val, refs[2].val))
	go run()
	<-exit
}

func gen() *Ref {
	return &Ref{val: "test"}
}

func getReferer() {

}

func updateReferer() {

	refs[0].val = refs[1].val
	refs[1].val = refs[2].val
	refs[2].val = strconv.FormatInt(time.Now().Unix(), 10)
}

func run() {
	fmt.Print("start run")
	ticker := time.NewTicker(5 * time.Second) // 60秒查询一次余额信息
	timer := time.NewTimer(3 * time.Second)   // 轮空3秒后执行
	for {
		for {
			select {
			case <-timer.C:
				fmt.Println("timer")
			case <-ticker.C:
				fmt.Println("ticker")
				//fmt.Println(fmt.Printf("ref0:%s, ref1:%s, ref2:%s", refs[0].val, refs[1].val, refs[2].val))
				log.Println(refs[0].val, refs[1].val, refs[2].val)
				updateReferer()
			}
		}
	}
}
