#pragma once
#include "typedef.h"
#include <queue>
#include <list>
#include "MemoryBlock.h"
#include <semaphore.hpp>
#include <log.hpp>
#include <Timer.h>

extern module::CLog *log;

#define SUCCESS 0
#define FAILED -1
struct MemoryBlock;

class CMemoryPool:public module::CThread
{
public:
	~CMemoryPool(void);
	
	GHCA_INT32 Init(GHCA_INT32 size , GHCA_INT32 blocksize);
	MemoryBlock* Allocate();
	GHCA_INT32 Free(MemoryBlock* pBlock);
	int DoRunning();
	static CMemoryPool *GetInstance()
	{
		static CMemoryPool instance;
		return &instance;
	}

private:
	CMemoryPool(void){}
	void ForceFree();
	std::queue<MemoryBlock*> _free_memory;
	std::list<MemoryBlock*> _allocated_memory;
 
	module::CSemaphore _sem;
	Timer _timer;
};
