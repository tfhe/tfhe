#ifdef USE_TFHE_ALLOCATOR

#include <stdlib.h>
#include "TFHEAllocator.h"
#include "allocator.h"

void *TFHEAllocator::allocate(size_t alignment, size_t byte_size) {
    if (beginAddress==0ul) {
        //malloc mode
        return aligned_alloc(alignment, byte_size);
    } else {
        //stack mode
        size_t newBegin = ceilalign(beginAddress, alignment);
        beginAddress = newBegin+byte_size;
        return (void *) newBegin;
    }
}

void TFHEAllocator::deallocate(void *ptr) {
    if (beginAddress==0ul) {
        //malloc mode
        free(ptr);
    } else {
        //stack mode
        //nothing
    }
}

Allocator TFHEAllocator::createStackChildAllocator(const size_t expected_size) {
    if (beginAddress==0ul) {
        //malloc mode
        size_t addr = size_t(aligned_alloc(expected_size,1));
        return Allocator(this, addr);
    } else {
        //stack mode
        return Allocator(nullptr, beginAddress);
    }
}

TFHEAllocator::TFHEAllocator(TFHEAllocator *const father, const size_t beginAddr):
father(father),beginAddress(beginAddr)
{
}

#endif