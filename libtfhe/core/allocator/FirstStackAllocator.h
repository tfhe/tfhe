#ifndef TFHE_FIRSTSTACKALLOCATOR_H
#define TFHE_FIRSTSTACKALLOCATOR_H


#include "allocator.h"
#include "StackAllocator.h"

class FirstStackAllocator : public StackAllocator {
    AllocatorImpl *const father;
    void *const initialAddress;
public:
    FirstStackAllocator(AllocatorImpl *father, void *initialAddress, size_t byte_size);

    ~FirstStackAllocator();
};


#endif //TFHE_FIRSTSTACKALLOCATOR_H
