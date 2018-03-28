#ifndef TFHE_MALLOCALLOCATOR_H
#define TFHE_MALLOCALLOCATOR_H


#include "allocator.h"

/**
 * This class represents a Malloc allocator.
 * allocate calls aligned_alloc, and deallocate calls free.
 */
class MallocAllocator : public AllocatorImpl {
public:
    void *allocate(size_t alignment, size_t byte_size) override;

    void deallocate(void *ptr) override;

    ~MallocAllocator() override;

    MallocAllocator() {}

    Allocator createStackChildAllocator(const size_t expected_size) override;
};


#endif //TFHE_MALLOCALLOCATOR_H
