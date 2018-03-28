#ifndef TFHE_STRICTMALLOCALLOCATOR_H
#define TFHE_STRICTMALLOCALLOCATOR_H


#include <set>
#include "allocator.h"

class ValgrindAllocator : public AllocatorImpl {
    ValgrindAllocator *const father;
    std::set<void *> addresses;
    bool has_child;
public:
    void *allocate(size_t alignment, size_t byte_size) override;

    void deallocate(void *ptr) override;

    ValgrindAllocator(ValgrindAllocator *father = nullptr);

    ~ValgrindAllocator() override;

    Allocator createStackChildAllocator(const size_t expected_size) override;
};


#endif //TFHE_STRICTMALLOCALLOCATOR_H
