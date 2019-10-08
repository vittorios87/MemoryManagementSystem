#include "SmallObjectAllocator.h"



void SOAChunk::Init(unsigned char blkSize, unsigned char blks)
{
	//ptrData_ = (unsigned char*)VirtualAlloc(NULL, blkSize * blks, MEM_COMMIT, PAGE_READWRITE);
	ptrData_ = (unsigned char*)malloc(blkSize * blks);

	//new unsigned char[blkSize * blks];
	firstAvailBlk_ = 0;
	blksAvail_ = blks;
	ptrLastChar_ = ptrData_ + blkSize * blks;
	unsigned char i = 0;
	unsigned char* p = ptrData_;
	//the for cycle stores at the beginning of each block the number of the subsequent one
	// unsigned char requires only 1 byte
	for (; i != blks; p += blkSize)
	{
		*p = ++i;
	}

}

void* SOAChunk::Allocate(unsigned int blkSize)
{
	if (!blksAvail_) { return 0; };
	unsigned char* ptrResult = ptrData_ + (firstAvailBlk_* blkSize);
	firstAvailBlk_ = *ptrResult;
	--blksAvail_;
	return ptrResult;

}
void SOAChunk::Deallocate(void* ptr, unsigned int blkSize)
{
	assert(ptr >= ptrData_);
	unsigned char* toRelease = static_cast<unsigned char*>(ptr);
	assert((toRelease - ptrData_) % blkSize == 0);
	*toRelease = static_cast<unsigned char>(firstAvailBlk_);
	firstAvailBlk_ = static_cast<unsigned int>((toRelease - ptrData_) / blkSize);
	assert(firstAvailBlk_ == (toRelease - ptrData_) / blkSize); //checks if during static_cast operation the value was truncated
	++blksAvail_;
}


void SOAFixedAllocator::Init(unsigned char numChunks,unsigned char blockSize, unsigned char numBlocks)
{
	blkSize_= blockSize ? blockSize : 1;
	numBlks_= numBlocks ? numBlocks : 255;
	numChunks_ = numChunks ? numChunks : 50;
	Chunks_ = (SOAChunk*)calloc(numChunks_,sizeof(SOAChunk));
	for (unsigned char i = 0; i < numChunks_; ++i)
	{
		(Chunks_ + i)->Init(blkSize_, numBlks_);
	}
}
void * SOAFixedAllocator::Allocate() 
{
	unsigned char i = 0;
	if (allocChunk_ == 0 || allocChunk_->blksAvail_ == 0)
	{
		//SOAChunks::iterator it = Chunks_.begin();
		for (; i<numChunks_; ++i)
		{
			/*
			if ((Chunks_ + i) == 0)
			{
				//Chunks_.reserve(Chunks_.size() + 1);
				SOAChunk* newChunk = (SOAChunk*) malloc (sizeof(SOAChunk));
				newChunk->Init(blkSize_, numBlks_);
				*(Chunks_ + i)= newChunk;
				allocChunk_ = newChunk;
				deallocChunk_ = newChunk;
				break;
			}
			*/
			if ((Chunks_+i)->blksAvail_>0)
			{
				allocChunk_ = (Chunks_ + i);
				break;
			}
		}
		if (i == numChunks_)
		{
			return NULL;
		}
	}
	assert(i != numChunks_);
	assert(allocChunk_ != 0);
	assert(allocChunk_->blksAvail_ > 0);
	return allocChunk_->Allocate(blkSize_);
}
bool SOAFixedAllocator::IsThere(void* pointer)
{
	unsigned char* toRelease = static_cast<unsigned char*>(pointer);
	//unsigned short numChunks = Chunks_.size();
	chunkToDeallocateFromFound_ = false;
	deallocChunkIndex_ = numChunks_;
	for (unsigned char i = 0; i < numChunks_; ++i)
	{
		if (toRelease >= (Chunks_+ i )->ptrData_ && toRelease < (Chunks_+i )->ptrLastChar_)
		{
			deallocChunkIndex_ = i;
			chunkToDeallocateFromFound_ = true;
			return true;
		}
	}
	return false;
}
unsigned long SOAFixedAllocator::GetSize()
{
	return  blkSize_;
}


void SOAFixedAllocator::Deallocate(void* ptr)
{

	/**/
	if (chunkToDeallocateFromFound_) 
	{
		assert(deallocChunkIndex_ <numChunks_);

		unsigned char* toRelease = static_cast<unsigned char*>(ptr);
		(Chunks_+deallocChunkIndex_)->Deallocate(toRelease, blkSize_);
		chunkToDeallocateFromFound_ = false;
	}
	return;


}

void SmallObjectAllocator::Init(unsigned char maxBlockSize, unsigned short chunkSize) 
{
	maxBlkSize_= maxBlockSize ? maxBlockSize : 16;
	chunkSize_ = chunkSize;
	unsigned int i = 0;
	//FixedAllocators = std::vector<SOAFixedAllocator>(maxBlkSize_, std::allocator<SOAFixedAllocator>{});
	FixedAllocators = (SOAFixedAllocator*)calloc(16, sizeof(SOAFixedAllocator));
	while (i < maxBlkSize_)
	{
		(FixedAllocators+i)->Init(0, i + 1, (chunkSize_ ? (chunkSize_ / (i+1)) : 0));
		++i;
		//SOAFixedAllocator fixedAllocator;
		//fixedAllocator.Init(i, (unsigned char)(chunkSize_ ? (chunkSize_ / i) : 0));
		//FixedAllocators.push_back(fixedAllocator);
	}
}


