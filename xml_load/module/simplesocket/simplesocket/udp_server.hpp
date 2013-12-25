/*
* Copyright(C) 2013,Company ghca. All rights reserved.
* 
* @file    udp_server.hpp
* @brief   UDP长连接服务器类
* @version 1.0
* @author  zhangyawen
* @data    2013-3-7
*/

#ifndef __UDP_SERVER_HPP
#define __UDP_SERVER_HPP

#include "socket_udp.hpp"
#include "socket_run.hpp"

namespace module{

	class CUDPServer:
		private CSocketRunable
	{
	public:

		CUDPServer();

		~CUDPServer();

	private:

		/*
		* @brief  DoHandling
		* @param  
		* @return int 
		* @note   重载CSocketRunable的DoHandling函数，内部会回调OnRead等函数
		* @data   2013-3-7 By zhangyawen
		*/
		int DoHandling();

	public:

		virtual int OnRead(void *pdata,size_t szlen,SOCKADDR addr) = 0;

		int Write(const void *pin,size_t szin,const char *pip,uint16 uport);

		int Write(const void *pin,size_t szin,uint32 uip,uint16 uport);

		int Write(const void *pin,size_t szin,SOCKADDR addr);

		int CreateServer(const char* pip,uint16 uport);

		const SOCKET GetConnectSocket() const;

	private:

		CUDPSocket *__m_pUdpSocket;

	private:

		volatile bool __m_bQuit;
	};


	class CUDPSingleBind;
	class CUDPMultiBind;

	class CUDPSingleBind:
		public CUDPServer
	{
	public:

		CUDPSingleBind();

		~CUDPSingleBind();

	public:

		virtual int OnRead(void *pdata,size_t szlen,SOCKADDR addr);

	public:

		int Create(const char *pip,unsigned short uport,CUDPMultiBind *pCreater);

	private:

		CUDPMultiBind *__m_pCreater;
	};


	class CUDPMultiBind
	{
	public:

		CUDPMultiBind();

		~CUDPMultiBind();

	public:

		virtual int OnRead(void *pdata,size_t szlen,SOCKADDR addr,CUDPSingleBind *pTrans) = 0;

		int Write(void *pdata,size_t szlen,SOCKADDR addr,CUDPSingleBind *pTrans);

	public:

		int Create(ADDRESS_LIST address_list);

	private:

		CUDPSingleBind *__m_pSingleBind;

	};

}

#endif	//__UDP_SERVER_HPP
