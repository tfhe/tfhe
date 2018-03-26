#include "FirstStackAllocator.h"


FirstStackAllocator::FirstStackAllocator(AllocatorImpl *father, void *initialAddress, size_t byte_size) :
        StackAllocator(size_t(initialAddress), size_t(initialAddress) + byte_size),
        father(father), initialAddress(initialAddress) {
}

FirstStackAllocator::~FirstStackAllocator() {
    father->deallocate(initialAddress);
}
