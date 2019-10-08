#pragma once
#include<corecrt_malloc.h>
#include <windows.h>
#include <iostream>
#include "SmallObjectAllocator.h"
#include "PagedAllocator.h"
#include "MemoryTracker.h"


using namespace std;
using namespace MemoryTracker;
//class MemoryManager
//{


	const unsigned short numBins{ 10 };
	const unsigned short numPages{ 8 };
	static SmallObjectAllocator* SOAllocator= (SmallObjectAllocator*)malloc(sizeof(SmallObjectAllocator));
	static PagedAllocator PAllocators[8];
	static PagedAllocator TrackerPAllocator;
	inline void Init() 
	{
		SOAllocator->Init(16, 0);
		for (unsigned short i = 0; i < numPages; ++i)
		{
			PAllocators[i].Init(0);
		}
		TrackerPAllocator.Init(1);
	}
// 	void* operator new (size_t size, void* pBlockHeader)
// 	{
// 		return pBlockHeader;
// 	}

	inline unsigned long GetClassMaxSize(unsigned short classBin)
	{
		return 1 << (classBin + 3); //number 1 is shifted by 3 position to the left = from 2^0 to 2^3 = 8
	}

	inline unsigned short GetSizeClass(unsigned long size)
	{
		if (size > GetClassMaxSize(numBins - 1))
		{
			return numBins;
		}
		unsigned long sizeToDestroy{ size };
		unsigned short c = 0;
		--sizeToDestroy;
		while (sizeToDestroy > 7)
		{
			sizeToDestroy >>= 1;
			++c;
		}
		return c;
	}
	inline unsigned long GetSize(void* pntr)
	{
		unsigned short i;
		for (i = 0; i < 8; i++)
		{
			if (PAllocators[i].IsThere(pntr))
			{
				return GetClassMaxSize(i + 2);
			}
		}
		for (i = 0; i < 16; i++)
		{
			if (SOAllocator->FixedAllocators[i].IsThere(pntr))
			{
				return SOAllocator->FixedAllocators[i].GetSize();
			}
		}
		return 4097;
	}

	inline void* MM_Malloc_Tracker(size_t size)
	{
		return TrackerPAllocator.Allocate(size);
	}
	inline void MM_Free_Tracker(void* pntr)
	{
		TrackerPAllocator.Deallocate(pntr);
		return;
	}

	inline void* MM_Malloc(unsigned long size)
	{
		char allocType;
		void* ptrToReturn;
		void* pBlHeader =MM_Malloc_Tracker(sizeof(BlockHeader));


		unsigned short sizeClass = GetSizeClass(size);
		if (sizeClass >= numBins)
		{
			allocType = 'G';
			ptrToReturn = malloc(size);
		}
		else if (sizeClass == 0 || sizeClass == 1)
		{
			void* ptr = SOAllocator->FixedAllocators[size - 1].Allocate();
			if (ptr == NULL)
			{
				allocType = 'G';
				ptrToReturn= malloc(size);
			}
			else
			{
				allocType = 'S';
				ptrToReturn = ptr;
			}
		}
		else
		{
			allocType = 'P';
			ptrToReturn = PAllocators[sizeClass - 2].Allocate(GetClassMaxSize(sizeClass));

		}
		assert(ptrToReturn != NULL);
		pBlHeader = new(pBlHeader) BlockHeader(size, allocType);
		BlockHeader::AddNode((BlockHeader*)pBlHeader);
		return ptrToReturn;
	}

	inline void MM_Free(void* pntr)
	{
		const BlockHeader* headerPntr=BlockHeader::FindNode(pntr);
		if (headerPntr)
		{
			BlockHeader::RemoveNode(const_cast<BlockHeader*>(headerPntr));
			MM_Free_Tracker(const_cast<BlockHeader*>(headerPntr));
		}
		unsigned short i;
		for (i = 0; i < 8; i++)
		{
			if (PAllocators[i].IsThere(pntr))
			{
				PAllocators[i].Deallocate(pntr);
				return;
			}
		}
		for (i = 0; i < 16; i++)
		{
			if (SOAllocator->FixedAllocators[i].IsThere(pntr))
			{
				SOAllocator->FixedAllocators[i].Deallocate(pntr);
				return;
			}
		}

		free(pntr);
		return;

	}
	inline void* MM_Realloc(void* pntr, unsigned long size)
	{
		if (pntr)
		{
			if (size) 
			{
				unsigned long objSize = GetSize(pntr);
				unsigned short objSizeClass = GetSizeClass(objSize);
				if (objSizeClass >= numBins)
				{
					return realloc(pntr, size);

				}
				if(size > objSize || size < objSize/2)
				{ 
					void * newPntr = MM_Malloc(size);
					if (newPntr)
					{
						unsigned long toCopySize = min(size, objSize);
						memcpy(newPntr, pntr, toCopySize);
					}
					MM_Free(pntr);
					return newPntr;
				}
				return pntr;
			}
			MM_Free(pntr);
			return NULL;
		}
		return MM_Malloc(size);
	}

	inline void* MM_New(unsigned long size)
	{
		cout << "memory manager 'new' call: size " << size << endl;
		return MM_Malloc(size);
	}
	inline void* MM_New_A (unsigned long size)
	{
		cout << "memory manager 'array new' call: size " << size << endl;
		return MM_Malloc(size);

	}

	inline void MM_Delete(void* pntr)
	{
		cout << "memory manager 'delete' call: pointer " << pntr << endl;
		MM_Free(pntr);
		return;
	}
	inline void MM_Delete_A(void* pntr)
	{
		cout << "memory manager 'array delete' call: pointer " << pntr << endl;
		MM_Free(pntr);
		return;

	}
//};





