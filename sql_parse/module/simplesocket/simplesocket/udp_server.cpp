#include "udp_server.hpp"

namespace module{

	CUDPServer::CUDPServer():__m_pUdpSocket(NULL),__m_bQuit(true)
	{}

	CUDPServer::~CUDPServer()
	{
		__m_bQuit = false;
	}

	int CUDPServer::DoHandling()
	{
		SOCKADDR addr = { 0 };

		char buffer[65535] = { 0 };

		int nret = 0;

		while(!__m_bQuit)	
		{
			if((nret = __m_pUdpSocket->Read(buffer,sizeof(buffer),addr)) > 0)
			{
				this->OnRead((void*)buffer,nret,addr);
			}
		}

		return 0;
	}

	int CUDPServer::Write(const void *pin,size_t szin,const char *pip,uint16 uport)
	{
		if(__m_pUdpSocket != NULL)
		{
			return __m_pUdpSocket->Write(pin,szin,pip,uport);
		}
		return -1;
	}

	int CUDPServer::Write(const void *pin,size_t szin,uint32 uip,uint16 uport)
	{
		if(__m_pUdpSocket != NULL)
		{
			return __m_pUdpSocket->Write(pin,szin,uip,uport);
		}
		return -1;
	}

	int CUDPServer::Write(const void *pin,size_t szin,SOCKADDR addr)
	{
		if(__m_pUdpSocket != NULL)
		{
			return __m_pUdpSocket->Write(pin,szin,addr);
		}
		return -1;
	}

	int CUDPServer::CreateServer(const char* pip,uint16 uport)
	{
		__m_bQuit = true;

		if(__m_pUdpSocket == NULL)
		{
			__m_pUdpSocket = new CUDPSocket();
		}

		if(__m_pUdpSocket->Create(pip,uport,false) == -1)
		{	
			return -1;
		}

		__m_bQuit = false;

		return this->StartRunning();
	}

	const SOCKET CUDPServer::GetConnectSocket()const 
	{
		if(__m_pUdpSocket == NULL)
			return INVALID_SOCKET;

		return __m_pUdpSocket->GetConnectSocket();
	}

	CUDPSingleBind::CUDPSingleBind():__m_pCreater(NULL)
	{}

	CUDPSingleBind::~CUDPSingleBind()
	{}

	int CUDPSingleBind::OnRead(void *pdata,size_t szlen,SOCKADDR addr)
	{
		if(__m_pCreater != NULL)
		{
			return __m_pCreater->OnRead(pdata,szlen,addr,this);
		}

		return 0;
	}

	int CUDPSingleBind::Create(const char *pip,unsigned short uport,CUDPMultiBind *pCreater)
	{
		__m_pCreater = pCreater;
		return CUDPServer::CreateServer(pip,uport);
	}

	CUDPMultiBind::CUDPMultiBind():__m_pSingleBind(NULL)
	{}

	CUDPMultiBind::~CUDPMultiBind()
	{
		if(__m_pSingleBind != NULL)
			delete[] __m_pSingleBind;
	}

	int CUDPMultiBind::Write(void *pdata,size_t szlen,SOCKADDR addr,CUDPSingleBind *pTrans)
	{
		if(pTrans != NULL)
		{
			return pTrans->Write(pdata,szlen,addr);
		}

		return -1;
	}

	int CUDPMultiBind::Create(ADDRESS_LIST address_list)
	{
		if(address_list.size() == 0)
			return 0;

		__m_pSingleBind = new CUDPSingleBind[address_list.size()];

		for(size_t i = 0;i < address_list.size();i++)
		{
			if(__m_pSingleBind[i].Create(address_list[i].strip.c_str(),address_list[i].uport,this) == -1)
			{
				delete[] __m_pSingleBind;
				__m_pSingleBind = NULL;

				return -1;
			}
		}

		return 0;
	}
}
