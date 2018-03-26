#include <iostream>
#include "StrictMallocAllocator.h"

using namespace std;

/** utility function that crashes if condition is not met */
static void REQUIRE_DRAMATICALLY(bool condition, string error_message) {
    if (!condition) {
        cerr << "FATAL ERROR: " << error_message << endl;
        abort();
    }
}


void *StrictMallocAllocator::allocate(size_t byte_size, size_t alignment) {
    REQUIRE_DRAMATICALLY(!has_child,
                         "trying to allocate an address on a parent allocator that has a child");
    void *addr = aligned_alloc(alignment, byte_size);
    this->addresses.insert(addr);
    return addr;
}

void StrictMallocAllocator::deallocate(void *ptr) {
    REQUIRE_DRAMATICALLY(!has_child,
                         "trying to free an address on a parent allocator that has a child");
    REQUIRE_DRAMATICALLY(this->addresses.erase(ptr) == 1,
                         "trying to free an address that was not allocated by this allocator");
    free(ptr);
}


StrictMallocAllocator::~StrictMallocAllocator() {
    if (father != nullptr) father->has_child = false;
}

StrictMallocAllocator::StrictMallocAllocator(StrictMallocAllocator *father) :
        father(father), has_child(false) {
}

Allocator StrictMallocAllocator::createStackChildAllocator(const size_t expected_size) {
    StrictMallocAllocator *cfather = dynamic_cast<StrictMallocAllocator *> (this);
    REQUIRE_DRAMATICALLY(cfather != nullptr,
                         "A strict Malloc Allocator can only generate Strict Malloc Allocators");
    return Allocator(new StrictMallocAllocator(cfather));
}
