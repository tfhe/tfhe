#ifndef TFHE_STACKALLOCATOR_H
#define TFHE_STACKALLOCATOR_H

#include <cstddef>
#include "allocator.h"

/**
 * This class is the optimized allocator.
 */
template<>
class AllocatorImpl<TFHE_ALLOCATOR> {
    AllocatorImpl *father;
    size_t beginAddress;
public:
    void *allocate(size_t alignment, size_t byte_size);

    void deallocate(void *ptr);

    AllocatorImpl(AllocatorImpl *const father, const size_t beginAddr);

    AllocatorImpl(AllocatorImpl &&);

    AllocatorImpl createStackChildAllocator(const size_t expected_size);

    ~AllocatorImpl() {};

#include "allocator-shortcut-functions.h"
};


#endif //TFHE_STACKALLOCATOR_H
