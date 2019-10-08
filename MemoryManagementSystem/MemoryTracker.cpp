#include "MemoryTracker.h"


using namespace MemoryTracker;

BlockHeader* BlockHeader::firstNode = NULL;

BlockHeader::BlockHeader(size_t requestedSize, char allocType)
{
	myPrevNode = NULL;
	myNextNode = NULL;
	myRequestedSize = requestedSize;
	myFileName = "[unknown]";
	myLineNum = 0;
	myTypeName = "[unknown]";
	myAllocType = allocType;
}
BlockHeader::~BlockHeader() 
{

}
void BlockHeader::Stamp(void* ptrToTrackedMemory, char const* fileName, int lineNum, char const* typeName)
{
	myFileName = fileName;
	myLineNum = lineNum;
	myTypeName = typeName;
	myPtrToTrackedMemory = ptrToTrackedMemory;
}
void BlockHeader::AddNode(BlockHeader* node)
{
	assert(node != NULL);
	assert(node->myPrevNode == NULL);
	assert(node->myNextNode == NULL);

	if (firstNode != NULL)
	{
		assert(firstNode->myPrevNode == NULL);
		firstNode->myPrevNode = node;
	}
	node->myNextNode = firstNode;
	firstNode = node;
}
const BlockHeader* BlockHeader::FindNode(void* pntrToFind)
{
	BlockHeader* currNode = firstNode;
	bool found = false;
	while (currNode != NULL)
	{
		if (currNode->myPtrToTrackedMemory == pntrToFind)
		{
			found = true;
			break;
		}
		currNode = currNode->myNextNode;		
	}
	if (found)
	{
		return currNode;
	}
	return NULL;
}

void BlockHeader::RemoveNode(BlockHeader* node)
{
	assert(node != NULL);
	assert(firstNode != NULL);
	if (firstNode == node)
	{
		assert(firstNode->myPrevNode == NULL);
		firstNode = node->myNextNode;
	}
	if (node->myPrevNode != NULL)
	{
		node->myPrevNode->myNextNode = node->myNextNode;
	}
	if (node->myNextNode != NULL)
	{
		node->myNextNode->myPrevNode = node->myPrevNode;
	}
	node->myPrevNode = NULL;
	node->myNextNode = NULL;
}
size_t BlockHeader::CountBlocks() 
{
	size_t count = 0;
	BlockHeader* currNode=firstNode;
	while (currNode != NULL)
	{
		++count;
		currNode = currNode->myNextNode;
	}
	return count;
}
void BlockHeader::GetBlocks(BlockHeader** ppBlockHeader) 
{
	BlockHeader* currNode = firstNode;
	while (currNode != NULL)
	{
		*ppBlockHeader = currNode;
		++ppBlockHeader;
		currNode = currNode->myNextNode;
	}
}

