#pragma once
#include <assert.h>
#include<corecrt_malloc.h>
#include <iostream>

#ifndef MemTrack_H_
#define MemTrack_H_

using namespace std;
namespace MemoryTracker
{
	class BlockHeader
	{
	private:
		static BlockHeader* firstNode;
	private:
		BlockHeader* myPrevNode;
		BlockHeader* myNextNode;
		size_t myRequestedSize;
		int myLineNum;
		char const* myFileName;
		char const* myTypeName;
		char myAllocType; // S = small allocator, P = paged allocator, G = general allocator
		void* myPtrToTrackedMemory;
	public:
		BlockHeader(size_t requestedSize, char allocType);
		~BlockHeader();

		//getters
		size_t GetRequestedSize() const { return myRequestedSize; }
		char const * GetFileName() const { return myFileName; }
		int GetLineNum() const { return myLineNum; }
		char const * GetTypeName() const { return myTypeName; }
		char GetAllocType() const { return myAllocType; }
		
		static BlockHeader* GetFirstNodePtr() { return firstNode; }

		void Stamp(void* ptrToTrackedMemory, char const* fileName, int lineNum, char const* typeName);

		static void AddNode(BlockHeader* node);
		static void RemoveNode(BlockHeader* node);
		static const BlockHeader* FindNode(void* pntrToFind);
		static size_t CountBlocks();
		static void GetBlocks(BlockHeader** ppBlockHeader);

		

	};



	class MemStamp
	{
	public:		// member variables
		char const * const filename;
		int const lineNum;
	public:		// construction/destruction
		MemStamp(char const *filename, int lineNum)
			: filename(filename), lineNum(lineNum) { }
		~MemStamp() { }

		
	};
	
	static void TrackStamp(const MemStamp& stampToTrack, const char* typeName, void* ptrToTrackedMemory)	{
		BlockHeader* firstNodePtr = BlockHeader::GetFirstNodePtr();
		assert(firstNodePtr != NULL);
		firstNodePtr->Stamp(ptrToTrackedMemory, stampToTrack.filename, stampToTrack.lineNum, typeName);
		return;
	}

	static void PrintMemoryUsage()
	{
		size_t numBlocks = BlockHeader::CountBlocks();
		BlockHeader** ppBlockHeader = (BlockHeader**)calloc(numBlocks, sizeof(BlockHeader*));
		BlockHeader::GetBlocks(ppBlockHeader);
		cout << endl << endl << "Allocated blocks" << endl<<endl;
		cout << "type name" << "\t" << "req size" << "\t" << "allocator" <<"\t"<<"file name" << "\t\t\t\t"<<"line num" << endl << endl;
		for (size_t i = 0; i < numBlocks; i++)
		{
			BlockHeader* pBlockHeader = ppBlockHeader[i];
			char const* typeName= pBlockHeader->GetTypeName();
			size_t const size = pBlockHeader->GetRequestedSize();
			char const allocType = pBlockHeader->GetAllocType();
			char const* fileName = pBlockHeader->GetFileName();
			int const lineNum = pBlockHeader->GetLineNum();

			cout << typeName << "\t" << size << "\t" << allocType << "\t" << fileName << "\t"<< lineNum << endl ;

		}
		cout << endl << endl;
		free(ppBlockHeader);
	}
	
}

template <typename T> inline T* operator*(const MemoryTracker::MemStamp& stamp, T *p)
{
	MemoryTracker::TrackStamp(stamp, typeid(T).name(), p);
	//std::cout << p << "  " << stamp.filename << "  " << stamp.lineNum << "  " << typeid(T).name() << std::endl;
	return p;
}



#endif  
