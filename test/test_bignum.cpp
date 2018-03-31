#include <gtest/gtest.h>
#include <core/arithmetic/BigInt.h>
#include <core/allocator/allocator.h>
#include <core/arithmetic/BigTorus.h>

#include "gmpxx.h"

using namespace std;

TEST(BigNum, BigIntAllocSet) {
    // here, we test a few constructors with or without value
    // we also test the copyvalue functions
    Allocator alloc;
    BigIntParams *params = alloc.newObject<BigIntParams>(2);
    BigInt *a = alloc.newObject<BigInt>();
    BigInt *b = alloc.newObject<BigInt>(42);
    BigInt *c = alloc.newObject<BigInt>(std::numeric_limits<int64_t>::max());
    BigInt *d = alloc.newObject<BigInt>(std::numeric_limits<int64_t>::min());
    ASSERT_EQ(mpz_class(a->data), 0);
    ASSERT_EQ(mpz_class(b->data), 42);
    ASSERT_EQ(mpz_class(c->data), std::numeric_limits<int64_t>::max());
    ASSERT_EQ(mpz_class(d->data), std::numeric_limits<int64_t>::min());
    setvalue(a, b, params);
    setvalue(b, 43, params);
    ASSERT_EQ(mpz_class(a->data), 42);
    ASSERT_EQ(mpz_class(b->data), 43);
    alloc.deleteObject(a);
    alloc.deleteObject(b);
    alloc.deleteObject(c);
    alloc.deleteObject(d);
    alloc.deleteObject(params);
}

TEST(BigNum, BigIntAdd) {
    // here, we do a small random walk, performing a few additions and comparing with a reference
    // mpz_class implementation
    Allocator alloc;
    BigIntParams *params = alloc.newObject<BigIntParams>(2);
    int64_t tval = std::numeric_limits<int64_t>::max() / 3;
    mpz_class aref = tval;
    BigInt *a = alloc.newObject<BigInt>(tval);
    ASSERT_EQ(mpz_class(a->data), aref);
    for (int i = 0; i < 20; i++) {
        switch (i % 3) {
            case 0:
                add(a, a, a, params);
                aref = aref + aref;
                break;
            case 1:
                add(a, a, tval, params);
                aref = aref + tval;
                break;
            case 2:
                add(a, tval, a, params);
                aref = tval + aref;
                break;
        }
        ASSERT_EQ(mpz_class(a->data), aref);
    }
    alloc.deleteObject(a);
    alloc.deleteObject(params);
}

TEST(BigNum, BigIntSub) {
    // here, we do a small random walk, performing a few multiplications and comparing with a reference
    // mpz_class implementation
    Allocator alloc;
    BigIntParams *params = alloc.newObject<BigIntParams>(2);
    int64_t tval = std::numeric_limits<int64_t>::max() / 3;
    int64_t tval2 = std::numeric_limits<int64_t>::max() / 5;
    mpz_class aref = tval;
    BigInt *a = alloc.newObject<BigInt>(tval);
    mpz_class bref = tval2;
    BigInt *b = alloc.newObject<BigInt>(tval2);
    ASSERT_EQ(mpz_class(a->data), aref);
    ASSERT_EQ(mpz_class(b->data), bref);
    for (int i = 0; i < 20; i++) {
        switch (i % 3) {
            case 0:
                sub(a, a, b, params);
                aref = aref - bref;
                break;
            case 1:
                sub(a, a, tval, params);
                aref = aref - tval;
                break;
            case 2:
                sub(a, tval, a, params);
                aref = tval - aref;
                break;
        }
        ASSERT_EQ(mpz_class(a->data), aref);
    }
    alloc.deleteObject(b);
    alloc.deleteObject(a);
    alloc.deleteObject(params);
}

TEST(BigNum, BigIntMul) {
    // here, we do a small random walk, performing a few multiplications and comparing with a reference
    // mpz_class implementation
    Allocator alloc;
    BigIntParams *params = alloc.newObject<BigIntParams>(2);
    //everything is modulo p
    mpz_class p;
    mpz_ui_pow_ui(p.get_mpz_t(), 2, params->p);

    int64_t tval = std::numeric_limits<int64_t>::min() / 3;
    int64_t tval2 = std::numeric_limits<int64_t>::min() / 5;

    mpz_class aref = tval;
    BigInt *a = alloc.newObject<BigInt>(tval);
    ASSERT_EQ(mpz_class(a->data), aref);

    mpz_class bref = tval2;
    BigInt *b = alloc.newObject<BigInt>(tval2);
    ASSERT_EQ(mpz_class(b->data), bref);

    for (int i = 0; i < 20; i++) {
        switch (i % 3) {
            case 0:
                mul(a, a, tval, params);
                aref = (aref * tval) % p;
                break;
            case 1:
                mul(a, tval, a, params);
                aref = (aref * tval) % p;
                break;
            case 2:
                mul(a, a, b, params);
                aref = (aref * bref) % p;
                break;
        }
        ASSERT_EQ(mpz_class(a->data), aref);
    }
    alloc.deleteObject(b);
    alloc.deleteObject(a);
    alloc.deleteObject(params);
}

TEST(BigNum, BigIntNeg) {
    // here, we do a few negations and comparing with a reference
    // mpz_class implementation
    Allocator alloc;
    BigIntParams *params = alloc.newObject<BigIntParams>(2);
    //everything is modulo p
    mpz_class p;
    mpz_ui_pow_ui(p.get_mpz_t(), 2, params->p);

    int64_t tval = std::numeric_limits<int64_t>::min() / 3;
    int64_t tval2 = std::numeric_limits<int64_t>::min() / 5;

    mpz_class aref = tval;
    BigInt *a = alloc.newObject<BigInt>(tval);
    ASSERT_EQ(mpz_class(a->data), aref);

    mul(a, a, tval2, params);
    aref = aref * tval2;
    ASSERT_EQ(mpz_class(a->data), aref);

    neg(a, a, params);
    ASSERT_EQ(mpz_class(a->data), -aref);

    neg(a, a, params);
    ASSERT_EQ(mpz_class(a->data), aref);

    neg(a, tval, params);
    ASSERT_EQ(mpz_class(a->data), -tval);

    neg(a, a, params);
    ASSERT_EQ(mpz_class(a->data), tval);

    alloc.deleteObject(a);
    alloc.deleteObject(params);
}

TEST(BigTorus, FromDouble) {
    Allocator alloc;
    BigIntParams* params = alloc.newObject<BigIntParams>(2);
    BigTorus* a = alloc.newObject<BigTorus>(params, &alloc);
    from_double(a, pow(2., 52), params);
    from_double(a, pow(2., -140), params);
    alloc.deleteObject(a, params, &alloc);
    alloc.deleteObject(params);
}