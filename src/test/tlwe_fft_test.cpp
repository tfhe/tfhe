/* 
 * tlwe_fft_test.cpp
 * Tests the fft functions defined in /src/include/tlwe_functions.h
 * A set of parameters ad keys is defined in the beginning (N=512,1024,2048 and k=1,2)
 */


#include <gtest/gtest.h>
#include <tlwe.h>
#include <tlwe_functions.h>
#include <tgsw_functions.h>
#include <numeric_functions.h>
#include <polynomials_arithmetic.h>
#include <lagrangehalfc_arithmetic.h>
#include "fakes/lagrangehalfc.h"

#define TFHE_TEST_ENVIRONMENT 1

using namespace std;
using namespace ::testing;

namespace {

    /* Tolerance factor for the equality between two TorusPolynomial */
    //const double toler = 1e-8;

    const TLweParams *params1024 = new TLweParams(1024, 1, 0.1, 0.2);

    const set<const TLweParams *> all_params = {params1024};

    class TLweFFTTest : public ::testing::Test {
    public:

        USE_FAKE_new_LagrangeHalfCPolynomial;

        USE_FAKE_new_LagrangeHalfCPolynomial_array;

        USE_FAKE_delete_LagrangeHalfCPolynomial;

        USE_FAKE_delete_LagrangeHalfCPolynomial_array;

        USE_FAKE_TorusPolynomial_ifft;

        USE_FAKE_TorusPolynomial_fft;

        USE_FAKE_LagrangeHalfCPolynomialClear;

        USE_FAKE_LagrangeHalfCPolynomialAddMul;

#define INCLUDE_ALL

#include "../libtfhe/tlwe-fft-operations.cpp"

        inline void tlweSampleUniform(TLweSample *sample, const TLweParams *params) {
            for (int32_t i = 0; i <= params->k; i++)
                torusPolynomialUniform(sample->a + i);
            sample->current_variance = rand() / double(RAND_MAX);
        }

        inline void tlweSampleFFTUniform(TLweSampleFFT *sample, const TLweParams *params) {
            FakeLagrangeHalfCPolynomial *samplea = fake(sample->a);
            TorusPolynomial *tmp = new_TorusPolynomial(params->N);
            for (int32_t i = 0; i <= params->k; i++) {
                torusPolynomialUniform(tmp);
                samplea[i].set_torusPolynomial(tmp);
            }
            sample->current_variance = rand() / double(RAND_MAX);
            delete_TorusPolynomial(tmp);
        }

        inline void intLagrangeHalfCPolynomialUniform(LagrangeHalfCPolynomial *a, int32_t N) {
            FakeLagrangeHalfCPolynomial *fa = fake(a);
            IntPolynomial *tmp = new_IntPolynomial(N);
            for (int32_t i = 0; i < N; i++) {
                tmp->coefs[i] = rand() % 1000 - 500;
            }
            fa->set_intPolynomial(tmp);
            delete_IntPolynomial(tmp);
        }

    };



    /* ***************************************************************
     *************************** TESTS ********************************
     *************************************************************** */


    // Computes the inverse FFT of the coefficients of the TLWE sample
    //EXPORT void tLweToFFTConvert(TLweSampleFFT* result, const TLweSample* source, const TLweParams* params)
    TEST_F(TLweFFTTest, tLweToFFTConvert) {
        for (const TLweParams *params: all_params) {
            const int32_t k = params->k;
            TLweSample *source = new_TLweSample(params);
            TLweSampleFFT *result = new_TLweSampleFFT(params);
            FakeLagrangeHalfCPolynomial *resulta = fake(result->a);


            tlweSampleUniform(source, params);

            tLweToFFTConvert(result, source, params);

            for (int32_t i = 0; i <= k; i++)
                ASSERT_EQ(torusPolynomialNormInftyDist(source->a + i, resulta[i].getTorusPolynomialPtr()), 0);
            ASSERT_EQ(result->current_variance, source->current_variance);

            delete_TLweSampleFFT(result);
            delete_TLweSample(source);
        }
    }


    // Computes the FFT of the coefficients of the TLWEfft sample
    //EXPORT void fake_tLweFromFFTConvert(TLweSample* result, const TLweSampleFFT* source, const TLweParams* params);
    TEST_F(TLweFFTTest, tLweFromFFTConvert) {
        for (const TLweParams *params: all_params) {
            const int32_t k = params->k;
            TLweSampleFFT *source = new_TLweSampleFFT(params);
            TLweSample *result = new_TLweSample(params);
            FakeLagrangeHalfCPolynomial *sourcea = fake(source->a);


            tlweSampleFFTUniform(source, params);

            tLweFromFFTConvert(result, source, params);

            for (int32_t i = 0; i <= k; i++)
                ASSERT_EQ(torusPolynomialNormInftyDist(result->a + i, sourcea[i].getTorusPolynomialPtr()), 0);
            ASSERT_EQ(result->current_variance, source->current_variance);

            delete_TLweSample(result);
            delete_TLweSampleFFT(source);
        }
    }


    //Arithmetic operations on TLwe samples
    /** result = (0,0) */
    //EXPORT void fake_tLweFFTClear(TLweSampleFFT* result, const TLweParams* params);
    TEST_F(TLweFFTTest, tLweFFTClear) {
        for (const TLweParams *params: all_params) {
            const int32_t k = params->k;
            TLweSampleFFT *result = new_TLweSampleFFT(params);
            FakeLagrangeHalfCPolynomial *resulta = fake(result->a);
            TorusPolynomial *zero = new_TorusPolynomial(params->N);
            torusPolynomialClear(zero);

            tlweSampleFFTUniform(result, params);

            tLweFFTClear(result, params);

            for (int32_t i = 0; i <= k; i++)
                ASSERT_EQ(torusPolynomialNormInftyDist(resulta[i].getTorusPolynomialPtr(), zero), 0);
            ASSERT_EQ(result->current_variance, 0);

            delete_TorusPolynomial(zero);
            delete_TLweSampleFFT(result);
        }
    }

    // result = result + p*sample
    //EXPORT void fake_tLweFFTAddMulRTo(TLweSampleFFT* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT* sample, const TLweParams* params);
    TEST_F(TLweFFTTest, tLweAddMulRTo) {
        for (const TLweParams *params: all_params) {
            const int32_t k = params->k;
            const int32_t N = params->N;
            TLweSampleFFT *result = new_TLweSampleFFT(params);
            TLweSampleFFT *sample = new_TLweSampleFFT(params);
            LagrangeHalfCPolynomial *p = new_LagrangeHalfCPolynomial(N);
            FakeLagrangeHalfCPolynomial *fp = fake(p);
            FakeLagrangeHalfCPolynomial *resulta = fake(result->a);
            FakeLagrangeHalfCPolynomial *samplea = fake(sample->a);

            tlweSampleFFTUniform(result, params);
            tlweSampleFFTUniform(sample, params);
            intLagrangeHalfCPolynomialUniform(p, N);

            //copy the initial sample
            TorusPolynomial *tmp = new_TorusPolynomial_array(k + 1, N);
            for (int32_t i = 0; i <= k; i++) {
                torusPolynomialCopy(tmp + i, resulta[i].getTorusPolynomialPtr());
            }
            //double initial_variance = result->current_variance;
            //double norm2p = intPolynomialNormSq2(fp->getIntPolynomialPtr());

            //do the tested operation
            tLweFFTAddMulRTo(result, p, sample, params);

            //check the result
            for (int32_t i = 0; i <= k; i++) {
                torusPolynomialAddMulRKaratsuba(tmp + i, fp->getIntPolynomialPtr(), samplea[i].getTorusPolynomialPtr());
                ASSERT_EQ(torusPolynomialNormInftyDist(resulta[i].getTorusPolynomialPtr(), tmp + i), 0);
            }
            //ASSERT_EQ(result->current_variance, initial_variance+norm2p*sample->current_variance);
            //TODO: we have no way of computing the variance

            delete_TorusPolynomial_array(k + 1, tmp);
            delete_LagrangeHalfCPolynomial(p);
            delete_TLweSampleFFT(sample);
            delete_TLweSampleFFT(result);
        }
    }

}//namespace

