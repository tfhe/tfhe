#include <gtest/gtest.h>
#include <core/arithmetic/BigInt.h>
#include <core/allocator/allocator.h>
#include <core/arithmetic/BigTorus.h>
#include <gmp.h>

#include "gmpxx.h"

using namespace std;

TEST(BigNum, BigIntAllocSet) {
    // here, we test a few constructors with or without value
    // we also test the copyvalue functions
    Allocator alloc;
    ZModuleParams<BigTorus> *params = alloc.newObject<ZModuleParams<BigTorus>>(2);
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
    ZModuleParams<BigTorus> *params = alloc.newObject<ZModuleParams<BigTorus>>(2);
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
    ZModuleParams<BigTorus> *params = alloc.newObject<ZModuleParams<BigTorus>>(2);
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
    ZModuleParams<BigTorus> *params = alloc.newObject<ZModuleParams<BigTorus>>(2);
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
    ZModuleParams<BigTorus> *params = alloc.newObject<ZModuleParams<BigTorus>>(2);
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

//quick and dirty function to convert a bigTorus to a gmp float
mpf_class fval(const BigTorus *x, const ZModuleParams<BigTorus> *params) {
    __mpz_struct tmp;
    tmp._mp_d = x->data;
    tmp._mp_size = params->max_nbLimbs;
    tmp._mp_alloc = params->max_nbLimbs;
    return mpf_class(mpz_class(&tmp)) / pow(2., params->p);
}

//quick and dirty function to convert a bigTorus to a gmp integer
mpz_class zval(const BigTorus *x, const ZModuleParams<BigTorus> *params) {
    int K;
    for (K = params->max_nbLimbs; K > 0; K--) {
        if (x->data[K - 1] != 0) {
            __mpz_struct tmp;
            tmp._mp_d = x->data;
            tmp._mp_size = K;
            tmp._mp_alloc = params->max_nbLimbs;
            return mpz_class(&tmp);
        }
    }
    return 0;
}

//quick and dirty function to convert a bigTorus modulus to 2^p
mpz_class zmodu(const ZModuleParams<BigTorus> *params) {
    return mpz_class(1) << params->p;
}

//quick and dirty function to fill a bigtorus with random
void randomBigTorus(BigTorus *res, const ZModuleParams<BigTorus> *params) {
    for (int i = 0; i < params->max_nbLimbs; i++) {
        res->data[i] = rand();
    }
}

//quick and dirty function to fill a bigtorus with random
void randomBigInt(BigInt *res, const ZModuleParams<BigTorus> *params) {
    setvalue(res, rand() - RAND_MAX / 2, params);
    for (int i = 1; i < 2 * params->max_nbLimbs; i++) {
        mul(res, res, rand(), params);
    }
}

TEST(BigTorus, FromDouble) {
    Allocator alloc;
    ZModuleParams<BigTorus> *params = alloc.newObject<ZModuleParams<BigTorus>>(2);
    BigTorus *a = alloc.newObject<BigTorus>(params, &alloc);
    randomBigTorus(a, params);
    for (int i = 0; i < 100; i++) {
        double tval = 2. * (rand() / double(RAND_MAX) - 0.5);
        tval *= pow(2., -(i / 3));
        from_double(a, tval, params);
        mpf_class atrans = fval(a, params);
        //cout << "test val " << tval << " vs " << atrans << endl;
        mpf_class diff = atrans - tval;
        ASSERT_EQ(diff - floor(diff + 0.5), 0.);
    }
    from_double(a, pow(2., -140), params);
    alloc.deleteObject(a, params, &alloc);
    alloc.deleteObject(params);
}

TEST(BigTorus, toDouble) {
    Allocator alloc;
    ZModuleParams<BigTorus> *params = alloc.newObject<ZModuleParams<BigTorus>>(2);
    BigTorus *a = alloc.newObject<BigTorus>(params, &alloc);
    for (int i = 0; i < 100; i++) {
        double tval = 2. * (rand() / double(RAND_MAX) - 0.5);
        tval *= pow(2., -(i / 3));
        from_double(a, tval, params);
        double rval = to_double(a, params);
        double diff = rval - tval;
        ASSERT_LE(abs(diff - floor(diff + 0.5)), 1e-15);
    }
    alloc.deleteObject(a, params, &alloc);
    alloc.deleteObject(params);
}

/*
 * @brief res = 0
 */
//void zero(BigTorus *res, const ZModuleParams *params);
TEST(BigTorus, zero) {
    Allocator alloc;
    for (int K = 1; K < 10; K++) {
        ZModuleParams<BigTorus> *params = alloc.newObject<ZModuleParams<BigTorus>>(K);
        BigTorus *a = alloc.newObject<BigTorus>(params, &alloc);
        randomBigTorus(a, params);
        zero(a, params);
        ASSERT_TRUE(mpn_zero_p(a->data, K));
        alloc.deleteObject(a, params, &alloc);
        alloc.deleteObject(params);
    }
}

void testPow2(const BigTorus *a, const int k, const ZModuleParams<BigTorus> *params) {
    int blocpos = k / 64;
    int inpos = k % 64;
    for (int i = 0; i < params->max_nbLimbs; i++) {
        if (i != blocpos) {
            ASSERT_EQ(a->data[i], 0);
        } else {
            ASSERT_EQ(a->data[i], UINT64_C(1) << inpos);
        }
    }
}

/*
 * @brief res = 2^-k
 */
//void setPowHalf(BigTorus *res, const int k, const ZModuleParams *params);
TEST(BigTorus, setPowHalf) {
    Allocator alloc;
    for (int K: set<int>{2, 5, 10}) {
        ZModuleParams<BigTorus> *params = alloc.newObject<ZModuleParams<BigTorus>>(K);
        BigTorus *a = alloc.newObject<BigTorus>(params, &alloc);
        {
            //k negative
            randomBigTorus(a, params);
            setPowHalf(a, -1, params);
            ASSERT_TRUE(mpn_zero_p(a->data, K));
            randomBigTorus(a, params);
            setPowHalf(a, 0, params);
            ASSERT_TRUE(mpn_zero_p(a->data, K));
            //k > p
            randomBigTorus(a, params);
            setPowHalf(a, params->p + 1, params);
            ASSERT_TRUE(mpn_zero_p(a->data, K));
            randomBigTorus(a, params);
            setPowHalf(a, params->p + 5, params);
            ASSERT_TRUE(mpn_zero_p(a->data, K));
        }
        for (int k = 1; k < params->p; k += 5) {
            randomBigTorus(a, params);
            setPowHalf(a, k, params);
            testPow2(a, params->p - k, params);
        }
        alloc.deleteObject(a, params, &alloc);
        alloc.deleteObject(params);
    }
}

void testBinaryOperation(
        void(*BigTorusOp)(BigTorus *res, const BigTorus *a, const BigTorus *b, const ZModuleParams<BigTorus> *params),
        mpz_class(*testOp)(mpz_class a, mpz_class b, const ZModuleParams<BigTorus> *params)
) {
    Allocator alloc;
    for (int K: set<int>{2, 5, 10}) {
        ZModuleParams<BigTorus> *params = alloc.newObject<ZModuleParams<BigTorus>>(K);
        BigTorus *a = alloc.newObject<BigTorus>(params, &alloc);
        BigTorus *b = alloc.newObject<BigTorus>(params, &alloc);
        BigTorus *c = alloc.newObject<BigTorus>(params, &alloc);
        {
            // a, b, and c different
            randomBigTorus(a, params);
            randomBigTorus(b, params);
            randomBigTorus(c, params);
            mpz_class aa = zval(a, params);
            mpz_class bb = zval(b, params);
            BigTorusOp(c, a, b, params);
            mpz_class cc = zval(c, params);
            ASSERT_EQ(zval(a, params), aa); //a did not change
            ASSERT_EQ(zval(b, params), bb); //b did not change
            ASSERT_EQ(testOp(aa, bb, params), cc); //result is ok
        }
        {
            // a = a + b
            randomBigTorus(a, params);
            randomBigTorus(b, params);
            randomBigTorus(c, params);
            mpz_class aa = zval(a, params);
            mpz_class bb = zval(b, params);
            BigTorusOp(a, a, b, params);
            mpz_class cc = zval(a, params);
            ASSERT_EQ(zval(b, params), bb); //b did not change
            ASSERT_EQ(testOp(aa, bb, params), cc);
        }
        {
            // b = a + b
            randomBigTorus(a, params);
            randomBigTorus(b, params);
            randomBigTorus(c, params);
            mpz_class aa = zval(a, params);
            mpz_class bb = zval(b, params);
            BigTorusOp(b, a, b, params);
            mpz_class cc = zval(b, params);
            ASSERT_EQ(zval(a, params), aa); //a did not change
            ASSERT_EQ(testOp(aa, bb, params), cc);
        }
        {
            // a = a + a
            randomBigTorus(a, params);
            randomBigTorus(b, params);
            randomBigTorus(c, params);
            mpz_class aa = zval(a, params);
            BigTorusOp(a, a, a, params);
            mpz_class cc = zval(a, params);
            ASSERT_EQ(testOp(aa, aa, params), cc);
        }
        alloc.deleteObject(c, params, &alloc);
        alloc.deleteObject(b, params, &alloc);
        alloc.deleteObject(a, params, &alloc);
        alloc.deleteObject(params);
    }
}


mpz_class testBigTAdd(mpz_class aa, mpz_class bb, const ZModuleParams<BigTorus> *params) {
    return (aa + bb) % zmodu(params);
}

/*
 * @brief res = a + b
 */
//void add(BigTorus *res, const BigTorus *a, const BigTorus *b, const ZModuleParams *params);
TEST(BigTorus, addition) {
    testBinaryOperation(add, testBigTAdd);
}

mpz_class testBigTSub(mpz_class aa, mpz_class bb, const ZModuleParams<BigTorus> *params) {
    return (aa - bb + zmodu(params)) % zmodu(params);
}

/*
 * @brief res = a - b
 */
//void sub(BigTorus *res, const BigTorus *a, const BigTorus *b, const ZModuleParams *params);
TEST(BigTorus, subtraction) {
    testBinaryOperation(sub, testBigTSub);
}

mpz_class posmod(mpz_class a, mpz_class b) { return ((a % b) + b) % b; }

/*
 * @brief res = a * b
 */
//void mul(BigTorus *res, int64_t a, const BigTorus *b, const ZModuleParams *params);
TEST(BigTorus, multInt) {
    Allocator alloc;
    for (int K: set<int>{2, 5, 10}) {
        ZModuleParams<BigTorus> *params = alloc.newObject<ZModuleParams<BigTorus>>(K);
        BigTorus *a = alloc.newObject<BigTorus>(params, &alloc);
        BigTorus *b = alloc.newObject<BigTorus>(params, &alloc);
        for (int trials = 0; trials < 5; trials++) {
            // a, b different
            randomBigTorus(a, params);
            randomBigTorus(b, params);
            int64_t x = rand() - RAND_MAX / 2;
            mpz_class aa = zval(a, params);
            mpz_class bb = zval(b, params);
            mul(b, x, a, params);
            mpz_class cc = zval(b, params);
            ASSERT_EQ(zval(a, params), aa); //a did not change
            ASSERT_EQ(posmod(x * aa, zmodu(params)), cc); //result is ok
        }
        for (int trials = 0; trials < 5; trials++) {
            // a = x*a
            randomBigTorus(a, params);
            int64_t x = rand() - RAND_MAX / 2;
            mpz_class aa = zval(a, params);
            mul(a, x, a, params);
            mpz_class cc = zval(a, params);
            ASSERT_EQ(posmod(x * aa, zmodu(params)), cc);
        }
        alloc.deleteObject(b, params, &alloc);
        alloc.deleteObject(a, params, &alloc);
        alloc.deleteObject(params);
    }

}

/*
 * @brief res = a * b
 * WARNING: for this function, res and b must not overlap.
 */
//void mul(BigTorus *res, const BigInt *a, const BigTorus *b, const ZModuleParams *params);
TEST(BigTorus, multBigInt) {
    Allocator alloc;
    for (int K: set<int>{2, 5, 10}) {
        ZModuleParams<BigTorus> *params = alloc.newObject<ZModuleParams<BigTorus>>(K);
        BigTorus *a = alloc.newObject<BigTorus>(params, &alloc);
        BigInt *x = alloc.newObject<BigInt>();
        BigTorus *b = alloc.newObject<BigTorus>(params, &alloc);
        for (int trials = 0; trials < 5; trials++) {
            // a, c different
            randomBigTorus(a, params);
            randomBigInt(x, params);
            randomBigTorus(b, params);
            mpz_class aa = zval(a, params);
            mpz_class bb = zval(b, params);
            mul(b, x, a, params, alloc.createStackChildAllocator());
            mpz_class cc = zval(b, params);
            ASSERT_EQ(zval(a, params), aa); //a did not change
            ASSERT_EQ(posmod(aa * mpz_class(x->data), zmodu(params)), cc); //result is ok
        }
        for (int trials = 0; trials < 5; trials++) {
            // a = x*a
            randomBigTorus(a, params);
            randomBigInt(x, params);
            mpz_class aa = zval(a, params);
            mul(a, x, a, params, alloc.createStackChildAllocator());
            mpz_class cc = zval(a, params);
            ASSERT_EQ(posmod(aa * mpz_class(x->data), zmodu(params)), cc);
        }
        alloc.deleteObject(b, params, &alloc);
        alloc.deleteObject(x);
        alloc.deleteObject(a, params, &alloc);
        alloc.deleteObject(params);
    }
}

/*
 * @brief res = -a
 */
//void neg(BigTorus *res, BigTorus *a, const ZModuleParams *params);
TEST(BigTorus, negate) {
    Allocator alloc;
    for (int K: set<int>{2, 5, 10}) {
        ZModuleParams<BigTorus> *params = alloc.newObject<ZModuleParams<BigTorus>>(K);
        BigTorus *a = alloc.newObject<BigTorus>(params, &alloc);
        BigTorus *b = alloc.newObject<BigTorus>(params, &alloc);
        for (int trials = 0; trials < 5; trials++) {
            // a, c different
            randomBigTorus(a, params);
            randomBigTorus(b, params);
            mpz_class aa = zval(a, params);
            mpz_class bb = zval(b, params);
            neg(b, a, params);
            mpz_class cc = zval(b, params);
            ASSERT_EQ(zval(a, params), aa); //a did not change
            ASSERT_EQ(posmod(-aa, zmodu(params)), cc); //result is ok
        }
        for (int trials = 0; trials < 5; trials++) {
            // a = x*a
            randomBigTorus(a, params);
            mpz_class aa = zval(a, params);
            neg(a, a, params);
            mpz_class cc = zval(a, params);
            ASSERT_EQ(posmod(-aa, zmodu(params)), cc);
        }
        alloc.deleteObject(b, params, &alloc);
        alloc.deleteObject(a, params, &alloc);
        alloc.deleteObject(params);
    }
}

/*
 * @brief Rounds the torus value to the nearest multiple of 1/Msize
 * @param phase torus value
 * @param Msize discrete space size
 * @return approximated torus value
 */
//void approxPhase(BigTorus *res, const BigTorus *phase, uint64_t Msize, ZModuleParams *params, Allocator alloc);

/*
 * @brief Mod-Rescale from the torus to Z/Msize.Z
 *  i.e. computes roundToNearestInteger(Msize*phase)
 * @param phase torus value to convert
 * @param Msize discrete space size
 * @return discrete space value in [0, MSize[
 */
//uint64_t modSwitchFromTorus(const BigTorus *phase, uint64_t Msize, ZModuleParams *params, Allocator alloc);
TEST(BigTorus, modSwitchFromTorus) {
    Allocator alloc;
    for (int K: set<int>{2, 5, 10}) {
        ZModuleParams<BigTorus> *params = alloc.newObject<ZModuleParams<BigTorus>>(K);
        BigTorus *a = alloc.newObject<BigTorus>(params, &alloc);
        for (int trials = 0; trials < 10; trials++) {
            //pick a random phase and a random message space
            double x = rand() / double(RAND_MAX) + rand() / pow(double(RAND_MAX), 2.);
            uint64_t Msize = rand() + 2;
            uint64_t expected_mu = (uint64_t) floor(Msize * (x + 1. / (2. * Msize)));
            from_double(a, x, params);
            uint64_t mu = modSwitchFromTorus(a, Msize, params, alloc.createStackChildAllocator());
            ASSERT_EQ(mu, expected_mu);
        }
        alloc.deleteObject(a, params, &alloc);
        alloc.deleteObject(params);
    }
}

/*
 * @brief Converts discrete message space to torus
 *  i.e. value mu/Msize to a torus for mu
 * @param mu discrete space value (from [0,Msize[) to convert
 * @param Msize discrete space size
 * @return torus value
 */
//void modSwitchToTorus(BigTorus *res, const uint64_t mu, const uint64_t Msize, ZModuleParams *params, Allocator alloc);
TEST(BigTorus, modSwitchToTorus) {
    Allocator alloc;
    for (int K: set<int>{2, 5, 10}) {
        ZModuleParams<BigTorus> *params = alloc.newObject<ZModuleParams<BigTorus>>(K);
        BigTorus *a = alloc.newObject<BigTorus>(params, &alloc);
        for (int trials = 0; trials < 10; trials++) {
            //pick a random phase and a random message space
            uint64_t Msize = rand() + 2;
            uint64_t mu = rand() % Msize;
            modSwitchToTorus(a, mu, Msize, params, alloc.createStackChildAllocator());
            double r = to_double(a, params) - double(mu) / Msize;
            ASSERT_LE(abs(r - rint(r)), 1e-20);
        }
        alloc.deleteObject(a, params, &alloc);
        alloc.deleteObject(params);
    }
}

