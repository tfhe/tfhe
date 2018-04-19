#include <gtest/gtest.h>
#include <core/allocator/allocator.h>
#include <cmath>

using namespace std;

TEST(ALLOCATOR, ceilalign) {
    set<size_t> alignments({1, 2, 4, 8, 16, 32});
    set<size_t> sizes({1, 3, 5, 7, 100, 202, 403});
    for (size_t alignment : alignments) {
        for (size_t l : sizes) {
            //cerr << l << " " << alignment << endl;
            ASSERT_EQ(ceilalign(l, alignment), alignment * ceil(l / double(alignment)));
        }
    }
}

// crazy recursive function: every allocated object is destroyed
void crazyrec(int i, Allocator alloc) {
    if (i == 100) return;
    size_t *r = alloc.newObject<size_t>();
    *r = i;
    crazyrec(i + 1, alloc.createStackChildAllocator());
    ASSERT_EQ(*r, i);
    alloc.deleteObject(r);
}

TEST(ALLOCATOR, stack) {
    Allocator a;
    crazyrec(0, a.createStackChildAllocator(1000));
}


struct Toto {
    int p;

    Toto(int p) : p(p) {}

    void destroy() {}
};

TEST(ALLOCATOR, array) {
    Allocator a;
    Toto *t43 = a.newArray<Toto>(17, 43);
    Toto *t42 = a.newArrayAligned<Toto>(100, 32, 42);
    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(t42[i].p, 42);
    }
    for (int i = 0; i < 17; i++) {
        ASSERT_EQ(t43[i].p, 43);
    }
    ASSERT_EQ(size_t(t42)%32, 0);
    a.deleteArray(100, t42);
    a.deleteArray(17, t43);
}
