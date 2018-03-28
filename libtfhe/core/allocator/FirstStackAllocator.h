#ifndef TFHE_FIRSTSTACKALLOCATOR_H
#define TFHE_FIRSTSTACKALLOCATOR_H


#include "allocator.h"
#include "StackAllocator.h"

/**
 * This class represents a Stack allocator which is the first child of a MallocAllocator.
 * The memory region is allocated on his parent, with the predefined byte_size.
 * When the allocator gets out of scope, it frees the region on his parent automatically.
 */
class FirstStackAllocator : public StackAllocator {
    AllocatorImpl *const father;
    void *const initialAddress;
public:
    FirstStackAllocator(AllocatorImpl *father, void *initialAddress, size_t byte_size);

    ~FirstStackAllocator();
};


#endif //TFHE_FIRSTSTACKALLOCATOR_H
