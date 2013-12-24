#pragma once
#include "typedef.h"
#include "MemoryPool.h"
#include "AppConfig.h"

struct MemoryBlock
{
	GHCA_INT32 _count;
	GHCA_INT32 _len;
	GHCA_INT32 _blocksize;
	GHCA_INT8 *_buf;

	MemoryBlock(){}
	~MemoryBlock(){	delete _buf;}
};
