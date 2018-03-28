#include <iostream>
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
                         "some allocated addresses were not freed");
    if (father != nullptr) father->has_child = false;
}

ValgrindAllocator::ValgrindAllocator(ValgrindAllocator *father) :
        father(father), has_child(false) {
}

Allocator ValgrindAllocator::createStackChildAllocator(const size_t expected_size) {
    ValgrindAllocator *cfather = dynamic_cast<ValgrindAllocator *> (this);
    return Allocator(new ValgrindAllocator(cfather));
}
