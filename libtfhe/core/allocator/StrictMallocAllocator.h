#ifndef TFHE_STRICTMALLOCALLOCATOR_H
#define TFHE_STRICTMALLOCALLOCATOR_H


#include <set>
#include "allocator.h"

class StrictMallocAllocator : public AllocatorImpl {
    StrictMallocAllocator *const father;
    std::set<void *> addresses;
    bool has_child;
public:
    void *allocate(size_t byte_size, size_t alignment) override;

    void deallocate(void *ptr) override;

    StrictMallocAllocator(StrictMallocAllocator *father = nullptr);

    ~StrictMallocAllocator() override;

    Allocator createStackChildAllocator(const size_t expected_size) override;
};


#endif //TFHE_STRICTMALLOCALLOCATOR_H
