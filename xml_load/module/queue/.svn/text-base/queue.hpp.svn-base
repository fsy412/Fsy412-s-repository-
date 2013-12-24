/*
* Copyright(C) 2013,Company ghca. All rights reserved.
* 
* @file    queue.hpp
* @brief   
* @version 1.0
* @author  zhangyawen
* @data    2013-3-8
*/

#ifndef __QUEUE_HPP
#define __QUEUE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <module/common/common.hpp>
#include <module/thread/mutex.hpp>
#include <module/thread/semaphore.hpp>
#include <module/thread/thread.hpp>

#ifndef DEL_P
#	define DEL_P(p)\
	if(p != NULL)\
	{\
	delete[] p;\
	p = NULL;\
}
#endif

namespace module{

	class CCircularQueue
	{
		typedef struct tagVal 
		{
			char* pData;
			size_t szLen;
		}DataVal,*pDataVal;

	public:

		CCircularQueue(): __m_pArray(NULL), __m_nCapacity(0), __m_nUnit(0), __m_nHead(0), __m_nTail(0), __m_nThrow(0)
		{}

		~CCircularQueue()
		{
			__Free();
		}

	public:

		bool IsFull()
		{
			int offset = (__m_nTail + 1) % (__m_nCapacity + 1);

			return (offset == __m_nHead);
		}

		bool IsEmpty()
		{
			return (__m_nTail == __m_nHead);
		}

		int Assgin(int ncapacity,int nbuf)
		{
			__m_nCapacity = ncapacity;

			__m_nUnit = nbuf;

			__m_pArray = new DataVal[__m_nCapacity + 1];

			for (int i = 0;i < __m_nCapacity + 1;i++)
			{
				__m_pArray[i].pData = new char[__m_nUnit];

				__m_pArray[i].szLen = __m_nUnit;
			}

			return 0;
		}

		int PushData(const void* pdata,size_t szlen,size_t &szconlen)
		{
			if (IsFull())
			{
				if (__Throw() == -1)
				{
					szconlen = 0;
					return -1;
				}
			}

			memcpy(__m_pArray[__m_nTail].pData,pdata,szlen);

			__m_pArray[__m_nTail].szLen = szlen;

			__m_nTail = (__m_nTail + 1) % (__m_nCapacity + 1);

			szconlen = (__m_nTail + 1 + __m_nCapacity - __m_nHead) % (__m_nCapacity + 1);

			return 0;
		}

		size_t PopData(void* pdata,size_t szlen,size_t& szconlen)
		{
			if (IsEmpty())
			{
				szconlen = 0;

				return 0;
			}

			int index = __m_nHead;

			__m_nHead = (__m_nHead + 1) % (__m_nCapacity + 1);

			size_t szcopy = __m_pArray[index].szLen > szlen ? szlen : __m_pArray[index].szLen;

			memcpy(pdata,__m_pArray[index].pData,szcopy);

			szconlen = (__m_nTail + 1 + __m_nCapacity - __m_nHead) % (__m_nCapacity + 1);

			return szcopy;	
		}

		size_t GetUnitSize()
		{
			return __m_nUnit;
		}

		size_t GetLength()
		{
			return (__m_nTail + 1 + __m_nCapacity - __m_nHead) % (__m_nCapacity + 1);
		}

		size_t GetSize()
		{
			return __m_nCapacity;
		}

		size_t GetThrow()
		{
			return __m_nThrow;
		}

	private:

		void __Free()
		{
			for (int i = 0;i < __m_nCapacity;i++)
			{
				DEL_P(__m_pArray[i].pData);
			}

			DEL_P(__m_pArray);

			__m_nCapacity = __m_nHead = __m_nTail = 0;
		}

		int __Throw()
		{
			if(!__m_nCapacity)
				return -1;

			__m_nHead = (__m_nHead + 1) % (__m_nCapacity + 1);

			__m_nThrow ++;

			return 0;
		}

	private:

		pDataVal __m_pArray;    

	private:

		volatile int __m_nCapacity,__m_nUnit;

		volatile int __m_nHead,__m_nTail;

		volatile int __m_nThrow;
	};

	class CSingleQueue
	{
	public:

		CSingleQueue();

		~CSingleQueue();

	public:

		int Assgin(size_t szsize,size_t szbuf,bool bpopwait = true);

		int PushData(const void* pdata,size_t szlen,size_t &szcontlen);

		size_t PopData(void* pdata,size_t szlen,size_t &szcontlen);

		void SetPopWait(bool bpopwait = true);

		size_t GetUnitSize();

		size_t GetLength();

		size_t GetSize();

		size_t GetThrow();

		size_t GetWaterLevel();

	private:

		CCircularQueue __m_queBuffer;

	private:

		CMutex __m_mutLock;

	private:

		CSemaphore __m_semDate;

	private:

		int __m_nSeed;

	private:

		volatile bool __m_bWait;

	private:	

		volatile size_t __m_szLevel;
	};

	class CDoubleQueue:
		public CThread
	{
	public:

		CDoubleQueue();

		~CDoubleQueue();

	public:

		void Flush()
		{
			__m_semRead.Post();
			__m_semWrite.Post();
		}

	public:

		int Assgin(size_t szsize,size_t szbuf,bool bpopwait = true,int ntime = 10);

		int PushData(const void* pdata,size_t szlen,size_t &szcontlen);

		size_t PopData(void* pdata,size_t szlen,size_t &szcontlen);

		void SetPopWait(bool bpopwait = true);

		size_t GetUnitSize();

		size_t GetLength();

		size_t GetSize();

		size_t GetSingleSize();

		size_t GetThrow();

		size_t GetWaterLevel();

	private:

		CCircularQueue* __SwapQueue(CCircularQueue* p)
		{
			if((p != &__m_queRead) && (p != &__m_queWrite))
			{
				return 0;
			}
			return (p == &__m_queRead ? &__m_queWrite : &__m_queRead);
		}

		int DoRunning();

	private:

		CCircularQueue __m_queRead,__m_queWrite;

		CCircularQueue *__m_pQueRead,*__m_pQueWrite;

	private:

		CSemaphore __m_semRead,__m_semWrite;

	private:

		CMutex __m_mutRead,__m_mutWrite;

		CMutex __m_mutLock;

	private:

		volatile bool __m_bWait;

		volatile bool __m_bQuitSwap,__m_bReadSwap,__m_bWriteSwap;

	private:

		volatile int __m_nTime;

	private:	

		volatile size_t __m_szLevel;
	};

}

#endif	//__QUEUE_HPP
