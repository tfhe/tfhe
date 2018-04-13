#ifndef TFHE_STRICTMALLOCALLOCATOR_H
#define TFHE_STRICTMALLOCALLOCATOR_H


#include <set>

class Allocator;

/**
 * this class will in fact allocate everything with malloc and deallocate everything with free.
 * Il will also check that everything that has been allocated is freed according to the allocator policy.
 */
class ValgrindAllocator {
    ValgrindAllocator *const father;
    std::set<void *> addresses;
    bool has_child;
    size_t beginAddress;
    const size_t endAddress;
public:
    void *allocate(size_t alignment, size_t byte_size);

    void deallocate(void *ptr);

    ValgrindAllocator(ValgrindAllocator *father = nullptr, size_t beginAddress = 0, size_t endAddress = 0);

    ~ValgrindAllocator();

    ValgrindAllocator(const ValgrindAllocator &alloc) = delete;

    ValgrindAllocator(ValgrindAllocator &&alloc) = default;

    ValgrindAllocator &operator=(const ValgrindAllocator &alloc)= delete;

    ValgrindAllocator &operator=(ValgrindAllocator &&alloc)= default;

    Allocator createStackChildAllocator(const size_t expected_size = default_stack_size);
};


#endif //TFHE_STRICTMALLOCALLOCATOR_H
