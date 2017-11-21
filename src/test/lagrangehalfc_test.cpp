#include <gtest/gtest.h>
#include <polynomials.h>
#include <cassert>
#include <cmath>
#include <tfhe.h>
#include <polynomials.h>

using namespace std;


/**
 * FFT functions 
 */
//EXPORT void TorusPolynomial_ifft(LagrangeHalfCPolynomial* result, const TorusPolynomial* p);
//EXPORT void TorusPolynomial_fft(TorusPolynomial* result, const LagrangeHalfCPolynomial* p);
TEST(LagrangeHalfcTest, fftIsBijective) {
    const int32_t NBTRIALS = 10;
    const double toler = 1e-9;
    const int32_t N = 1024;
    for (int32_t trials = 0; trials < NBTRIALS; ++trials) {
        TorusPolynomial *a = new_TorusPolynomial(N);
        TorusPolynomial *acopy = new_TorusPolynomial(N);
        TorusPolynomial *b = new_TorusPolynomial(N);
        LagrangeHalfCPolynomial *afft = new_LagrangeHalfCPolynomial(N);
        torusPolynomialUniform(a);
        torusPolynomialCopy(acopy, a);
        TorusPolynomial_ifft(afft, a);
        TorusPolynomial_fft(b, afft);
        ASSERT_EQ(torusPolynomialNormInftyDist(a, acopy), 0);
        ASSERT_LE(torusPolynomialNormInftyDist(a, b), toler);
        delete_LagrangeHalfCPolynomial(afft);
        delete_TorusPolynomial(b);
        delete_TorusPolynomial(acopy);
        delete_TorusPolynomial(a);
    }
}


//EXPORT void IntPolynomial_ifft(LagrangeHalfCPolynomial* result, const IntPolynomial* p);

//MISC OPERATIONS
/** sets to zero */
//EXPORT void LagrangeHalfCPolynomialClear(LagrangeHalfCPolynomial* result);
TEST(LagrangeHalfcTest, LagrangeHalfCPolynomialClear) {
    const int32_t NBTRIALS = 10;
    //const double toler=1e-9;
    const int32_t N = 1024;
    for (int32_t trials = 0; trials < NBTRIALS; ++trials) {
        TorusPolynomial *a = new_TorusPolynomial(N);
        TorusPolynomial *zero = new_TorusPolynomial(N);
        LagrangeHalfCPolynomial *afft = new_LagrangeHalfCPolynomial(N);
        LagrangeHalfCPolynomialClear(afft);
        torusPolynomialUniform(a);
        torusPolynomialClear(zero);
        TorusPolynomial_fft(a, afft);
        ASSERT_EQ(torusPolynomialNormInftyDist(zero, a), 0);
        delete_LagrangeHalfCPolynomial(afft);
        delete_TorusPolynomial(zero);
        delete_TorusPolynomial(a);
    }
}

/** sets to this torus32 constant */
//EXPORT void LagrangeHalfCPolynomialSetTorusConstant(LagrangeHalfCPolynomial* result, const Torus32 mu);
TEST(LagrangeHalfcTest, LagrangeHalfCPolynomialSetTorusConstant) {
    const int32_t NBTRIALS = 10;
    //const double toler=1e-9;
    const int32_t N = 1024;
    for (int32_t trials = 0; trials < NBTRIALS; ++trials) {
        Torus32 mu = uniformTorus32_distrib(generator);
        TorusPolynomial *a = new_TorusPolynomial(N);
        TorusPolynomial *cste = new_TorusPolynomial(N);
        LagrangeHalfCPolynomial *afft = new_LagrangeHalfCPolynomial(N);
        torusPolynomialUniform(a);

        //tested function
        LagrangeHalfCPolynomialSetTorusConstant(afft, mu);
        TorusPolynomial_fft(a, afft);

        //expected result
        torusPolynomialClear(cste);
        cste->coefsT[0] = mu;

        ASSERT_EQ(torusPolynomialNormInftyDist(cste, a), 0);

        delete_LagrangeHalfCPolynomial(afft);
        delete_TorusPolynomial(cste);
        delete_TorusPolynomial(a);
    }
}


//EXPORT void LagrangeHalfCPolynomialAddTorusConstant(LagrangeHalfCPolynomial* result, const Torus32 cst);
TEST(LagrangeHalfcTest, LagrangeHalfCPolynomialAddTorusConstant) {
    const int32_t NBTRIALS = 10;
    const double toler = 1e-9;
    const int32_t N = 1024;
    for (int32_t trials = 0; trials < NBTRIALS; ++trials) {
        Torus32 mu = uniformTorus32_distrib(generator);
        TorusPolynomial *a = new_TorusPolynomial(N);
        TorusPolynomial *aPlusCste = new_TorusPolynomial(N);
        TorusPolynomial *b = new_TorusPolynomial(N);
        LagrangeHalfCPolynomial *afft = new_LagrangeHalfCPolynomial(N);

        torusPolynomialUniform(a);
        TorusPolynomial_ifft(afft, a);
        LagrangeHalfCPolynomialAddTorusConstant(afft, mu);
        TorusPolynomial_fft(b, afft);

        torusPolynomialCopy(aPlusCste, a);
        aPlusCste->coefsT[0] += mu;

        ASSERT_LE(torusPolynomialNormInftyDist(aPlusCste, b), toler);

        delete_LagrangeHalfCPolynomial(afft);
        delete_TorusPolynomial(b);
        delete_TorusPolynomial(aPlusCste);
        delete_TorusPolynomial(a);
    }
}


/** sets to X^ai-1 */
//NICOLAS: do not test this function, as it will likely be removed (cmux formula)
//EXPORT void LagrangeHalfCPolynomialSetXaiMinusOne(LagrangeHalfCPolynomial* result, const int32_t ai);

/** multiplication via direct FFT */
EXPORT void torusPolynomialMultFFT(TorusPolynomial *result, const IntPolynomial *poly1, const TorusPolynomial *poly2);

TEST(LagrangeHalfcTest, torusPolynomialMultFFT) {
    const int32_t NBTRIALS = 10;
    const double toler = 1e-9;
    const int32_t N = 1024;
    for (int32_t trials = 0; trials < NBTRIALS; ++trials) {
        IntPolynomial *a = new_IntPolynomial(N);
        TorusPolynomial *b = new_TorusPolynomial(N);
        TorusPolynomial *aB = new_TorusPolynomial(N);
        TorusPolynomial *aBref = new_TorusPolynomial(N);


        for (int32_t i = 0; i < N; i++) a->coefs[i] = uniformTorus32_distrib(generator) % 1000 - 500;
        torusPolynomialUniform(b);
        torusPolynomialMultKaratsuba(aBref, a, b);

        torusPolynomialMultFFT(aB, a, b);

        ASSERT_LE(torusPolynomialNormInftyDist(aB, aBref), toler);

        delete_TorusPolynomial(aBref);
        delete_TorusPolynomial(aB);
        delete_TorusPolynomial(b);
        delete_IntPolynomial(a);
    }
}

EXPORT void
torusPolynomialAddMulRFFT(TorusPolynomial *result, const IntPolynomial *poly1, const TorusPolynomial *poly2);

TEST(LagrangeHalfcTest, torusPolynomialAddMulRFFT) {
    const int32_t NBTRIALS = 10;
    const double toler = 1e-9;
    const int32_t N = 1024;
    for (int32_t trials = 0; trials < NBTRIALS; ++trials) {
        IntPolynomial *a = new_IntPolynomial(N);
        TorusPolynomial *b = new_TorusPolynomial(N);
        TorusPolynomial *aB = new_TorusPolynomial(N);
        TorusPolynomial *aBref = new_TorusPolynomial(N);


        for (int32_t i = 0; i < N; i++) a->coefs[i] = uniformTorus32_distrib(generator) % 1000 - 500;
        torusPolynomialUniform(b);
        torusPolynomialUniform(aB);
        torusPolynomialCopy(aBref, aB);

        torusPolynomialAddMulRKaratsuba(aBref, a, b);

        torusPolynomialAddMulRFFT(aB, a, b);

        ASSERT_LE(torusPolynomialNormInftyDist(aB, aBref), toler);

        delete_TorusPolynomial(aBref);
        delete_TorusPolynomial(aB);
        delete_TorusPolynomial(b);
        delete_IntPolynomial(a);
    }
}

EXPORT void
torusPolynomialSubMulRFFT(TorusPolynomial *result, const IntPolynomial *poly1, const TorusPolynomial *poly2);

TEST(LagrangeHalfcTest, torusPolynomialSubMulRFFT) {
    const int32_t NBTRIALS = 10;
    const double toler = 1e-9;
    const int32_t N = 1024;
    for (int32_t trials = 0; trials < NBTRIALS; ++trials) {
        IntPolynomial *a = new_IntPolynomial(N);
        TorusPolynomial *b = new_TorusPolynomial(N);
        TorusPolynomial *aB = new_TorusPolynomial(N);
        TorusPolynomial *aBref = new_TorusPolynomial(N);


        for (int32_t i = 0; i < N; i++) a->coefs[i] = uniformTorus32_distrib(generator) % 1000 - 500;
        torusPolynomialUniform(b);
        torusPolynomialUniform(aB);
        torusPolynomialCopy(aBref, aB);

        torusPolynomialSubMulRKaratsuba(aBref, a, b);

        torusPolynomialSubMulRFFT(aB, a, b);

        ASSERT_LE(torusPolynomialNormInftyDist(aB, aBref), toler);

        delete_TorusPolynomial(aBref);
        delete_TorusPolynomial(aB);
        delete_TorusPolynomial(b);
        delete_IntPolynomial(a);
    }
}

/** termwise addTo in Lagrange space */
//EXPORT void LagrangeHalfCPolynomialAddTo(
//	LagrangeHalfCPolynomial* accum, 
//	const LagrangeHalfCPolynomial* a);
TEST(LagrangeHalfcTest, LagrangeHalfCPolynomialAddTo) {
    const int32_t NBTRIALS = 10;
    const double toler = 1e-9;
    const int32_t N = 1024;
    for (int32_t trials = 0; trials < NBTRIALS; ++trials) {
        TorusPolynomial *a = new_TorusPolynomial(N);
        TorusPolynomial *b = new_TorusPolynomial(N);
        TorusPolynomial *aPlusB = new_TorusPolynomial(N);
        TorusPolynomial *aPlusBbis = new_TorusPolynomial(N);

        LagrangeHalfCPolynomial *afft = new_LagrangeHalfCPolynomial(N);
        LagrangeHalfCPolynomial *bfft = new_LagrangeHalfCPolynomial(N);

        torusPolynomialUniform(a);
        TorusPolynomial_ifft(afft, a);
        torusPolynomialUniform(b);
        TorusPolynomial_ifft(bfft, b);

        LagrangeHalfCPolynomialAddTo(afft, bfft);
        TorusPolynomial_fft(aPlusBbis, afft);

        torusPolynomialAdd(aPlusB, b, a);

        ASSERT_LE(torusPolynomialNormInftyDist(aPlusBbis, aPlusB), toler);

        delete_LagrangeHalfCPolynomial(bfft);
        delete_LagrangeHalfCPolynomial(afft);
        delete_TorusPolynomial(aPlusBbis);
        delete_TorusPolynomial(aPlusB);
        delete_TorusPolynomial(b);
        delete_TorusPolynomial(a);
    }
}

/** termwise multiplication in Lagrange space */
//NICOLAS: do not test this function (implem dependent)
//EXPORT void LagrangeHalfCPolynomialMul(
//	LagrangeHalfCPolynomial* result, 
//	const LagrangeHalfCPolynomial* a, 
//	const LagrangeHalfCPolynomial* b);


//NICOLAS: do not test this function (implem dependent)
//EXPORT void LagrangeHalfCPolynomialAddMul(
//	LagrangeHalfCPolynomial* accum, 
//	const LagrangeHalfCPolynomial* a, 
//	const LagrangeHalfCPolynomial* b);

//NICOLAS: do not test this function (implem dependent)
//EXPORT void LagrangeHalfCPolynomialSubMul(
//	LagrangeHalfCPolynomial* accum, 
//	const LagrangeHalfCPolynomial* a, 
//	const LagrangeHalfCPolynomial* b);
