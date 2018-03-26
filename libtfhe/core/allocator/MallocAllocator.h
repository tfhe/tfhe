#ifndef TFHE_MALLOCALLOCATOR_H
#define TFHE_MALLOCALLOCATOR_H


#include "allocator.h"

class MallocAllocator : public AllocatorImpl {
public:
    void *allocate(size_t byte_size, size_t alignment) override;

    void deallocate(void *ptr) override;

    ~MallocAllocator() override;

    MallocAllocator() {}

    Allocator createStackChildAllocator(const size_t expected_size) override;
};


#endif //TFHE_MALLOCALLOCATOR_H
