package controllers

import (
	//"fmt"
	"github.com/astaxie/beego"
	"github.com/bitly/go-simplejson"
	//	"hxServer/logs"
	//	"hxServer/session"
	//"hxServer/util"
	"net/http"
)

type HxController struct {
	beego.Controller
}

func (this *HxController) HxStop(status int, body string) {
	this.Ctx.Output.SetStatus(status)
	this.Ctx.Output.Body([]byte(body))
	this.StopRun()
}

//---------------------------------------------------------------------------
// 预处理
// 完成简单认证、QQ认证
// 认证失败，返回错误信息
func (this *HxController) Prepare() {
	/*
		logger.Error("HxController.Prepare")
		userid := this.Ctx.Request.Header.Get("hxuid")
		hxtoken := this.Ctx.Request.Header.Get("hxtoken")
			if session.VerifyToken_DB(userid, hxtoken) == false {
				this.HxStop(http.StatusUnauthorized, HTTP_AUTH_FAIL)
			}
	*/
}

// HXAPI_V1协议入口
// 版本：1.0.0
// 作者：danny
// 时间：2015-1-8
// 确保验证通过后再进行JS解析及命令处理
func (this *HxController) HXAPI_V1() {
	newtoken := this.Ctx.Request.Header.Get("hxtoken")
	if len(newtoken) > 0 {
		//登录成功，返回认证码
	}

	body := this.GetString("body")
	if body == "" {
		return
	}

	bodyjs, err := simplejson.NewJson([]byte(body))
	if err != nil {
		this.HxStop(http.StatusBadRequest, HTTP_BODY_INVALID)
		return
	}
	// 立即进行处理[本函数支持协程]
	// ....
	if bodyjs.Get("").MustBool(false) {
		return
	}

	jsr := simplejson.New()

	// 返回JS
	// 将返回的数据发送到客户端
	this.Data["json"] = jsr
	this.ServeJson()
}
