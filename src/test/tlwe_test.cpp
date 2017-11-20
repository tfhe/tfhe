/*
   tlwe_test.cpp
 * Tests the functions defined in /src/include/tlwe_functions.h
 * A set of parameters ad keys is defined in the beginning (N=512,1024,2048 and k=1,2)
 */


#include <gtest/gtest.h>
#include <tlwe.h>
#include <tlwe_functions.h>
#include <numeric_functions.h>
#include <polynomials_arithmetic.h>

using namespace std;


namespace {

    // we use the function rand because in the "const static" context the uniformly random generator doesn't work!
    const TLweKey *new_random_key(const TLweParams *params) {
        TLweKey *key = new_TLweKey(params);
        const int32_t N = params->N;
        const int32_t k = params->k;

        for (int32_t i = 0; i < k; ++i)
            for (int32_t j = 0; j < N; ++j)
                key->key[i].coefs[j] = rand() % 2;
        return key;
    }

    /*
     * Parameters and keys (for N=512,1024,2048 and k=1,2)
     * ILA: Creer un set des parametres avec tous les params à l'interieur et utiliser le set dans les tests (?)
     */
    const TLweParams *params512_1 = new_TLweParams(512, 1, 0., 1.);
    const TLweParams *params512_2 = new_TLweParams(512, 2, 0., 1.);
    const TLweParams *params1024_1 = new_TLweParams(1024, 1, 0., 1.);
    const TLweParams *params1024_2 = new_TLweParams(1024, 2, 0., 1.);
    const TLweParams *params2048_1 = new_TLweParams(2048, 1, 0., 1.);
    const TLweParams *params2048_2 = new_TLweParams(2048, 2, 0., 1.);
    vector<const TLweParams *> all_params = {params512_1, params512_2, params1024_1, params1024_2, params2048_1,
                                             params2048_2};

    const TLweKey *key512_1 = new_random_key(params512_1);
    const TLweKey *key512_2 = new_random_key(params512_2);
    const TLweKey *key1024_1 = new_random_key(params1024_1);
    const TLweKey *key1024_2 = new_random_key(params1024_2);
    const TLweKey *key2048_1 = new_random_key(params2048_1);
    const TLweKey *key2048_2 = new_random_key(params2048_2);
    vector<const TLweKey *> all_keys = {key512_1, key512_2, key1024_1, key1024_2, key2048_1, key2048_2};

    /* Tolerance factor for the equality between two TorusPolynomial */
    const double toler = 1e-8;

    class TLweTest : public ::testing::Test {
    };


    /*
     * Definition of the function absfrac: | frac(d) |
     * Computes the absolute value of the fractional part of a double d 
     */
    double absfrac(double d) { return abs(d - rint(d)); }


    /*
     * Definition of the function fillRandom
     * Fills a TLweSample with random Torus32 values (uniform distribution) 
     */
    void fillRandom(TLweSample *result, const TLweParams *params) {
        const int32_t k = params->k;
        const int32_t N = params->N;

        for (int32_t i = 0; i <= k; ++i)
            for (int32_t j = 0; j < N; ++j)
                result->a[i].coefsT[j] = uniformTorus32_distrib(generator);
        result->current_variance = 0.2;
    }


    /*
     * Definition of the function copySample
     * Copies a TLweSample
     */
    void copySample(TLweSample *result, const TLweSample *sample, const TLweParams *params) {
        const int32_t k = params->k;
        const int32_t N = params->N;

        for (int32_t i = 0; i <= k; ++i)
            for (int32_t j = 0; j < N; ++j)
                result->a[i].coefsT[j] = sample->a[i].coefsT[j];
        result->current_variance = sample->current_variance;
    }





    /* ***************************************************************
     *************************** TESTS ********************************
     *************************************************************** */

    /*
       Testing the function tLweKeyGen
     * EXPORT void tLweKeyGen(TLweKey* result);
     * 
     * This function generates a random TLwe key for the given parameters
     * The TLwe key for the result must be allocated and initialized
     * (this means that the parameters are already in the result)
     */
    TEST_F(TLweTest, tLweKeyGen) {
        for (const TLweParams *params: all_params) {

            // Generating the key
            TLweKey *key = new_TLweKey(params);
            tLweKeyGen(key);
            ASSERT_EQ(params, key->params);

            const int32_t N = key->params->N;
            const int32_t k = key->params->k;
            const IntPolynomial *s = key->key;

            //verify that the key is binary and kind-of random
            int32_t count = 0;
            for (int32_t i = 0; i < k; ++i) {
                for (int32_t j = 0; j < N; ++j) {
                    ASSERT_TRUE(s[i].coefs[j] == 0 || s[i].coefs[j] == 1);
                    count += s[i].coefs[j];
                }
            }
            ASSERT_LE(count, k * N - 20); // <=
            ASSERT_GE(count, 20); // >=

            delete_TLweKey(key);
        }
    }






    /*
       Testing the functions tLweSymEncryptT, tLwePhase, tLweSymDecryptT
     * EXPORT void tLweSymEncryptT(TLweSample* result, Torus32 message, double alpha, const TLweKey* key);
     * EXPORT void tLwePhase(TorusPolynomial* phase, const TLweSample* sample, const TLweKey* key);
     * EXPORT Torus32 tLweSymDecryptT(const TLweSample* sample, const TLweKey* key, int32_t Msize);
     *
     * This functions encrypt and decrypt a random Torus32 message by using the given key
     */
    TEST_F (TLweTest, tLweSymEncryptPhaseDecryptT) {
        //TODO: parallelization
        static const int32_t NB_SAMPLES = 10;
        static const int32_t M = 8;
        static const double alpha = 1. / (10. * M);
        vector<const TLweKey *> all_keys1024 = {key1024_1, key1024_2};

        for (const TLweKey *key: all_keys1024) {
            const TLweParams *params = key->params;
            const int32_t N = params->N;
            const int32_t k = params->k;
            TLweSample *samples = new_TLweSample_array(NB_SAMPLES, params);
            TorusPolynomial *phase = new_TorusPolynomial(N);
            Torus32 decrypt;

            //verify correctness of the decryption
            for (int32_t trial = 0; trial < NB_SAMPLES; trial++) {
                // The message is a Torus32
                Torus32 message = modSwitchToTorus32(rand() % M, M);

                // Encrypt and decrypt
                tLweSymEncryptT(&samples[trial], message, alpha, key);
                decrypt = tLweSymDecryptT(&samples[trial], key, M);
                //ILA: Testing APPROX correct decryption
                //the absolute value of the difference between message and decrypt is <= than toler
                ASSERT_LE(abs(t32tod(message - decrypt)), toler);

                // ILA: It is really necessary? phase used in decrypt!!!
                // Phase
                tLwePhase(phase, &samples[trial], key);
                // Testing phase
                double dmessage = t32tod(message);
                double dphase = t32tod(phase->coefsT[0]);
                ASSERT_LE(absfrac(dmessage - dphase), 10. * alpha); //ILA: why absfrac?
                ASSERT_EQ(alpha * alpha, samples[trial].current_variance);
            }

            // Verify that samples are random enough (all coordinates different)
            for (int32_t i = 0; i < k; ++i) {
                for (int32_t j = 0; j < N; ++j) {
                    set<Torus32> testset;
                    for (int32_t trial = 0; trial < NB_SAMPLES; trial++) {
                        testset.insert(samples[trial].a[i].coefsT[j]);
                    }
                    ASSERT_GE(testset.size(), 0.9 * NB_SAMPLES); // >=
                }
            }

            delete_TorusPolynomial(phase);
            delete_TLweSample_array(NB_SAMPLES, samples);
        }
    }






    /*
       Testing the functions tLweSymEncrypt, tLwePhase, tLweApproxPhase, tLweSymDecrypt
     * EXPORT void tLweSymEncrypt(TLweSample* result, TorusPolynomial* message, double alpha, const TLweKey* key);
     * EXPORT void tLwePhase(TorusPolynomial* phase, const TLweSample* sample, const TLweKey* key);
     * EXPORT void tLweApproxPhase(TorusPolynomial* message, const TorusPolynomial* phase, int32_t Msize, int32_t N);
     * EXPORT void tLweSymDecrypt(TorusPolynomial* result, const TLweSample* sample, const TLweKey* key, int32_t Msize);
     *
     * This functions encrypt and decrypt a random TorusPolynomial message by using the given key
     */
    TEST_F (TLweTest, tLweSymEncryptPhaseDecrypt) {
        //TODO: parallelization
        static const int32_t NB_SAMPLES = 10;
        static const int32_t M = 8;
        static const double alpha = 1. / (10. * M);
        vector<const TLweKey *> all_keys1024 = {key1024_1, key1024_2};

        for (const TLweKey *key: all_keys1024) {
            const TLweParams *params = key->params;
            const int32_t N = params->N;
            const int32_t k = params->k;
            TLweSample *samples = new_TLweSample_array(NB_SAMPLES, params);
            TorusPolynomial *message = new_TorusPolynomial(N);
            TorusPolynomial *phase = new_TorusPolynomial(N);
            TorusPolynomial *approxphase = new_TorusPolynomial(N);
            TorusPolynomial *decrypt = new_TorusPolynomial(N);

            //verify correctness of the decryption
            for (int32_t trial = 0; trial < NB_SAMPLES; trial++) {
                for (int32_t j = 0; j < N; ++j)
                    message->coefsT[j] = modSwitchToTorus32(rand() % M, M);

                // Encrypt and Decrypt
                tLweSymEncrypt(&samples[trial], message, alpha, key);
                tLweSymDecrypt(decrypt, &samples[trial], key, M);
                //ILA: Testing APPROX correct decryption
                ASSERT_LE(torusPolynomialNormInftyDist(message, decrypt), toler);
                // for (int32_t j = 0; j < N; ++j) ASSERT_EQ(message->coefsT[j],decrypt->coefsT[j]);


                // ILA: It is really necessary? phase and ApproxPhase used in decrypt!!!
                // Phase and ApproxPhase
                tLwePhase(phase, &samples[trial], key);
                tLweApproxPhase(approxphase, phase, M, N);
                // Testing Phase and ApproxPhase
                for (int32_t j = 0; j < N; ++j) {
                    double dmessage = t32tod(message->coefsT[j]);
                    double dphase = t32tod(phase->coefsT[j]);
                    double dapproxphase = t32tod(approxphase->coefsT[j]);
                    ASSERT_LE(absfrac(dmessage - dphase), 10. * alpha); // ILA: why absfrac?
                    ASSERT_LE(absfrac(dmessage - dapproxphase), alpha); // ILA verify
                }

                ASSERT_EQ(alpha * alpha, samples[trial].current_variance);
            }

            // Verify that samples are random enough (all coordinates different)
            for (int32_t i = 0; i < k; ++i) {
                for (int32_t j = 0; j < N; ++j) {
                    set<Torus32> testset;
                    for (int32_t trial = 0; trial < NB_SAMPLES; trial++) {
                        testset.insert(samples[trial].a[i].coefsT[j]);
                    }
                    ASSERT_GE(testset.size(), 0.9 * NB_SAMPLES); // >=
                }
            }

            delete_TorusPolynomial(decrypt);
            delete_TorusPolynomial(approxphase);
            delete_TorusPolynomial(phase);
            delete_TorusPolynomial(message);
            delete_TLweSample_array(NB_SAMPLES, samples);
        }
    }

















    /* **********************************
       Arithmetic operations on TLwe samples
     ********************************** */

    /*
       Testing the function tLweClear
     * EXPORT void tLweClear(TLweSample* result, const TLweParams* params)
     *
     * tLweClear sets the TLweSample to (0,0)
     */
    TEST_F(TLweTest, tLweClear) {
        for (const TLweKey *key: all_keys) {
            const TLweParams *params = key->params;
            const int32_t N = params->N;
            const int32_t k = params->k;
            TLweSample *sample = new_TLweSample(params);

            // Generate a random TLweSample and then set it to (0,0)
            fillRandom(sample, params);
            tLweClear(sample, params);

            // Verify that the sample as been correctly set to (0,0)
            for (int32_t i = 0; i <= k; ++i)
                for (int32_t j = 0; j < N; ++j)
                    ASSERT_EQ(0, sample->a[i].coefsT[j]);
            ASSERT_EQ(0., sample->current_variance);

            delete_TLweSample(sample);
        }
    }



    /*
       Testing the function tLweCopy
     * EXPORT void tLweCopy(TLweSample* result, const TLweSample* sample, const TLweParams* params)
     *
     * tLweCopy sets the (TLweSample) result equl to a given (TLweSample) sample
     */
    TEST_F(TLweTest, tLweCopy) {
        for (const TLweKey *key: all_keys) {
            const TLweParams *params = key->params;
            const int32_t N = params->N;
            const int32_t k = params->k;
            TLweSample *sample = new_TLweSample(params);
            TLweSample *result = new_TLweSample(params);

            // Generate a random TLweSample and copy it
            fillRandom(sample, params);
            tLweCopy(result, sample, params);

            // Verify that the sample as been correctly copied
            for (int32_t i = 0; i <= k; ++i)
                for (int32_t j = 0; j < N; ++j)
                    ASSERT_EQ(result->a[i].coefsT[j], sample->a[i].coefsT[j]);
            ASSERT_EQ(result->current_variance, sample->current_variance);

            delete_TLweSample(result);
            delete_TLweSample(sample);
        }
    }




    /*
       Testing the function tLweNoiselessTrivial
     * EXPORT void tLweNoiselessTrivial(TLweSample* result, const TorusPolynomial* mu, const TLweParams* params)
     *
     * tLweNoiselessTrivial sets the TLweSample to (0,mu)
     */
    TEST_F(TLweTest, tLweNoiselessTrivial) {
        for (const TLweKey *key: all_keys) {
            const TLweParams *params = key->params;
            const int32_t N = params->N;
            const int32_t k = params->k;

            TorusPolynomial *message = new_TorusPolynomial(N);
            for (int32_t j = 0; j < N; ++j)
                message->coefsT[j] = uniformTorus32_distrib(generator);
            TLweSample *sample = new_TLweSample(params);

            // Generate a random TLweSample and set it to (0,mu)
            fillRandom(sample, params);
            tLweNoiselessTrivial(sample, message, params);

            // Verify that the sample as been correctly set
            for (int32_t i = 0; i < k; ++i)
                for (int32_t j = 0; j < N; ++j)
                    ASSERT_EQ(0, sample->a[i].coefsT[j]);
            for (int32_t j = 0; j < N; ++j)
                ASSERT_EQ(message->coefsT[j], sample->b->coefsT[j]);
            ASSERT_EQ(0., sample->current_variance);

            delete_TorusPolynomial(message);
            delete_TLweSample(sample);
        }
    }




    /*
       Testing the function tLweAddTo
     * EXPORT void tLweAddTo(TLweSample* result, const TLweSample* sample, const TLweParams* params)
     *
     * tLweAddTo computes result = result + sample
     */
    TEST_F(TLweTest, tLweAddTo) {
        for (const TLweKey *key: all_keys) {
            const TLweParams *params = key->params;
            const int32_t N = params->N;
            const int32_t k = params->k;
            TLweSample *sample1 = new_TLweSample(params);
            TLweSample *sample2 = new_TLweSample(params);
            TLweSample *sample1copy = new_TLweSample(params);

            // Generate two random TLweSample and adds the second to the first
            fillRandom(sample1, params);
            fillRandom(sample2, params);
            copySample(sample1copy, sample1, params);
            tLweAddTo(sample1, sample2, params);

            // Verify if the operation was correctly executed
            for (int32_t i = 0; i <= k; ++i)
                // torusPolynomialAddTo(sample1copy->a[i], sample2->a[i]);
                // Test equality between sample1copy->a[i] and sample1->a[i]
                for (int32_t j = 0; j < N; ++j)
                    ASSERT_EQ(sample1copy->a[i].coefsT[j] + sample2->a[i].coefsT[j], sample1->a[i].coefsT[j]);
            ASSERT_EQ(sample1copy->current_variance + sample2->current_variance, sample1->current_variance);

            delete_TLweSample(sample1);
            delete_TLweSample(sample2);
            delete_TLweSample(sample1copy);
        }
    }




    /*
       Testing the function tLweSubTo
     * EXPORT void tLweSubTo(TLweSample* result, const TLweSample* sample, const TLweParams* params)
     *
     * tLweSubTo computes result = result - sample
     */
    TEST_F(TLweTest, tLweSubTo) {
        for (const TLweKey *key: all_keys) {
            const TLweParams *params = key->params;
            const int32_t N = params->N;
            const int32_t k = params->k;
            TLweSample *sample1 = new_TLweSample(params);
            TLweSample *sample2 = new_TLweSample(params);
            TLweSample *sample1copy = new_TLweSample(params);

            // Generate two random TLweSample and subs the second to the first
            fillRandom(sample1, params);
            fillRandom(sample2, params);
            copySample(sample1copy, sample1, params);
            tLweSubTo(sample1, sample2, params);

            // Verify if the operation was correctly executed
            for (int32_t i = 0; i <= k; ++i)
                for (int32_t j = 0; j < N; ++j)
                    ASSERT_EQ(sample1copy->a[i].coefsT[j] - sample2->a[i].coefsT[j], sample1->a[i].coefsT[j]);
            ASSERT_EQ(sample1copy->current_variance + sample2->current_variance, sample1->current_variance);

            delete_TLweSample(sample1);
            delete_TLweSample(sample2);
            delete_TLweSample(sample1copy);
        }
    }




    /*
       Testing the function tLweAddMulTo
     * EXPORT void tLweAddMulTo(TLweSample* result, int32_t p, const TLweSample* sample, const TLweParams* params)
     *
     * tLweAddMulTo computes result = result + p.sample
     */
    TEST_F(TLweTest, tLweAddMulTo) {
        const int32_t p = 3;
        for (const TLweKey *key: all_keys) {
            const TLweParams *params = key->params;
            const int32_t N = params->N;
            const int32_t k = params->k;
            TLweSample *sample1 = new_TLweSample(params);
            TLweSample *sample2 = new_TLweSample(params);
            TLweSample *sample1copy = new_TLweSample(params);

            // Generate two random TLweSample and adds the second multiplied by an integer p to the first
            fillRandom(sample1, params);
            fillRandom(sample2, params);
            copySample(sample1copy, sample1, params);
            tLweAddMulTo(sample1, p, sample2, params);

            // Verify if the operation was correctly executed
            for (int32_t i = 0; i <= k; ++i)
                for (int32_t j = 0; j < N; ++j)
                    ASSERT_EQ(sample1copy->a[i].coefsT[j] + p * sample2->a[i].coefsT[j], sample1->a[i].coefsT[j]);
            ASSERT_EQ(sample1copy->current_variance + p * p * sample2->current_variance, sample1->current_variance);

            delete_TLweSample(sample1);
            delete_TLweSample(sample2);
            delete_TLweSample(sample1copy);
        }
    }




    /*
       Testing the function tLweSubMulTo
     * EXPORT void tLweSubMulTo(TLweSample* result, int32_t p, const TLweSample* sample, const TLweParams* params)
     *
     * tLweSubMulTo computes result = result - p.sample
     */
    TEST_F(TLweTest, tLweSubMulTo) {
        const int32_t p = 3;
        for (const TLweKey *key: all_keys) {
            const TLweParams *params = key->params;
            const int32_t N = params->N;
            const int32_t k = params->k;
            TLweSample *sample1 = new_TLweSample(params);
            TLweSample *sample2 = new_TLweSample(params);
            TLweSample *sample1copy = new_TLweSample(params);

            // Generate two random TLweSample and subs the second multiplied by an integer p to the first
            fillRandom(sample1, params);
            fillRandom(sample2, params);
            copySample(sample1copy, sample1, params);
            tLweSubMulTo(sample1, p, sample2, params);

            // Verify if the operation was correctly executed
            for (int32_t i = 0; i <= k; ++i)
                for (int32_t j = 0; j < N; ++j)
                    ASSERT_EQ(sample1copy->a[i].coefsT[j] - p * sample2->a[i].coefsT[j], sample1->a[i].coefsT[j]);
            ASSERT_EQ(sample1copy->current_variance + p * p * sample2->current_variance, sample1->current_variance);

            delete_TLweSample(sample1);
            delete_TLweSample(sample2);
            delete_TLweSample(sample1copy);
        }
    }



    /** result += (0,x) */
    //EXPORT void tLweAddTTo(TLweSample* result, const Torus32 x, const TLweParams* params);
    TEST_F(TLweTest, tLweAddTTo) {
        const int32_t x = uniformTorus32_distrib(generator);
        for (const TLweKey *key: all_keys) {
            const TLweParams *params = key->params;
            const int32_t N = params->N;
            const int32_t k = params->k;
            const int32_t pos = rand() % params->k;
            TLweSample *sample1 = new_TLweSample(params);
            TLweSample *sample1copy = new_TLweSample(params);
            fillRandom(sample1, params);
            copySample(sample1copy, sample1, params);
            tLweAddTTo(sample1, pos, x, params);
            // Verify if the operation was correctly executed
            for (int32_t i = 0; i <= k; ++i)
                for (int32_t j = 0; j < N; ++j)
                    if (i == pos && j == 0)
                        ASSERT_EQ(sample1copy->a[i].coefsT[j] + x, sample1->a[i].coefsT[j]);
                    else
                        ASSERT_EQ(sample1copy->a[i].coefsT[j], sample1->a[i].coefsT[j]);
            ASSERT_EQ(sample1copy->current_variance, sample1->current_variance);

            delete_TLweSample(sample1);
            delete_TLweSample(sample1copy);
        }
    }

    /** result += p*(0,x) */
    //EXPORT void tLweAddRTTo(TLweSample* result, const IntPolynomial* p, const Torus32 x, const TLweParams* params);
    TEST_F(TLweTest, tLweAddRTTo) {
        const int32_t x = uniformTorus32_distrib(generator);
        for (const TLweKey *key: all_keys) {
            const TLweParams *params = key->params;
            const int32_t N = params->N;
            const int32_t k = params->k;
            const int32_t pos = rand() % params->k;
            TLweSample *sample1 = new_TLweSample(params);
            TLweSample *sample1copy = new_TLweSample(params);
            IntPolynomial *p = new IntPolynomial(N);
            fillRandom(sample1, params);
            for (int32_t i = 0; i < N; i++) p->coefs[i] = uniformTorus32_distrib(generator) % 1000;
            copySample(sample1copy, sample1, params);
            tLweAddRTTo(sample1, pos, p, x, params);
            // Verify if the operation was correctly executed
            for (int32_t i = 0; i <= k; ++i)
                for (int32_t j = 0; j < N; ++j)
                    if (i != pos)
                        ASSERT_EQ(sample1copy->a[i].coefsT[j], sample1->a[i].coefsT[j]);
                    else
                        ASSERT_EQ(sample1copy->a[i].coefsT[j] + p->coefs[j] * x, sample1->a[i].coefsT[j]);
            ASSERT_EQ(sample1copy->current_variance, sample1->current_variance);

            delete_TLweSample(sample1);
            delete_IntPolynomial(p);
            delete_TLweSample(sample1copy);
        }
    }














    /*
       Testing the function tLweMulByXaiMinusOne
     * EXPORT void tLweMulByXaiMinusOne(TLweSample* result, int32_t ai, const TLweSample* bk, const TLweParams* params)
     *
     * tLweMulByXaiMinusOne computes ...
     */


    /*
       Testing the function tLweExtractLweSample
     * EXPORT void tLweExtractLweSample(LweSample* result, const TLweSample* x, const LweParams* params,  const TLweParams* rparams)
     *
     * tLweExtractLweSample computes ...
     */












    /* ********************
       Extractions TLwe -> Lwe
     ******************** */

    /*
       Testing the function tLweExtractKey
     * EXPORT void tLweExtractKey(LweKey* result, const TLweKey*); //sans doute un param supplémentaire
     *
     * tLweExtractKey computes ...
     */


}//namespace

