/*
* Copyright(C) 2013,Company ghca. All rights reserved.
* 
* @file    socket_os.hpp
* @brief   多个平台的宏定义，暂时遇到的不多，Linux Windows Solaris支持无问题
* @version 1.0
* @author  zhangyawen
* @data    2013-3-5
*/

#ifndef __SOCKET_OS_HPP
#define __SOCKET_OS_HPP


/*    
The operating system, must be one of: (Q_OS_x)    

MACX    - Mac OS X    
MAC9    - Mac OS 9    
MSDOS   - MS-DOS and Windows    
OS2     - OS/2    
OS2EMX  - XFree86 on OS/2 (not PM)    
WIN32   - Win32 (Windows 95/98/ME and Windows NT/2000/XP)    
CYGWIN  - Cygwin    
SOLARIS - Sun Solaris    
HPUX    - HP-UX    
ULTRIX  - DEC Ultrix    
LINUX   - Linux    
FREEBSD - FreeBSD    
NETBSD  - NetBSD    
OPENBSD - OpenBSD    
BSDI    - BSD/OS    
IRIX    - SGI Irix    
OSF     - HP Tru64 UNIX    
SCO     - SCO OpenServer 5    
UNIXWARE- UnixWare 7, Open UNIX 8    
AIX     - AIX    
HURD    - GNU Hurd    
DGUX    - DG/UX    
RELIANT - Reliant UNIX    
DYNIX   - DYNIX/ptx    
QNX     - QNX    
QNX6    - QNX RTP 6.1    
LYNX    - LynxOS    
BSD4    - Any BSD 4.4 system    
UNIX    - Any UNIX BSD/SYSV system    
*/ 

#if     (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#define OS_WINDOWS
#elif   defined(linux)
#define OS_LINUX
#elif   defined(SOLARIS)
#define OS_SOLARIS
#endif


#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

#ifdef OS_WINDOWS
#	define WIN32_LEAN_AND_MEAN 
#	include <windows.h>
#	include <winsock2.h>
#	include <intrin.h>
#	pragma comment(lib,"WS2_32.lib")

#	define GetSocketError	WSAGetLastError()
#	define CloseSocket(a)	closesocket(a)
#	define Data_cast(p)		(char*)p
#	define FD_SIZE			FD_SETSIZE

#else

#	include <unistd.h>
#	include <poll.h>
#	include <limits.h>
#	include <fcntl.h>

#	if defined(OS_LINUX)
#	include <ifaddrs.h> 
#	endif 

#	include <arpa/inet.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <sys/select.h>
#	include <sys/un.h>
#	include <netinet/in.h>
#	include <netinet/tcp.h>

#	define CloseSocket(a)	close(a)
#	define Data_cast(p)		(void*)p
#	define FD_SIZE			64
#	define INVALID_SOCKET	-1
#	define SOCKET_ERROR		-1
#	define SOCKET			int

#endif

#define LISTEN_BLOCKS       6

namespace module{

#ifdef OS_WINDOWS
	typedef int 				SOCK_LEN;
#else
	typedef socklen_t			SOCK_LEN;
	typedef struct sockaddr		SOCKADDR;
#endif

	typedef char 				int8;
	typedef unsigned char		uint8;
	typedef short				int16;
	typedef unsigned short		uint16;
	typedef int 				int32;
	typedef unsigned int		uint32;
	typedef long long 			int64;
	typedef unsigned long long	uint64;

	typedef struct sockaddr_in  SOCKADDRV4;
	typedef struct sockaddr_in6 SOCKADDRV6;

	typedef std::vector<uint32>			IP_LIST;
	typedef std::vector<std::string>	IP_LIST_STRING;

	typedef struct tagNetAddress
	{
		tagNetAddress(std::string strip_in,uint16 uport_in)
		{
			strip = strip_in;
			uport = uport_in;
		}
		tagNetAddress()
		{}

		std::string strip;

		uint16 uport;

	}NetAddress,*pNetAddress;

	typedef std::vector<NetAddress>		ADDRESS_LIST;
}


#endif	//__SOCKET_OS_HPP
