//
// Created by zerophase on 5/17/15.
//

#ifndef SYSTEMARCHITECTURE_MEMORYWRAPPER_H
#define SYSTEMARCHITECTURE_MEMORYWRAPPER_H

#include <stddef.h>

namespace Allocators
{
    class MemoryWrapper
    {
    public:
	    static MemoryWrapper &Instance();

	    void Initialize();
	    void *Sbrk(int increment);

	   /* void DeInitialize();
	    void Reset_Brk();
	    void *Heap_lo();
	    void *Heap_hi();
	    size_t mem_heapsize();
	    size_t mem_pagesize();*/

    private:
	    MemoryWrapper();

	    MemoryWrapper(MemoryWrapper const&) = delete;
	    void operator=(MemoryWrapper const&) = delete;

    public:
	    static char *heap;
	    static char *brk;
	    static char *maxAddress;
    };
}
#endif //SYSTEMARCHITECTURE_MEMORYWRAPPER_H
