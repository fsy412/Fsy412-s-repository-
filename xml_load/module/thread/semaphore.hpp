#ifndef __SEMAPHORE_HPP 
#define __SEMAPHORE_HPP

#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN 
#	include <windows.h>
#	define SEM_HANDLE			HANDLE
#else
#	include <unistd.h>
#	include <semaphore.h>
#	define SEM_HANDLE			sem_t*
#	define  INFINITE			0
#endif // WIN32

namespace module{

	class CSemaphore 
	{
	public:

		CSemaphore();

		~CSemaphore();

	public:

		int Post();

		int Wait(int nTimeout = INFINITE);

	private: 

		SEM_HANDLE __m_hSem;
	};

}

#endif	// __SEMAPHORE_HPP
