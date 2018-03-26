#ifndef TFHE_STACKALLOCATOR_H
#define TFHE_STACKALLOCATOR_H


#include "allocator.h"

class StackAllocator : public AllocatorImpl {
    size_t beginAddr;
    const size_t endAddr;
public:
    virtual void *allocate(size_t byte_size, size_t alignment) override;

    virtual void deallocate(void *ptr) override;

    StackAllocator(const size_t beginAddr, const size_t endAddr);

    virtual Allocator createStackChildAllocator(const size_t expected_size) override;

    virtual ~StackAllocator() {};
};


#endif //TFHE_STACKALLOCATOR_H
