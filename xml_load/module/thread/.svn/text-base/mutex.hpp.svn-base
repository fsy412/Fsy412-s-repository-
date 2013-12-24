#ifndef __MUTEX_HPP
#define __MUTEX_HPP 

#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN 
#	include <windows.h>
#   define CRITICAL_HANDLE		CRITICAL_SECTION
#else
#	include <unistd.h>
#	include <pthread.h>
#	define CRITICAL_HANDLE      pthread_mutex_t*
#endif // WIN32

namespace module{

	class CMutex 
	{
	public:

		CMutex();

		~CMutex();

	public:

		int Lock();

		int Unlock();

	private: 

		CRITICAL_HANDLE __m_hMutex;				   
	};

}

#endif	// __MUTEX_HPP
