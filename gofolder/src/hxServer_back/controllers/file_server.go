package controllers

import (
	//	"encoding/json"
	//	"fmt"
	"github.com/astaxie/beego"
	"github.com/bitly/go-simplejson"
	//	"hxServer/logs"
	"hxServer/mongo"
	//	"hxServer/session"
	//	"hxServer/ssdb"
	"hxServer/ossapi"
	"hxServer/util"
	"io"
	"net/http"
	"os"
	"time"
	"unsafe"
)

const (
	T_FileServer = "fileserver"
)

var (
	oss    = ossapi.NewOSS()
	bucket = "boai"
)

type FileController struct {
	beego.Controller
}

func (this *FileController) HxStop(status int, body string) {
	this.Ctx.Output.SetStatus(status)
	this.Ctx.Output.Body([]byte(body))
	this.StopRun()
}

//---------------------------------------------------------------------------
// 预处理
// 完成简单认证、QQ认证
// 认证失败，返回错误信息
func (this *FileController) Prepare() {
	/*
		userid := this.Ctx.Request.Header.Get("uxuid")
		token := this.Ctx.Request.Header.Get("hxtoken")
			if session.VerifyToken_DB(userid, token) == false {
				logger.Error("FileController.Prepare stop userid:%s", userid)
				// 暂时禁用
				// this.HxStop(http.StatusUnauthorized, HTTP_AUTH_FAIL)
				// return
			} else {
				logger.Error("FileController.Prepare userid:%s", userid)
			}
	*/
}

//---------------------------------------------------------------------------
// 文件上传
// 支持多文件上传
// 由客户端指定名称，不能重名
func (this *FileController) Upload() {
	// 总包大小最大为64M
	if this.Ctx.Request.ContentLength > 64*1024*1024 {
		this.HxStop(http.StatusBadRequest, HTTPE_Upload_Toobig)
		return
	}
	err := this.Ctx.Request.ParseMultipartForm(this.Ctx.Request.ContentLength)
	if err != nil {
		this.HxStop(http.StatusBadRequest, HTTPE_ParseMultipartForm)
		return
	}

	rootpath := "./static/media/" + util.GetYmdhPath()
	_, err = os.Stat(rootpath)
	if err != nil {
		err = os.MkdirAll(rootpath, os.ModePerm)
		if err != nil {
			this.HxStop(http.StatusBadRequest, HTTPE_Upload_Mkdir)
			return
		}
	}

	jsr := simplejson.New()
	var jsfiles []interface{}
	sum := 0
	//	db := ssdb.Get()
	/*	if db.Err != nil {
		this.HxStop(http.StatusBadRequest, HTTPE_Database)
		return
	}*/
	//	defer ssdb.Put(db)
	for name, body := range this.Ctx.Request.MultipartForm.File {
		jsfile := simplejson.New()
		jsfile.Set("name", name)
		for _, header := range body {
			handle, err := header.Open()
			if err != nil {
				jsfile.Set("ret", -1)
				//jsfile.Set("msg", "系统错误")
				jsfiles = append(jsfiles, jsfile)
				break
			}
			defer handle.Close()

			// 文件名必需全局唯一，并做为数据库的唯一标识
			jsfile.Set("filename", header.Filename)
			if len(header.Filename) < 16 {
				jsfile.Set("ret", -2)
				//jsfile.Set("msg", "文件名太短")
				jsfiles = append(jsfiles, jsfile)
				break
			}

			//			data, err := db.Conn.Hget(T_FileServer, header.Filename)
			//			if data != nil {
			//				jsfile.Set("Ret", -3)
			//				jsfile.Set("msg", "文件名在数据表中重复")
			//				jsfiles = append(jsfiles, jsfile)
			//				break
			//			}

			fullname := rootpath + header.Filename

			if util.IsFileExist(fullname) {
				jsfile.Set("ret", -4)
				//jsfile.Set("msg", "文件名已经存在")
				jsfiles = append(jsfiles, jsfile)
				break
			}

			filehandle, err := os.Create(fullname)
			if err != nil {
				jsfile.Set("ret", -5)
				//jsfile.Set("msg", "创建文件失败")
				jsfiles = append(jsfiles, jsfile)
				break
			}
			defer filehandle.Close()
			// TODO 以后通过返回字节限制单文件大小
			_, err = io.Copy(filehandle, handle)
			if err != nil {
				jsfile.Set("ret", -6)
				//jsfile.Set("msg", "接收数据失败")
				jsfiles = append(jsfiles, jsfile.MustMap())
				break
			}
			jsfile.Set("ret", 0)
			jsfiles = append(jsfiles, jsfile)
			sum++
			//			db.Conn.Hset(T_FileServer, header.Filename, fullname)
		}
	}

	jsr.Set("chartest", "李孟秋")
	jsr.Set("files", jsfiles)
	jsr.Set("ret", sum)

	// 写MongoDB表
	// 记录上传人员，时间
	mdb := mongo.Get()
	//	if mdb.Err == nil {
	userid := this.Ctx.Request.Header.Get("userid")
	cc := mdb.Conn.DB("boai").C(T_FileServer)
	jsr.Set("userid", userid)
	jsr.Set("createtime", time.Now().Unix())
	cc.Insert(util.JsonInterface(jsr))
	jsr.Del("userid")
	jsr.Del("createtime")
	mongo.Put(mdb)
	//	}

	this.Data["json"] = jsr
	this.ServeJson()
}

//---------------------------------------------------------------------------
// 阿里oss文件上传
// 支持多文件上传
// 由客户端指定名称，不能重名
func (this *FileController) UploadOSS() {
	// 总包大小最大为64M
	if this.Ctx.Request.ContentLength > 64*1024*1024 {
		this.HxStop(http.StatusBadRequest, HTTPE_Upload_Toobig)
		return
	}
	err := this.Ctx.Request.ParseMultipartForm(this.Ctx.Request.ContentLength)
	if err != nil {
		this.HxStop(http.StatusBadRequest, HTTPE_ParseMultipartForm)
		return
	}

	jsr := simplejson.New()
	var jsfiles []interface{}
	sum := 0
	//	db := ssdb.Get()
	/*	if db.Err != nil {
		this.HxStop(http.StatusBadRequest, HTTPE_Database)
		return
	}*/
	//	defer ssdb.Put(db)
	for name, body := range this.Ctx.Request.MultipartForm.File {
		jsfile := simplejson.New()
		jsfile.Set("name", name)
		for _, header := range body {
			handle, err := header.Open()
			if err != nil {
				jsfile.Set("ret", -1)
				//jsfile.Set("msg", "系统错误")
				jsfiles = append(jsfiles, jsfile)
				break
			}
			defer handle.Close()

			// 文件名必需全局唯一，并做为数据库的唯一标识
			jsfile.Set("filename", header.Filename)
			if len(header.Filename) < 16 {
				jsfile.Set("ret", -2)
				//jsfile.Set("msg", "文件名太短")
				jsfiles = append(jsfiles, jsfile)
				break
			}

			//			data, err := db.Conn.Hget(T_FileServer, header.Filename)
			//			if data != nil {
			//				jsfile.Set("Ret", -3)
			//				jsfile.Set("msg", "文件名在数据表中重复")
			//				jsfiles = append(jsfiles, jsfile)
			//				break
			//			}

			response := oss.PutObjectFromFile(bucket, header.Filename, nil, (*os.File)(unsafe.Pointer(&handle)))
			if response.StatusCode != 200 {
				jsfile.Set("ret", -6)
				jsfile.Set("msg", "接收数据失败")
				jsfiles = append(jsfiles, jsfile.MustMap())
				break
			}

			//			jsfile.Set("ret", 0)
			//			jsfiles = append(jsfiles, jsfile)
			//			sum++
			//			db.Conn.Hset(T_FileServer, header.Filename, fullname)
		}
	}

	jsr.Set("chartest", "fsy")
	jsr.Set("files", jsfiles)
	jsr.Set("ret", sum)

	// 写MongoDB表
	// 记录上传人员，时间
	mdb := mongo.Get()
	if mdb.Err == nil {
		userid := this.Ctx.Request.Header.Get("userid")
		cc := mdb.Conn.DB("boai").C(T_FileServer)
		jsr.Set("userid", userid)
		jsr.Set("createtime", time.Now().Unix())
		cc.Insert(util.JsonInterface(jsr))
		jsr.Del("userid")
		jsr.Del("createtime")
		mongo.Put(mdb)
	}

	this.Data["json"] = jsr
	this.ServeJson()
}

//
// 下载文件
//
func (this *FileController) Download() {
	/*
		id := this.GetString("id")
			db := ssdb.Get()
					if db.Err != nil {
						this.HxStop(http.StatusBadRequest, HTTPE_Database)
						return
					}
					 	defer ssdb.Put(db)


				fullname, _ := db.Conn.Hget(T_FileServer, id)

			if fullname == nil {
				this.HxStop(http.StatusBadRequest, HTTPE_Download_ID_Not_Exist)
				return
			}
			this.Redirect(fullname.(string), 302)
	*/
}
