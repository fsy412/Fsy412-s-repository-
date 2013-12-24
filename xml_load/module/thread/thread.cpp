#include "thread.hpp"

namespace module{

	CThread::CThread():__m_hThread(NULL)
	{}

	CThread::~CThread()
	{
#ifdef WIN32
		if(__m_hThread)
		{
			WaitForSingleObject(__m_hThread,INFINITE);

			CloseHandle(__m_hThread);
		}
#else
		delete __m_hThread;
#endif
	}

	int CThread::DoRunning()
	{
		return this->DoRunning();
	}

	int CThread::CreateThread()
	{
#ifdef WIN32
		__m_hThread = (HANDLE)_beginthreadex(NULL, 0, (lpfnWorkThread)__WorkThread, this, 0, NULL); 

		if (__m_hThread != 0)
		{
			return 0;
		}
		else
		{
			return GetLastError();
		}
#else
		pthread_attr_t attr;

		pthread_attr_init(&attr);

		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

		__m_hThread = new pthread_t;

		pthread_create(__m_hThread, &attr,__WorkThread, this);

		pthread_attr_destroy(&attr);
#endif
		return 0;
	}

	THREAD_FUN CThread::__WorkThread(void *arg)
	{
		CThread *pThis = (CThread*) arg;
#ifdef WIN32
		return pThis->DoRunning();
#else
		pThis->DoRunning();
#endif
	}


}
