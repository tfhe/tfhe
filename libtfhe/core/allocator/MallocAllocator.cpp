#include "MallocAllocator.h"
#include "StackAllocator.h"
#include "FirstStackAllocator.h"

void *MallocAllocator::allocate(size_t byte_size, size_t alignment) {
    return aligned_alloc(alignment, byte_size);
}

void MallocAllocator::deallocate(void *ptr) {
    free(ptr);
}

MallocAllocator::~MallocAllocator() {

}

Allocator MallocAllocator::createStackChildAllocator(const size_t expected_size) {
    void *beginAddr = allocate(expected_size, 1);
    return Allocator(new FirstStackAllocator(this, beginAddr, expected_size));
}