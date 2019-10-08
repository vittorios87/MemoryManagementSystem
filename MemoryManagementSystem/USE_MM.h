#pragma once
#include<corecrt_malloc.h>


/*operator new, new[], delete, delete[]*/

#ifdef USE_MM_POWAAAA


inline void* operator new(size_t Size)
{
	return MM_NEW(size);
}
#endif

