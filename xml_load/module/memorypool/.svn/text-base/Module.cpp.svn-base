#include "Module.h"
#include "MemoryPool.h"
 
void CModule::Push(MemoryBlock* pMemoryBlock)
{
	_memBlockQueue.push(pMemoryBlock);

}

void CModule::Pop()
{
	MemoryBlock* pMemoryBlock = _memBlockQueue.front();
	_memBlockQueue.pop();
	CMemoryPool :: GetInstance()->Free(pMemoryBlock);
}
