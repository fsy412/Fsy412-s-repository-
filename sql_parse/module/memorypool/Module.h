#pragma once
#include <queue> 
#include <MemoryPool.h>

class CModule
{
public:
	CModule(void){}
	~CModule(void){}
	void Push(MemoryBlock* pMemoryBlock);
	void Pop();

private:
	std::queue<MemoryBlock*> _memBlockQueue;
};
