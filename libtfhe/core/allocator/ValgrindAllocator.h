#ifndef TFHE_STRICTMALLOCALLOCATOR_H
#define TFHE_STRICTMALLOCALLOCATOR_H


#include <set>
#include "allocator.h"

/**
 * this class will in fact allocate everything with malloc and deallocate everything with free.
 * Il will also check that everything that has been allocated is freed according to the allocator policy.
 */
template<>
class AllocatorImpl<VALGRIND_ALLOCATOR> {
    AllocatorImpl *const father;
    std::set<void *> addresses;
    bool has_child;
    bool has_moved;
    size_t beginAddress;
    const size_t endAddress;
public:
    void *allocate(size_t alignment, size_t byte_size);

    void deallocate(void *ptr);

    AllocatorImpl(AllocatorImpl *father = nullptr, size_t beginAddress = 0, size_t endAddress = 0);

    ~AllocatorImpl();


    AllocatorImpl(AllocatorImpl &&alloc);

    AllocatorImpl createStackChildAllocator(const size_t expected_size = default_stack_size);

#include "allocator-shortcut-functions.h"
};


#endif //TFHE_STRICTMALLOCALLOCATOR_H
