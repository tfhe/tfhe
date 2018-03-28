#include "MallocAllocator.h"
#include "StackAllocator.h"
#include "FirstStackAllocator.h"

void *MallocAllocator::allocate(size_t alignment, size_t byte_size) {
    return aligned_alloc(alignment, byte_size);
}

void MallocAllocator::deallocate(void *ptr) {
    free(ptr);
}

MallocAllocator::~MallocAllocator() {

}

Allocator MallocAllocator::createStackChildAllocator(const size_t expected_size) {
    void *beginAddr = allocate(1, expected_size);
    return Allocator(new FirstStackAllocator(this, beginAddr, expected_size));
}