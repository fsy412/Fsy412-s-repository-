#ifndef __THREAD_HPP
#define __THREAD_HPP 

#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN 
#	include <windows.h>
#	include <process.h>
#	define THREAD_HANDLE		HANDLE
#	define THREAD_FUN			DWORD WINAPI
	typedef unsigned (WINAPI *lpfnWorkThread)(LPVOID);
#else
#	include <unistd.h>
#	include <pthread.h>
#	define THREAD_HANDLE		pthread_t*
#	define THREAD_FUN			void*
#endif // WIN32

namespace module{

	class CThread 
	{
	public:

		CThread();

		virtual ~CThread();

	public:

		virtual int DoRunning();

		int CreateThread();

	private:

		static THREAD_FUN __WorkThread(void *arg);

	private:

		THREAD_HANDLE __m_hThread;
	};

}

#endif	// __THREAD_HPP
