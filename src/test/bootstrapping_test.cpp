#include <gtest/gtest.h>
//#include <gmock/gmock.h>
#include "tfhe.h"
#include "fakes/tgsw.h"
#include "fakes/lwe-bootstrapping.h"
#define TFHE_TEST_ENVIRONMENT 1
using namespace std;

using namespace ::testing;

namespace {

    const int N = 1024;
    const int k = 1;
    const int n = 500;
    const int l_bk = 3; //ell
    const int Bgbit_bk = 10;
    //const int ks_basebit = 4;
    const double alpha_in = 5e-4;
    const double alpha_bk = 9e-9;
    //const int alpha_ks = 1e-6;

    const LweParams* in_params = new_LweParams(n, alpha_in, 1./16.);
    const TLweParams* accum_params = new_TLweParams(N, k, alpha_bk, 1./16.);
    const TGswParams* bk_params = new_TGswParams(l_bk, Bgbit_bk, accum_params);
    const LweParams* extract_params = &accum_params->extracted_lweparams;

/*
    LweKey* key = new_LweKey(params_in);
    lweKeyGen(key);

    TGswKey* key_bk = new_TGswKey(params_bk);
    tGswKeyGen(key_bk);
*/


    class LweBootstrapRotateTest: public ::testing::Test {
	public:

	    USE_FAKE_new_TGswSample;
	    USE_FAKE_delete_TGswSample;
	    USE_FAKE_tGswMulByXaiMinusOne;
            USE_FAKE_tGswAddH;
	    USE_FAKE_tGswExternMulToTLwe;

#define INCLUDE_TFHE_BOOTSTRAPROTATE
#include "../libtfhe/lwe-bootstrapping-functions.cpp"
#undef INCLUDE_TFHE_BOOTSTRAPROTATE

    };


    /**
     * multiply the accumulator by X^sum(bara_i.s_i)
     * @param accum the TLWE sample to multiply
     * @param bk An array of n TGSW samples where bk_i encodes s_i
     * @param bara An array of n coefficients between 0 and 2N-1
     * @param bk_params The parameters of bk
     */
    //EXPORT void tfhe_bootstrapRotate(TLweSample* accum, 
    //	    const TGswSample* bk, 
    //	    const int* bara,
    //	    const int n,
    //	    const TGswParams* bk_params) {
    TEST_F(LweBootstrapRotateTest,tfheBootstrapRotateTest) {
	LweKey* key = new_LweKey(in_params);
	lweKeyGen(key);
	TGswKey* key_bk = new_TGswKey(bk_params);
	tGswKeyGen(key_bk);
	TGswSample* bk = fake_new_TGswSample_array(n, bk_params);
	//create bara
	int* bara = new int[n];
	for (int i=0; i<n; i++) bara[i]=rand()%(2*N);
	//create bk
	for (int i=0; i<n; i++) fake_tGswSymEncryptInt(bk+i,key->key[i],alpha_bk,key_bk);
	//create accum
	TorusPolynomial* initAccumMessage = new_TorusPolynomial(N);
	torusPolynomialUniform(initAccumMessage);
	const double initAlphaAccum=0.2;
	int expectedOffset=0;
	TorusPolynomial* expectedAccumMessage = new_TorusPolynomial(N);
	torusPolynomialCopy(expectedAccumMessage,initAccumMessage);
	//double expectedAccumVariance=initAlphaAccum*initAlphaAccum;
	TLweSample* accum = new_TLweSample(accum_params);
	tLweNoiselessTrivial(accum, initAccumMessage, accum_params);
	accum->current_variance=initAlphaAccum*initAlphaAccum;
	//call bootstraprotate: one iteration at a time
	for (int i=0; i<n; i++) {
	    tfhe_bootstrapRotate(accum,bk+i,bara+i,1,bk_params);
	    if (key->key[i]==1 && bara[i]!=0) {
		expectedOffset=(expectedOffset+bara[i])%(2*N);
		torusPolynomialMulByXai(expectedAccumMessage,expectedOffset,initAccumMessage);
	    }
	    //printf("i=%d,si=%d,barai=%d,offset=%d\n",i,key->key[i],bara[i],expectedOffset);
	    for (int j=0; j<N; j++) ASSERT_EQ(expectedAccumMessage->coefsT[j],accum->b->coefsT[j]);
	}
	//Now, bootstraprotate: all iterations at once (same offset)
	tLweNoiselessTrivial(accum, initAccumMessage, accum_params);
	accum->current_variance=initAlphaAccum*initAlphaAccum;
	tfhe_bootstrapRotate(accum,bk,bara,n,bk_params);
	for (int j=0; j<N; j++) ASSERT_EQ(expectedAccumMessage->coefsT[j],accum->b->coefsT[j]);
	//cleanup everything
	delete_TLweSample(accum);
	delete_TorusPolynomial(expectedAccumMessage);
	delete_TorusPolynomial(initAccumMessage);
	delete[] bara;
	fake_delete_TGswSample_array(n,bk);
	delete_TGswKey(key_bk);
	delete_LweKey(key);
    }

    class LweBootstrapRotateExtractTest: public ::testing::Test {
	public:

	    USE_FAKE_tfhe_bootstrapRotate;

#define INCLUDE_TFHE_BOOTSTRAPROTATEEXTRACT
#include "../libtfhe/lwe-bootstrapping-functions.cpp"
#undef INCLUDE_TFHE_BOOTSTRAPROTATEEXTRACT

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
    TEST_F(LweBootstrapRotateExtractTest,tfheBootstrapRotateExtractTest) {
	const int NB_TRIALS=30;

	LweKey* key = new_LweKey(in_params);
	lweKeyGen(key);
	TGswKey* key_bk = new_TGswKey(bk_params);
	tGswKeyGen(key_bk);
	TGswSample* bk = fake_new_TGswSample_array(n, bk_params);
	//create bara and b
	int* bara = new int[n];
	//create bk
	for (int i=0; i<n; i++) fake_tGswSymEncryptInt(bk+i,key->key[i],alpha_bk,key_bk);
	//create v
	TorusPolynomial* v = new_TorusPolynomial(N);
	//create result
	LweSample* result = new_LweSample(&accum_params->extracted_lweparams);

	for (int trial=0; trial<NB_TRIALS; trial++) {
	    for (int i=0; i<n; i++) bara[i]=rand()%(2*N);
	    int barb = rand()%(2*N);
	    torusPolynomialUniform(v);
	    //const double initAlphaAccum=0.2;

	    //run the function
	    tfhe_bootstrapRotateExtract(result,v,bk,barb,bara,n,bk_params);

	    //verify
	    int offset = barb;
	    for (int i=0; i<n; i++) offset = (offset + 2*N - key->key[i]*bara[i])%(2*N);
	    ASSERT_EQ(result->b,(offset<N)?(v->coefsT[offset]):(-v->coefsT[offset-N]));
	    //TODO variance
	}
	//clean up
	delete_LweSample(result);
	delete_TorusPolynomial(v);
	delete[] bara;
	fake_delete_TGswSample_array(n,bk);
	delete_TGswKey(key_bk);
	delete_LweKey(key);
    }


    class LweBootstrapTest: public ::testing::Test {
	public:

	    USE_FAKE_tfhe_bootstrapRotateExtract;

#define INCLUDE_TFHE_BOOTSTRAP
#include "../libtfhe/lwe-bootstrapping-functions.cpp"
#undef INCLUDE_TFHE_BOOTSTRAP

    };
    /**
     * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
     * @param result The resulting LweSample
     * @param bk The bootstrapping + keyswitch key
     * @param mu The output message (if phase(x)>0)
     * @param x The input sample
     */
    //EXPORT void tfhe_bootstrap(LweSample* result, 
    //	const LweBootstrappingKey* bk, 
    //	Torus32 mu, const LweSample* x)
    TEST_F(LweBootstrapTest,tfheBootstrapTest) {
	const Torus32 TEST_MU=123456789;
	const int NB_TRIALS=30;
	const int Nx2= 2*N;
	const int n = in_params->n;
	//create a fake bootstrapping key
	LweKey* key = new_LweKey(in_params);
	lweKeyGen(key);
	TGswKey* key_bk = new_TGswKey(bk_params);
	tGswKeyGen(key_bk);
	LweBootstrappingKey* bk = fake_new_LweBootstrappingKey(in_params, bk_params);
	fake_tfhe_createLweBootstrappingKey(bk, key, key_bk);

	//alloc the output lwe sample
	LweSample* result = new_LweSample(in_params);

	//create a random input sample
	LweSample* insample = new_LweSample(extract_params);
	for (int trial=0; trial<NB_TRIALS; trial++) {
	    lweSymEncrypt(insample,uniformTorus32_distrib(generator),0.001,key);

	    //compute the approx. phase mod 2N
	    int barb=modSwitchFromTorus32(insample->b,Nx2);
	    int phase=barb;
	    for (int i=0; i<n; i++) {
		int barai=modSwitchFromTorus32(insample->a[i],Nx2);
		phase-=key->key[i]*barai;
	    }
	    phase = (Nx2+(phase%Nx2))%Nx2; //positive modulo

	    //call the function
	    tfhe_bootstrap(result,bk,TEST_MU,insample);

	    //printf("trial=%d,hash=%d,phase=%d,result=%d\n",trial,hash,phase,result->b);
	    //verify the result
	    ASSERT_EQ(result->b,phase<N?TEST_MU:-TEST_MU);
	}

	//cleanup
	delete_LweSample(insample);
	delete_LweSample(result);
	fake_delete_LweBootstrappingKey(bk);
	delete_TGswKey(key_bk);
	delete_LweKey(key);

    }

    class LweCreateBootstrapKeyTest: public ::testing::Test {
	public:


	    LweKeySwitchKey* captured_result;
	    vector<int> captured_in_key_copy;
	    const LweKey* captured_out_key;
	    void lweCreateKeySwitchKey(LweKeySwitchKey* result, const LweKey* in_key, const LweKey* out_key) {
		const int N = in_key->params->n;
		captured_result=result;
		copy(in_key->key,in_key->key+N,back_inserter(captured_in_key_copy));
		captured_out_key=out_key;
		fake_lweCreateKeySwitchKey(result,in_key,out_key);
	    }

	    USE_FAKE_tGswSymEncryptInt;

#define INCLUDE_TFHE_CREATEBOOTSTRAPPINGKEY
#include "../libtfhe/lwe-bootstrapping-functions.cpp"
#undef INCLUDE_TFHE_CREATEBOOTSTRAPPINGKEY

    };

    //EXPORT void tfhe_createLweBootstrappingKey(
    //	LweBootstrappingKey* bk, 
    //	const LweKey* key_in, 
    //	const TGswKey* rgsw_key) 
    TEST_F(LweCreateBootstrapKeyTest,createBootstrappingKeyTest) {
	LweKey* key = new_LweKey(in_params);
	lweKeyGen(key);
	TGswKey* key_bk = new_TGswKey(bk_params);
	tGswKeyGen(key_bk);
	LweBootstrappingKey* bk = fake_new_LweBootstrappingKey(in_params, bk_params);

	//call the function
	tfhe_createLweBootstrappingKey(bk, key, key_bk);

	//make the checks
	//LweKeySwitchKey* ks; ///< the keyswitch key (s'->s)
	const TLweKey* accum_key = &key_bk->tlwe_key;
	LweKey* extracted_key = new_LweKey(extract_params);
	tLweExtractKey(extracted_key, accum_key);
	ASSERT_TRUE(captured_result==bk->ks);
	ASSERT_TRUE(captured_out_key==key);
	for (int i=0; i<N; i++) {
	    ASSERT_EQ(captured_in_key_copy[i],extracted_key->key[i]);
	}
	delete_LweKey(extracted_key);

	//TGswSample* bk; ///< the bootstrapping key (s->s")
	const int* kin = key->key;
	const int n = in_params->n;
	for (int i=0; i<n; i++) {
	    ASSERT_EQ(fake(&bk->bk[i])->message->coefs[0],kin[i]);
	    //TODO: check noise variance
	}

	//cleanup
	fake_delete_LweBootstrappingKey(bk);
	delete_TGswKey(key_bk);
	delete_LweKey(key);
    }

}
