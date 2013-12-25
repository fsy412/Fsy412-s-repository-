/*
* Copyright(C) 2013,Company ghca. All rights reserved.
* 
* @file    stl_queue.hpp
* @brief   简单的模板队列类，预先分配大小，若不预先分配，用std的,函数导出名字仿照std
* @version 1.0
* @author  zhangyawen
* @data    2013-3-8
*/

#ifndef __STL_QUEUE_HPP
#define __STL_QUEUE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <exception>
#include <stdexcept>  

namespace module{namespace stl{

	template <typename T> 
	class queue 
	{ 
	public: 
		queue():__m_szReadPoint(0),__m_szWritePoint(0),__m_bFull(false),__m_pData(NULL)
		{}  

		queue(size_t capacity):__m_szReadPoint(0),__m_szWritePoint(0),__m_bFull(false),__m_pData(NULL)
		{
			__m_szCapacity = capacity; 

			__m_pData = new T[__m_szCapacity];
			
			if(__m_pData == NULL) 
			{
				throw(std::exception());
			}
		}

		virtual ~queue() throw() 
		{ 
			__free();
		};  


		T& front() 
		{ 
			try
			{
				if (empty()) 
				{ 
					throw("queue::front() : the buffer is empty"); 
				}  
			}
			catch (const char *p)
			{
				std::cout << p << std::endl;
			}

			return __m_pData[__m_szReadPoint]; 
		} 

		T& back() 
		{ 
			try
			{
				if (empty()) 
				{ 
					throw("queue::back() : the buffer is empty"); 
				} 
			}
			catch (const char *p)
			{
				std::cout << p << std::endl;
			}

			return __m_pData[(__m_szWritePoint - 1 + __m_szCapacity) % __m_szCapacity]; 
		}  

		bool full()
		{
			return __m_bFull;
		}

		bool empty() 
		{ 
			return ((__m_szReadPoint == __m_szWritePoint) && (!__m_bFull)); 
		}  

		void push(const T data) 
		{ 
			try
			{
				if (__m_bFull) 
				{ 
					throw("queue::push() : the buffer is full!");
				}

				__m_pData[__m_szWritePoint] = data;
				
				__m_szWritePoint = (__m_szWritePoint + 1) % __m_szCapacity; 
				
				if(__m_szWritePoint == __m_szReadPoint) 
				{ 
					__m_bFull = true; 
				} 
			}
			catch(const char* p)
			{
				std::cout << p << std::endl;
			}
		}

		void pop() 
		{ 
			try
			{
				if (empty()) 
				{ 
					throw("queue::pop() : the buffer is empty!");
				} 
				
				__m_szReadPoint = (__m_szReadPoint + 1)%__m_szCapacity;
				
				__m_bFull = false; 
			}
			catch(const char* p)
			{
				std::cout << p << std::endl;
			}
		}  

		void assign(size_t capacity)
		{
			__free();

			__m_szCapacity = capacity; 
			
			__m_pData = new T[__m_szCapacity];

			if(__m_pData == NULL) 
			{ 
				throw(std::exception());
			}
		}

		size_t size() 
		{ 
			if(!__m_bFull) 
			{ 
				return (__m_szWritePoint - __m_szReadPoint + __m_szCapacity) % __m_szCapacity; 
			} 
			
			return __m_szCapacity; 
		}

	private:

		void __free()
		{
			if(__m_pData) 
			{ 
				delete[] __m_pData;
				__m_pData = NULL; 
			} 
		}

	private: 

		T* __m_pData; 

	private:

		bool __m_bFull; 

	private:

		size_t __m_szCapacity;

		size_t __m_szReadPoint,__m_szWritePoint; 
	};

}}

#endif	//__STL_QUEUE_HPP
