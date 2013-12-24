/*
 * Copyright(C) 2013,Company ghca. All rights reserved.
 * 
 * @file    common.hpp
 * @brief   常用API的跨平台封装
 * @version 1.0
 * @author  zhangyawen
 * @data    2013-3-8
 */

#ifndef __COMMON_HPP
#define __COMMON_HPP

#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef WIN32
#	include <windows.h>
#	include <process.h>
#else
#	include <unistd.h>
#	include <sys/time.h>
#	include <unistd.h>
#	include <pthread.h>
#	include <semaphore.h>
#endif // WIN32

namespace module{

	void Sleep(unsigned long long usec);

	unsigned long long GetTickTime();

	struct tm* GetLocalTime();

	//char* strupr(char *pIn);

	int stricmp(char const *pIn1,char const *pIn2);

	int strnicmp(char const *pIn1,char const *pIn2,unsigned int cnt);

}

#endif	//__COMMON_HPP
