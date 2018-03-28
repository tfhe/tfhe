#include "StackAllocator.h"

void *StackAllocator::allocate(size_t alignment, size_t byte_size) {
    size_t newBegin = ceilalign(beginAddr, alignment);
    beginAddr = ceilalign(newBegin + byte_size, alignment);
    if (beginAddr > endAddr) abort(); //the predefined zone was too small!
    return (void *) newBegin;
}

void StackAllocator::deallocate(void *ptr) {
    // we never deallocate memory inside a stack allocator
}

StackAllocator::StackAllocator(const size_t beginAddr, const size_t endAddr) :
        beginAddr(beginAddr), endAddr(endAddr) {
}

Allocator StackAllocator::createStackChildAllocator(const size_t expected_size) {
    return Allocator(new StackAllocator(beginAddr, endAddr));
}
