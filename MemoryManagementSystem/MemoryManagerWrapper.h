#pragma once
#ifdef USE_MMSYSTEM_VS
#include "MemoryManager.h"
#include <numeric>

//, const char* file, const unsigned long line
void* operator new (size_t size)
{
	return MM_New(size);
}


//, const char* file, const unsigned long line
void* operator new[] (size_t size)
{
	return MM_New_A(size);
}

#ifdef MemTrack_H_

#define MEMTRACK_NEW MemStamp(__FILE__, __LINE__)*new
#define new MEMTRACK_NEW

#endif

void operator delete(void* pntr)
{
	MM_Delete(pntr);
	return;
}

void operator delete[] (void* pntr)
{
	MM_Delete_A(pntr);
	return;
}


/*
void *operator new(size_t size, char const *filename, int lineNum)
{
	return ::operator new(size);
}
#define new  new(__FILE__, __LINE__)

#define new    MemStamp(__FILE__, __LINE__) * new
*/





//#define new [](SIZE)  new [](SIZE, __FILE__, __LINE__)
//#define delete(PTR)  delete(PTR, __FILE__, __LINE__)
//#define delete[](PTR)  delete[](PTR, __FILE__, __LINE__)


#endif