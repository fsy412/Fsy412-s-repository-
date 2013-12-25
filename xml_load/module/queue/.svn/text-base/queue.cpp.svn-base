#include "queue.hpp"

namespace module{

	CSingleQueue::CSingleQueue():__m_szLevel(0),__m_nSeed(0),__m_bWait(false)
	{}

	CSingleQueue::~CSingleQueue()
	{}

	int CSingleQueue::Assgin(size_t szsize,size_t szbuf,bool bpopwait/* = true*/)
	{
		__m_bWait = bpopwait;

		return __m_queBuffer.Assgin(szsize,szbuf);
	}

	int CSingleQueue::PushData(const void* pdata,size_t len,size_t &szcontlen)
	{
		int nret = 0;

		__m_mutLock.Lock();

		nret = __m_queBuffer.PushData(pdata,len,szcontlen);

		if(szcontlen == 1 && __m_nSeed < 0)
		{
			__m_nSeed++;
			__m_semDate.Post();
		}
		__m_szLevel = szcontlen > __m_szLevel ? szcontlen : __m_szLevel;

		__m_mutLock.Unlock();

		return nret;
	}

	size_t CSingleQueue::PopData(void* pdata,size_t szlen,size_t &szcontlen)
	{
		size_t szret = 0;

		while(true)
		{
			__m_mutLock.Lock();

			szret = __m_queBuffer.PopData(pdata,szlen,szcontlen);

			// 数据为空
			if (__m_bWait && szret == 0 && szcontlen == 0)
			{
				__m_nSeed--;

				__m_mutLock.Unlock();

				__m_semDate.Wait();
			}
			else 
				break;
		} 

		__m_mutLock.Unlock();

		return szret;
	}

	void CSingleQueue::SetPopWait(bool bpopwait/* = true*/)
	{
		__m_bWait = bpopwait;
	}

	size_t CSingleQueue::GetUnitSize()
	{
		return __m_queBuffer.GetUnitSize();
	}

	size_t CSingleQueue::GetLength()
	{
		size_t szret = 0;

		__m_mutLock.Lock();
		szret = __m_queBuffer.GetLength();
		__m_mutLock.Unlock();

		return szret;
	}

	size_t CSingleQueue::GetSize()
	{
		size_t szret = 0;

		szret = __m_queBuffer.GetSize();

		return szret;
	}

	size_t CSingleQueue::GetThrow()
	{
		return __m_queBuffer.GetThrow();
	}

	size_t CSingleQueue::GetWaterLevel()
	{
		return __m_szLevel;
	}

	CDoubleQueue::CDoubleQueue():
		__m_pQueRead(NULL),
		__m_pQueWrite(NULL),
		__m_bQuitSwap(true),
		__m_bReadSwap(false),
		__m_bWriteSwap(false),
		__m_bWait(false),
		__m_nTime(0),
		__m_szLevel(0)
	{
		__m_pQueRead = &__m_queRead;
		__m_pQueWrite = &__m_queWrite;
	}

	CDoubleQueue::~CDoubleQueue()
	{
		__m_bQuitSwap = true;
	}

	int CDoubleQueue::DoRunning()
	{
		CCircularQueue* pQue = NULL;

		while(!__m_bQuitSwap)
		{
			module::Sleep(__m_nTime);

			// 人工切换
			if(__m_bReadSwap&&__m_bWriteSwap)
			{}
			else if(__m_bReadSwap)
			{	
				__m_mutWrite.Lock();

				if(!__m_bWriteSwap && (!__m_pQueWrite->IsEmpty() || !__m_bWait))
				{
					pQue = __SwapQueue(__m_pQueWrite);
					__m_pQueWrite = pQue;
					__m_semWrite.Post();
				}

				__m_mutWrite.Unlock();
			}
			else if(__m_bWriteSwap)
			{
				__m_mutRead.Lock();

				if(!__m_bReadSwap && !__m_pQueRead->IsFull())
				{
					pQue = __SwapQueue(__m_pQueRead);
					__m_pQueRead = pQue;
					__m_semRead.Post();
				}

				__m_mutRead.Unlock();
			}

		}

		return 0;
	}

	int CDoubleQueue::Assgin(size_t szsize,size_t szbuf,bool bpopwait/* = true*/,int ntime/* = 10*/)
	{
		int nret = 0;

		__m_bQuitSwap = true;

		if((nret = __m_queRead.Assgin(szsize,szbuf)) == -1)
		{
			return -1;
		}

		if((nret = __m_queWrite.Assgin(szsize,szbuf)) == -1)
		{
			return -1;
		}

		__m_nTime = ntime;
		__m_bQuitSwap = false;
		__m_bWait = bpopwait;

		this->CreateThread();

		return nret;
	}

	int CDoubleQueue::PushData(const void* pdata,size_t szlen,size_t &szcontlen)
	{
		static CCircularQueue* pQueRead = __m_pQueRead;
		int nret = 0;

		__m_mutWrite.Lock();

		if(__m_pQueWrite->IsFull())
		{
			__m_semWrite.Post();
			pQueRead = __SwapQueue(__m_pQueWrite);
			__m_bWriteSwap = true;
			__m_pQueWrite = pQueRead;
			__m_semRead.Wait();
			__m_bWriteSwap = false;
		}

		nret = __m_pQueWrite->PushData(pdata,szlen,szcontlen);

		__m_szLevel = szcontlen > __m_szLevel ? szcontlen : __m_szLevel;

		__m_mutWrite.Unlock();

		return nret;
	}

	size_t CDoubleQueue::PopData(void* pdata,size_t szlen,size_t &szcontlen)
	{
		static CCircularQueue* pQueWrite = __m_pQueWrite;
		size_t szret = 0;

		__m_mutRead.Lock();

		if(__m_pQueRead->IsEmpty())
		{
			__m_semRead.Post();
			pQueWrite = __SwapQueue(__m_pQueRead);
			__m_bReadSwap = true;
			__m_pQueRead = pQueWrite;
			__m_semWrite.Wait();
			__m_bReadSwap = false;
		}

		szret = __m_pQueRead->PopData(pdata,szlen,szcontlen);

		__m_mutRead.Unlock();

		return szret;
	}

	void CDoubleQueue::SetPopWait(bool bpopwait/* = true*/)
	{
		__m_bWait = bpopwait;
	}

	size_t CDoubleQueue::GetUnitSize()
	{
		return __m_queRead.GetUnitSize();
	}

	size_t CDoubleQueue::GetLength()
	{
		size_t szret = 0;

		__m_mutLock.Lock();

		szret = __m_queRead.GetLength() + __m_queWrite.GetLength();

		__m_mutLock.Unlock();

		return szret;
	}

	size_t CDoubleQueue::GetSize()
	{
		size_t szret = 0;

		szret = __m_queRead.GetSize() + __m_queWrite.GetSize();

		return szret;
	}

	size_t CDoubleQueue::GetSingleSize()
	{		
		return __m_queRead.GetSize();
	}

	size_t CDoubleQueue::GetThrow()
	{
		return (__m_queRead.GetThrow() + __m_queWrite.GetThrow());
	}

	size_t CDoubleQueue::GetWaterLevel()
	{
		return __m_szLevel;
	}

}
