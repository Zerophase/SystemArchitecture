//
// Created by zerophase on 5/17/15.
//

#include "MemoryWrapper.h"
#include "MemoryConfig.h"
#include <cstdlib>
#include <iostream>

using namespace Allocators;

char *MemoryWrapper::heap;
char *MemoryWrapper::brk;
char *MemoryWrapper::maxAddress;

MemoryWrapper &MemoryWrapper::Instance()
{
	static MemoryWrapper memoryWrapper;
	return memoryWrapper;
}

MemoryWrapper::MemoryWrapper(){}

void MemoryWrapper::Initialize()
{
	heap = (char*) malloc(MAX_HEAP);
	brk = (char*)heap;
	maxAddress = (char*)(heap + MAX_HEAP);
}

void* MemoryWrapper::Sbrk(int increment)
{
	char *oldBrk = brk;

	if( (increment < 0) || (brk + increment) > maxAddress)
	{
		errno = ENOMEM;
		std::cout << "ERROR: Sbrk failed. Ran out of memory \n";
		return  (void *)-1;
	}

	brk += increment;

	return (void*) oldBrk;
}