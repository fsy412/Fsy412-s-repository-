/*
* Copyright(C) 2013,Company 保留所有权利。( All rights reserved. )
* 
* 文件名称：log.hpp
* 摘    要：简单的日志输出类，读写同步，效率可以
* 当前版本：1.0
* 作    者：zhangyawen
* 创建日期：2013年2月22日
*/
#ifndef __LOG_HPP
#define __LOG_HPP

#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <queue>
#include <string>
#include <iostream>
#include <fstream>

#include <module/thread/thread.hpp>
#include <module/thread/mutex.hpp>
#include <module/common/common.hpp>

enum Level
{
	ERROR = 0,
	WARN  = 1,
	INFO,
	DEBUG,
	enumLOG_4,
	enumLOG_5
};

namespace module{

	class CLog : public CThread
	{
#define DEFAULT_TIME 500
#define LOG_NAME	 ".log"

		enum TimeType
		{
			enumDATE = 1,
			enumTIME,
			enumALL
		};

	public:

		/*
		* 函数名称：CLog
		* 参    数：
		：- const char* logpath			日志路径
		：- const char* name = LOG_NAME 标示名字
		：- int nlv = enumLOG_1			默认打印级别
		：- int nscan = DEFAULT_TIME	日志扫描打印时间
		* 函数作用：类构造函数
		* 修改日期：2013年2月22日  By zhangyawen
		*/
		CLog(const char* logpath,const char* name = LOG_NAME,int nlv = DEBUG,int nscan = DEFAULT_TIME);

		CLog();

		~CLog();

	private:

		std::string m_strpath;

		std::string m_strfilename;

		volatile int m_nprlve;

		volatile int m_nscan;

	private:

		volatile bool m_bquit;

	private:

		std::queue<std::string> m_quelog1;

		std::queue<std::string> m_quelog2;

		std::queue<std::string>* m_pwrite;

		std::queue<std::string>* m_pread;

	private:

		CMutex m_mutex;

	private:

		int DoRunning();

	public:

		std::string GetTimeString(int type = enumALL);

		std::string GetDate();

		std::string GetTime();

	public:

		/*
		* 函数名称：Init
		* 参    数：
		：- const char* logpath			日志路径
		：- const char* name = LOG_NAME 标示名字
		：- int nlv = enumLOG_1			默认打印级别
		：- int nscan = DEFAULT_TIME	日志扫描打印时间
		* 返    回：int
		* 函数作用：若没在函数构造是申明参数，可以调用此函数进行初始化
		* 修改日期：2013年2月22日  By zhangyawen
		*/
		int Init(const char* logpath,const char* filename = LOG_NAME,int nlv = DEBUG,int nscan = DEFAULT_TIME);

		/*
		* 函数名称：PushLog
		* 参    数：
		：- int nlv
		：- const char* fmt
		：- ...
		* 返    回：int
		* 函数作用：写入日志
		* 修改日期：2013年2月22日  By zhangyawen
		*/
		int PushLog(int nlv,const char* fmt,...);
	};

}

#endif	//__LOG_HPP
