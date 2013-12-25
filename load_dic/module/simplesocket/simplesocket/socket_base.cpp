#include "socket_base.hpp"

namespace module{

	CBaseSocket::CBaseSocket()
	{
#ifdef OS_WINDOWS
		WSADATA wsaData = { 0 };
		if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0 || 
			LOBYTE( wsaData.wVersion ) != 2 || 
			HIBYTE( wsaData.wVersion ) != 2 )
		{}
#endif
	}

	CBaseSocket::~CBaseSocket()
	{
#ifdef OS_WINDOWS
		WSACleanup();
#endif
	}

	SOCKET CBaseSocket::Create(int family, int type, int protocol)
	{
		return socket(family, type, protocol);
	}

	SOCKET CBaseSocket::Accept(SOCKET fd,SOCKADDR &clientaddr, SOCK_LEN &addrlen)
	{
		return accept(fd,&clientaddr,&addrlen);
	}
	
	#if (defined(OS_LINUX) || defined(OS_WINDOWS))

	int CBaseSocket::GetIpList(IP_LIST &Ip_List)
	{
		Ip_List.clear();

#ifdef OS_WINDOWS
		char name[MAX_PATH];
		if(gethostname(name, sizeof(name)) == 0)
		{
			struct hostent* phe = gethostbyname(name);
			if (0 == phe)
			{
				return -1;
			}
			for (int i = 0; phe->h_addr_list[i] != 0; ++i)
			{
				SOCKADDRV4 addr;

				memcpy(&addr.sin_addr, phe->h_addr_list[i], sizeof(addr.sin_addr));

				Ip_List.push_back(addr.sin_addr.s_addr);
			}
		} 
#else
		struct ifaddrs *ifa = NULL, *oifa = NULL;
		if(getifaddrs(&ifa) < 0)
		{
			return -1;
		}
		oifa = ifa;
		while(ifa != NULL)
		{
			// IPv4 localhost
			if (ifa->ifa_addr != NULL && ifa->ifa_addr->sa_family == AF_INET && strncmp(ifa->ifa_name, "lo", 2) != 0)
			{
				SOCKADDRV4* addr = (SOCKADDRV4*)ifa->ifa_addr;
				Ip_List.push_back(addr->sin_addr.s_addr);
			}
			ifa = ifa->ifa_next;
		}
		freeifaddrs(oifa);
#endif
		return 0;
	}
	int CBaseSocket::GetIpList(IP_LIST_STRING &Ip_List_String)
	{
		Ip_List_String.clear();
		IP_LIST Ip_List;

		if(GetIpList(Ip_List) != -1)
		{
			for(size_t i = 0;i < Ip_List.size();i++)
			{
				SOCKADDRV4 addr;
				uint32 udata = Ip_List[i];
				memcpy(&addr.sin_addr,&udata,sizeof(uint32));
				Ip_List_String.push_back(inet_ntoa(addr.sin_addr));
			}
		}
		else
			return -1;

		return 0;
	}
	#endif
	
	int CBaseSocket::Close(SOCKET fd)
	{
		if(fd != INVALID_SOCKET)
			return CloseSocket(fd);
		return -1;
	}

	int CBaseSocket::Bind(SOCKET fd,const SOCKADDR *paddr,SOCK_LEN len)
	{
		return bind(fd,paddr,len);
	}

	int CBaseSocket::Listen(SOCKET fd,int backlog)
	{
		return listen(fd,backlog);
	}

	int CBaseSocket::Connect(SOCKET fd,const SOCKADDR *paddr,SOCK_LEN len)
	{
		return connect(fd,paddr,len);
	}

	int CBaseSocket::Recv(SOCKET fd,void *pdata, uint32 nbytes,uint32 flags)
	{
		return recv(fd,Data_cast(pdata),nbytes,flags);
	}

	int CBaseSocket::RecvFrom(SOCKET fd,void *pdata, uint32 nbytes,uint32 flags,SOCKADDR &addr)
	{
		SOCK_LEN len = sizeof(addr);
		return recvfrom(fd,Data_cast(pdata),nbytes,flags,&addr,&len);
	}

	int CBaseSocket::Send(SOCKET fd,const void *pdata, uint32 nbytes,uint32 flags)
	{
		return send(fd,Data_cast(pdata), nbytes, flags);
	}

	int CBaseSocket::SendTo(SOCKET fd,const void *pdata, uint32 nbytes,uint32 flags,SOCKADDR addr)
	{
		return sendto(fd,Data_cast(pdata),nbytes,flags,&addr,sizeof(addr));
	}

	int CBaseSocket::SetSocketOption(SOCKET fd,int level,int optname,const char *optval,int optlen)
	{
		return setsockopt(fd,level,optname,optval,optlen);
	}

	int CBaseSocket::SetBlocking(SOCKET fd,bool blocking)
	{

#ifdef OS_WINDOWS
		uint32 flags = (blocking == true) ? 0 : 1;
		return ioctlsocket(fd, FIONBIO, (u_long*)&flags) == 0 ? 0 : -1;
#else
		int32 flags = 0;

		if ((flags = fcntl(fd, F_GETFL)) == -1) 
		{
			return -1;
		}

		if (blocking)
			flags &= ~O_NONBLOCK;
		else
			flags |= O_NONBLOCK;

		if (fcntl(fd, F_SETFL, flags) == -1) 
		{
			return -1;
		}

		return 0;
#endif
	}

	int CBaseSocket::Select(SOCKET maxfd,fd_set *readfds,fd_set *writefds,fd_set *exceptfds,struct timeval*timeout)
	{
		return select(maxfd,readfds,writefds,exceptfds,timeout);
	}

	int CBaseSocket::SelectServer(SOCKET fd,SOCKET fd_conn[FD_SIZE],int &nfd_count,SOCKET &fd_use,SOCKADDR &addr,void *buff,uint32 nbytes)
	{
		int nRet = 0,nMaxsocket = fd;

		SOCKET sClient = INVALID_SOCKET;

		SOCKADDR sClientaddr = { 0 };
		SOCK_LEN sLen = sizeof(sClientaddr);

		fd_set fdSocket = { 0 },fdRead = { 0 };

		FD_ZERO(&fdSocket);

		FD_SET(fd,&fdSocket);

		fdRead = fdSocket;

		nMaxsocket = fd;

		fd_use = INVALID_SOCKET;

		for(int i = 0; i < nfd_count; i++)
		{
			FD_SET(fd_conn[i], &fdRead);

			if((int)fd_conn[i] > nMaxsocket)
			{
				nMaxsocket = fd_conn[i];
			}
		}

		nRet = Select(nMaxsocket + 1, &fdRead, NULL, NULL, NULL);

		if(nRet <= 0)
		{
			return SOCKET_ERROR;
		}

		// Check all the socket
		for (int i = 0; i < nfd_count; i++)
		{
			if(FD_ISSET(fd_conn[i], &fdRead))
			{
				// Recv data
				nRet = Recv(fd_conn[i],buff,nbytes, 0);

				if (nRet <= 0)
				{
					fd_use = fd_conn[i];

					Close(fd_conn[i]);

					FD_CLR(fd_conn[i], &fdRead);

					fd_conn[i] = INVALID_SOCKET;

					// Repeated use of the socket
					for(;i < nfd_count - 1;i++)
					{
						fd_conn[i] = fd_conn[i+1];
					}

					nfd_count--;

					return nRet;
				}
				else
				{
					fd_use = fd_conn[i];

					return nRet;
				}
			}
		}
		// Check the socket
		if (FD_ISSET(fd, &fdRead))
		{
			sLen = sizeof(SOCKADDR);

			// Accept socket
			sClient = Accept(fd, sClientaddr,sLen);

			if (sClient <= 0)
			{
				return SOCKET_ERROR;
			}

			// Add socket
			if (nfd_count < FD_SIZE)
			{
				addr = sClientaddr;

				fd_conn[nfd_count++] = sClient;

				fd_use = sClient;
			}
			else
			{
				Close(sClient);
				return SOCKET_ERROR;
			}
		}

		return 0;
	}

}
