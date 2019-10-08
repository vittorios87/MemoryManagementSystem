#pragma once
#include<corecrt_malloc.h>
//#include <windows.h>
//#include "memoryapi.h"

class PagedAllocator
{
	unsigned char* firstFree;

	unsigned char* lastPntr;
	unsigned char* firstPntr;


	unsigned long pageSize = 65536;
	unsigned long numAllocationsPerBatch;
public:
	void Init(unsigned long pageSizeInit=0);

	void* Allocate(unsigned long size);
	void Deallocate(void* pointer);

	bool IsThere(void* pointer);

};

