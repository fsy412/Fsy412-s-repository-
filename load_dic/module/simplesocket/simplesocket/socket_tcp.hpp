/*
* Copyright(C) 2013,Company ghca. All rights reserved.
* 
* @file    socket_tcp.hpp
* @brief   基于TCP套接字的封装
* @version 1.0
* @author  zhangyawen
* @data    2013-3-5
*/

#ifndef __SOCKET_TCP_HPP
#define __SOCKET_TCP_HPP

#include "socket_base.hpp"

namespace module{

	/*
	* @brief CTCPSocket
	* @note  TCP套接字封装
	* @data  2013-3-5 By zhangyawen
	*/
	class CTCPSocket:
		public CBaseSocket
	{
	public:

		CTCPSocket();

		~CTCPSocket();

	public:

		/*
		* @brief  Create
		* @param  const char* pip
		* @param  uint16 uport
		* @param  bool bclient = true
		* @return int 
		* @note   创建套接字分为服务器模式和客户端模式
		* @data   2013-3-5 By zhangyawen
		*/
		int Create(const char* pip,uint16 uport,bool bclient = true);

		/*
		* @brief  Read
		* @param  void *pout
		* @param  size_t szout
		* @return int 
		* @note   
		* @data   2013-3-5 By zhangyawen
		*/
		int Read(void *pout,size_t szout);

		/*
		* @brief  Write
		* @param  const void *pin
		* @param  size_t szin
		* @return int 
		* @note   向通信套接字中写入数据
		* @data   2013-3-7 By zhangyawen
		*/
		int Write(const void *pin,size_t szin);

		/*
		* @brief  Write
		* @param  SOCKET fd
		* @param  const void *pin
		* @param  size_t szin
		* @return int 
		* @note   向指定的TCP套接字中写入数据
		* @data   2013-3-7 By zhangyawen
		*/
		int Write(SOCKET fd,const void *pin,size_t szin);

		/*
		* @brief  Accept
		* @param  SOCKADDR &addr
		* @param  SOCK_LEN &addrlen
		* @return SOCKET 
		* @note   接受TCP连接
		* @data   2013-3-7 By zhangyawen
		*/
		SOCKET Accept(SOCKADDR &addr,SOCK_LEN &addrlen);

		/*
		* @brief  GetListenSocket
		* @param  
		* @return const SOCKET 
		* @note   得到监听套接字
		* @data   2013-3-7 By zhangyawen
		*/
		const SOCKET GetListenSocket() const;

		/*
		* @brief  GetConnectSocket
		* @param  
		* @return const SOCKET 
		* @note   得到通信时用的套接字
		* @data   2013-3-7 By zhangyawen
		*/
		const SOCKET GetConnectSocket() const;

	private:

		SOCKET __m_sListen,__m_sConnect;
	};

}

#endif	//__SOCKET_TCP_HPP
