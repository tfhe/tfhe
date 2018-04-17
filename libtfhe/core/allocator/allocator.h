#ifndef TFHE_ALLOCATOR_H
#define TFHE_ALLOCATOR_H

/**
 * @file allocator interface description
 * @brief allocator interface description
 */

#include <cstddef>
#include <memory>

const size_t default_stack_size = 50000000;


/**
 * returns the smallest multiple of alignment larger or equal to size
 * @param size must be positive
 * @param alignment must be a power of 2
 * @return the smallest multiple of alignment larger or equal to size
 */
inline size_t ceilalign(const size_t size, const size_t alignment) {
    return (size + alignment - 1) & (-alignment);
}

enum AllocatorType {
    VALGRIND_ALLOCATOR = 0,
    TFHE_ALLOCATOR = 1,
    STACK_ONLY_ALLOCATOR = 2
};

template<enum AllocatorType>
class AllocatorImpl;

#include "TFHEAllocator.h"
#include "ValgrindAllocator.h"

// for now, we'll work only with the valgrind allocator, who will check all usages
// in release mode, we'll switch to TFHE_ALLOCATOR
//typedef AllocatorImpl<VALGRIND_ALLOCATOR> Allocator;
typedef AllocatorImpl<TFHE_ALLOCATOR> Allocator;

#endif //TFHE_ALLOCATOR_H
