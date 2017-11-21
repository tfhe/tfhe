#include <gtest/gtest.h>
#include <lwe-functions.h>
#include <numeric_functions.h>

using namespace std;

namespace {
    //this function creates a new lwekey and initializes it with random
    //values. We do not use the c++11 random generator, since it gets in
    //a deadlock mode on static const initializers
    const LweKey *new_random_LweKey(const LweParams *params) {
        LweKey *key = new_LweKey(params);
        const int32_t n = params->n;
        for (int32_t i = 0; i < n; i++)
            key->key[i] = rand() % 2;
        return key;
    }

    class LweTest : public ::testing::Test {
    public:
        //TODO: parallelization
        static const LweParams *params500;
        static const LweParams *params750;
        static const LweParams *params1024;
        static const LweKey *key500;
        static const LweKey *key750;
        static const LweKey *key1024;
        static const vector<const LweParams *> all_params;
        static const vector<const LweKey *> all_keys;
    };

    const LweParams *LweTest::params500 = new_LweParams(500, 0., 1.);
    const LweParams *LweTest::params750 = new_LweParams(750, 0., 1.);
    const LweParams *LweTest::params1024 = new_LweParams(1024, 0., 1.);
    const LweKey *LweTest::key500 = new_random_LweKey(params500);
    const LweKey *LweTest::key750 = new_random_LweKey(params750);
    const LweKey *LweTest::key1024 = new_random_LweKey(params1024);
    const vector<const LweParams *> LweTest::all_params = {params500, params750, params1024};
    const vector<const LweKey *> LweTest::all_keys = {key500, key750, key1024};

    // | frac(x) |
    double absfrac(double d) { return abs(d - rint(d)); }

    // fills a LweSample with random Torus32
    void fillRandom(LweSample *result, const LweParams *params) {
        const int32_t n = params->n;
        for (int32_t i = 0; i < n; i++) result->a[i] = uniformTorus32_distrib(generator);
        result->b = uniformTorus32_distrib(generator);
        result->current_variance = 0.2;
    }

    // copy a LweSample
    void copySample(LweSample *result, const LweSample *sample, const LweParams *params) {
        const int32_t n = params->n;
        for (int32_t i = 0; i < n; i++) result->a[i] = sample->a[i];
        result->b = sample->b;
        result->current_variance = sample->current_variance;
    }


    // This function generates a random Lwe key for the given parameters.
    // The Lwe key for the result must be allocated and initialized
    // (this means that the parameters are already in the result)
    TEST_F(LweTest, lweKeyGen) {
        for (const LweParams *params: all_params) {
            LweKey *key = new_LweKey(params);
            lweKeyGen(key);
            ASSERT_EQ(params, key->params);
            int32_t n = key->params->n;
            int32_t *s = key->key;
            //verify that the key is binary and kind-of random
            int32_t count = 0;
            for (int32_t i = 0; i < n; i++) {
                ASSERT_TRUE(s[i] == 0 || s[i] == 1);
                count += s[i];
            }
            ASSERT_LE(count, n - 20);
            ASSERT_GE(count, 20);
            delete_LweKey(key);
        }
    }

    // This function encrypts message by using key, with stdev alpha
    // The Lwe sample for the result must be allocated and initialized
    // (this means that the parameters are already in the result)
    TEST_F (LweTest, lweSymEncryptPhaseDecrypt) {
        //TODO: parallelization
        static const int32_t NB_SAMPLES = 10;
        static const int32_t M = 8;
        static const double alpha = 1. / (10. * M);
        for (const LweKey *key: all_keys) {
            const LweParams *params = key->params;
            LweSample *samples = new_LweSample_array(NB_SAMPLES, params);
            //verify correctness of the decryption
            for (int32_t trial = 0; trial < NB_SAMPLES; trial++) {
                Torus32 message = modSwitchToTorus32(trial, M);
                lweSymEncrypt(&samples[trial], message, alpha, key);
                Torus32 phase = lwePhase(&samples[trial], key);
                Torus32 decrypt = lweSymDecrypt(&samples[trial], key, M);
                double dmessage = t32tod(message);
                double dphase = t32tod(phase);
                ASSERT_EQ(message, decrypt);
                ASSERT_LE(absfrac(dmessage - dphase), 10. * alpha);
                ASSERT_EQ(alpha * alpha, samples[trial].current_variance);
            }
            //verify that samples are random enough (all coordinates different)
            const int32_t n = params->n;
            for (int32_t i = 0; i < n; i++) {
                set<Torus32> testset;
                for (int32_t trial = 0; trial < NB_SAMPLES; trial++) {
                    testset.insert(samples[trial].a[i]);
                }
                ASSERT_GE(testset.size(), 0.9 * NB_SAMPLES);
            }
            delete_LweSample_array(NB_SAMPLES, samples);
        }
    }

    //Arithmetic operations on Lwe samples
    // result = (0,0)
    TEST_F(LweTest, lweClear) {
        for (const LweKey *key: all_keys) {
            const LweParams *params = key->params;
            const int32_t n = params->n;
            LweSample *sample = new_LweSample(params);
            fillRandom(sample, params);
            lweClear(sample, params);
            for (int32_t i = 0; i < n; i++) {
                ASSERT_EQ(0, sample->a[i]);
            }
            ASSERT_EQ(0, sample->b);
            ASSERT_EQ(0., sample->current_variance);
            delete_LweSample(sample);
        }
    }

    // result = (0,mu)
    TEST_F(LweTest, lweNoiselessTrivial) {
        for (const LweKey *key: all_keys) {
            const Torus32 message = uniformTorus32_distrib(generator);
            const LweParams *params = key->params;
            const int32_t n = params->n;
            LweSample *sample = new_LweSample(params);
            fillRandom(sample, params);
            lweNoiselessTrivial(sample, message, params);
            for (int32_t i = 0; i < n; i++) {
                ASSERT_EQ(0, sample->a[i]);
            }
            ASSERT_EQ(message, sample->b);
            ASSERT_EQ(0., sample->current_variance);
            delete_LweSample(sample);
        }
    }


    // result = result + sample */
    //EXPORT void lweAddTo(LweSample* result, const LweSample* sample, const LweParams* params);
    TEST_F(LweTest, lweAddTo) {
        for (const LweKey *key: all_keys) {
            const LweParams *params = key->params;
            const int32_t n = params->n;
            LweSample *a = new_LweSample(params);
            LweSample *b = new_LweSample(params);
            LweSample *acopy = new_LweSample(params);
            fillRandom(a, params);
            fillRandom(b, params);
            copySample(acopy, a, params);
            lweAddTo(a, b, params);
            for (int32_t i = 0; i < n; i++) {
                ASSERT_EQ(acopy->a[i] + b->a[i], a->a[i]);
            }
            ASSERT_EQ(acopy->b + b->b, a->b);
            ASSERT_EQ(acopy->current_variance + b->current_variance, a->current_variance);
            delete_LweSample(a);
            delete_LweSample(b);
            delete_LweSample(acopy);
        }
    }

    // result = result - sample
    //EXPORT void lweSubTo(LweSample* result, const LweSample* sample, const LweParams* params);
    TEST_F(LweTest, lweSubTo) {
        for (const LweKey *key: all_keys) {
            const LweParams *params = key->params;
            const int32_t n = params->n;
            LweSample *a = new_LweSample(params);
            LweSample *b = new_LweSample(params);
            LweSample *acopy = new_LweSample(params);
            fillRandom(a, params);
            fillRandom(b, params);
            copySample(acopy, a, params);
            lweSubTo(a, b, params);
            for (int32_t i = 0; i < n; i++) {
                ASSERT_EQ(acopy->a[i] - b->a[i], a->a[i]);
            }
            ASSERT_EQ(acopy->b - b->b, a->b);
            ASSERT_EQ(acopy->current_variance + b->current_variance, a->current_variance);
            delete_LweSample(a);
            delete_LweSample(b);
            delete_LweSample(acopy);
        }
    }


    // result = result + p.sample
    //EXPORT void lweAddMulTo(LweSample* result, int32_t p, const LweSample* sample, const LweParams* params);
    TEST_F(LweTest, lweAddMulTo) {
        const int32_t p = 3;
        for (const LweKey *key: all_keys) {
            const LweParams *params = key->params;
            const int32_t n = params->n;
            LweSample *a = new_LweSample(params);
            LweSample *b = new_LweSample(params);
            LweSample *acopy = new_LweSample(params);
            fillRandom(a, params);
            fillRandom(b, params);
            copySample(acopy, a, params);
            lweAddMulTo(a, p, b, params);
            for (int32_t i = 0; i < n; i++) {
                ASSERT_EQ(acopy->a[i] + p * b->a[i], a->a[i]);
            }
            ASSERT_EQ(acopy->b + p * b->b, a->b);
            ASSERT_EQ(acopy->current_variance + p * p * b->current_variance, a->current_variance);
            delete_LweSample(a);
            delete_LweSample(b);
            delete_LweSample(acopy);
        }
    }

    // result = result - p.sample
    //EXPORT void lweSubMulTo(LweSample* result, int32_t p, const LweSample* sample, const LweParams* params);
    TEST_F(LweTest, lweSubMulTo) {
        const int32_t p = 3;
        for (const LweKey *key: all_keys) {
            const LweParams *params = key->params;
            const int32_t n = params->n;
            LweSample *a = new_LweSample(params);
            LweSample *b = new_LweSample(params);
            LweSample *acopy = new_LweSample(params);
            fillRandom(a, params);
            fillRandom(b, params);
            copySample(acopy, a, params);
            lweSubMulTo(a, p, b, params);
            for (int32_t i = 0; i < n; i++) {
                ASSERT_EQ(acopy->a[i] - p * b->a[i], a->a[i]);
            }
            ASSERT_EQ(acopy->b - p * b->b, a->b);
            ASSERT_EQ(acopy->current_variance + p * p * b->current_variance, a->current_variance);
            delete_LweSample(a);
            delete_LweSample(b);
            delete_LweSample(acopy);
        }
    }

#if 0

    //TODO: à tester!!

    // 
    // creates a Key Switching Key between the two keys
    ///
    EXPORT void lweCreateKeySwitchKey(LweKeySwitchKey* result, const LweKey* in_key, const LweKey* out_key);


    //
    // applies keySwitching
    ///
    EXPORT void lweKeySwitch(LweSample* result, const LweKeySwitchKey* ks, const LweSample* sample);

#endif //Lwe_FUNCTIONS_H


}//namespace
