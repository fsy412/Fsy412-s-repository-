package controllers

import (
	"fmt"
	"github.com/astaxie/beego"
	//	"github.com/bitly/go-simplejson"
	//	"hxServer/auth"
	"hxServer/logs"
	//	"hxServer/session"
	"hxServer/util"
	//	"io/ioutil"
	"net/http"
	"strconv"
	"time"
)

type RegController struct {
	beego.Controller
}

func (this *RegController) HxStop(status int, body string) {
	this.Ctx.Output.SetStatus(status)
	this.Ctx.Output.Body([]byte(body))
	this.StopRun()
}

//---------------------------------------------------------------------------
// 预处理
// 完成简单认证、QQ认证
// 认证失败，返回错误信息
func (this *RegController) Prepare() {
	// TODO 测试代码
	test := fmt.Sprintf("%d", time.Now().Unix())
	test = util.GetMD5([]byte(test))
	logger.Debug("RegController Prepare. DebugInfo:hxts:%d hxkey:%s", time.Now().Unix(), test)

	ts := this.Ctx.Request.Header.Get("hxts")
	key := this.Ctx.Request.Header.Get("hxkey")

	if len(key) > 0 {
		tsi, err := strconv.ParseUint(ts, 10, 64)
		if err == nil {
			tson := time.Unix(int64(tsi), 0)
			hrange := tson.Sub(time.Now()).Hours()
			// 允许时间误差1小时
			if hrange < 1.0 && hrange > -1.0 {
				key2 := util.GetMD5([]byte(ts))
				if key == key2 {
					//this.Ctx.Request.Header.Set("myname", "limengqiu")  // test
					return
				}
			}
		}
	}
	logger.Warn("RegController.Prepare fail!")
	this.HxStop(http.StatusUnauthorized, HTTP_AUTH_FAIL)
}

// RegAPI_V1协议入口
// 版本：1.0.0
// 作者：danny
// 时间：2015-1-27
// 功能表：
// 		1、获取验证码
// 		2、验证码校验
//  	3、修改密码
//  	4、用户注册
func (this *RegController) RegAPI_V1() {
	logger.Debug("RegAPI_V1")
	// 登录处理,直接根据Header里的数据进行处理(不解析JSON)
	// 简易密码验证
	/*	ai := this.Ctx.Request.Header.Get("Authorization")
		if len(ai) > 0 {
			jsr := simplejson.New()
			jsr.Set("cmd", "reg_auth")
			jsr.Set("ret", -1)
			// 基本验证
			user, pwd, err := auth.DecodeBasicAuth(this.Ctx.Request.Header.Get("Authorization"))
			if err == nil {
				userid, err := auth.SimpleLogin(user, pwd)
				if err == nil {
					token := util.GenToken()
					session.Create(token, userid)
					jsr.Set("hxtoken", token)
					jsr.Set("ret", 0)
				}
			}
			this.Data["json"] = jsr
			this.ServeJson()
			return
		}

		// QQ验证
		openid := this.Ctx.Request.Header.Get("OpenID")
		if len(openid) > 0 {
			jsr := simplejson.New()
			jsr.Set("cmd", "reg_auth")
			jsr.Set("ret", -1)
			accessid := this.Ctx.Request.Header.Get("AccessID")
			ret := auth.QQValid(openid, accessid)
			if ret {
				jsr.Set("ret", 1)
				mobile := this.Ctx.Request.Header.Get("mobile")
				if len(mobile) == 0 {
					// 仅验证或登录
					userid := auth.QQGetUserID(openid)
					if len(userid) > 0 {
						token := util.GenToken()
						session.Create(token, userid)
						jsr.Set("hxtoken", token)
						jsr.Set("userid", userid)
						jsr.Set("ret", 0)
					}
				} else {
					// 注册
					code := this.Ctx.Request.Header.Get("code")
					pwd := this.Ctx.Request.Header.Get("pwd")
					petname := this.Ctx.Request.Header.Get("petname")
					if len(code) > 0 && len(pwd) > 0 {
						userid := auth.Reg_Register(mobile, code, petname, pwd)
						if len(userid) > 0 {
							// 新用户注册，执行绑定
							// auth.Reg_BindQQ(mobile, openid)
							jsr.Set("newuser", true)
							token := util.GenToken()
							session.Create(token, userid)
							jsr.Set("hxtoken", token)
							jsr.Set("userid", userid)
							jsr.Set("ret", 0)
						}
					}
				}
			}
			this.Data["json"] = jsr
			this.ServeJson()
			return
		}

		// 其他命令处理
		body, err := ioutil.ReadAll(this.Ctx.Request.Body)
		if err != nil || len(body) == 0 {
			logger.Error("Test 002_1:%s", string(body))
			this.HxStop(http.StatusBadRequest, HTTP_BODY_INVALID)
			return
		}
		js, err := simplejson.NewJson(body)
		if err != nil {
			logger.Error("Test 002_2:%s", string(body))
			this.HxStop(http.StatusBadRequest, HTTP_BODY_INVALID)
			return
		}

		cmd := js.Get("cmd").MustString("")
		if len(cmd) == 0 {
			this.HxStop(http.StatusBadRequest, HTTP_CMD_INVALID)
			return
		}

		mobile := js.Get("mobile").MustString("")

		jsr := simplejson.New()
		jsr.Set("cmd", js.Get("cmd"))

		switch cmd {
		case "hxr_req_code": // 发送验证码
			code := auth.Reg_GenCode(mobile)
			jsr.Set("code", code)
			logger.Debug("RegController.RegAPI_V1 DebugInfo:code:%s", code)
		case "hxr_verify_code": // 校验验证码
			code := js.Get("code").MustString("")
			if len(code) != 4 {
				jsr.Set("ret", -1)
			} else {
				ret := auth.Reg_CheckCode(mobile, code)
				jsr.Set("ret", ret)
			}
		case "hxr_register": // 注册
			code := js.Get("code").MustString("")
			petname := jsr.Get("petname").MustString("")
			pwd := jsr.Get("pwd").MustString("")
			if len(code) != 4 || len(pwd) != 32 {
				jsr.Set("ret", -1)
			} else {
				userid := auth.Reg_Register(mobile, code, petname, pwd)
				if len(userid) == 0 {
					jsr.Set("result", -1)
				} else {
					jsr.Set("uid", userid)
					jsr.Set("ret", 0)
				}
			}
		case "hxr_change_pwd": // 修改密码
			code := js.Get("code").MustString("")
			newpwd := jsr.Get("newpwd").MustString("")
			if len(code) != 4 || len(newpwd) != 32 {
				jsr.Set("result", -1)
			} else {
				ret := auth.Reg_ChangePwd(mobile, code, newpwd)
				jsr.Set("result", ret)
			}
		default:
			this.HxStop(http.StatusBadRequest, HTTP_CMD_INVALID)
			return
		}

		this.Data["json"] = jsr
		this.ServeJson()
	*/
}
