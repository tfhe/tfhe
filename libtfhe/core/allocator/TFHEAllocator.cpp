#include <stdlib.h>
#include "TFHEAllocator.h"
#include "allocator.h"

void *AllocatorImpl<TFHE_ALLOCATOR>::allocate(size_t alignment, size_t byte_size) {
    if (beginAddress == 0ul) {
        //malloc mode
        return aligned_alloc(alignment, byte_size); //TODO: fix strange bug!!!
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

AllocatorImpl<TFHE_ALLOCATOR>::~AllocatorImpl() {
    if (baseAddr != nullptr) {
        //first stack mode
        free(baseAddr);
    } else {
        //malloc mode
        //nothing
    }
}

AllocatorImpl<TFHE_ALLOCATOR> AllocatorImpl<TFHE_ALLOCATOR>::createStackChildAllocator(const size_t expected_size) {
    if (beginAddress == 0ul) {
        //malloc mode
        void* baseAddress = aligned_alloc(1, expected_size);
        return AllocatorImpl(baseAddress, size_t(baseAddress));
    } else {
        //stack mode
        return AllocatorImpl(nullptr, beginAddress);
    }
}

AllocatorImpl<TFHE_ALLOCATOR>::AllocatorImpl(void *const baseAddr, const size_t beginAddr) :
        baseAddr(baseAddr), beginAddress(beginAddr) {
}

AllocatorImpl<TFHE_ALLOCATOR>::AllocatorImpl(AllocatorImpl<TFHE_ALLOCATOR> &&origin) :
        baseAddr(origin.baseAddr), beginAddress(origin.beginAddress) {
    origin.baseAddr = nullptr;
    origin.beginAddress = 0;
}
