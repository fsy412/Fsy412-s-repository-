/*
* Copyright(C) 2013,Company ghca. All rights reserved.
* 
* @file    socket_base.hpp
* @brief   套接字操作的跨平台封装
* @version 1.0
* @author  zhangyawen
* @data    2013-3-5
*/

#ifndef __SOCKET_OP_HPP
#define __SOCKET_OP_HPP

#include "socket_os.hpp"

namespace module{

	/*
	* @brief CBaseSocket
	* @note  套接字跨平台基本操作封装
	* @data  2013-3-5 By zhangyawen
	*/
	class CBaseSocket
	{
	public:

		CBaseSocket();

		~CBaseSocket();

	public:

		SOCKET Create(int family, int type, int protocol);

		SOCKET Accept(SOCKET fd,SOCKADDR &addr,SOCK_LEN &len);
		
		#if (defined(OS_LINUX) || defined(OS_WINDOWS))
		
 		int GetIpList(IP_LIST &Ip_List);
 
 		int GetIpList(IP_LIST_STRING &Ip_List_String);
		
		#endif 
		
		int Close(SOCKET fd);

		int Bind(SOCKET fd,const SOCKADDR *paddr,SOCK_LEN len);

		int Listen(SOCKET fd,int backlog);

		int Connect(SOCKET fd,const SOCKADDR *paddr,SOCK_LEN len);

		int Recv(SOCKET fd,void *pdata, uint32 nbytes,uint32 flags);

		int RecvFrom(SOCKET fd,void *pdata, uint32 nbytes,uint32 flags,SOCKADDR &addr);

		int Send(SOCKET fd,const void *pdata, uint32 nbytes,uint32 flags);

		int SendTo(SOCKET fd,const void *pdata, uint32 nbytes,uint32 flags,SOCKADDR addr);

		int SetSocketOption(SOCKET fd,int level,int optname,const char *optval,int optlen);

		int SetBlocking(SOCKET fd,bool blocking);

		int Select(SOCKET maxfd,fd_set *readfds,fd_set *writefds,fd_set *exceptfds,struct timeval*timeout);

		int SelectServer(SOCKET fd,SOCKET fd_conn[FD_SIZE],int &nfd_count,SOCKET &fd_use,SOCKADDR &addr,void *buff,uint32 nbytes);
	};

}

#endif	//__SOCKET_OP_HPP
