/* 
 * tgsw_test.cpp
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
#include "fakes/tgsw.h"

#define TFHE_TEST_ENVIRONMENT 1

using namespace std;
using namespace ::testing;

namespace {

    // we use the function rand because in the "const static" context the uniformly random generator doesn't work!
    const TGswKey *new_random_key(const TGswParams *params) {
        TGswKey *key = new_TGswKey(params);
        const int32_t N = params->tlwe_params->N;
        const int32_t k = params->tlwe_params->k;

        for (int32_t i = 0; i < k; ++i)
            for (int32_t j = 0; j < N; ++j)
                key->key[i].coefs[j] = rand() % 2;
        return key;
    }


    // we use the function rand because in the "const static" context the uniformly random generator doesn't work!
    IntPolynomial *new_random_IntPolynomial(const int32_t N) {
        IntPolynomial *poly = new_IntPolynomial(N);

        for (int32_t i = 0; i < N; ++i)
            poly->coefs[i] = rand() % 10 - 5;
        return poly;
    }


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

    const TGswKey *key512_1 = new_random_key(params512_1);
    const TGswKey *key512_2 = new_random_key(params512_2);
    const TGswKey *key1024_1 = new_random_key(params1024_1);
    const TGswKey *key1024_2 = new_random_key(params1024_2);
    const TGswKey *key2048_1 = new_random_key(params2048_1);
    const TGswKey *key2048_2 = new_random_key(params2048_2);
    vector<const TGswKey *> all_keys = {key512_1, key512_2, key1024_1, key1024_2, key2048_1, key2048_2};
    vector<const TGswKey *> all_keys1024 = {key1024_1, key1024_2};

    /* Tolerance factor for the equality between two TorusPolynomial */
    //const double toler = 1e-8;

    /* This class fixture is for testing tgsw functions that entirely
     * rely on the Tlwe api, by faking ideal Tlwe behaviour */
    class TGswTest : public ::testing::Test {
    public:
        USE_FAKE_tLweSymEncrypt;

        USE_FAKE_tLweSymEncryptT;

        USE_FAKE_tLwePhase;

        USE_FAKE_tLweApproxPhase;

        USE_FAKE_tLweSymDecrypt;

        USE_FAKE_tLweSymDecryptT;

        USE_FAKE_tLweClear;

        USE_FAKE_tLweCopy;

        USE_FAKE_tLweNoiselessTrivial;

        USE_FAKE_tLweAddTo;

        USE_FAKE_tLweSubTo;

        USE_FAKE_tLweAddMulTo;

        USE_FAKE_tLweSubMulTo;

        USE_FAKE_tLweSymEncryptZero;

        USE_FAKE_tLweAddMulRTo;

        USE_FAKE_tLweMulByXaiMinusOne;

        USE_FAKE_new_TLweSample_array;

        USE_FAKE_delete_TLweSample_array;

        USE_FAKE_new_TLweSample;

        USE_FAKE_delete_TLweSample;

        inline void tGswAddH(TGswSample *result, const TGswParams *params) {
            abort();
            FAIL() << "FORBIDDEN";
        }

        inline void tGswAddMuH(TGswSample *result, const IntPolynomial *message, const TGswParams *params) {
            abort();
            FAIL() << "FORBIDDEN";
        }

        inline void tGswAddMuIntH(TGswSample *result, const int32_t message, const TGswParams *params) {
            abort();
            FAIL() << "FORBIDDEN";
        }

        inline void tGswTorus32PolynomialDecompH(IntPolynomial *result, const TorusPolynomial *sample,
                                                 const TGswParams *params) {
            abort();
            FAIL() << "FORBIDDEN";
        }

        //this function creates a fixed (random-looking) result,
        //whose seed is the sample
        inline void tGswTLweDecompH(IntPolynomial *result, const TLweSample *sample, const TGswParams *params) {
            int32_t kpl = params->kpl;
            int32_t N = params->tlwe_params->N;
            const FakeTLwe *seed = fake(sample);
            for (int32_t i = 0; i < kpl; i++)
                for (int32_t j = 0; j < N; j++)
                    result[i].coefs[j] = (i + 3 * j + seed->message->coefsT[j]) % 25 - 12;
        }

        //this function will create a fixed (random-looking) TGsw sample
        inline void fullyRandomTGsw(TGswSample *result, double alpha, const TGswParams *params) {
            int32_t kpl = params->kpl;
            FakeTLwe *resrows = fake(result->all_sample);
            for (int32_t i = 0; i < kpl; i++) {
                torusPolynomialUniform(resrows[i].message);
                resrows[i].current_variance = alpha * alpha;
            }
        }


#define INCLUDE_TGSW_INIT
#define INCLUDE_TGSW_DESTROY
#define INCLUDE_TGSW_KEYGEN
#define INCLUDE_TGSW_CLEAR
//#define INCLUDE_TGSW_ADD_H
//#define INCLUDE_TGSW_ADD_MU_H
//#define INCLUDE_TGSW_ADD_MU_INT_H
#define INCLUDE_TGSW_ENCRYPT_ZERO
#define INCLUDE_TGSW_MUL_BY_XAI_MINUS_ONE
#define INCLUDE_TGSW_EXTERN_MUL_TO_TLWE
#define INCLUDE_TGSW_SYM_ENCRYPT
#define INCLUDE_TGSW_SYM_ENCRYPT_INT
#define INCLUDE_TGSW_ENCRYPT_B
#define INCLUDE_TGSW_DECRYPT
//#define INCLUDE_TGSW_TLWE_DECOMP_H
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
    };

    /* This class fixture is for testing tgsw functions that entirely
     * do direct access to raw coefficients: no fake is used */
    //EXPORT void tGswAddMuH(TGswSample* result, const IntPolynomial* message, const TGswParams* params);
    //EXPORT void tGswAddMuIntH(TGswSample* result, const int32_t message, const TGswParams* params);
    //EXPORT void tGswTorus32PolynomialDecompH(IntPolynomial* result, const TorusPolynomial* sample, const TGswParams* params);
    //EXPORT void tGswTLweDecompH(IntPolynomial* result, const TLweSample* sample,const TGswParams* params);	
    class TGswDirectTest : public ::testing::Test {
    public:

        //this function will create a fixed (random-looking) TGsw sample
        inline void fullyRandomTGsw(TGswSample *result, double alpha, const TGswParams *params) {
            int32_t l = params->l;
            int32_t k = params->tlwe_params->k;

            for (int32_t i = 0; i <= k; i++) {
                for (int32_t j = 0; j < l; j++) {
                    TLweSample *row = &result->bloc_sample[i][j];
                    for (int32_t u = 0; u <= k; u++) {
                        torusPolynomialUniform(row->a + u);
                    }
                    row->current_variance = alpha * alpha;
                }
            }
        }
    };

    /* This class fixture is for testing tgsw functions that call other 
     * already tested tgsw functions: Tgsw is faked */
    class TGswFakeTest : public ::testing::Test {
    public:

        USE_FAKE_init_TGswSample;

        USE_FAKE_destroy_TGswSample;

        USE_FAKE_tGswAddH;

        USE_FAKE_tGswAddMuH;

        USE_FAKE_tGswAddMuIntH;

        USE_FAKE_tGswEncryptZero;

        USE_FAKE_tGswClear;

#define INCLUDE_TGSW_SYM_ENCRYPT
#define INCLUDE_TGSW_SYM_ENCRYPT_INT
#define INCLUDE_TGSW_ENCRYPT_B
#define INCLUDE_TGSW_NOISELESS_TRIVIAL

#include "../libtfhe/tgsw-functions.cpp"
    };


#if 0
    /*
     * Definition of the function absfrac: | frac(d) |
     * Computes the absolute value of the fractional part of a double d 
     */
    double absfrac(double d) {return abs(d-rint(d));}


    /*
     * Definition of the function fillRandom
     * Fills a TLweSample with random Torus32 values (uniform distribution) 
     */
    void fillRandom(TLweSample* result, const TLweParams* params) {
    const int32_t k = params->k;
    const int32_t N = params->N;

    for (int32_t i = 0; i <= k; ++i)
        for (int32_t j = 0; j < N; ++j)
        result->a[i].coefsT[j] = uniformTorus32_distrib(generator);
    result->current_variance=0.2;
    }


    /*
     * Definition of the function copySample
     * Copies a TLweSample
     */
    void copySample(TLweSample* result, const TLweSample* sample, const TLweParams* params) {
    const int32_t k = params->k;
    const int32_t N = params->N;

    for (int32_t i = 0; i <= k; ++i)
        for (int32_t j = 0; j < N; ++j)
        result->a[i].coefsT[j] = sample->a[i].coefsT[j];
    result->current_variance=sample->current_variance;
    }
#endif




    /* ***************************************************************
     *************************** TESTS ********************************
     *************************************************************** */


    /*
     *  Testing the function tGswKeyGen
     * EXPORT void tLweKeyGen(TLweKey* result);
     * 
     * This function generates a random TLwe key for the given parameters
     * The TLwe key for the result must be allocated and initialized
     * (this means that the parameters are already in the result)
     */
    //EXPORT void tGswKeyGen(TGswKey* result);
    TEST_F(TGswTest, tGswKeyGen) {
        for (const TGswParams *param: all_params) {
            TGswKey *key = new_TGswKey(param);
            int32_t k = param->tlwe_params->k;
            int32_t N = param->tlwe_params->N;

            tGswKeyGen(key);
            for (int32_t i = 0; i < k; i++) {
                for (int32_t j = 0; j < N; ++j) {
                    ASSERT_TRUE(key->key[i].coefs[j] == 0 || key->key[i].coefs[j] == 1);
                }
            }

            delete_TGswKey(key);
        }
    }



    //EXPORT void tGswSymEncrypt(TGswSample* result, const IntPolynomial* message, double alpha, const TGswKey* key);
    TEST_F(TGswFakeTest, tGswSymEncrypt) {
        for (const TGswKey *key: all_keys) {
            int32_t N = key->params->tlwe_params->N;
            TGswSample *s = new_TGswSample(key->params);
            IntPolynomial *mess = new_random_IntPolynomial(N);
            double alpha = 4.2; // valeur pseudo aleatoire fixé

            tGswSymEncrypt(s, mess, alpha, key);

            FakeTGsw *fs = fake(s);
            for (int32_t j = 0; j < N; j++)
                ASSERT_EQ(fs->message->coefs[j], mess->coefs[j]);
            ASSERT_EQ(fs->current_variance, alpha * alpha);

            delete_IntPolynomial(mess);
            delete_TGswSample(s);
        }
    }



    //EXPORT void tGswSymEncryptInt(TGswSample* result, const int32_t message, double alpha, const TGswKey* key);
    TEST_F(TGswFakeTest, tGswSymEncryptInt) {
        for (const TGswKey *key: all_keys) {
            int32_t N = key->params->tlwe_params->N;
            TGswSample *s = new_TGswSample(key->params);

            int32_t mess = rand() % 1000 - 500;
            double alpha = 3.14; // valeur pseudo aleatoire fixé

            tGswSymEncryptInt(s, mess, alpha, key);

            FakeTGsw *fs = fake(s);
            ASSERT_EQ(fs->message->coefs[0], mess);
            for (int32_t j = 1; j < N; j++)
                ASSERT_EQ(fs->message->coefs[j], 0);
            ASSERT_EQ(fs->current_variance, alpha * alpha);

            delete_TGswSample(s);
        }
    }



    // ILA: we do not decrypt TGSW, no test for now
    //EXPORT void tGswSymDecrypt(IntPolynomial* result, const TGswSample* sample, const TGswKey* key, const int32_t Msize);
    //EXPORT int32_t tGswSymDecryptInt(const TGswSample* sample, const TGswKey* key); 

    //// support Functions for TGsw
    //// Result = 0
    //EXPORT void tGswClear(TGswSample* result, const TGswParams* params);
    //ILA: on devrait verifier aussi la variance?
    TEST_F(TGswTest, tGswClear) {
        for (const TGswParams *param: all_params) {
            TGswSample *s = new_TGswSample(param);
            int32_t kpl = param->kpl;
            TorusPolynomial *zeroPol = new_TorusPolynomial(param->tlwe_params->N);

            torusPolynomialClear(zeroPol);
            tGswClear(s, param);
            for (int32_t i = 0; i < kpl; i++) {
                FakeTLwe *si = fake(&s->all_sample[i]);
                ASSERT_EQ(torusPolynomialNormInftyDist(si->message, zeroPol), 0);
            }

            delete_TorusPolynomial(zeroPol);
            delete_TGswSample(s);
        }
    }









    // ***************************************************************************************************************
    // TGswDirectTest will be used to do direct tests while the function to be tested is not compatible with the fakes
    // ***************************************************************************************************************

    //// Result += H
    //EXPORT void tGswAddH(TGswSample* result, const TGswParams* params);
    TEST_F(TGswDirectTest, tGswAddH) {
        for (const TGswParams *params: all_params) {
            TGswSample *s = new_TGswSample(params);
            TGswSample *stemp = new_TGswSample(params);
            int32_t kpl = params->kpl;
            int32_t l = params->l;
            int32_t k = params->tlwe_params->k;
            int32_t N = params->tlwe_params->N;
            Torus32 *h = params->h;
            double alpha = 4.2; // valeur pseudo aleatoire fixé

            // make a full random TGSW
            fullyRandomTGsw(s, alpha, params);

            // copy s to stemp
            for (int32_t i = 0; i < kpl; ++i) {
                tLweCopy(&stemp->all_sample[i], &s->all_sample[i], params->tlwe_params);
            }

            tGswAddH(s, params);

            //verify all coefficients
            for (int32_t bloc = 0; bloc <= k; bloc++) {
                for (int32_t i = 0; i < l; ++i) {
                    ASSERT_EQ(s->bloc_sample[bloc][i].current_variance, stemp->bloc_sample[bloc][i].current_variance);
                    for (int32_t u = 0; u <= k; u++) {
                        //verify that pol[bloc][i][u]=initial[bloc][i][u]+(bloc==u?hi:0)
                        TorusPolynomial *newpol = &s->bloc_sample[bloc][i].a[u];
                        TorusPolynomial *oldpol = &stemp->bloc_sample[bloc][i].a[u];
                        ASSERT_EQ(newpol->coefsT[0], oldpol->coefsT[0] + (bloc == u ? h[i] : 0));
                        for (int32_t j = 1; j < N; j++)
                            ASSERT_EQ(newpol->coefsT[j], oldpol->coefsT[j]);
                    }
                }
            }

            delete_TGswSample(stemp);
            delete_TGswSample(s);
        }
    }


    //// Result += mu*H
    //EXPORT void tGswAddMuH(TGswSample* result, const IntPolynomial* message, const TGswParams* params);
    TEST_F(TGswDirectTest, tGswAddMuH) {
        for (const TGswParams *params: all_params) {
            TGswSample *s = new_TGswSample(params);
            TGswSample *stemp = new_TGswSample(params);
            int32_t kpl = params->kpl;
            int32_t l = params->l;
            int32_t k = params->tlwe_params->k;
            int32_t N = params->tlwe_params->N;
            Torus32 *h = params->h;
            double alpha = 4.2; // valeur pseudo aleatoire fixé
            IntPolynomial *mess = new_random_IntPolynomial(N);

            // make a full random TGSW
            fullyRandomTGsw(s, alpha, params);


            // copy s to stemp
            for (int32_t i = 0; i < kpl; ++i) {
                tLweCopy(&stemp->all_sample[i], &s->all_sample[i], params->tlwe_params);
            }

            tGswAddMuH(s, mess, params);

            //verify all coefficients
            for (int32_t bloc = 0; bloc <= k; bloc++) {
                for (int32_t i = 0; i < l; ++i) {
                    ASSERT_EQ(s->bloc_sample[bloc][i].current_variance, stemp->bloc_sample[bloc][i].current_variance);
                    for (int32_t u = 0; u <= k; u++) {
                        //verify that pol[bloc][i][u]=initial[bloc][i][u]+(bloc==u?hi*mess:0)
                        TorusPolynomial *newpol = &s->bloc_sample[bloc][i].a[u];
                        TorusPolynomial *oldpol = &stemp->bloc_sample[bloc][i].a[u];
                        if (bloc == u) {
                            for (int32_t j = 0; j < N; j++)
                                ASSERT_EQ(newpol->coefsT[j], oldpol->coefsT[j] + h[i] * mess->coefs[j]);
                        } else {
                            for (int32_t j = 0; j < N; j++)
                                ASSERT_EQ(newpol->coefsT[j], oldpol->coefsT[j]);
                        }
                    }
                }
            }

            delete_IntPolynomial(mess);
            delete_TGswSample(stemp);
            delete_TGswSample(s);
        }
    }




    //// Result += mu*H, mu integer
    //EXPORT void tGswAddMuIntH(TGswSample* result, const int32_t message, const TGswParams* params);
    TEST_F(TGswDirectTest, tGswAddMuIntH) {
        for (const TGswParams *params: all_params) {
            TGswSample *s = new_TGswSample(params);
            TGswSample *stemp = new_TGswSample(params);
            int32_t kpl = params->kpl;
            int32_t l = params->l;
            int32_t k = params->tlwe_params->k;
            int32_t N = params->tlwe_params->N;
            Torus32 *h = params->h;
            double alpha = 4.2; // valeur pseudo aleatoire fixé
            int32_t mess = rand() * 2345 - 1234;

            // make a full random TGSW
            fullyRandomTGsw(s, alpha, params);


            // copy s to stemp
            for (int32_t i = 0; i < kpl; ++i) {
                tLweCopy(&stemp->all_sample[i], &s->all_sample[i], params->tlwe_params);
            }

            tGswAddMuIntH(s, mess, params);

            //verify all coefficients
            for (int32_t bloc = 0; bloc <= k; bloc++) {
                for (int32_t i = 0; i < l; ++i) {
                    ASSERT_EQ(s->bloc_sample[bloc][i].current_variance, stemp->bloc_sample[bloc][i].current_variance);
                    for (int32_t u = 0; u <= k; u++) {
                        //verify that pol[bloc][i][u]=initial[bloc][i][u]+(bloc==u?hi*mess:0)
                        TorusPolynomial *newpol = &s->bloc_sample[bloc][i].a[u];
                        TorusPolynomial *oldpol = &stemp->bloc_sample[bloc][i].a[u];
                        ASSERT_EQ(newpol->coefsT[0], oldpol->coefsT[0] + (bloc == u ? h[i] * mess : 0));
                        for (int32_t j = 1; j < N; j++)
                            ASSERT_EQ(newpol->coefsT[j], oldpol->coefsT[j]);
                    }
                }
            }

            delete_TGswSample(stemp);
            delete_TGswSample(s);
        }
    }





    //// Result = tGsw(0)
    //EXPORT void tGswEncryptZero(TGswSample* result, double alpha, const TGswKey* key);
    TEST_F(TGswTest, tGswEncryptZero) {
        for (const TGswKey *key: all_keys) {
            int32_t kpl = key->params->kpl;
            TGswSample *s = new_TGswSample(key->params);
            double alpha = 4.2; // valeur pseudo aleatoire fixé

            // Zero polynomial
            TorusPolynomial *zeroPol = new_TorusPolynomial(key->params->tlwe_params->N);
            torusPolynomialClear(zeroPol);

            tGswEncryptZero(s, alpha, key);
            for (int32_t i = 0; i < kpl; ++i) {
                FakeTLwe *si = fake(&s->all_sample[i]);
                ASSERT_EQ(torusPolynomialNormInftyDist(si->message, zeroPol), 0);
                ASSERT_EQ(si->current_variance, alpha * alpha);
            }

            delete_TorusPolynomial(zeroPol);
            delete_TGswSample(s);
        }
    }





    //EXPORT void tGswTorus32PolynomialDecompH(IntPolynomial* result, const TorusPolynomial* sample, const TGswParams* params);
    // sample: torus polynomial with N coefficients
    // result: int32_t polynomial with Nl coefficients
    TEST_F(TGswDirectTest, tGswTorus32PolynomialDecompH) {
        for (const TGswParams *param: all_params) {
            int32_t N = param->tlwe_params->N;
            int32_t l = param->l;
            int32_t Bgbit = param->Bgbit;
            Torus32 *h = param->h;

            //compute the tolerance
            int32_t toler = 0;
            if (Bgbit * l < 32) toler = 1 << (32 - Bgbit * l);
            //printf("%d,%d,%d\n",Bgbit,l,toler);

            IntPolynomial *result = new_IntPolynomial_array(l, N);
            TorusPolynomial *sample = new_TorusPolynomial(N);
            torusPolynomialUniform(sample);

            tGswTorus32PolynomialDecompH(result, sample, param);


            for (int32_t i = 0; i < N; ++i) {
                // recomposition
                Torus32 test = 0;
                for (int32_t j = 0; j < l; ++j) {
                    test += result[j].coefs[i] * h[j];
                }
                ASSERT_LE(abs(test - sample->coefsT[i]), toler); //exact or approx decomposition
            }

            delete_TorusPolynomial(sample);
            delete_IntPolynomial_array(l, result);
        }
    }



    //EXPORT void tGswTLweDecompH(IntPolynomial* result, const TLweSample* sample,const TGswParams* params);	
    // Test direct Result*H donne le bon resultat
    // sample: TLweSample composed by k+1 torus polynomials, each with N coefficients
    // result: int32_t polynomial with Nl(k+1) coefficients
    TEST_F(TGswDirectTest, tGswTLweDecompH) {
        for (const TGswParams *param: all_params) {
            int32_t N = param->tlwe_params->N;
            int32_t k = param->tlwe_params->k;
            int32_t Bgbit = param->Bgbit;
            int32_t l = param->l;
            int32_t kpl = param->kpl;
            Torus32 *h = param->h;

            //compute the tolerance
            int32_t toler = 0;
            if (Bgbit * l < 32) toler = 1 << (32 - Bgbit * l);
            //printf("%d,%d,%d\n",Bgbit,l,toler);

            IntPolynomial *result = new_IntPolynomial_array(kpl, N);
            TLweSample *sample = new_TLweSample(param->tlwe_params);

            // sample randomly generated
            for (int32_t bloc = 0; bloc <= k; ++bloc) {
                torusPolynomialUniform(&sample->a[bloc]);
            }

            tGswTLweDecompH(result, sample, param);

            for (int32_t bloc = 0; bloc <= k; ++bloc) {
                for (int32_t i = 0; i < N; i++) {
                    Torus32 test = 0;
                    for (int32_t j = 0; j < l; ++j) {
                        test += result[bloc * l + j].coefs[i] * h[j];
                    }
                    ASSERT_LE(abs(test - sample->a[bloc].coefsT[i]), toler); //exact or approx decomposition
                }
            }

            delete_TLweSample(sample);
            delete_IntPolynomial_array(kpl, result);
        }
    }



    ////TODO: Ilaria.Theoreme3.5
    //EXPORT void tGswExternProduct(TLweSample* result, const TGswSample* a, const TLweSample* b, const TGswParams* params);
    TEST_F(TGswTest, tGswExternProduct) {
        for (const TGswParams *params: all_params) {
            const int32_t N = params->tlwe_params->N;
            const int32_t kpl = params->kpl;

            TGswSample *sample = new_TGswSample(params);

            TLweSample *result = new_TLweSample(params->tlwe_params);
            TLweSample *b = new_TLweSample(params->tlwe_params);
            FakeTLwe *fresult = fake(result);
            FakeTLwe *fb = fake(b);
            FakeTLwe *fsamplerows = fake(sample->all_sample);
            double alpha = 4.2; // valeur pseudo aleatoire fixé


            fullyRandomTGsw(sample, alpha, params);
            torusPolynomialUniform(fb->message);

            IntPolynomial *decomp = new_IntPolynomial_array(kpl, N);
            tGswTLweDecompH(decomp, b, params);
            TorusPolynomial *expectedRes = new_TorusPolynomial(N);
            TorusPolynomial *tmp = new_TorusPolynomial(N);

            torusPolynomialClear(expectedRes);
            for (int32_t i = 0; i < kpl; i++) {
                torusPolynomialMultKaratsuba(tmp, decomp + i, fsamplerows[i].message);
                torusPolynomialAddTo(expectedRes, tmp);
            }

            tGswExternProduct(result, sample, b, params);

            ASSERT_EQ(torusPolynomialNormInftyDist(fresult->message, expectedRes), 0);

            delete_TorusPolynomial(tmp);
            delete_TorusPolynomial(expectedRes);
            delete_IntPolynomial_array(kpl, decomp);
            delete_TLweSample(b);
            delete_TLweSample(result);
            delete_TGswSample(sample);
        }
    }





    //// result=result+ (X^ai-1)*bi (ligne 5 de l'algo)
    //EXPORT void tGswMulByXaiMinusOne(TGswSample* result, int32_t ai, const TGswSample* bk, const TGswParams* params);
    TEST_F(TGswTest, tGswMulByXaiMinusOne) {
        for (const TGswKey *key: all_keys) {
            int32_t kpl = key->params->kpl;
            int32_t N = key->params->tlwe_params->N;
            for (int32_t ai = 0; ai < 2 * N; ai += 235) {
                TGswSample *res = new_TGswSample(key->params);
                TGswSample *bk = new_TGswSample(key->params);
                double alpha = 4.2; // valeur pseudo aleatoire fixé
                TorusPolynomial *poly = new_TorusPolynomial(N);

                //generate all rows at random
                FakeTLwe *fresrows = fake(res->all_sample);
                FakeTLwe *fbkrows = fake(bk->all_sample);
                fullyRandomTGsw(bk, alpha, key->params);

                tGswMulByXaiMinusOne(res, ai, bk, key->params);

                for (int32_t i = 0; i < kpl; i++) {
                    torusPolynomialMulByXaiMinusOne(poly, ai, fbkrows[i].message);
                    ASSERT_EQ(torusPolynomialNormInftyDist(fresrows[i].message, poly), 0);
                    ASSERT_EQ(fresrows[i].current_variance, (ai == 0 ? 1 : 2) * fbkrows[i].current_variance);
                }

                delete_TorusPolynomial(poly);
                delete_TGswSample(bk);
                delete_TGswSample(res);
            }
        }
    }





    ////ligne 5 algo,mult externe
    //EXPORT void tGswExternMulToTLwe(TLweSample* accum, const TGswSample* sample, const TGswParams* params);
    //accum *= sample
    TEST_F(TGswTest, tGswExternMulToTLwe) {
        for (const TGswKey *key: all_keys) {
            const TGswParams *params = key->params;
            const int32_t N = params->tlwe_params->N;
            const int32_t kpl = params->kpl;

            TGswSample *sample = new_TGswSample(params);
            TLweSample *accum = new_TLweSample(params->tlwe_params);
            FakeTLwe *faccum = fake(accum);
            FakeTLwe *fsamplerows = fake(sample->all_sample);
            double alpha = 4.2; // valeur pseudo aleatoire fixé


            fullyRandomTGsw(sample, alpha, params);
            torusPolynomialUniform(faccum->message);

            IntPolynomial *decomp = new_IntPolynomial_array(kpl, N);
            tGswTLweDecompH(decomp, accum, params);
            TorusPolynomial *expectedRes = new_TorusPolynomial(N);
            TorusPolynomial *tmp = new_TorusPolynomial(N);

            torusPolynomialClear(expectedRes);
            for (int32_t i = 0; i < kpl; i++) {
                torusPolynomialAddMulRKaratsuba(expectedRes, decomp + i, fsamplerows[i].message);
            }

            tGswExternMulToTLwe(accum, sample, params);

            ASSERT_EQ(torusPolynomialNormInftyDist(faccum->message, expectedRes), 0);

            delete_TorusPolynomial(tmp);
            delete_TorusPolynomial(expectedRes);
            delete_IntPolynomial_array(kpl, decomp);
            delete_TLweSample(accum);
            delete_TGswSample(sample);
        }
    }




    /** result = (0,mu) */
    //EXPORT void tGswNoiselessTrivial(TGswSample* result, const IntPolynomial* mu, const TGswParams* params);
    TEST_F(TGswFakeTest, tGswNoiselessTrivial) {
        for (const TGswParams *param: all_params) {
            int32_t N = param->tlwe_params->N;
            TGswSample *res = new_TGswSample(param);
            IntPolynomial *mu = new_random_IntPolynomial(N);
            FakeTGsw *fres = fake(res);

            tGswNoiselessTrivial(res, mu, param);

            for (int32_t j = 0; j < N; j++)
                ASSERT_EQ(fres->message->coefs[j], mu->coefs[j]);
            ASSERT_EQ(fres->current_variance, 0.);

            delete_IntPolynomial(mu);
            delete_TGswSample(res);
        }
    }




    // ILA: Not used for now
    /** result = result + sample */
    //EXPORT void tGswAddTo(TGswSample* result, const TGswSample* sample, const TGswParams* params);
    /** result = result - sample */
    //EXPORT void tGswSubTo(TLweSample* result, const TLweSample* sample, const TLweParams* params);
    /** result = result + p.sample */
    //EXPORT void tGswAddMulTo(TLweSample* result, int32_t p, const TLweSample* sample, const TLweParams* params);
    /** result = result - p.sample */
    //EXPORT void tGswSubMulTo(TLweSample* result, int32_t p, const TLweSample* sample, const TLweParams* params);

}//namespace

