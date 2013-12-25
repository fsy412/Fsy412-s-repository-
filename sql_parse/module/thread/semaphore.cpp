#include "semaphore.hpp"

namespace module{

	// Class Semaphore 
	CSemaphore::CSemaphore()
	{

#ifdef WIN32
		__m_hSem = CreateSemaphore(NULL, 0 , 2147483647, NULL); // 2^31 - 1 
#else
		__m_hSem = new sem_t;

		sem_init(__m_hSem,0,0);
#endif

	}


	CSemaphore::~CSemaphore()
	{

#ifdef WIN32	
		CloseHandle(__m_hSem);
#else
		sem_destroy(__m_hSem);

		delete __m_hSem;
#endif

	}



	int CSemaphore::Post ()
	{

#ifdef WIN32
		if(ReleaseSemaphore(__m_hSem,1,NULL)!=0)
		{
			return 0;
		}
		else
		{
			return GetLastError();
		}
#else
		return sem_post(__m_hSem);
#endif

	}

	int CSemaphore::Wait (int nTimeout)
	{

#ifdef WIN32
		if(nTimeout != INFINITE)
		{
			nTimeout*=1000;
		}
		
		int val = WaitForSingleObject(__m_hSem,nTimeout);

		if(val == WAIT_OBJECT_0)
		{
			return 0;
		}
		else
		{
			return val;
		}
#else
		if(nTimeout == INFINITE)
		{
			return sem_wait(__m_hSem);
		}
		else
		{
			for(int i=0;i<nTimeout;i++)
			{
				if(sem_trywait(__m_hSem)==0)
				{
					return 0;
				}
				sleep(1);
			}
			return -1;
		}

#endif

	}

}
