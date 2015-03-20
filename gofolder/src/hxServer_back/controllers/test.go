package controllers

import (
	"fmt"
	"github.com/astaxie/beego"
	"hxServer/sms"
	"strconv"
)

//测试controller
type TestController struct {
	beego.Controller
}

//测试sms发送短信服务,短信接收入口
func (this *TestController) TestSMS() {
	//取得post参数,手机号码，短信内容，有效时间(秒)
	mobile := this.Input().Get("mobile")
	info := this.Input().Get("info")
	times, _ := strconv.ParseInt(this.Input().Get("times"), 10, 64)

	s := fmt.Sprintf("TestSMS:going to send:%s, %s, %d", mobile, info, times)
	fmt.Println(s)
	sms.SendSMS(mobile, info, times) //发送

	this.Ctx.WriteString("ok")
}
