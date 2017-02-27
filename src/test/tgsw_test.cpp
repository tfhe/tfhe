/* 
 * tgsw_test.cpp
 * Tests the functions defined in /src/include/tgsw_functions.h
 * A set of parameters ad keys is defined in the beginning (N=512,1024,2048 and k=1,2)
 */


#include <gtest/gtest.h>
#include <tlwe.h>
#include <tlwe_functions.h>
#include <numeric_functions.h>
#include <polynomials_arithmetic.h>
#include "fakes/tlwe.h"
#define TFHE_TEST_ENVIRONMENT 1

using namespace std;
using namespace ::testing;

namespace {

    // we use the function rand because in the "const static" context the uniformly random generator doesn't work!
    const TLweKey* new_random_key(const TLweParams* params) {
	TLweKey* key = new_TLweKey(params);
	const int N = params->N;
	const int k = params->k;

	for (int i = 0; i < k; ++i)
	    for (int j = 0; j < N; ++j)
		key->key[i].coefs[j] = rand()%2;	
	return key;
    }	

    /*
     * Parameters and keys (for N=512,1024,2048 and k=1,2)
     * ILA: Creer un set des parametres avec tous les params à l'interieur et utiliser le set dans les tests (?)
     */
    const TLweParams* params512_1 = new_TLweParams(512,1,0.,1.);
    const TLweParams* params512_2 = new_TLweParams(512,2,0.,1.);
    const TLweParams* params1024_1 = new_TLweParams(1024,1,0.,1.);
    const TLweParams* params1024_2 = new_TLweParams(1024,2,0.,1.);
    const TLweParams* params2048_1 = new_TLweParams(2048,1,0.,1.);
    const TLweParams* params2048_2 = new_TLweParams(2048,2,0.,1.);
    vector<const TLweParams*> all_params = {params512_1, params512_2, params1024_1, params1024_2, params2048_1, params2048_2};

    const TLweKey* key512_1 = new_random_key(params512_1);
    const TLweKey* key512_2 = new_random_key(params512_2);
    const TLweKey* key1024_1 = new_random_key(params1024_1);
    const TLweKey* key1024_2 = new_random_key(params1024_2);
    const TLweKey* key2048_1 = new_random_key(params2048_1);
    const TLweKey* key2048_2 = new_random_key(params2048_2);
    vector<const TLweKey*> all_keys = {key512_1, key512_2, key1024_1, key1024_2, key2048_1, key2048_2};

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
	    
    };


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
    }

    EXPORT void tGswSymEncrypt(TGswSample* result, const IntPolynomial* message, double alpha, const TGswKey* key);
    EXPORT void tGswSymEncryptInt(TGswSample* result, const int message, double alpha, const TGswKey* key);
    EXPORT void tGswSymDecrypt(IntPolynomial* result, const TGswSample* sample, const TGswKey* key, const int Msize);
    EXPORT int tGswSymDecryptInt(const TGswSample* sample, const TGswKey* key); 
    //do we really decrypt Gsw samples?

    // support Functions for TGsw
    // Result = 0
    EXPORT void tGswClear(TGswSample* result, const TGswParams* params);
    // Result += H
    EXPORT void tGswAddH(TGswSample* result, const TGswParams* params);
    // Result += mu*H
    EXPORT void tGswAddMuH(TGswSample* result, const IntPolynomial* message, const TGswParams* params);
    // Result += mu*H, mu integer
    EXPORT void tGswAddMuIntH(TGswSample* result, const int message, const TGswParams* params);
    // Result = tGsw(0)
    EXPORT void tGswEncryptZero(TGswSample* result, double alpha, const TGswKey* key);

    //fonction de decomposition
    EXPORT void tGswTLweDecompH(IntPolynomial* result, const TLweSample* sample, const TGswParams* params);

    EXPORT void tGswTorus32PolynomialDecompH(IntPolynomial* result, const TorusPolynomial* sample, const TGswParams* params);
    EXPORT void tGswTLweDecompH(IntPolynomial* result, const TLweSample* sample,const TGswParams* params);	

    //TODO: Ilaria.Theoreme3.5
    EXPORT void tGswExternProduct(TLweSample* result, const TGswSample* a, const TLweSample* b, const TGswParams* params);

    // result=result+ (X^ai-1)*bi (ligne 5 de l'algo)
    EXPORT void tGswMulByXaiMinusOne(TGswSample* result, int ai, const TGswSample* bk, const TGswParams* params);

    //ligne 5 algo,mult externe
    EXPORT void tGswExternMulToTLwe(TLweSample* accum, const TGswSample* sample,const TGswParams* params);

    /** result = (0,mu) */
    EXPORT void tGswNoiselessTrivial(TGswSample* result, const TorusPolynomial* mu, const TGswParams* params);

    /** result = result + sample */
    EXPORT void tGswAddTo(TGswSample* result, const TGswSample* sample, const TGswParams* params);

    /** result = result - sample */
    //EXPORT void tGswSubTo(TLweSample* result, const TLweSample* sample, const TLweParams* params);
    /** result = result + p.sample */
    //EXPORT void tGswAddMulTo(TLweSample* result, int p, const TLweSample* sample, const TLweParams* params);
    /** result = result - p.sample */
    //EXPORT void tGswSubMulTo(TLweSample* result, int p, const TLweSample* sample, const TLweParams* params);




}//namespace

