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
#define TFHE_TEST_ENVIRONMENT 1

using namespace std;
using namespace ::testing;

namespace {

    // we use the function rand because in the "const static" context the uniformly random generator doesn't work!
    const TGswKey* new_random_key(const TGswParams* params) {
	TGswKey* key = new_TGswKey(params);
	const int N = params->tlwe_params->N;
	const int k = params->tlwe_params->k;

	for (int i = 0; i < k; ++i)
	    for (int j = 0; j < N; ++j)
		key->key[i].coefs[j] = rand()%2;	
	return key;
    }	



    // we use the function rand because in the "const static" context the uniformly random generator doesn't work!
    IntPolynomial* new_random_IntPolynomial(const int N) {
        IntPolynomial* poly = new_IntPolynomial(N);
        
        for (int i = 0; i < N; ++i)
            poly->coefs[i] = rand()%2; // ILA: ça suffit?   
        return poly;
    }





    /*
     * Parameters and keys (for N=512,1024,2048 and k=1,2)
     */
    const TGswParams* params512_1 = new_TGswParams(4,8,new_TLweParams(512,1,0.,1.));
    const TGswParams* params512_2 = new_TGswParams(3,10,new_TLweParams(512,2,0.,1.));
    const TGswParams* params1024_1 = new_TGswParams(3,10,new_TLweParams(1024,1,0.,1.));
    const TGswParams* params1024_2 = new_TGswParams(4,8,new_TLweParams(1024,2,0.,1.));
    const TGswParams* params2048_1 = new_TGswParams(4,8,new_TLweParams(2048,1,0.,1.));
    const TGswParams* params2048_2 = new_TGswParams(3,10,new_TLweParams(2048,2,0.,1.));
    vector<const TGswParams*> all_params = {params512_1, params512_2, params1024_1, params1024_2, params2048_1, params2048_2};

    const TGswKey* key512_1 = new_random_key(params512_1);
    const TGswKey* key512_2 = new_random_key(params512_2);
    const TGswKey* key1024_1 = new_random_key(params1024_1);
    const TGswKey* key1024_2 = new_random_key(params1024_2);
    const TGswKey* key2048_1 = new_random_key(params2048_1);
    const TGswKey* key2048_2 = new_random_key(params2048_2);
    vector<const TGswKey*> all_keys = {key512_1, key512_2, key1024_1, key1024_2, key2048_1, key2048_2};

    /* Tolerance factor for the equality between two TorusPolynomial */
    const double toler = 1e-8; 

    class TGswTest: public ::testing::Test {
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
#define INCLUDE_TGSW_TORUS32POLYNOMIAL_DECOMP_H
#define INCLUDE_TGSW_EXTERN_PRODUCT
#define INCLUDE_TGSW_NOISELESS_TRIVIAL
#include "../libtfhe/tgsw-functions.cpp"
    };

    class TGswDirectTest: public ::testing::Test {
	public:

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
#define INCLUDE_TGSW_TORUS32POLYNOMIAL_DECOMP_H
#define INCLUDE_TGSW_EXTERN_PRODUCT
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
    	const int k = params->k;
	const int N = params->N;

	for (int i = 0; i <= k; ++i)
	    for (int j = 0; j < N; ++j)
		result->a[i].coefsT[j] = uniformTorus32_distrib(generator);
	result->current_variance=0.2;
    }


    /*
     * Definition of the function copySample
     * Copies a TLweSample
     */
    void copySample(TLweSample* result, const TLweSample* sample, const TLweParams* params) {
	const int k = params->k;
	const int N = params->N;

	for (int i = 0; i <= k; ++i) 
	    for (int j = 0; j < N; ++j)
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
        for (const TGswParams* param: all_params) {
            TGswKey* key = new_TGswKey(param);
            int k = param->tlwe_params->k;
            int N = param->tlwe_params->N;

            tGswKeyGen(key);
            for (int i=0; i<k; i++) {
                for (int j=0; j<N; ++j){
                    ASSERT_TRUE(key->key[i].coefs[j]==0 || key->key[i].coefs[j]==1);
                }                
            }

            delete_TGswKey(key);
        }
    }



    //EXPORT void tGswSymEncrypt(TGswSample* result, const IntPolynomial* message, double alpha, const TGswKey* key);
    TEST_F(TGswTest, tGswSymEncrypt) {
        for (const TGswKey* key: all_keys) {
            int l = key->params->l;
            int k = key->params->tlwe_params->k;
            int N = key->params->tlwe_params->N;
            Torus32* h = key->params->h;
            TGswSample* s = new_TGswSample(key->params);
            IntPolynomial* mess = new_random_IntPolynomial(N);
            TorusPolynomial* tempPoly = new_TorusPolynomial(N);
            double alpha = 4.2; // valeur pseudo aleatoire fixé
            
            tGswSymEncrypt(s, mess, alpha, key);
            for (int bloc=0; bloc<=k; bloc++) {
                for (int i=0; i<l; ++i)
                {
                    int t = bloc*l+i;
                    FakeTLwe* st = fake(&s->all_sample[t]);

                    for (int j=0; j<N; j++) tempPoly->coefsT[j] = mess->coefs[j]*h[i];
                    ASSERT_TRUE(torusPolynomialNormInftyDist(st->message,tempPoly)==0);
                    ASSERT_TRUE(st->current_variance==alpha);
                }
            }

            delete_TorusPolynomial(tempPoly);
            delete_IntPolynomial(mess);
            delete_TGswSample(s);
        }
    }



    //EXPORT void tGswSymEncryptInt(TGswSample* result, const int message, double alpha, const TGswKey* key);
    TEST_F(TGswTest, tGswSymEncryptInt) {
        for (const TGswKey* key: all_keys) {
            int l = key->params->l;
            int k = key->params->tlwe_params->k;
            int N = key->params->tlwe_params->N;
            Torus32* h = key->params->h;
            TGswSample* s = new_TGswSample(key->params);
            int mess = rand()%2; // ILA: ça suffit?
            double alpha = 4.2; // valeur pseudo aleatoire fixé
            
            tGswSymEncryptInt(s, mess, alpha, key);
            for (int bloc=0; bloc<=k; bloc++) {
                for (int i=0; i<l; ++i)
                {
                    int t = bloc*l+i;
                    FakeTLwe* st = fake(&s->all_sample[t]);
                    ASSERT_TRUE(st->message->coefsT[0]==mess*h[i]);
                    for (int j=1; j<N; j++) ASSERT_TRUE(st->message->coefsT[j]==mess*h[i]);
                    ASSERT_TRUE(st->current_variance==alpha);
                }
            }

            delete_TGswSample(s);
        }
    }



    // ILA: we do not decrypt TGSW, no test for now
    //EXPORT void tGswSymDecrypt(IntPolynomial* result, const TGswSample* sample, const TGswKey* key, const int Msize);
    //EXPORT int tGswSymDecryptInt(const TGswSample* sample, const TGswKey* key); 
    
    //// support Functions for TGsw
    //// Result = 0
    //EXPORT void tGswClear(TGswSample* result, const TGswParams* params);
    //ILA: on devrait verifier aussi la variance?
    TEST_F(TGswTest, tGswClear) {
    	for (const TGswParams* param: all_params) {
    	    TGswSample* s = new_TGswSample(param);
    	    int kpl = param->kpl;
    	    TorusPolynomial* zeroPol = new_TorusPolynomial(param->tlwe_params->N);

    	    torusPolynomialClear(zeroPol);
    	    tGswClear(s,param);
    	    for (int i=0; i<kpl; i++) {
        		FakeTLwe* si = fake(&s->all_sample[i]);
        		ASSERT_TRUE(torusPolynomialNormInftyDist(si->message,zeroPol)==0);
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
    //ILA: on devrait verifier aussi la variance?
    TEST_F(TGswDirectTest, tGswAddH) {
        for (const TGswKey* key: all_keys) {
            TGswSample* s = new_TGswSample(key->params);
            TGswSample* stemp = new_TGswSample(key->params);
            int kpl = key->params->kpl;
            int l = key->params->l;
            int k = key->params->tlwe_params->k;
            int N = key->params->tlwe_params->N;
            Torus32* h = key->params->h;
            IntPolynomial* mess = new_random_IntPolynomial(N);
            double alpha = 4.2; // valeur pseudo aleatoire fixé
            tGswSymEncrypt(s, mess, alpha, key);

            // stemp is equal to s
            for (int i = 0; i < kpl; ++i){
                tLweCopy(&stemp->all_sample[i], &s->all_sample[i], key->params->tlwe_params);
            }

            tGswAddH(s,key->params);
            for (int bloc=0; bloc<=k; bloc++) {
                for (int i=0; i<l; ++i)
                {
                    int t = bloc*l+i;
                    stemp->all_sample[t].a[bloc].coefsT[0] += h[i];

                    //ILA: the function torusPolynomialNormInftyDist(poly1,poly2) doesn't work
                    double dist = 0;
                    for (int j=0; j<N; ++j){
                        double r = abs(t32tod(s->all_sample[t].a[bloc].coefsT[j] - stemp->all_sample[t].a[bloc].coefsT[j]));
                        if (r>dist) {dist = r;}
                    }
                    ASSERT_TRUE(dist==0);
                }                              
            }

            delete_IntPolynomial(mess);
            delete_TGswSample(stemp);
            delete_TGswSample(s);
        }
    }


    //// Result += mu*H
    //EXPORT void tGswAddMuH(TGswSample* result, const IntPolynomial* message, const TGswParams* params);
    //ILA: on devrait verifier aussi la variance?
    TEST_F(TGswDirectTest, tGswAddMuH) {
        for (const TGswKey* key: all_keys) {
            TGswSample* s = new_TGswSample(key->params);
            TGswSample* stemp = new_TGswSample(key->params);
            int kpl = key->params->kpl;
            int l = key->params->l;
            int k = key->params->tlwe_params->k;
            int N = key->params->tlwe_params->N;
            Torus32* h = key->params->h;
            IntPolynomial* mess = new_random_IntPolynomial(N);
            IntPolynomial* mu = new_random_IntPolynomial(N);
            double alpha = 4.2; // valeur pseudo aleatoire fixé
            tGswSymEncrypt(s, mess, alpha, key);

            // stemp is equal to s
            for (int i = 0; i < kpl; ++i){
                tLweCopy(&stemp->all_sample[i], &s->all_sample[i], key->params->tlwe_params);
            }

            tGswAddMuH(s, mu, key->params);
            for (int bloc=0; bloc<=k; bloc++) {
                for (int i=0; i<l; ++i)
                {
                    int t = bloc*l+i;
                    for (int j=0; j<N; j++) stemp->all_sample[t].a[bloc].coefsT[j] = mu->coefs[j]*h[i];
                    
                    //ILA: the function torusPolynomialNormInftyDist(poly1,poly2) doesn't work
                    double dist = 0;
                    for (int j=0; j<N; ++j){
                        double r = abs(t32tod(s->all_sample[t].a[bloc].coefsT[j] - stemp->all_sample[t].a[bloc].coefsT[j]));
                        if (r>dist) {dist = r;}
                    }
                    ASSERT_TRUE(dist==0);
                }                              
            }

            delete_IntPolynomial(mu);
            delete_IntPolynomial(mess);
            delete_TGswSample(stemp);
            delete_TGswSample(s);
        }
    }




    //// Result += mu*H, mu integer
    //EXPORT void tGswAddMuIntH(TGswSample* result, const int message, const TGswParams* params);
    TEST_F(TGswDirectTest, tGswAddMuIntH) {
        for (const TGswKey* key: all_keys) {
            TGswSample* s = new_TGswSample(key->params);
            TGswSample* stemp = new_TGswSample(key->params);
            int kpl = key->params->kpl;
            int l = key->params->l;
            int k = key->params->tlwe_params->k;
            int N = key->params->tlwe_params->N;
            Torus32* h = key->params->h;
            int mess = rand()%2; // ILA: ça suffit?
            int mu = rand()%2; // ILA: ça suffit?
            double alpha = 4.2; // valeur pseudo aleatoire fixé
            tGswSymEncryptInt(s, mess, alpha, key);

            // stemp is equal to s
            for (int i = 0; i < kpl; ++i){
                tLweCopy(&stemp->all_sample[i], &s->all_sample[i], key->params->tlwe_params);
            }

            tGswAddMuIntH(s, mu, key->params);
            for (int bloc=0; bloc<=k; bloc++) {
                for (int i=0; i<l; ++i)
                {
                    int t = bloc*l+i;
                    stemp->all_sample[t].a[bloc].coefsT[0] = mu*h[i];
                    
                    //ILA: the function torusPolynomialNormInftyDist(poly1,poly2) doesn't work
                    double dist = 0;
                    for (int j=0; j<N; ++j){
                        double r = abs(t32tod(s->all_sample[t].a[bloc].coefsT[j] - stemp->all_sample[t].a[bloc].coefsT[j]));
                        if (r>dist) {dist = r;}
                    }
                    ASSERT_TRUE(dist==0);
                }                              
            }

            delete_TGswSample(stemp);
            delete_TGswSample(s);
        }
    }





    //// Result = tGsw(0)
    //EXPORT void tGswEncryptZero(TGswSample* result, double alpha, const TGswKey* key);
    TEST_F(TGswTest, tGswEncryptZero) {
        for (const TGswKey* key: all_keys) {
            int kpl = key->params->kpl;
            TGswSample* s = new_TGswSample(key->params);
            double alpha = 4.2; // valeur pseudo aleatoire fixé

            // Zero polynomial
            TorusPolynomial* zeroPol = new_TorusPolynomial(key->params->tlwe_params->N);
            torusPolynomialClear(zeroPol);

            tGswEncryptZero(s, alpha, key);
            for (int i=0; i<kpl; ++i) {
                FakeTLwe* si = fake(&s->all_sample[i]);
                ASSERT_TRUE(torusPolynomialNormInftyDist(si->message,zeroPol)==0);
                ASSERT_TRUE(si->current_variance==alpha);
            }

            delete_TorusPolynomial(zeroPol);
            delete_TGswSample(s);
        }
    }





    //EXPORT void tGswTorus32PolynomialDecompH(IntPolynomial* result, const TorusPolynomial* sample, const TGswParams* params);
    // sample: torus polynomial with N coefficients
    // result: int polynomial with Nl coefficients
    // ILA: reverifier!
    TEST_F(TGswDirectTest, tGswTorus32PolynomialDecompH) {
        for (const TGswParams* param: all_params) {
            int N = param->tlwe_params->N;
            int l = param->l;
            Torus32* h = param->h;
            
            IntPolynomial* result = new_IntPolynomial(N*l);
            TorusPolynomial* sample = new_TorusPolynomial(N);
            torusPolynomialUniform(sample);
            Torus32 test;

            tGswTorus32PolynomialDecompH(result, sample, param);
            for (int i=0; i<N; ++i) {
                // recomposition
                test = 0;
                for (int j=0; j<l; ++j) {
                    int t = i*l+j; 
                    test += result->coefs[t]*h[j];
                }
                ASSERT_TRUE(abs(t32tod(test - sample->coefsT[i]))==0);
            }
            
            delete_TorusPolynomial(sample);
            delete_IntPolynomial(result);
        }
	}



    //EXPORT void tGswTLweDecompH(IntPolynomial* result, const TLweSample* sample,const TGswParams* params);	
    // Test direct Result*H donne le bon resultat
    // sample: TLweSample composed by k+1 torus polynomials, each with N coefficients
    // result: int polynomial with Nl(k+1) coefficients
    // ILA: reverifier!
    TEST_F(TGswDirectTest, tGswTLweDecompH) {
        for (const TGswParams* param: all_params) {
            int N = param->tlwe_params->N;
            int k = param->tlwe_params->k;
            int l = param->l;
            Torus32* h = param->h;

            IntPolynomial* result = new_IntPolynomial(N*l*(k+1));
            TLweSample* sample = new_TLweSample(param->tlwe_params);
            // sample randomly generated 
            for (int bloc=0; bloc<=k; ++bloc) {
                torusPolynomialUniform(&sample->a[bloc]);
            }
            Torus32 test;

            tGswTLweDecompH(result, sample, param);
            for (int bloc=0; bloc<=k; ++bloc) {
                for (int i=0; i<N; ++i) {
                    // recomposition
                    test = 0;
                    for (int j=0; j<l; ++j) {
                        int t = (bloc*N*l)+i*l+j;
                        test += result->coefs[t]*h[j];
                    }
                    ASSERT_TRUE(abs(t32tod(test - sample->a[bloc].coefsT[i]))==0);
                }
            }

            delete_TLweSample(sample);
            delete_IntPolynomial(result);
        }
    }



    ////TODO: Ilaria.Theoreme3.5
    //EXPORT void tGswExternProduct(TLweSample* result, const TGswSample* a, const TLweSample* b, const TGswParams* params);
    TEST_F(TGswTest, tGswExternProduct) {
        for (const TGswKey* key: all_keys) {
            int N = key->params->tlwe_params->N;
            TGswSample* a = new_TGswSample(key->params);
            TLweSample* b = new_TLweSample(key->params->tlwe_params);
            TLweSample* res = new_TLweSample(key->params->tlwe_params);
            double alpha = 4.2; // valeur pseudo aleatoire fixé
            
            IntPolynomial* mua = new_random_IntPolynomial(N);
            TorusPolynomial* mub = new_TorusPolynomial(N);
            torusPolynomialUniform(mub);
            TorusPolynomial* mures = new_TorusPolynomial(N);
            
            tGswSymEncrypt(a, mua, alpha, key);
            tLweSymEncrypt(b, mub, alpha, &key->tlwe_key);

            torusPolynomialMultNaive(mures, mua, mub);
            tGswExternProduct(res, a, b, key->params);
            
            FakeTLwe* r = fake(res);
            ASSERT_TRUE(torusPolynomialNormInftyDist(r->message,mures)==0);
            //ASSERT_TRUE(si->current_variance==alpha);

            delete_TorusPolynomial(mures);
            delete_TorusPolynomial(mub);
            delete_IntPolynomial(mua);
            delete_TLweSample(res);
            delete_TLweSample(b);
            delete_TGswSample(a);
        }
    }





    //// result=result+ (X^ai-1)*bi (ligne 5 de l'algo)
    //EXPORT void tGswMulByXaiMinusOne(TGswSample* result, int ai, const TGswSample* bk, const TGswParams* params);
    TEST_F(TGswTest, tGswMulByXaiMinusOne) {
        for (const TGswKey* key: all_keys) {
            int l = key->params->l;
            int k = key->params->tlwe_params->k;
            int N = key->params->tlwe_params->N;
            Torus32* h = key->params->h;
            TGswSample* res = new_TGswSample(key->params);
            int ai = rand()%2; // ILA?
            TGswSample* bk = new_TGswSample(key->params);
            double alpha = 4.2; // valeur pseudo aleatoire fixé

            IntPolynomial* mess = new_random_IntPolynomial(N);
            IntPolynomial* mbk = new_random_IntPolynomial(N);
            IntPolynomial* mtemp = new_IntPolynomial(N);
            TorusPolynomial* poly = new_TorusPolynomial(N);
            tGswSymEncrypt(res, mess, alpha, key);
            tGswSymEncrypt(bk, mbk, alpha, key);
            
            // mess = mess + (X^ai-1)*mbk
            intPolynomialMulByXaiMinusOne(mtemp, ai, mbk);
            intPolynomialAddTo(mess, mtemp);

            tGswMulByXaiMinusOne(res, ai, bk, key->params);
            for (int bloc=0; bloc<=k; bloc++) {
                for (int i=0; i<l; ++i)
                {
                    int t = bloc*l+i;
                    FakeTLwe* rt = fake(&res->all_sample[t]);

                    for (int j=0; j<N; j++) poly->coefsT[j] = mess->coefs[j]*h[i];
                    ASSERT_TRUE(torusPolynomialNormInftyDist(poly,rt->message)==0);
                }                              
            }

            delete_TorusPolynomial(poly);
            delete_IntPolynomial(mtemp);
            delete_IntPolynomial(mbk);
            delete_IntPolynomial(mess);
            delete_TGswSample(bk);
            delete_TGswSample(res);
        }
    }





    ////ligne 5 algo,mult externe
    //EXPORT void tGswExternMulToTLwe(TLweSample* accum, const TGswSample* sample, const TGswParams* params);
    //accum *= sample
    TEST_F(TGswTest, tGswExternMulToTLwe) {
        for (const TGswKey* key: all_keys) {
            int N = key->params->tlwe_params->N;
            TGswSample* sample = new_TGswSample(key->params);
            TLweSample* accum = new_TLweSample(key->params->tlwe_params);
            double alpha = 4.2; // valeur pseudo aleatoire fixé
            
            IntPolynomial* mu = new_random_IntPolynomial(N);
            TorusPolynomial* mess = new_TorusPolynomial(N);
            torusPolynomialUniform(mess);
            
            tGswSymEncrypt(sample, mu, alpha, key);
            tLweSymEncrypt(accum, mess, alpha, &key->tlwe_key);

            // mess *= mu
            torusPolynomialMultNaive(mess, mu, mess);
            tGswExternMulToTLwe(accum,sample,key->params);

            FakeTLwe* acc = fake(accum);
            ASSERT_TRUE(torusPolynomialNormInftyDist(acc->message,mess)==0);
            //ASSERT_TRUE(si->current_variance==alpha);

            delete_TorusPolynomial(mess);
            delete_IntPolynomial(mu);
            delete_TLweSample(accum);
            delete_TGswSample(sample);
        }
    }




    /** result = (0,mu) */
    //EXPORT void tGswNoiselessTrivial(TGswSample* result, const IntPolynomial* mu, const TGswParams* params);
    TEST_F(TGswTest, tGswNoiselessTrivial) {
        for (const TGswParams* param: all_params) {
            int l = param->l;
            int k = param->tlwe_params->k;
            int N = param->tlwe_params->N;
            Torus32* h = param->h;
            TGswSample* res = new_TGswSample(param);
            IntPolynomial* mu = new_random_IntPolynomial(N);
            TorusPolynomial* poly = new_TorusPolynomial(N);
            
            tGswNoiselessTrivial(res, mu, param);
            for (int bloc=0; bloc<=k; bloc++) {
                for (int i=0; i<l; ++i)
                {
                    int t = bloc*l+i;
                    FakeTLwe* rt = fake(&res->all_sample[t]);

                    for (int j=0; j<N; j++) poly->coefsT[j] = mu->coefs[j]*h[i];
                    ASSERT_TRUE(torusPolynomialNormInftyDist(poly,rt->message)==0);
                    ASSERT_TRUE(rt->current_variance==0);
                }                              
            }

            delete_TorusPolynomial(poly);
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
    //EXPORT void tGswAddMulTo(TLweSample* result, int p, const TLweSample* sample, const TLweParams* params);
    /** result = result - p.sample */
    //EXPORT void tGswSubMulTo(TLweSample* result, int p, const TLweSample* sample, const TLweParams* params);

}//namespace

