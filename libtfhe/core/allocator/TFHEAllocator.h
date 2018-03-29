#ifndef TFHE_STACKALLOCATOR_H
#define TFHE_STACKALLOCATOR_H

#include <cstddef>

class Allocator;

/**
 * This class is the optimized allocator.
 */
class TFHEAllocator {
    TFHEAllocator* const father;
    size_t beginAddress;
public:
    void *allocate(size_t alignment, size_t byte_size);

    void deallocate(void *ptr);

    TFHEAllocator(TFHEAllocator* const father, const size_t beginAddr);

    Allocator createStackChildAllocator(const size_t expected_size);

    ~TFHEAllocator() {};
};


#endif //TFHE_STACKALLOCATOR_H
