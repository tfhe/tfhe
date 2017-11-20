#include <gtest/gtest.h>
#include "tfhe.h"
#include "fakes/tgsw.h"
#include "fakes/tgsw-fft.h"
#include "fakes/lwe-bootstrapping-fft.h"

#define TFHE_TEST_ENVIRONMENT 1
using namespace std;

using namespace ::testing;

namespace {

    const int32_t N = 1024;
    const int32_t k = 1;
    const int32_t n = 500;
    const int32_t l_bk = 3; //ell
    const int32_t Bgbit_bk = 10;
    const int32_t ks_t = 15;
    const int32_t ks_basebit = 1;
    const double alpha_in = 5e-4;
    const double alpha_bk = 9e-9;
    //const int32_t alpha_ks = 1e-6;

    const LweParams *in_params = new_LweParams(n, alpha_in, 1. / 16.);
    const TLweParams *accum_params = new_TLweParams(N, k, alpha_bk, 1. / 16.);
    const TGswParams *bk_params = new_TGswParams(l_bk, Bgbit_bk, accum_params);
    const LweParams *extract_params = &accum_params->extracted_lweparams;


    LweSample *real_new_LweSample(const LweParams *params) {
        return new_LweSample(params);
    }

    void real_delete_LweSample(LweSample *sample) {
        delete_LweSample(sample);
    }


    vector<bool> random_binary_key(const int32_t n) {
        vector<bool> rand_vect(n);
        for (int32_t i = 0; i < n; ++i) {
            rand_vect[i] = rand() % 2;
        }
        return rand_vect;
    }


/*
    LweKey* key = new_LweKey(params_in);
    lweKeyGen(key);

    TGswKey* key_bk = new_TGswKey(params_bk);
    tGswKeyGen(key_bk);
*/




    class TfheBlindRotateFFTTest : public ::testing::Test {
    public:

        USE_FAKE_new_TGswSampleFFT;

        USE_FAKE_new_TLweSample;

        USE_FAKE_delete_TLweSample;

        USE_FAKE_delete_TGswSampleFFT;

        USE_FAKE_tLweMulByXaiMinusOne;

        USE_FAKE_tGswFFTExternMulToTLwe;

        USE_FAKE_tLweAddTo;

        USE_FAKE_tLweCopy;


#define INCLUDE_TFHE_BLIND_ROTATE_FFT

#include "../libtfhe/lwe-bootstrapping-functions-fft.cpp"

    };

    /**
     * multiply the accumulator by X^sum(bara_i.s_i)
     * @param accum the TLWE sample to multiply
     * @param bk An array of n TGSW samples where bk_i encodes s_i
     * @param bara An array of n coefficients between 0 and 2N-1
     * @param bk_params The parameters of bk
     */
    //EXPORT void tfhe_BlindRotate(TLweSample* accum, 
    //      const TGswSample* bk, 
    //      const int32_t* bara,
    //      const int32_t n,
    //      const TGswParams* bk_params) {

    TEST_F(TfheBlindRotateFFTTest, tfheBlindRotateFFTTest) {

        vector<bool> key = random_binary_key(n);
        TGswSampleFFT *bkFFT = fake_new_TGswSampleFFT_array(n, bk_params);
        FakeTGswFFT *fbkFFT = fake(bkFFT);
        for (int32_t i = 0; i < n; i++) fbkFFT[i].setMessageVariance(key[i], alpha_bk * alpha_bk);

        //create bara
        int32_t *bara = new int32_t[n];
        for (int32_t i = 0; i < n; i++) bara[i] = rand() % (2 * N);
        //create accum
        TorusPolynomial *initAccumMessage = new_TorusPolynomial(N);
        torusPolynomialUniform(initAccumMessage);
        const double initAlphaAccum = 0.2;
        int32_t expectedOffset = 0;
        TorusPolynomial *expectedAccumMessage = new_TorusPolynomial(N);
        torusPolynomialCopy(expectedAccumMessage, initAccumMessage);
        //double expectedAccumVariance=initAlphaAccum*initAlphaAccum;
        TLweSample *accum = fake_new_TLweSample(accum_params);
        FakeTLwe *faccum = fake(accum);
        torusPolynomialCopy(faccum->message, initAccumMessage);
        faccum->current_variance = initAlphaAccum * initAlphaAccum;

        //call bootstraprotate: one iteration at a time
        for (int32_t i = 0; i < n; i++) {
            tfhe_blindRotate_FFT(accum, bkFFT + i, bara + i, 1, bk_params);
            if (key[i] == 1 && bara[i] != 0) {
                expectedOffset = (expectedOffset + bara[i]) % (2 * N);
                torusPolynomialMulByXai(expectedAccumMessage, expectedOffset, initAccumMessage);
            }
            //printf("i=%d,si=%d,barai=%d,offset=%d\n",i,key->key[i],bara[i],expectedOffset);
            for (int32_t j = 0; j < N; j++) ASSERT_EQ(expectedAccumMessage->coefsT[j], accum->b->coefsT[j]);
        }
        //Now, bootstraprotate: all iterations at once (same offset)
        torusPolynomialCopy(faccum->message, initAccumMessage);
        faccum->current_variance = initAlphaAccum * initAlphaAccum;
        tfhe_blindRotate_FFT(accum, bkFFT, bara, n, bk_params);
        for (int32_t j = 0; j < N; j++) ASSERT_EQ(expectedAccumMessage->coefsT[j], accum->b->coefsT[j]);

        //cleanup everything
        fake_delete_TLweSample(accum);
        delete_TorusPolynomial(expectedAccumMessage);
        delete_TorusPolynomial(initAccumMessage);
        delete[] bara;
        fake_delete_TGswSampleFFT_array(n, bkFFT);
    }


    class TfheBlindRotateAndExtractFFTTest : public ::testing::Test {
    public:

        USE_FAKE_new_LweSample;

        USE_FAKE_delete_LweSample;
        //USE_FAKE_new_TGswSample;
        USE_FAKE_new_TLweSample;

        USE_FAKE_delete_TLweSample;
        //USE_FAKE_delete_TGswSample;
        USE_FAKE_new_TGswSampleFFT_array;

        USE_FAKE_delete_TGswSampleFFT_array;

        USE_FAKE_tLweNoiselessTrivial;

        USE_FAKE_tGswSymEncryptInt;

        USE_FAKE_tLweExtractLweSample;

        USE_FAKE_tfhe_blindRotate_FFT;

#define INCLUDE_TFHE_BLIND_ROTATE_AND_EXTRACT_FFT

#include "../libtfhe/lwe-bootstrapping-functions-fft.cpp"

    };
    /**
     * result = LWE(v_p) where p=barb-sum(bara_i.s_i) mod 2N
     * @param result the output LWE sample
     * @param v a 2N-elt anticyclic function (represented by a TorusPolynomial)
     * @param bk An array of n TGSW samples where bk_i encodes s_i
     * @param barb A coefficients between 0 and 2N-1
     * @param bara An array of n coefficients between 0 and 2N-1
     * @param bk_params The parameters of bk
     */

    TEST_F(TfheBlindRotateAndExtractFFTTest, tfheBlindRotateAndExtractFFTTest) {
        const int32_t NB_TRIALS = 30;

        vector<bool> key = random_binary_key(n);
        TGswSampleFFT *bkFFT = fake_new_TGswSampleFFT_array(n, bk_params);
        FakeTGswFFT *fbkFFT = fake(bkFFT);
        for (int32_t i = 0; i < n; i++) fbkFFT[i].setMessageVariance(key[i], alpha_bk * alpha_bk);

        //create bara and b
        int32_t *bara = new int32_t[n];
        //create v
        TorusPolynomial *v = new_TorusPolynomial(N);
        //create result
        LweSample *result = fake_new_LweSample(&accum_params->extracted_lweparams);
        FakeLwe *fres = fake(result);

        for (int32_t trial = 0; trial < NB_TRIALS; trial++) {
            for (int32_t i = 0; i < n; i++) bara[i] = rand() % (2 * N);
            int32_t barb = rand() % (2 * N);
            torusPolynomialUniform(v);
            //const double initAlphaAccum=0.2;

            //run the function
            tfhe_blindRotateAndExtract_FFT(result, v, bkFFT, barb, bara, n, bk_params);

            //verify
            int32_t offset = barb;
            for (int32_t i = 0; i < n; i++) offset = (offset + 2 * N - key[i] * bara[i]) % (2 * N);
            ASSERT_EQ(fres->message, (offset < N) ? (v->coefsT[offset]) : (-v->coefsT[offset - N]));
            //TODO variance
        }
        //clean up
        fake_delete_LweSample(result);
        delete_TorusPolynomial(v);
        delete[] bara;
        fake_delete_TGswSampleFFT_array(n, bkFFT);
    }


    class TfheBootstrapWoKSFFTTest : public ::testing::Test {
    public:

        USE_FAKE_new_LweSample;

        USE_FAKE_delete_LweSample;
        //USE_FAKE_new_LweSample_array;
        //USE_FAKE_delete_LweSample_array;
        USE_FAKE_new_TGswSampleFFT_array;

        USE_FAKE_delete_TGswSampleFFT_array;

        USE_FAKE_new_LweKeySwitchKey;

        USE_FAKE_delete_LweKeySwitchKey;

        USE_FAKE_lweCreateKeySwitchKey;

        USE_FAKE_tfhe_createLweBootstrappingKey;

        USE_FAKE_tGswToFFTConvert;

        USE_FAKE_tfhe_blindRotateAndExtract_FFT;

#define INCLUDE_TFHE_BOOTSTRAP_WO_KS_FFT

#include "../libtfhe/lwe-bootstrapping-functions-fft.cpp"

    };

    /**
     * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
     * @param result The resulting LweSample
     * @param bk The bootstrapping + keyswitch key
     * @param mu The output message (if phase(x)>0)
     * @param x The input sample
     */
    //EXPORT void tfhe_bootstrap(LweSample* result, 
    //  const LweBootstrappingKey* bk, 
    //  Torus32 mu, const LweSample* x)

    TEST_F(TfheBootstrapWoKSFFTTest, tfheBootstrapWoKSFFTTest) {
        const Torus32 TEST_MU = 123456789;
        const int32_t NB_TRIALS = 30;
        const int32_t Nx2 = 2 * N;
        const int32_t n = in_params->n;

        //create a fake bootstrapping key
        LweKey *key = new_LweKey(in_params);
        lweKeyGen(key);
        TGswKey *key_bk = new_TGswKey(bk_params);
        tGswKeyGen(key_bk);
        LweBootstrappingKey *bk = fake_new_LweBootstrappingKey(ks_t, ks_basebit, in_params, bk_params);
        fake_tfhe_createLweBootstrappingKey(bk, key, key_bk);

        LweBootstrappingKeyFFT *bkFFT = fake_new_LweBootstrappingKeyFFT(bk);

        //alloc the output lwe sample
        LweSample *result = fake_new_LweSample(in_params);
        FakeLwe *fres = fake(result);

        //create a random input sample
        LweSample *insample = real_new_LweSample(extract_params);
        for (int32_t trial = 0; trial < NB_TRIALS; trial++) {
            lweSymEncrypt(insample, uniformTorus32_distrib(generator), 0.001, key);

            //compute the approx. phase mod 2N
            int32_t barb = modSwitchFromTorus32(insample->b, Nx2);
            int32_t phase = barb;
            for (int32_t i = 0; i < n; i++) {
                int32_t barai = modSwitchFromTorus32(insample->a[i], Nx2);
                phase -= key->key[i] * barai;
            }
            phase = (Nx2 + (phase % Nx2)) % Nx2; //positive modulo

            //call the function
            tfhe_bootstrap_woKS_FFT(result, bkFFT, TEST_MU, insample);

            //printf("trial=%d,hash=%d,phase=%d,result=%d\n",trial,hash,phase,result->b);
            //verify the result
            ASSERT_EQ(fres->message, phase < N ? TEST_MU : -TEST_MU);
        }

        //cleanup
        real_delete_LweSample(insample);
        fake_delete_LweSample(result);
        fake_delete_LweBootstrappingKeyFFT(bkFFT);
        fake_delete_LweBootstrappingKey(bk);
        delete_TGswKey(key_bk);
        delete_LweKey(key);
    }


    class TfheBootstrapFFTTest : public ::testing::Test {
    public:

        USE_FAKE_new_LweSample;

        USE_FAKE_delete_LweSample;

        USE_FAKE_lweSymEncrypt;
        //USE_FAKE_new_LweSample_array;
        //USE_FAKE_delete_LweSample_array;
        USE_FAKE_new_TGswSampleFFT_array;

        USE_FAKE_delete_TGswSampleFFT_array;

        USE_FAKE_new_LweKeySwitchKey;

        USE_FAKE_delete_LweKeySwitchKey;

        USE_FAKE_lweCreateKeySwitchKey;

        USE_FAKE_tfhe_createLweBootstrappingKey;

        USE_FAKE_tGswToFFTConvert;

        USE_FAKE_lweKeySwitch;

        USE_FAKE_tfhe_blindRotateAndExtract_FFT;

        USE_FAKE_tfhe_bootstrap_woKS_FFT;

#define INCLUDE_TFHE_BOOTSTRAP_FFT

#include "../libtfhe/lwe-bootstrapping-functions-fft.cpp"

    };

    /**
     * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
     * @param result The resulting LweSample
     * @param bk The bootstrapping + keyswitch key
     * @param mu The output message (if phase(x)>0)
     * @param x The input sample
     */
    //EXPORT void tfhe_bootstrap(LweSample* result, 
    //  const LweBootstrappingKey* bk, 
    //  Torus32 mu, const LweSample* x)

    TEST_F(TfheBootstrapFFTTest, tfheBootstrapFFTTest) {
        const Torus32 TEST_MU = 123456789;
        const int32_t NB_TRIALS = 30;

        //fake keys
        LweKey *key = 0x0;
        FakeLweKeySwitchKey *ks = new FakeLweKeySwitchKey(1024, 15, 1);
        LweBootstrappingKeyFFT *bkFFT = new LweBootstrappingKeyFFT((LweParams *) 0, (TGswParams *) 0, (TLweParams *) 0,
                                                                   (LweParams *) 0, (TGswSampleFFT *) 0,
                                                                   (LweKeySwitchKey *) ks);

        //alloc the output lwe sample
        LweSample *result = fake_new_LweSample(in_params);
        FakeLwe *fres = fake(result);

        //create a random input sample
        LweSample *insample = fake_new_LweSample(extract_params);
        FakeLwe *fin = fake(insample);
        for (int32_t trial = 0; trial < NB_TRIALS; trial++) {
            lweSymEncrypt(insample, uniformTorus32_distrib(generator), 0.001, key);

            //call the function
            tfhe_bootstrap_FFT(result, bkFFT, TEST_MU, insample);

            //verify the result
            ASSERT_EQ(fres->message, fin->message >= 0 ? TEST_MU : -TEST_MU);
        }

        //cleanup
        delete ((FakeLweKeySwitchKey *) ks);
        delete bkFFT;
        fake_delete_LweSample(insample);
        fake_delete_LweSample(result);
    }


    class TfheInitLweBootstrappingKeyFFTTest : public ::testing::Test {
    public:

        USE_FAKE_new_LweSample;

        USE_FAKE_delete_LweSample;

        USE_FAKE_new_LweSample_array;

        USE_FAKE_delete_LweSample_array;

        USE_FAKE_new_TGswSample;

        USE_FAKE_delete_TGswSample;

        USE_FAKE_new_TGswSample_array;

        USE_FAKE_delete_TGswSample_array;

        USE_FAKE_new_TGswSampleFFT;

        USE_FAKE_delete_TGswSampleFFT;

        USE_FAKE_new_TGswSampleFFT_array;

        USE_FAKE_delete_TGswSampleFFT_array;

        USE_FAKE_lweCopy;

        USE_FAKE_tGswToFFTConvert;

        USE_FAKE_lweSymEncrypt;

        USE_FAKE_tGswSymEncryptInt;
        // because of new create KS key
        USE_FAKE_lweSymEncryptWithExternalNoise;

        USE_FAKE_lweNoiselessTrivial;

        USE_FAKE_lwePhase;

#define INCLUDE_TFHE_INIT_LWEBOOTSTRAPPINGKEY_FFT
#define INCLUDE_TFHE_CREATEBOOTSTRAPPINGKEY

#include "../libtfhe/lwe-keyswitch-functions.cpp"

#include "../libtfhe/lwe-bootstrapping-functions.cpp"

#include "../libtfhe/lwe-bootstrapping-functions-fft.cpp"
    };

    /**
     * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
     * @param result The resulting LweSample
     * @param bk The bootstrapping + keyswitch key
     * @param mu The output message (if phase(x)>0)
     * @param x The input sample
     */
    //EXPORT void tfhe_bootstrap(LweSample* result, 
    //  const LweBootstrappingKey* bk, 
    //  Torus32 mu, const LweSample* x)

    TEST_F(TfheInitLweBootstrappingKeyFFTTest, tfheInitLweBootstrappingKeyFFTTest) {

        // create the BK
        LweKey *key = new_LweKey(in_params);
        lweKeyGen(key);
        TGswKey *key_bk = new_TGswKey(bk_params);
        tGswKeyGen(key_bk);
        LweBootstrappingKey *bk = new_LweBootstrappingKey(ks_t, ks_basebit, in_params, bk_params);
        tfhe_createLweBootstrappingKey(bk, key, key_bk);

        // create (allocate and initialize) the BK FFT
        LweBootstrappingKeyFFT *bkFFT = new_LweBootstrappingKeyFFT(bk);

        const int32_t n = in_params->n;
        const int32_t t = bk->ks->t;
        const int32_t base = bk->ks->base;
        const int32_t N = bk_params->tlwe_params->extracted_lweparams.n;

        // KeySwitching 
        for (int32_t i = 0; i < N; i++) {
            for (int32_t j = 0; j < t; j++) {
                for (int32_t p = 0; p < base; p++) {
                    ASSERT_EQ(fake(&bkFFT->ks->ks[i][j][p])->message, fake(&bk->ks->ks[i][j][p])->message);
                    ASSERT_EQ(fake(&bkFFT->ks->ks[i][j][p])->current_variance,
                              fake(&bk->ks->ks[i][j][p])->current_variance);
                }
            }
        }

        // Bootstrapping Key FFT 
        for (int32_t i = 0; i < n; ++i) {
            ASSERT_EQ(intPolynomialNormInftyDist(fake(&bkFFT->bkFFT[i])->message, fake(&bk->bk[i])->message), 0);
            ASSERT_EQ(fake(&bkFFT->bkFFT[i])->current_variance, fake(&bk->bk[i])->current_variance);
        }

        delete_LweBootstrappingKeyFFT(bkFFT);
        delete_LweBootstrappingKey(bk);
        delete_TGswKey(key_bk);
        delete_LweKey(key);
    }


}

