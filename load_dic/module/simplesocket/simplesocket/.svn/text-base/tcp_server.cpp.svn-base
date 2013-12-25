#include "tcp_server.hpp"

namespace module{

	CTCPServer::CTCPServer():__m_pTcpSocket(NULL),__m_bQuit(true)
	{}

	CTCPServer::~CTCPServer()
	{
		__m_bQuit = false;
	}

	int CTCPServer::DoHandling()
	{
		SOCKET fd_Listen = __m_pTcpSocket->GetListenSocket();
		SOCKET fd_Con[FD_SIZE] = { INVALID_SOCKET };
		SOCKET fd_Use = INVALID_SOCKET;

		int nconn = 0,nret = 0,nlastconn = 0;

		SOCKADDR addr = { 0 };

		char buffer[65535] = { 0 };

		while(!__m_bQuit)	
		{
			nlastconn = nconn;

			if((nret = __m_pTcpSocket->CBaseSocket::SelectServer(fd_Listen,fd_Con,nconn,fd_Use,addr,(void*)buffer,sizeof(buffer))) > 0)
			{
				this->OnRead((void*)buffer,nret,fd_Use);
			}
			else
			{
				if(nconn > nlastconn)
				{
					this->OnAccept(fd_Use,addr);
				}
				else if(nconn < nlastconn)
				{
					this->OnClose(fd_Use);
				}
			}
		}

		return 0;
	}

	int CTCPServer::CreateServer(const char* pip,uint16 uport)
	{
		__m_bQuit = true;

		if(__m_pTcpSocket == NULL)
		{
			__m_pTcpSocket = new CTCPSocket();
		}

		if(__m_pTcpSocket->Create(pip,uport,false) == -1)
		{	
			return -1;
		}

		__m_bQuit = false;

		return this->StartRunning();
	}

	inline const SOCKET CTCPServer::GetConnectSocket() const
	{
		if(__m_pTcpSocket == NULL)
			return INVALID_SOCKET;

		return __m_pTcpSocket->GetConnectSocket();
	}

	CTCPSingleBind::CTCPSingleBind():__m_pCreater(NULL)
	{}

	CTCPSingleBind::~CTCPSingleBind()
	{}

	int CTCPSingleBind::OnRead(void *pdata,size_t szlen,SOCKET fd_use)
	{
		if(__m_pCreater != NULL)
			__m_pCreater->OnRead(pdata,szlen,fd_use);

		return 0;
	}

	int CTCPSingleBind::OnAccept(SOCKET fd_use,SOCKADDR addr)
	{
		if(__m_pCreater != NULL)
			__m_pCreater->OnAccept(fd_use,addr);

		return 0;
	}

	int CTCPSingleBind::OnClose(SOCKET fd_use)
	{
		if(__m_pCreater != NULL)
			__m_pCreater->OnClose(fd_use);

		return 0;
	}

	int CTCPSingleBind::Create(const char *pip,unsigned short uport,CTCPMultiBind *pCreater/* = NULL*/)
	{
		__m_pCreater = pCreater;
		return CTCPServer::CreateServer(pip,uport);
	}

	CTCPMultiBind::CTCPMultiBind():__m_pSingleBind(NULL)
	{}

	CTCPMultiBind::~CTCPMultiBind()
	{}

	int CTCPMultiBind::Write(SOCKET fd_use,void *pdata,size_t szlen)
	{
		return CBaseSocket::Send(fd_use,pdata,szlen,0);
	}

	int CTCPMultiBind::Create(ADDRESS_LIST address_list)
	{
		if(address_list.size() == 0)
			return 0;

		__m_pSingleBind = new CTCPSingleBind[address_list.size()];

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
