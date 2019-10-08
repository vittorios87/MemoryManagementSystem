#include "PagedAllocator.h"



void PagedAllocator::Init(unsigned long pageSizeInit)
{
	pageSize = pageSizeInit == 0 ? 65536 : 262144;
	firstFree = NULL;
}

void* PagedAllocator::Allocate(unsigned long size)
{
	if (!firstFree)
	{
		numAllocationsPerBatch = pageSize / size;
		//unsigned char* allocatedBatch = (unsigned char*)VirtualAlloc(NULL, pageSize, MEM_COMMIT, PAGE_READWRITE);
		unsigned char* allocatedBatch = (unsigned char*)malloc(pageSize);
		lastPntr = allocatedBatch;
		for (unsigned long i = 0; i < numAllocationsPerBatch; i++)
		{
			Deallocate(allocatedBatch);
			allocatedBatch += size;
		}
		firstPntr = allocatedBatch;
	}
	unsigned char* result = firstFree;
	firstFree = *((unsigned char**)firstFree);
	return result;

}

void PagedAllocator::Deallocate(void* pointer)
{
	*(unsigned char**)pointer = firstFree;
	firstFree = (unsigned char*)pointer;
}

bool PagedAllocator::IsThere(void* pointer)
{
	return (pointer >= lastPntr && pointer < firstPntr);
}


