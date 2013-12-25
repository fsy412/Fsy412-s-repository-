#include "MemoryPool.h"
#include <semaphore.hpp>

CMemoryPool::~CMemoryPool(void)
{
	std::list<MemoryBlock*>::iterator it = _allocated_memory.begin();
	for (;it != _allocated_memory.end(); ++it)
	{
		delete *it;
	} 
}

MemoryBlock* CMemoryPool::Allocate()
{
 	while(_free_memory.empty())
	{
		log->PushLog(DEBUG,"CMemoryPool::Allocate() not enough memory ,waiting");
		_timer.restart();
		_sem.Wait();
		log->PushLog(DEBUG,"CMemoryPool::Allocate()  memory pool ok");
	}
	MemoryBlock* pBlock = _free_memory.front();
	log->PushLog(DEBUG,"CMemoryPool::Allocate() allocate memory %x", pBlock);
	_free_memory.pop();
	return pBlock;
}

GHCA_INT32 CMemoryPool::Free( MemoryBlock* pBlock )
{
	if (pBlock == NULL)
	{
		log->PushLog(ERROR,"CMemoryPool::Free pBlock is NULL");
		return FAILED;
	}
	else
	{	
		if (--pBlock->_count == 0)
		{
 			log->PushLog(DEBUG,"CMemoryPool::Free() free memory %x",pBlock);
			pBlock->_count = 3;
			_free_memory.push(pBlock);
			_sem.Post();
		}
		return 0;
	}
	return SUCCESS;
}

GHCA_INT32 CMemoryPool::Init( GHCA_INT32 size , GHCA_INT32 blocksize )
{
	log->PushLog(DEBUG,"CMemoryPool::Init() initialise pool size is %d ", size);
	for(GHCA_INT32 i=0;i<size;i++)
	{
		MemoryBlock* pBlock = new MemoryBlock();
		if (pBlock== NULL)
			return FAILED;

		pBlock->_count = 3; 
		pBlock->_blocksize = blocksize;
		pBlock->_buf = new GHCA_INT8[blocksize];

		_free_memory.push(pBlock);
		_allocated_memory.push_back(pBlock);
	}
	
	CreateThread();
	_timer.set_interval(2);
	return SUCCESS;
}

int CMemoryPool::DoRunning()
{
	while(1)
	{
		if(_timer.isRun()) 
		{ 
			if (_timer.timeout())
			{
				ForceFree();
				_sem.Post();
			}
		}
	}
	return 0;
}

void CMemoryPool::ForceFree()
{
	log->PushLog(DEBUG,"CMemoryPool::ForceFree() free pool memory");
	std::list<MemoryBlock*>::iterator it = _allocated_memory.begin();
	for (;it != _allocated_memory.end(); ++it)
	{
		(*it)->_count = 3;
		_free_memory.push (*it);
 	} 
}
 