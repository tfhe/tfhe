/* 
 * tgsw_fft_test.cpp
 * Tests the functions defined in /src/include/tgsw_functions.h
 * A set of parameters ad keys is defined in the beginning (N=512,1024,2048 and k=1,2)
 */


#include <gtest/gtest.h>
#include <tlwe.h>
#include <tlwe_functions.h>
#include <tgsw_functions.h>
#include <numeric_functions.h>
#include <polynomials_arithmetic.h>
#include <lagrangehalfc_arithmetic.h>
#include "fakes/tlwe.h"
#include "fakes/tlwe-fft.h"

#define TFHE_TEST_ENVIRONMENT 1

using namespace std;
using namespace ::testing;

namespace {

    /*
     * Parameters and keys (for N=512,1024,2048 and k=1,2)
     */
    const TGswParams *params512_1 = new_TGswParams(4, 8, new_TLweParams(512, 1, 0., 1.));
    const TGswParams *params512_2 = new_TGswParams(3, 10, new_TLweParams(512, 2, 0., 1.));
    const TGswParams *params1024_1 = new_TGswParams(3, 10, new_TLweParams(1024, 1, 0., 1.));
    const TGswParams *params1024_2 = new_TGswParams(4, 8, new_TLweParams(1024, 2, 0., 1.));
    const TGswParams *params2048_1 = new_TGswParams(4, 8, new_TLweParams(2048, 1, 0., 1.));
    const TGswParams *params2048_2 = new_TGswParams(3, 10, new_TLweParams(2048, 2, 0., 1.));
    vector<const TGswParams *> all_params = {params512_1, params512_2, params1024_1, params1024_2, params2048_1,
                                             params2048_2};
    vector<const TGswParams *> all_params1024 = {params1024_1, params1024_2};

    /* Tolerance factor for the equality between two TorusPolynomial */
    //const double toler = 1e-8;

    class TGswFFTTest : public ::testing::Test {
    public:
        USE_FAKE_new_TLweSample;

        USE_FAKE_new_TLweSample_array;

        USE_FAKE_new_TLweSampleFFT;

        USE_FAKE_new_TLweSampleFFT_array;

        USE_FAKE_delete_TLweSample;

        USE_FAKE_delete_TLweSample_array;

        USE_FAKE_delete_TLweSampleFFT;

        USE_FAKE_delete_TLweSampleFFT_array;

        USE_FAKE_tLweToFFTConvert;

        USE_FAKE_tLweFromFFTConvert;

        USE_FAKE_tLweFFTClear;

        USE_FAKE_new_LagrangeHalfCPolynomial_array;

        USE_FAKE_delete_LagrangeHalfCPolynomial_array;

        USE_FAKE_IntPolynomial_ifft;

        USE_FAKE_tLweFFTAddMulRTo;

        //this function generates a totally random fake integer decomposition, using just the address
        //of bla as a seed.
        void
        fake_tGswTorus32PolynomialDecompH(IntPolynomial *result, const TorusPolynomial *bla, const TGswParams *params) {
            const int32_t N = params->tlwe_params->N;
            const int32_t l = params->l;
            const size_t seed = (size_t) bla;
            for (int32_t p = 0; p < l; p++)
                for (int32_t i = 0; i < N; i++)
                    result[p].coefs[i] = (17 + i * seed) % 3;
        }

        void tGswTorus32PolynomialDecompH(IntPolynomial *result, const TorusPolynomial *bla, const TGswParams *params) {
            fake_tGswTorus32PolynomialDecompH(result, bla, params);
        }


#define INCLUDE_ALL

#include "../libtfhe/tgsw-fft-operations.cpp"

#define INCLUDE_TGSW_INIT
#define INCLUDE_TGSW_DESTROY
#define INCLUDE_TGSW_KEYGEN
#define INCLUDE_TGSW_CLEAR
#define INCLUDE_TGSW_ADD_H
#define INCLUDE_TGSW_ADD_MU_H
#define INCLUDE_TGSW_ADD_MU_INT_H
#define INCLUDE_TGSW_ENCRYPT_ZERO
#define INCLUDE_TGSW_MUL_BY_XAI_MINUS_ONE
#define INCLUDE_TGSW_EXTERN_MUL_TO_TLWE
#define INCLUDE_TGSW_SYM_ENCRYPT
#define INCLUDE_TGSW_SYM_ENCRYPT_INT
#define INCLUDE_TGSW_ENCRYPT_B
#define INCLUDE_TGSW_DECRYPT
#define INCLUDE_TGSW_TLWE_DECOMP_H
//#define INCLUDE_TGSW_TORUS32POLYNOMIAL_DECOMP_H_OLD
//#define INCLUDE_TGSW_TORUS32POLYNOMIAL_DECOMP_H
#define INCLUDE_TGSW_EXTERN_PRODUCT
#define INCLUDE_TGSW_NOISELESS_TRIVIAL
#define INCLUDE_TGSW_ALLOC_FUNCTION
#define INCLUDE_TGSW_ALLOC_ARRAY_FUNCTION
#define INCLUDE_TGSW_FREE_FUNCTION
#define INCLUDE_TGSW_FREE_ARRAY_FUNCTION
#define INCLUDE_TGSW_INIT_ARRAY_FUNCTION
#define INCLUDE_TGSW_DESTROY_ARRAY_FUNCTION
#define INCLUDE_TGSW_NEW_FUNCTION
#define INCLUDE_TGSW_NEW_ARRAY_FUNCTION
#define INCLUDE_TGSW_DELETE_FUNCTION
#define INCLUDE_TGSW_DELETE_ARRAY_FUNCTION

#include "../libtfhe/tgsw-functions.cpp"


        //this function will be used to generate a fake GSW sample: all
        //rows are fake uniformly distributed tlwe. (there is no integer
        //message in the GSW)
        void tGswSampleUniform(TGswSample *result, const TGswParams *params) {
            const int32_t kpl = params->kpl;

            for (int32_t i = 0; i < kpl; i++) {
                FakeTLwe *fs = fake(result->all_sample + i);
                torusPolynomialUniform(fs->message);
                fs->current_variance = rand() / double(RAND_MAX);
            }
        }

        //this function will be used to generate a fake GSWFFT sample: all
        //rows are fake uniformly distributed tlwe. (there is no integer
        //message in the GSW)
        void tGswSampleFFTUniform(TGswSampleFFT *result, const TGswParams *params) {
            const int32_t kpl = params->kpl;

            for (int32_t i = 0; i < kpl; i++) {
                FakeTLweFFT *fs = fake(result->all_samples + i);
                torusPolynomialUniform(fs->message);
                fs->current_variance = rand() / double(RAND_MAX);
            }
        }

        //this function will be used to generate a fake TLWE sample
        void tLweSampleUniform(TLweSample *result, const TLweParams *params) {
            FakeTLwe *fres = fake(result);

            torusPolynomialUniform(fres->message);
            fres->current_variance = rand() / double(RAND_MAX);
        }
    };



    /* ***************************************************************
     *************************** TESTS ********************************
     *************************************************************** */

    //EXPORT void tGswToFFTConvert(TGswSampleFFT* result, const TGswSample* source, const TGswParams* params);
    TEST_F(TGswFFTTest, tGswToFFTConvert) {
        for (const TGswParams *params: all_params) {
            const int32_t kpl = params->kpl;
            TGswSampleFFT *result = new_TGswSampleFFT(params);
            TGswSample *source = new_TGswSample(params);
            FakeTLwe *fs = fake(source->all_sample);
            FakeTLweFFT *fr = fake(result->all_samples);

            tGswSampleUniform(source, params);

            //do the operation
            tGswToFFTConvert(result, source, params);

            //verify the result
            for (int32_t i = 0; i < kpl; i++) {
                ASSERT_EQ(torusPolynomialNormInftyDist(fs[i].message, fr[i].message), 0);
                ASSERT_EQ(fs[i].current_variance, fr[i].current_variance);
            }

            delete_TGswSample(source);
            delete_TGswSampleFFT(result);
        }
    }
    //EXPORT void tGswFromFFTConvert(TGswSample* result, const TGswSampleFFT* source, const TGswParams* params);
    TEST_F(TGswFFTTest, tGswFromFFTConvert) {
        for (const TGswParams *params: all_params) {
            const int32_t kpl = params->kpl;
            TGswSample *result = new_TGswSample(params);
            TGswSampleFFT *source = new_TGswSampleFFT(params);
            FakeTLweFFT *fs = fake(source->all_samples);
            FakeTLwe *fr = fake(result->all_sample);

            tGswSampleFFTUniform(source, params);

            //do the operation
            tGswFromFFTConvert(result, source, params);

            //verify the result
            for (int32_t i = 0; i < kpl; i++) {
                ASSERT_EQ(torusPolynomialNormInftyDist(fs[i].message, fr[i].message), 0);
                ASSERT_EQ(fs[i].current_variance, fr[i].current_variance);
            }

            delete_TGswSampleFFT(source);
            delete_TGswSample(result);
        }
    }
    //EXPORT void tGswFFTAddH(TGswSampleFFT* result, const TGswParams* params);
    TEST_F(TGswFFTTest, tGswFFTAddH) {
        //TODO
    }
    //EXPORT void tGswFFTClear(TGswSampleFFT* result, const TGswParams* params);
    TEST_F(TGswFFTTest, tGswFFTClear) {
        for (const TGswParams *params: all_params) {
            const int32_t kpl = params->kpl;
            const int32_t N = params->tlwe_params->N;
            TGswSampleFFT *result = new_TGswSampleFFT(params);
            FakeTLweFFT *fr = fake(result->all_samples);
            TorusPolynomial *zero = new_TorusPolynomial(N);

            torusPolynomialClear(zero);
            tGswSampleFFTUniform(result, params);

            //do the operation
            tGswFFTClear(result, params);

            //verify the result
            for (int32_t i = 0; i < kpl; i++) {
                ASSERT_EQ(torusPolynomialNormInftyDist(fr[i].message, zero), 0);
                ASSERT_EQ(fr[i].current_variance, 0);
            }

            delete_TorusPolynomial(zero);
            delete_TGswSampleFFT(result);
        }
    }
    //EXPORT void tGswFFTExternMulToTLwe(TLweSample* accum, TGswSampleFFT* gsw, const TGswParams* params);
    TEST_F(TGswFFTTest, tGswFFTExternMulToTLwe) {
        for (const TGswParams *params: all_params) {
            const int32_t kpl = params->kpl;
            const int32_t l = params->l;
            const TLweParams *tlwe_params = params->tlwe_params;
            const int32_t N = tlwe_params->N;
            const int32_t k = tlwe_params->k;

            TLweSample *accum = new_TLweSample(tlwe_params);
            TGswSampleFFT *gsw = new_TGswSampleFFT(params);
            FakeTLweFFT *fgsw = fake(gsw->all_samples);
            FakeTLwe *faccum = fake(accum);

            //initialize random values
            tGswSampleFFTUniform(gsw, params);
            tLweSampleUniform(accum, tlwe_params);

            //simulate the same decomposition as in the fake function
            IntPolynomial *dec = new_IntPolynomial_array(kpl, N);
            for (int32_t i = 0; i <= k; i++) {
                fake_tGswTorus32PolynomialDecompH(dec + i * l, accum->a + i, params);
            }

            //compute the same product
            TorusPolynomial *expected_res = new_TorusPolynomial(N);
            torusPolynomialClear(expected_res);
            for (int32_t i = 0; i < kpl; i++) {
                torusPolynomialAddMulRKaratsuba(expected_res, dec + i, fgsw[i].message);
            }

            //do the operation: accum *= gsw
            tGswFFTExternMulToTLwe(accum, gsw, params);

            //verify the result
            ASSERT_EQ(torusPolynomialNormInftyDist(faccum->message, expected_res), 0);

            delete_TorusPolynomial(expected_res);
            delete_IntPolynomial_array(kpl, dec);
            delete_TGswSampleFFT(gsw);
            delete_TLweSample(accum);
        }
    }
    //EXPORT void tGswFFTMulByXaiMinusOne(TGswSampleFFT* result, const int32_t ai, const TGswSampleFFT* bki, const TGswParams* params);
    TEST_F(TGswFFTTest, tGswFFTMulByXaiMinusOne) {
        //TODO: A supprimer
    }

}//namespace

