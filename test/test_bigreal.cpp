#include <gtest/gtest.h>
#include <core/arithmetic/big_real.h>
#include <core/allocator/allocator.h>
#include "tfhetestlib/ntl_utils.h"
#include "tfhetestlib/random_sample.h"


using namespace std;
using namespace tfhe_backend;
using namespace tfhe_test;
using NTL::ZZ;
using NTL::RR;

using namespace std;

TEST(BigReal, zero) {
    Allocator alloc;
    ZModuleParams<BigTorus> *zparams = alloc.newObject<ZModuleParams<BigTorus>>(3);
    RealParams<BigTorus> *params = alloc.newObject<RealParams<BigTorus>>(zparams, 1);
    BigReal *a = alloc.newObject<BigReal>(params, &alloc);

    tfhe_backend::zero(a, params);
    ZZ azz = to_ntl_ZZ(a, params);
    ASSERT_EQ(azz, 0);

    alloc.deleteObject(a, params, &alloc);
    alloc.deleteObject(params);
    alloc.deleteObject(zparams);
}

TEST(BigReal, to_double) {
    Allocator alloc;
    ZModuleParams<BigTorus> *zparams = alloc.newObject<ZModuleParams<BigTorus>>(3);
    RealParams<BigTorus> *params = alloc.newObject<RealParams<BigTorus>>(zparams, 1);
    BigReal *a = alloc.newObject<BigReal>(params, &alloc);

    for (int i = 0; i < 100; ++i) {
        randomBigReal(a, params);
        RR b = centermod_to_ntl_RR(a, params);

        double aval = tfhe_backend::to_double(a, params);
        double bval = to_double(b);

        ASSERT_LE(abs(aval - bval), 10);
    }

    alloc.deleteObject(a, params, &alloc);
    alloc.deleteObject(params);
    alloc.deleteObject(zparams);
}

TEST(BigReal, to_real) {
    Allocator alloc;
    ZModuleParams<BigTorus> *zparams = alloc.newObject<ZModuleParams<BigTorus>>(3);
    RealParams<BigTorus> *params = alloc.newObject<RealParams<BigTorus>>(zparams, 1);
    BigReal *a = alloc.newObject<BigReal>(params, &alloc);
    BigTorus *b = alloc.newObject<BigTorus>(zparams, &alloc);

    for (int i = 0; i < 100; ++i) {
        randomBigTorus(b, zparams);
        tfhe_backend::to_real(a, b, params);

        ZZ azz = to_ntl_ZZ(a, params);
        ZZ bzz = to_ntl_ZZ(b, zparams);

        ZZ p = NTL::power2_ZZ(params->nbBitsFrac);
        ASSERT_EQ(bzz, azz % p); // fractionary part is the same

        azz /= p;
        //switch sign if b is negative
        if (bit(bzz, params->nbBitsFrac - 1))
            ASSERT_EQ(azz, NTL::power2_ZZ(params->nbBitsInt) - 1); // all bits must be 1
        else
            ASSERT_EQ(azz, 0); // all bits are zero
    }

    alloc.deleteObject(b, zparams, &alloc);
    alloc.deleteObject(a, params, &alloc);
    alloc.deleteObject(params);
    alloc.deleteObject(zparams);
}

TEST(BigReal, to_torus) {
    Allocator alloc;
    ZModuleParams<BigTorus> *zparams = alloc.newObject<ZModuleParams<BigTorus>>(3);
    RealParams<BigTorus> *params = alloc.newObject<RealParams<BigTorus>>(zparams, 1);
    BigReal *a = alloc.newObject<BigReal>(params, &alloc);
    BigTorus *b = alloc.newObject<BigTorus>(zparams, &alloc);

    for (int i = 0; i < 100; ++i) {
        randomBigReal(a, params);
        tfhe_backend::to_torus(b, a, params);

        // for (int i = 0; i < params->nbLimbs; ++i)
        //     printf("%ld ", a->data[i]);
        // printf("\n");
        // for (int i = 0; i < params->nbLimbsFrac; ++i)
        //     printf("%ld ", b->data[i]);
        // printf("\n");

        ZZ azz = to_ntl_ZZ(a, params);
        ZZ bzz = to_ntl_ZZ(b, zparams);

        ASSERT_EQ(azz % NTL::power2_ZZ(params->nbBitsFrac), bzz);
    }

    alloc.deleteObject(b, zparams, &alloc);
    alloc.deleteObject(a, params, &alloc);
    alloc.deleteObject(params);
    alloc.deleteObject(zparams);
}


void testBinaryOperation(
        void(*BigRealOp)(BigReal *res, const BigReal *a, const BigReal *b, const RealParams<BigTorus> *params),
        ZZ(*testOp)(ZZ a, ZZ b, const RealParams<BigTorus> *params)
) {
    Allocator alloc;
    for (int K: set<int>{1, 2, 5, 10}) {
        for (int L: set<int>{1, 2, 5, 10}) {
            ZModuleParams<BigTorus> *zparams = alloc.newObject<ZModuleParams<BigTorus>>(K);
            RealParams<BigTorus> *params = alloc.newObject<RealParams<BigTorus>>(zparams, L);
            BigReal *a = alloc.newObject<BigReal>(params, &alloc);
            BigReal *b = alloc.newObject<BigReal>(params, &alloc);
            BigReal *c = alloc.newObject<BigReal>(params, &alloc);
            {
                // a, b, and c different
                randomBigReal(a, params);
                randomBigReal(b, params);
                randomBigReal(c, params);
                ZZ aa = to_ntl_ZZ(a, params);
                ZZ bb = to_ntl_ZZ(b, params);
                BigRealOp(c, a, b, params);
                ZZ cc = to_ntl_ZZ(c, params);
                ASSERT_EQ(to_ntl_ZZ(a, params), aa); //a did not change
                ASSERT_EQ(to_ntl_ZZ(b, params), bb); //b did not change
                ASSERT_EQ(testOp(aa, bb, params), cc); //result is ok
            }
            {
                // a = a + b
                randomBigReal(a, params);
                randomBigReal(b, params);
                randomBigReal(c, params);
                ZZ aa = to_ntl_ZZ(a, params);
                ZZ bb = to_ntl_ZZ(b, params);
                BigRealOp(a, a, b, params);
                ZZ cc = to_ntl_ZZ(a, params);
                ASSERT_EQ(to_ntl_ZZ(b, params), bb); //b did not change
                ASSERT_EQ(testOp(aa, bb, params), cc);
            }
            {
                // b = a + b
                randomBigReal(a, params);
                randomBigReal(b, params);
                randomBigReal(c, params);
                ZZ aa = to_ntl_ZZ(a, params);
                ZZ bb = to_ntl_ZZ(b, params);
                BigRealOp(b, a, b, params);
                ZZ cc = to_ntl_ZZ(b, params);
                ASSERT_EQ(to_ntl_ZZ(a, params), aa); //a did not change
                ASSERT_EQ(testOp(aa, bb, params), cc);
            }
            {
                // a = a + a
                randomBigReal(a, params);
                randomBigReal(b, params);
                randomBigReal(c, params);
                ZZ aa = to_ntl_ZZ(a, params);
                BigRealOp(a, a, a, params);
                ZZ cc = to_ntl_ZZ(a, params);
                ASSERT_EQ(testOp(aa, aa, params), cc);
            }
            alloc.deleteObject(c, params, &alloc);
            alloc.deleteObject(b, params, &alloc);
            alloc.deleteObject(a, params, &alloc);
            alloc.deleteObject(params);
            alloc.deleteObject(zparams);
        }
    }
}

static ZZ testBigRealAdd(ZZ aa, ZZ bb, const RealParams<BigTorus> *params) {
    return (aa + bb) % NTL::power2_ZZ(params->nbBits);
}

TEST(BigReal, add) {
    for (int i = 0; i < 100; ++i)
        testBinaryOperation(tfhe_backend::add, testBigRealAdd);
}


void testBinaryOperation(
        void(*BigRealOp)(BigReal *res, const BigReal *a, const BigReal *b, const RealParams<BigTorus> *params,
                         Allocator alloc),
        ZZ(*testOp)(ZZ a, ZZ b, const RealParams<BigTorus> *params),
        void(*compRes)(ZZ a, ZZ b)
) {
    Allocator alloc0;
    Allocator alloc = alloc0.createStackChildAllocator();
    for (int K: set<int>{1, 2, 5, 10}) {
        for (int L: set<int>{1, 2, 5, 10}) {
            ZModuleParams<BigTorus> *zparams = alloc.newObject<ZModuleParams<BigTorus>>(K);
            RealParams<BigTorus> *params = alloc.newObject<RealParams<BigTorus>>(zparams, L);
            BigReal *a = alloc.newObject<BigReal>(params, &alloc);
            BigReal *b = alloc.newObject<BigReal>(params, &alloc);
            BigReal *c = alloc.newObject<BigReal>(params, &alloc);
            {
                // a, b, and c different
                randomBigReal(a, params);
                randomBigReal(b, params);
                randomBigReal(c, params);
                ZZ aa = to_ntl_ZZ(a, params);
                ZZ bb = to_ntl_ZZ(b, params);
                BigRealOp(c, a, b, params, alloc.createStackChildAllocator());
                ZZ cc = to_ntl_ZZ(c, params);
                ASSERT_EQ(to_ntl_ZZ(a, params), aa); //a did not change
                ASSERT_EQ(to_ntl_ZZ(b, params), bb); //b did not change
                compRes(testOp(aa, bb, params), cc); //result is ok
            }
            {
                // a = a + b
                randomBigReal(a, params);
                randomBigReal(b, params);
                randomBigReal(c, params);
                ZZ aa = to_ntl_ZZ(a, params);
                ZZ bb = to_ntl_ZZ(b, params);
                BigRealOp(a, a, b, params, alloc.createStackChildAllocator());
                ZZ cc = to_ntl_ZZ(a, params);
                ASSERT_EQ(to_ntl_ZZ(b, params), bb); //b did not change
                compRes(testOp(aa, bb, params), cc);
            }
            {
                // b = a + b
                randomBigReal(a, params);
                randomBigReal(b, params);
                randomBigReal(c, params);
                ZZ aa = to_ntl_ZZ(a, params);
                ZZ bb = to_ntl_ZZ(b, params);
                BigRealOp(b, a, b, params, alloc.createStackChildAllocator());
                ZZ cc = to_ntl_ZZ(b, params);
                ASSERT_EQ(to_ntl_ZZ(a, params), aa); //a did not change
                compRes(testOp(aa, bb, params), cc);
            }
            {
                // a = a + a
                randomBigReal(a, params);
                randomBigReal(b, params);
                randomBigReal(c, params);
                ZZ aa = to_ntl_ZZ(a, params);
                BigRealOp(a, a, a, params, alloc.createStackChildAllocator());
                ZZ cc = to_ntl_ZZ(a, params);
                compRes(testOp(aa, aa, params), cc);
            }
            alloc.deleteObject(c, params, &alloc);
            alloc.deleteObject(b, params, &alloc);
            alloc.deleteObject(a, params, &alloc);
            alloc.deleteObject(params);
            alloc.deleteObject(zparams);
        }
    }
}

void compEq(ZZ a, ZZ b) {
    ASSERT_EQ(a, b);
}

void compEqApprox(ZZ a, ZZ b) {
    ASSERT_LE(abs(a - b), 1);
}

static ZZ testBigRealMulNaiveRound(ZZ aa, ZZ bb, const RealParams<BigTorus> *params) {
    ZZ cc = aa * bb;
    cc %= NTL::power2_ZZ(params->nbBits + params->nbBitsFrac);
    cc >>= params->nbBitsFrac - 1;
    cc += 1;
    cc >>= 1;

    return cc;
}

TEST(BigReal, mul_naive_round) {
    for (int i = 0; i < 100; ++i)
        testBinaryOperation(tfhe_backend::mul_naive_round, testBigRealMulNaiveRound, compEq);
}

static ZZ testBigRealMulNaiveTrunc(ZZ aa, ZZ bb, const RealParams<BigTorus> *params) {
    ZZ cc = aa * bb;
    cc %= NTL::power2_ZZ(params->nbBits + params->nbBitsFrac);
    cc >>= params->nbBitsFrac;
    return cc;
}

TEST(BigReal, mul_naive_trunc) {
    for (int i = 0; i < 100; ++i)
        testBinaryOperation(tfhe_backend::mul_naive_trunc, testBigRealMulNaiveTrunc, compEq);
}

TEST(BigReal, mul) {
    for (int i = 0; i < 100; ++i)
        testBinaryOperation(tfhe_backend::mul, testBigRealMulNaiveTrunc, compEqApprox);
}
