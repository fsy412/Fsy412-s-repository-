#include "socket_udp.hpp"

namespace module{

	CUDPSocket::CUDPSocket():__m_sConnect(INVALID_SOCKET)
	{}

	CUDPSocket::~CUDPSocket()
	{
		CBaseSocket::Close(__m_sConnect);
	}

	int CUDPSocket::Create(const char *pip/* = NULL*/,uint16 uport/* = 0*/,bool bclient/* = true*/)
	{
		if((__m_sConnect = CBaseSocket::Create(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == INVALID_SOCKET)
		{
			perror("Create udp socket failed!");
			return -1;
		}

		if(bclient)
		{
			return 0;
		}


#ifndef OS_WINDOWS

		int on = 1;

		// Set port reuse
		if(CBaseSocket::SetSocketOption(__m_sConnect, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(int)) == -1)
		{
			perror("Set port reuse failed!");
			return -1;
		}
#endif

		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_port = htons(uport);
		sin.sin_addr.s_addr = inet_addr(pip);

		if(CBaseSocket::Bind(__m_sConnect,(sockaddr*)&sin,sizeof(sin)) == SOCKET_ERROR)
		{
			perror("Bind failed!");
			return -1;
		}

		return 0;
	}

	int CUDPSocket::Read(void *pout,size_t szout,SOCKADDR &addr)
	{
		return CBaseSocket::RecvFrom(__m_sConnect,pout,szout,0,addr);
	}

	int CUDPSocket::Write(const void *pin,size_t szin,const char *pip,uint16 uport)
	{
		//ÃÓ≥‰‘∂≥Ãµÿ÷∑
		SOCKADDRV4 addr;
		addr.sin_family = AF_INET;
		addr.sin_port = ntohs(uport);
		addr.sin_addr.s_addr = inet_addr(pip);

		return CBaseSocket::SendTo(__m_sConnect,pin,szin,0,*(SOCKADDR*)&addr);
	}

	int CUDPSocket::Write(const void *pin,size_t szin,uint32 uip,uint16 uport)
	{
		//ÃÓ≥‰‘∂≥Ãµÿ÷∑
		SOCKADDRV4 addr;
		addr.sin_family = AF_INET;
		addr.sin_port = uport;
		addr.sin_addr.s_addr = uip;

		return CBaseSocket::SendTo(__m_sConnect,pin,szin,0,*(SOCKADDR*)&addr);
	}

	int CUDPSocket::Write(const void *pin,size_t szin,SOCKADDR addr)
	{
		return CBaseSocket::SendTo(__m_sConnect,pin,szin,0,addr);
	}

	const SOCKET CUDPSocket::GetConnectSocket() const 
	{
		return __m_sConnect;
	}

}
