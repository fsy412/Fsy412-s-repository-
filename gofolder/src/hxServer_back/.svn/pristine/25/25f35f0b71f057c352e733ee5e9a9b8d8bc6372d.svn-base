package test

import (
	"fmt"
	"hxServer/ossapi"
	"os"
	//	"sync"
	"time"
)

var (
	upload_count = 10 //测试线程数
	//waitgroup sync.WaitGroup
)

//上传文件
func upload(file_name string) {
	var oss = ossapi.New()

	file, err := os.Open(file_name)
	if err != nil {
		fmt.Println(err.Error())
	}

	log.Println("start uploading file:", file_name)
	response := oss.PutObjectFromFile("boai", file_name, nil, file) //开始上传
	if response.StatusCode != 200 {
		fmt.Println("Unable to put Object")
	}
	log.Println("upload done")
	waitgroup.Done()
}

//阿里云oss上传测试
func AliossUploadTest() {
	start := time.Now()
	for i := 0; i < upload_count; i++ {
		var file_name = fmt.Sprintf("1m%d.file", i)
		waitgroup.Add(1)
		go upload(file_name)
	}
	waitgroup.Wait()

	end := time.Now()
	du := end.Sub(start)
	s := fmt.Sprintf("time used:%d  ", du.Seconds()) //输出上传所用时间
	fmt.Println(s)
}
