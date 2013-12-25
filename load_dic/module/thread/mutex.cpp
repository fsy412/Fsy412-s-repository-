#include "mutex.hpp"

namespace module{

	CMutex::CMutex()
	{
#ifdef WIN32	
		InitializeCriticalSection(&__m_hMutex);
#else
		__m_hMutex = new pthread_mutex_t;

		pthread_mutex_init(__m_hMutex, NULL);
#endif
	}

	CMutex::~CMutex()
	{
#ifdef WIN32	
		DeleteCriticalSection(&__m_hMutex);
#else
		pthread_mutex_destroy(__m_hMutex);
		
		delete __m_hMutex;
#endif
	}

	int CMutex::Lock()
	{
#ifdef WIN32	
		EnterCriticalSection(&__m_hMutex); 
		return 0;
#else
		return pthread_mutex_lock(__m_hMutex);
#endif

	}

	int CMutex::Unlock()
	{
#ifdef WIN32	
		LeaveCriticalSection(&__m_hMutex);
		return 0;
#else
		return pthread_mutex_unlock( __m_hMutex );
#endif
	}

}
