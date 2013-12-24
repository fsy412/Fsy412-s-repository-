/*
* Copyright(C) 2013,Company ghca. All rights reserved.
* 
* @file    socket_udp.hpp
* @brief   UDP套接字封装类
* @version 1.0
* @author  zhangyawen
* @data    2013-3-7
*/

#ifndef __SOCKET_UDP_HPP
#define __SOCKET_UDP_HPP

#include "socket_base.hpp"

namespace module{

	class CUDPSocket:
		private CBaseSocket
	{
	public:

		CUDPSocket();

		~CUDPSocket();

	public:

		/*
		* @brief  Create
		* @param  const char *pip
		* @param  uint16 uport
		* @param  bool bclient = true
		* @return int 
		* @note   创建套接字，如果是客户端模式，pip和uport无效
		* @data   2013-3-7 By zhangyawen
		*/
		int Create(const char *pip = NULL,uint16 uport = 0,bool bclient = true);

		/*
		* @brief  Read
		* @param  void *pout
		* @param  size_t szout
		* @param  SOCKADDR &addr
		* @return int 
		* @note   读取数据
		* @data   2013-3-7 By zhangyawen
		*/
		int Read(void *pout,size_t szout,SOCKADDR &addr);

		/*
		* @brief  Write
		* @param  const void *pin
		* @param  size_t szin
		* @param  const char *pip
		* @param  uint16 uport
		* @return int 
		* @note   向某一地址写入数据，第二三个重载函数中ip和port必须是通信中得到的网络信息，无htonl转化的数据
		* @data   2013-3-7 By zhangyawen
		*/
		int Write(const void *pin,size_t szin,const char *pip,uint16 uport);

		int Write(const void *pin,size_t szin,uint32 uip,uint16 uport);

		int Write(const void *pin,size_t szin,SOCKADDR addr);

		/*
		* @brief  GetConnectSocket
		* @param  
		* @return const SOCKET 
		* @note   获取连接所用的套接字
		* @data   2013-3-7 By zhangyawen
		*/
		const SOCKET GetConnectSocket() const;

	private:

		SOCKET __m_sConnect;
	};

}

#endif	//__SOCKET_UDP_HPP
