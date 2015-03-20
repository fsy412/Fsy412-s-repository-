/*
系统日志包
	李孟秋 2014-3-25
	重新封装Beego的BeeLogger包
*/
package logger

import (
	"github.com/astaxie/beego/logs"
)

// 系统全局日志
var sys_logger *logs.BeeLogger

// 错误全局日志
var err_logger *logs.BeeLogger

//初始化全局日志对象
func init() {
	sys_logger = NewLogger("./log/hxServer.txt", true)
	//err_logger = NewLogger("./log/systemerr.txt", true)
	Info("System Start!")
}

// 返回日志对象,以访问所有的方法
func GetSysLogger() *logs.BeeLogger {
	return sys_logger
}

//初始化操作对象，并设置控制台显示以及文件记录
func NewLogger(filename string, console bool) *logs.BeeLogger {
	log := logs.NewLogger(10240) // 缓存大小：10240
	if console {
		log.SetLogger("console", "")
	}
	json := `{"filename":"` + filename + `"}`
	log.SetLogger("file", json)
	return log
}

//---------------------------------------------------------------------------
// 系统全局错误日志输出
// ****系统重大错误信息输出****
//func Gerr(format string, v ...interface{}) {
//	err_logger.Error(format, v...)
//}

//// 系统全局错误操作
//// ***出现错误需要退出系统时调用***
//func GerrFlush() {
//	err_logger.Flush()
//}

//---------------------------------------------------------------------------
// 全局普通日志便捷访问函数
// 输出跟踪信息
func Trace(format string, v ...interface{}) {
	sys_logger.Trace(format, v...)
}

// 输出调试信息
func Debug(format string, v ...interface{}) {
	sys_logger.Debug(format, v...)
}

// 输出运行信息
func Info(format string, v ...interface{}) {
	sys_logger.Info(format, v...)
}

// 输出错误消息
func Warn(format string, v ...interface{}) {
	sys_logger.Warn(format, v...)
}

// 输出错误消息
func Error(format string, v ...interface{}) {
	sys_logger.Error(format, v...)
}

// 输出危险消息
func Critical(format string, v ...interface{}) {
	sys_logger.Critical(format, v...)
}
