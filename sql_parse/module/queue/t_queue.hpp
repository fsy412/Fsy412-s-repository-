/*
* Copyright(C) 2013,Company ghca. All rights reserved.
* 
* @file    t_queue.hpp
* @brief   
* @version 1.0
* @author  zhangyawen
* @data    2013-3-8
*/

#ifndef __T_QUEUE_HPP
#define __T_QUEUE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <module/common/common.hpp>
#include <module/thread/mutex.hpp>
#include <module/thread/semaphore.hpp>
#include <module/thread/thread.hpp>

namespace module{

	template <typename T>
	class CTCircularQueue
	{
	public:

		CTCircularQueue(): __m_pData(NULL), __m_nCapacity(0), __m_nHead(0), __m_nTail(0), __m_nThrow(0)
		{}

		virtual ~CTCircularQueue() throw()
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

		int Assgin(int ncapacity)
		{
			__Free();

			__m_pData = new T[ncapacity];

			__m_nCapacity = ncapacity;

			return 0;
		}

		int PushData(const T data,size_t &szconlen)
		{
			if (IsFull())
			{
				if (__Throw() == -1)
				{
					szconlen = 0;
					return -1;
				}
			}

			__m_pData[__m_nTail] = data;

			__m_nTail = (__m_nTail + 1) % (__m_nCapacity + 1);

			szconlen = (__m_nTail + 1 + __m_nCapacity - __m_nHead) % (__m_nCapacity + 1);

			return 0;
		}

		int PopData(T &data,size_t &szconlen)
		{
			if (IsEmpty())
			{
				szconlen = 0;
				return -1;
			}

			data = __m_pData[__m_nHead];

			__m_nHead = (__m_nHead + 1) % (__m_nCapacity + 1);

			szconlen = (__m_nTail + 1 + __m_nCapacity - __m_nHead) % (__m_nCapacity + 1);

			return 0;	
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
			if (__m_pData != NULL)
				delete[] __m_pData;

			__m_pData = NULL;

			__m_nCapacity = __m_nHead = __m_nTail = 0;
		}

		int __Throw()
		{
			if (!__m_nCapacity)
				return -1;

			__m_nHead = (__m_nHead + 1) % (__m_nCapacity + 1);

			__m_nThrow ++;

			return 0;
		}

	private:

		T *__m_pData;

	private:

		volatile int __m_nCapacity;

		volatile int __m_nHead,__m_nTail;

		volatile int __m_nThrow;

	};

	template <typename T>
	class CTSingleQueue
	{
	public:

		CTSingleQueue():__m_szLevel(0),__m_nSeed(0),__m_bWait(true)
		{}

		~CTSingleQueue()
		{}

	public:

		int Assgin(size_t szsize,bool bpopwait = true)
		{
			__m_bWait = bpopwait;

			return __m_queBuffer.Assgin(szsize);
		}

		int PushData(const T data,size_t &szcontlen)
		{
			int nret = 0;

			__m_mutLock.Lock();

			nret = __m_queBuffer.PushData(data,szcontlen);

			if (szcontlen == 1 && __m_nSeed < 0)
			{
				__m_nSeed++;
				__m_semData.Post();
			}

			__m_szLevel = szcontlen > __m_szLevel ? szcontlen : __m_szLevel;

			__m_mutLock.Unlock();

			return nret;
		}

		int PopData(T &data,size_t &szcontlen)
		{
			int nret = 0;

			while(true)
			{
				__m_mutLock.Lock();

				nret = __m_queBuffer.PopData(data,szcontlen);

				// 数据为空
				if (__m_bWait && nret == -1 && szcontlen == 0)
				{
					__m_nSeed--;

					__m_mutLock.Unlock();

					__m_semData.Wait();
				}
				else 
					break;
			} 

			__m_mutLock.Unlock();

			return nret;
		}

		void SetPopWait(bool bpopwait = true)
		{
			__m_bWait = bpopwait;
		}

		size_t GetLength()
		{
			size_t szret = 0;

			__m_mutLock.Lock();

			szret = __m_queBuffer.GetLength();

			__m_mutLock.Unlock();

			return szret;
		}

		size_t GetSize()
		{
			return __m_queBuffer.GetSize();
		}

		size_t GetThrow()
		{
			return __m_queBuffer.GetThrow();
		}

		size_t GetWaterLevel()
		{
			return __m_szLevel;
		}

	private:

		CTCircularQueue<T> __m_queBuffer;

	private:

		CMutex __m_mutLock;

	private:

		CSemaphore __m_semData;

	private:

		int __m_nSeed;

	private:

		volatile bool __m_bWait;

	private:	

		volatile size_t __m_szLevel;
	};

	template<typename T>
	class CTDoubleQueue:
		public CThread
	{
	public:

		CTDoubleQueue():
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

		  ~CTDoubleQueue()
		  {
			  __m_bQuitSwap = true;
		  }


	public:

		void Flush()
		{
			__m_semRead.Post();
			__m_semWrite.Post();
		}

		int Assgin(size_t szsize,bool bpopwait = true,int ntime = 10)
		{
			__m_bQuitSwap = true;

			if (__m_queRead.Assgin(szsize) == -1)
			{
				return -1;
			}

			if (__m_queWrite.Assgin(szsize) == -1)
			{
				return -1;
			}

			__m_nTime = ntime;

			__m_bQuitSwap = false;

			__m_bWait = bpopwait;

			return this->CreateThread();
		}

		int PushData(const T data,size_t &szcontlen)
		{
			static CTCircularQueue<T>* pQueRead = __m_pQueRead;

			__m_mutexWrite.Lock();

			if (__m_pQueWrite->IsFull())
			{
				__m_semWrite.Post();

				pQueRead = __SwapQueue(__m_pQueWrite);

				__m_bWriteSwap = true;

				__m_pQueWrite = pQueRead;

				__m_semRead.Wait();

				__m_bWriteSwap = false;
			}

			int nret = __m_pQueWrite->PushData(data,szcontlen);

			__m_szLevel = szcontlen > __m_szLevel ? szcontlen : __m_szLevel;

			__m_mutexWrite.Unlock();

			return nret;
		}

		int PopData(T &data,size_t &szcontlen)
		{
			static CTCircularQueue<T>* pQueWrite = __m_pQueWrite;

			__m_mutexRead.Lock();

			if (__m_pQueRead->IsEmpty())
			{
				__m_semRead.Post();

				pQueWrite = __SwapQueue(__m_pQueRead);

				__m_bReadSwap = true;

				__m_pQueRead = pQueWrite;

				__m_semWrite.Wait();

				__m_bReadSwap = false;
			}

			int nret = __m_pQueRead->PopData(data,szcontlen);

			__m_mutexRead.Unlock();

			return nret;
		}

		void SetPopWait(bool bpopwait = true)
		{
			__m_bWait = bpopwait;
		}

		size_t GetLength()
		{
			size_t szret = 0;

			__m_mutLock.Lock();

			szret = __m_queRead.GetLength() + __m_queWrite.GetLength();

			__m_mutLock.Unlock();

			return szret;
		}

		size_t GetSize()
		{
			return (__m_queRead.GetSize() + __m_queWrite.GetSize());
		}

		size_t GetThrow()
		{		
			return (__m_queRead.GetThrow() + __m_queWrite.GetThrow());
		}

		size_t GetWaterLevel()
		{
			return __m_szLevel;
		}

	private:

		CTCircularQueue<T>* __SwapQueue(CTCircularQueue<T>* p)
		{
			if ((p != &__m_queRead) && (p != &__m_queWrite))
			{
				return 0;
			}
			return (p == &__m_queRead ? &__m_queWrite : &__m_queRead);
		}

		int DoRunning()
		{
			CTCircularQueue<T>* pQue = NULL;

			while(!__m_bQuitSwap)
			{
				module::Sleep(__m_nTime);

				// 人工切换
				if (__m_bReadSwap&&__m_bWriteSwap)
				{}
				else if (__m_bReadSwap)
				{	
					__m_mutexWrite.Lock();

					if (!__m_bWriteSwap && (!__m_pQueWrite->IsEmpty() || !__m_bWait))
					{
						pQue = __SwapQueue(__m_pQueWrite);

						__m_pQueWrite = pQue;

						__m_semWrite.Post();
					}

					__m_mutexWrite.Unlock();
				}
				else if (__m_bWriteSwap)
				{
					__m_mutexRead.Lock();

					if (!__m_bReadSwap && !__m_pQueRead->IsFull())
					{
						pQue = __SwapQueue(__m_pQueRead);

						__m_pQueRead = pQue;

						__m_semRead.Post();
					}

					__m_mutexRead.Unlock();
				}

			}

			return 0;
		}

	private:

		CTCircularQueue<T> __m_queRead,__m_queWrite;

		CTCircularQueue<T> *__m_pQueRead,*__m_pQueWrite;

	private:

		CSemaphore __m_semRead,__m_semWrite;

	private:

		CMutex __m_mutexRead,__m_mutexWrite;

		CMutex __m_mutLock;

	private:

		volatile bool __m_bWait;

		volatile bool __m_bQuitSwap,__m_bReadSwap,__m_bWriteSwap;

		volatile int __m_nTime;

	private:	

		volatile size_t __m_szLevel;
	};
}


#endif	// __T_QUEUE_HPP