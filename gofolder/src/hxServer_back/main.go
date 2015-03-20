package main

import (
	"fmt"
	"github.com/bitly/go-simplejson"
	"hxServer_back/logs"
	"hxServer_back/ossapi"
	"strings"
	//	"encoding/xml"
	//	"github.com/astaxie/beego"
	"hxServer_back/ali-oss"
	//	"hxServer_back/router"
	"io/ioutil"
	"math/rand"
	"net/http"
	"os"
	"runtime"
	"time"
)

func CheckError(err error) {
	if err == nil {
		return
	}
	logger.Error("CheckError.Exit:%s", err.Error())
	logger.GetSysLogger().Flush()
	os.Exit(1)
}

func TestPutObjectFromFile() {
	var testBucket = "ossreferer"
	var oss = ossapi.New()
	file, err := os.Open("run.sh")
	if err != nil {
		fmt.Println(err.Error())
		return
	}

	var testObject = "test/uploadtest"
	headers := make(http.Header)
	response := oss.PutObjectFromFile(testBucket, testObject, headers, file)
	if response.StatusCode != 200 {
		fmt.Println("Unable to put Object")
	}
}

func TestPutObjectFromFileWithReferer() {
	var testBucket = "ossreferer"
	var oss = ossapi.New()
	file, err := os.Open("run.sh")
	if err != nil {
		fmt.Println(err.Error())
		return
	}

	var testObject = "uploadtest"
	headers := make(http.Header)
	headers.Add("Referer", "http://www.aliyun.com")
	resp := oss.PutObjectFromFile(testBucket, testObject, headers, file)
	if resp.StatusCode != 200 {
		fmt.Println("Unable to put Object")
	}
	response, _ := ioutil.ReadAll(resp.Body)
	fmt.Println("PutReferer response:", string(response))
}

func TestGetService() {
	oss := ossapi.New()
	resp := oss.GetService()
	if resp.StatusCode != 200 {
		fmt.Print("err response")
	}

	response, _ := ioutil.ReadAll(resp.Body)
	fmt.Println("resp:", string(response))
}

func TestGetBucketAcl() {
	oss := ossapi.New()
	resp := oss.GetBucketAcl("boai")
	if resp.StatusCode != 200 {
		fmt.Print("err response")
	}

	response, _ := ioutil.ReadAll(resp.Body)
	fmt.Println("resp:", string(response))
}

func TestDelObject() {
	var testBucket = "boai"
	var oss = ossapi.New()
	var testObject = "api_handler.go"
	response := oss.DelObject(testBucket, testObject, nil)
	if response.StatusCode != 204 {

		fmt.Println(response)
	}
}
func TestPutBacket() {
	oss := ossapi.New()
	//headers := make(http.Header)
	resp := oss.PutBucket("ldjfsldf", "", nil)
	if resp.StatusCode != 200 {
		fmt.Println("Unable to create a new bucket with no acl specified")
		response, _ := ioutil.ReadAll(resp.Body)
		fmt.Println("resp:", string(response))
	}
}

func TestGetObj() {
	oss := ossapi.New()
	var testBucket = "boai"
	var testObject = "api_handler.go"
	err := oss.GetObjectToFile(testBucket, testObject, "mydownload", nil)

	if err != nil {
		fmt.Println(err)
	}

}
func TestPutBucketReferer() {
	oss := ossapi.New()
	var testBucket = "ossreferer"

	resp := oss.PutBucketReferer(testBucket, "", nil)
	response, _ := ioutil.ReadAll(resp.Body)
	fmt.Println("resp:", string(response))
}

func Testalioss() {
	var header map[string]string
	client := alioss.NewClient("DbmxIp5toP6v3kRW", "IZCnAyS09q5DnNzE0R8VBicY8el51G")
	bucket := alioss.NewBucket("ossreferer", alioss.REGION_HANGZHOU, client)
	bucket.Put("ossreferer", strings.NewReader("test"), header)
}

func TestPutReferer_() {

	var header map[string]string
	client := alioss.NewClient("DbmxIp5toP6v3kRW", "IZCnAyS09q5DnNzE0R8VBicY8el51G")
	bucket := alioss.NewBucket("ossreferer", alioss.REGION_HANGZHOU, client)
	body := "<RefererConfiguration><AllowEmptyReferer>true</AllowEmptyReferer><RefererList><Referer>http://www.aliyun.com</Referer><Referer>https://www.aliyun.com</Referer><Referer>http://www.*.com</Referer><Referer>https://www.?.aliyuncs.com</Referer></RefererList></RefererConfiguration>"
	bucket.PutReferer("ossreferer", strings.NewReader(body), header)
}

/*func TestaliossGetReferer() {

	var header map[string]string
	client := alioss.NewClient("DbmxIp5toP6v3kRW", "IZCnAyS09q5DnNzE0R8VBicY8el51G")
	bucket := alioss.NewBucket("ossreferer", alioss.REGION_HANGZHOU, client)

	bucket.GetReferer(strings.NewReader(""), header)
}
*/
func TestPUtObj() {
	var header map[string]string
	header["test"] = "test"
	client := alioss.NewClient("DbmxIp5toP6v3kRW", "IZCnAyS09q5DnNzE0R8VBicY8el51G")
	bucket := alioss.NewBucket("ossreferer", alioss.REGION_HANGZHOU, client)
	bucket.Put("ossreferer", strings.NewReader("sdfsdflsd"), header)
}

func TestJson() {
	jsr := simplejson.New()
	var jsfiles []interface{}
	jsfile := simplejson.New()
	jsfile2 := simplejson.New()

	jsfile.Set("filename", "test")
	jsfile2.Set("filename", "test2")

	jsfiles = append(jsfiles, jsfile)
	jsfiles = append(jsfiles, jsfile2)
	jsr.Set("files", jsfiles)
	ret, _ := jsr.Encode()
	fmt.Println("jsr ret:", string(ret))

	js := simplejson.New()
	var arr []interface{}
	arr, _ = jsfile.Array()
	arr = append(arr, "mytest")
	arr = append(arr, "mytest2")
	js.Set("arr", arr)
	result, _ := js.Encode()
	fmt.Println("result:", string(result))

	btnjs := simplejson.New()
	var buttons []interface{}
	buttona := simplejson.New()
	buttona.Set("name", "test1")
	buttona.Set("pic_url", "http://127.0.0.1/pic.jpg")
	buttona.Set("property", "test property")

	buttonb := simplejson.New()
	buttonb.Set("name", "test1")
	buttonb.Set("pic_url", "http://127.0.0.1/pic.jpg")
	buttonb.Set("property", "test property")

	buttons = append(buttons, buttona)
	buttons = append(buttons, buttonb)
	btnjs.Set("btn", buttons)
	btns, _ := btnjs.Encode()
	fmt.Println("button json", string(btns))

}
func main() {
	rand.Seed(time.Now().UTC().UnixNano())
	cpus := runtime.NumCPU()
	if cpus > 1 {
		runtime.GOMAXPROCS(cpus)
	}
	TestJson()
	//TestGetBucketAcl()
	//TestGetObj()
	//TestaliossGetReferer()
	//TestPutReferer_()
	//TestPutObjectFromFileWithReferer()
	//TestPUtObj()
	//TestPutBucketReferer()
}
