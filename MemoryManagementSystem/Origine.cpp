#pragma once

#define USE_MMSYSTEM_VS 1
#include "MemoryManagerWrapper.h"
#include "MySTDlikeAllocator.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>
using namespace std;
using namespace MySTDlikeAllocator;
int main()
{


	struct MyMemoryStruct
	{
		int a;
		int b;
		int c;
	};
	struct MyMemoryStructULL
	{
		unsigned long long a;
		unsigned long long b;
		unsigned long long c;
	};
	Init();
 	//cout << sizeof(unsigned char) << endl; 
 	//cout << sizeof(unsigned short) << endl;
	//cout << sizeof(unsigned long long) << endl;

	cout << sizeof(unsigned long) << endl;
	cout << sizeof(unsigned int) << endl;
	cout << sizeof(unsigned short) << endl;
	cout << sizeof(unsigned char) << endl;
	cout << sizeof(char) << endl;
	cout << sizeof(bool) << endl;

	cout << sizeof(SmallObjectAllocator) << endl;  //12
	cout << sizeof(SOAFixedAllocator) << endl; //32
	cout << sizeof(SOAFixedAllocator*) << endl;
	cout << sizeof(SOAChunk) << endl; //16

	cout << sizeof(MyMemoryStruct) << endl;

	cout << sizeof(MyMemoryStructULL) << endl;
	cout << sizeof(BlockHeader) << endl;

//	unsigned char a = '1';
// 	unsigned short b =6;
// 	cout << a << endl;
// 	cout << b << endl;

	//new(__FILE__, __LINE__)
	MyMemoryStruct* memoryStruct1 = new MyMemoryStruct;
	cout << BlockHeader::CountBlocks()<<endl;
	MyMemoryStruct* memoryStruct2 = new MyMemoryStruct[15];
	cout << BlockHeader::CountBlocks() << endl;
	MyMemoryStructULL* memoryStructULL1 = new MyMemoryStructULL;
	cout << BlockHeader::CountBlocks() << endl;

	PrintMemoryUsage();
	cout << endl << "GetSize Examples" << endl << endl;
	cout << "memoryStruct2  " << GetSize(memoryStruct2)<<endl;
	cout << "memoryStruct1  " << GetSize(memoryStruct1) << endl;
	cout << "memoryStructULL  " << GetSize(memoryStructULL1) << endl;

	cout << endl;
	memoryStruct1->a = 1;
	memoryStruct1->b = 2;
	memoryStruct1->c = 3;

	memoryStructULL1->a = 34568;

	for (unsigned short i = 0; i < 15; i++)
	{
		memoryStruct2[i].a = (i * 1);
		memoryStruct2[i].b = (i * 2);
		memoryStruct2[i].c = (i * 3);
	}
	cout << memoryStruct1->a << "  " << memoryStruct1->b << "  " << memoryStruct1->c << endl;
	for (unsigned short i = 0; i < 15; i++)
	{
		cout << memoryStruct2[i].a << "  "<< memoryStruct2[i].b << "  " << memoryStruct2[i].c << endl;
	}

	delete[] memoryStruct2;
	cout << BlockHeader::CountBlocks() << endl;

	PrintMemoryUsage();

	delete memoryStruct1;
	cout << BlockHeader::CountBlocks() << endl;

	

	delete memoryStructULL1;
	cout << BlockHeader::CountBlocks() << endl;

	vector<int, Allocator<int>> myVec;
	PrintMemoryUsage();
	myVec.push_back(2);
	PrintMemoryUsage();

	myVec.push_back(3);
	myVec.push_back(24);
	myVec.push_back(564);
	myVec.push_back(534);


	PrintMemoryUsage();
	myVec.reserve(100);
	//myVec.resize(100);
	PrintMemoryUsage();

	while (1)
	{

	}
}