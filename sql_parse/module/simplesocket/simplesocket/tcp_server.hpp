/*
* Copyright(C) 2013,Company ghca. All rights reserved.
* 
* @file    tcp_server.hpp
* @brief   针对低并发TCP连接封装的类，可以通过派生继承来使用
* @version 1.0
* @author  zhangyawen
* @data    2013-3-7
*/

#ifndef __TCP_SERVER_HPP
#define __TCP_SERVER_HPP

#include "socket_tcp.hpp"
#include "socket_run.hpp"

namespace module{

	class CTCPServer:
		private CSocketRunable
	{
	public:

		CTCPServer();

		~CTCPServer();

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

		virtual int OnRead(void *pdata,size_t szlen,SOCKET fd_use) = 0;

		virtual int OnAccept(SOCKET fd_use,SOCKADDR addr) = 0;

		virtual int OnClose(SOCKET fd_use) = 0;

		int CreateServer(const char* pip,uint16 uport);

	public:

		const SOCKET GetConnectSocket() const;

	private:

		CTCPSocket *__m_pTcpSocket;

	private:

		volatile bool __m_bQuit;
	};



	class CTCPSingleBind;
	class CTCPMultiBind;

	class CTCPSingleBind:
		public CTCPServer
	{
	public:

		CTCPSingleBind();

		~CTCPSingleBind();

	protected:

		virtual int OnRead(void *pdata,size_t szlen,SOCKET fd_use);

		virtual int OnAccept(SOCKET fd_use,SOCKADDR addr);

		virtual int OnClose(SOCKET fd_use);

	public:

		int Create(const char *pip,unsigned short uport,CTCPMultiBind *pCreater = NULL);

	private:

		CTCPMultiBind *__m_pCreater;

	};

	class CTCPMultiBind:
		private CBaseSocket
	{
	public:

		CTCPMultiBind();

		~CTCPMultiBind();

	public:

		virtual int OnRead(void *pdata,size_t szlen,SOCKET fd_use) = 0;

		virtual int OnAccept(SOCKET fd_use,SOCKADDR addr) = 0;

		virtual int OnClose(SOCKET fd_use) = 0;

	public:

		int Write(SOCKET fd_use,void *pdata,size_t szlen);

		int Create(ADDRESS_LIST address_list);

	private:

		CTCPSingleBind *__m_pSingleBind;
	};

}

#endif	//__TCP_SERVER_HPP
