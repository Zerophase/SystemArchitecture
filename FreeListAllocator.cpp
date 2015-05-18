//
// Created by zerophase on 5/17/15.
//

#include "FreeListAllocator.h"
#include "MemoryWrapper.h"
#include <cstring>
#include <cstdlib>
#include <iostream>

using namespace Allocators;

char *FreeListAllocator::heapList = nullptr;

int FreeListAllocator::Initialize()
{
	MemoryWrapper::Instance().Initialize();

	heapList = (char*)MemoryWrapper::Instance().Sbrk(4 *WSIZE);
	if(heapList == (void *) -1)
		return -1;

	PUT(heapList, 0); // alignment padding
	PUT(heapList + (1*WSIZE), PACK(DSIZE, 1)); // prologue header
	PUT(heapList + (2 *WSIZE), PACK(DSIZE, 1)); // prologue footer
	PUT(heapList + (3 * WSIZE), PACK(0, 1)); // epilogue header
	heapList += (2* WSIZE);

	if(extendHeap(CHUNKSIZE/WSIZE) == nullptr)
	return 0;
}

void* FreeListAllocator::extendHeap(size_t words)
{
	char *bp;
	size_t size;

	size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;

	bp = (char*)MemoryWrapper::Instance().Sbrk(size);
	if(bp == (char*)-1)
		return nullptr;

	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

	return coalesce(bp);
}

void* FreeListAllocator::coalesce(void *bp)
{
	size_t prevAlloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
	size_t nextAlloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size = GET_SIZE(HDRP(bp));

	if(prevAlloc && nextAlloc)
		return bp;
	else if(prevAlloc && !nextAlloc)
	{
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
	}
	else if(!prevAlloc && nextAlloc)
	{
		size += GET_ALLOC(HDRP(PREV_BLKP(bp)));
		PUT(FTRP(bp), PACK(size, 0));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);
	}
	else
	{
		size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
	            GET_SIZE(FTRP(NEXT_BLKP(bp)));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);
	}

	return bp;
}

void* FreeListAllocator::Malloc(size_t size)
{
	size_t asize;
	size_t extendSize;
	char *bp;

	if(heapList == nullptr)
		Initialize();

	if(size == 0)
		return nullptr;

	if(size <= DSIZE)
		asize = 2 * DSIZE;
	else
		asize = DSIZE * ((size + (DSIZE) + (DSIZE -1)) / DSIZE);

	if((bp = (char*)findFit(asize)) != nullptr)
	{
		place(bp, asize);
		return bp;
	}

	extendSize = MAX(asize, CHUNKSIZE);
	if((bp = (char*)extendHeap(extendSize/WSIZE)) == nullptr)
		return nullptr;
	place(bp, asize);
	return bp;
}

void* FreeListAllocator::findFit(size_t asize)
{
	static void *bp = heapList;
	/*do
	{*/
		bp = NEXT_BLKP(bp);
	/*}
	while
			(bp != MemoryWrapper::Instance().brk &&
					 (GET_ALLOC(HDRP(bp))) || GET_SIZE(HDRP(bp)) < asize);*/

	if(bp == MemoryWrapper::Instance().brk)
		return nullptr;
	else
		return bp;
}

void FreeListAllocator::place(void *bp, size_t asize)
{
	size_t csize = GET_SIZE(HDRP(bp));

	if((csize - asize) >= (2*DSIZE))
	{
		PUT(HDRP(bp), PACK(asize, 1));
		csize -= 2 * DSIZE;
		bp = NEXT_BLKP(bp);
		PUT(HDRP(bp), PACK(csize, 0));
		PUT(FTRP(bp), PACK(csize, 0));
	}
	else
	{
		PUT(HDRP(bp), PACK(csize, 1));
	}
}

void FreeListAllocator::Free(void *bp)
{
	if(bp == nullptr)
		return;

	size_t size = GET_SIZE(HDRP(bp));

	if(heapList == nullptr)
		Initialize();

	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	coalesce(bp);
}

void FreeListAllocator::CheckHeap(int verbose)
{
	char *bp = heapList;

	if(verbose)
		printf("Heap (%p):\n", heapList);

	if((GET_SIZE(HDRP(heapList)) != DSIZE) || !GET_ALLOC(HDRP(heapList)))
		printf("Bad prologue header\n");
	checkBlock(heapList);

	for (bp = heapList; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp))
	{
		if(verbose)
			printBlock(bp);
	}
}

void FreeListAllocator::printBlock(void *bp)
{
	size_t hsize, halloc, fsize, falloc;

	checkHeap(0);
	hsize = GET_SIZE(HDRP(bp));
	halloc = GET_ALLOC(HDRP(bp));
	fsize = GET_SIZE(FTRP(bp));
	falloc = GET_ALLOC(FTRP(bp));

	if(hsize == 0)
	{
		printf("%p: E0L\n", bp);
		return;
	}

}

void FreeListAllocator::checkHeap(int verbose)
{
	char *bp = heapList;

	if(verbose)
		printf("Heap (%p):\n", heapList);

	if((GET_SIZE(HDRP(heapList))) != DSIZE || !GET_ALLOC(HDRP(heapList)))
		printf("Bad prologue header\n");
	checkBlock(heapList);

	for (bp = heapList; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp))
	{
		if(verbose)
			printBlock(bp);
		checkBlock(bp);
	}

	if(verbose)
		printBlock(bp);
	if((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp))))
		printf("Bad epilogue header\n");
}


void FreeListAllocator::checkBlock(void *bp)
{
	if((size_t)bp % 8)
		printf("Error: %p is not doubleword aligned\n", bp);
	if(GET(HDRP(bp)) != GET(FTRP(bp)))
		printf("Error: header does not match footer\n");
}