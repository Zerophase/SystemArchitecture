//
// Created by zerophase on 5/17/15.
//

#ifndef SYSTEMARCHITECTURE_FREELISTALLOCATOR_H
#define SYSTEMARCHITECTURE_FREELISTALLOCATOR_H

#include <stddef.h>

namespace Allocators
{
    class FreeListAllocator
    {
    public:
	    int Initialize();
	    void *Malloc(size_t size);
	    void Free(void *pointer);

	    void CheckHeap(int verbose);

    private:
	    void *extendHeap(size_t words);

	    void *coalesce(void *bp);

	    void *findFit(size_t asize);
	    void place(void *bp, size_t asize);

	    void checkBlock(void *bp);

	    void checkHeap(int verbose);

	    void printBlock(void *bp);

    private:
	    static char *heapList;

    private:
	    #define WSIZE 4
		#define DSIZE 8
		#define CHUNKSIZE (1 << 12)

        #define MAX(x, y) ((x) > (y)? (x) : (y))

		#define GET(p) (*(unsigned int *)(p))
        #define PUT(p, value) (*(unsigned int *)(p) = (value))

        #define PACK(size, allocate) ((size)| (allocate))

        #define HDRP(bp) ((char*)(bp) - WSIZE)
        #define FTRP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

        #define GET_SIZE(p) (GET(p) & ~0x7)
		#define GET_ALLOC(p) (GET(p) & 0x1)

        #define NEXT_BLKP(bp) ((char*)(bp) + GET_SIZE(((char*)(bp) - WSIZE)))
		#define PREV_BLKP(bp) ((char*)(bp) - GET_SIZE(((char*)(bp) - DSIZE)))
    };
}




#endif //SYSTEMARCHITECTURE_FREELISTALLOCATOR_H
