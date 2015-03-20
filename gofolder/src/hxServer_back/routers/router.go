package routers

import (
	"github.com/astaxie/beego"
	"hxServer/controllers"
)

func init() {
	beego.Router("/", &controllers.HomeController{})

	// 文件服务器协议  (2015年2月5日)
	// 多文件上传/文件
	beego.Router("/upload", &controllers.FileController{}, "post:Upload")
	beego.Router("/download", &controllers.FileController{}, "get:Download")

	// HXAPI协议(2015年1月8日)
	// 注册/认证协议
	beego.Router("/v1/hxreg", &controllers.RegController{}, "post:RegAPI_V1")
	// HXAPI协议(2015年2月5日)
	// REST数据操作协议
	beego.Router("/v1/hxapi", &controllers.HxController{}, "post:HXAPI_V1")

	//beego.Router("/huangupgo", &controllers.FileController{}, "post:Huangupgo")
	//beego.Router("/ctiupload", &controllers.FileController{}, "post:CtiUpload")
	//beego.Router("/manager", &controllers.ManagerController{}, "post:Manager")
	//beego.Router("/esapi", &controllers.EShowController{}, "post:EShowAPI")
}
