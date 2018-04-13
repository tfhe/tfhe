#include <stdlib.h>
#include "TFHEAllocator.h"
#include "allocator.h"

void *AllocatorImpl<TFHE_ALLOCATOR>::allocate(size_t alignment, size_t byte_size) {
    if (beginAddress == 0ul) {
        //malloc mode
        return aligned_alloc(alignment, byte_size);
    } else {
        //stack mode
        size_t newBegin = ceilalign(beginAddress, alignment);
        beginAddress = newBegin + byte_size;
        return (void *) newBegin;
    }
}

void AllocatorImpl<TFHE_ALLOCATOR>::deallocate(void *ptr) {
    if (beginAddress == 0ul) {
        //malloc mode
        free(ptr);
    } else {
        //stack mode
        //nothing
    }
}

AllocatorImpl<TFHE_ALLOCATOR> AllocatorImpl<TFHE_ALLOCATOR>::createStackChildAllocator(const size_t expected_size) {
    if (beginAddress == 0ul) {
        //malloc mode
        size_t addr = size_t(aligned_alloc(expected_size, 1));
        return AllocatorImpl(this, addr);
    } else {
        //stack mode
        return AllocatorImpl(nullptr, beginAddress);
    }
}

AllocatorImpl<TFHE_ALLOCATOR>::AllocatorImpl(AllocatorImpl<TFHE_ALLOCATOR> *const father, const size_t beginAddr) :
        father(father), beginAddress(beginAddr) {
}

AllocatorImpl<TFHE_ALLOCATOR>::AllocatorImpl(AllocatorImpl<TFHE_ALLOCATOR> &&origin) :
        father(origin.father), beginAddress(origin.beginAddress) {
    //in the move constructor, we explicitely reset the father's origin
    AllocatorImpl<TFHE_ALLOCATOR> **x = (AllocatorImpl<TFHE_ALLOCATOR> **) &origin.father;
    *x = nullptr;
    origin.beginAddress = 0;
}
