#pragma once
//#include <windows.h>
//#include "memoryapi.h"
#include <assert.h>
#include <vector>
//#include <algorithm>
struct SOAChunk
{
	void Init(unsigned char blkSize, unsigned char blks);
	void* Allocate(unsigned int blkSize);
	void Deallocate(void* ptr, unsigned int blkSize);
	unsigned char* ptrData_;
	unsigned char* ptrLastChar_;
	unsigned char firstAvailBlk_, blksAvail_;

	inline bool operator==( const SOAChunk& rhs) 
	{
		return ( (this->ptrData_==rhs.ptrData_));   // && (this->firstAvailBlk_ == rhs.firstAvailBlk_) && (this->blksAvail_ == rhs.blksAvail_) 
	}
};

class SOAFixedAllocator
{
	unsigned char blkSize_;  //1byte
	unsigned char numBlks_; //1byte
	unsigned char numChunks_; //1byte

	//typedef std::vector<SOAChunk*> SOAChunks;
	SOAChunk* Chunks_;  //4byte
	SOAChunk* allocChunk_;  //4byte
	//SOAChunk* deallocChunk_; //4byte

	unsigned char deallocChunkIndex_;  //1byte
	bool chunkToDeallocateFromFound_; //1byte

public:
	void Init(unsigned char numChunks =0,unsigned char blockSize = 0, unsigned char numBlocks = 0);
	void* Allocate();
	void Deallocate(void* ptr);
	bool IsThere(void* pointer);
	unsigned long GetSize();

};


class SmallObjectAllocator
{
	unsigned char maxBlkSize_;  //1byte
	unsigned short chunkSize_; //2byte

public:
	SOAFixedAllocator* FixedAllocators;  //4byte
	//SOAFixedAllocs FixedAllocators;

	void Init(unsigned char maxBlockSize = 0, unsigned short chunkSize =0);


};

