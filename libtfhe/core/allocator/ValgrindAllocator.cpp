#ifndef USE_TFHE_ALLOCATOR

#include <iostream>
#include "allocator.h"
#include "ValgrindAllocator.h"

using namespace std;

/** utility function that crashes if condition is not met */
static void REQUIRE_DRAMATICALLY(bool condition, string error_message) {
    if (!condition) {
        cerr << "FATAL ERROR: " << error_message << endl;
        abort();
    }
}


void *ValgrindAllocator::allocate(size_t alignment, size_t byte_size) {
    REQUIRE_DRAMATICALLY(!has_child,
                         "trying to allocate an address on a parent allocator that has a child");
    REQUIRE_DRAMATICALLY(byte_size%alignment==0,
                         "byte_size is not multiple of alignment");
    REQUIRE_DRAMATICALLY((alignment & (alignment-1)) == 0,
                         "alignment is not a power of 2");
    // if in stack mode, virtually allocate
    if (beginAddress!=0ul) {
        //stack mode
        size_t newBegin = ceilalign(beginAddress, alignment);
        beginAddress = newBegin+byte_size;
        REQUIRE_DRAMATICALLY(beginAddress<=endAddress,
                             "Stack Allocator: out of memory!!!");
    }
    // allocate via malloc
    void *addr = aligned_alloc(alignment, byte_size);
    this->addresses.insert(addr);
    return addr;
}

void ValgrindAllocator::deallocate(void *ptr) {
    REQUIRE_DRAMATICALLY(!has_child,
                         "trying to free an address on a parent allocator that has a child");
    REQUIRE_DRAMATICALLY(this->addresses.erase(ptr) == 1,
                         "trying to free an address that was not allocated by this allocator");
    free(ptr);
}


ValgrindAllocator::~ValgrindAllocator() {
    REQUIRE_DRAMATICALLY(addresses.empty(),
                         "some allocated addresses were not freed!");
    if (father != nullptr) father->has_child = false;
}

ValgrindAllocator::ValgrindAllocator(ValgrindAllocator *father, size_t beginAddress, size_t endAddress) :
        father(father), has_child(false), beginAddress(beginAddress), endAddress(endAddress) {
    if (father) father->has_child=true;
}

Allocator ValgrindAllocator::createStackChildAllocator(const size_t expected_size) {
    // if in stack mode, virtually allocate
    if (beginAddress!=0ul) {
        //stack mode
        return Allocator(this, beginAddress, endAddress);
    } else {
        //malloc mode
        return Allocator(this, 1, expected_size + 1);
    }
}

#endif
