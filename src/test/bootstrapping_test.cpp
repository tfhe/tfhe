#include <gtest/gtest.h>
//#include <gmock/gmock.h>
#include "tfhe.h"
#include "fakes/tgsw.h"
#define TFHE_TEST_ENVIRONMENT 1
using namespace std;

using namespace ::testing;

namespace {


    const LweParams* params250_0 = new_LweParams(250,0.,1.);
    const LweParams* params500_0 = new_LweParams(500,0.,1.);
    const LweParams* params500_1em5 = new_LweParams(500,1e-5,1.);
    const LweKey* key250 = new_LweKey(params250_0);
    const LweKey* key500 = new_LweKey(params500_0);


    class LweBootstrappingTest: public ::testing::Test {
	public:

	    //MOCK_METHOD4(lweSymEncrypt, void(LweSample*,const Torus32,const double, const LweKey*));

#define INCLUDE_TFHE_BOOTSTRAPROTATE
#include "../libtfhe/lwe-bootstrapping-functions.cpp"
#undef INCLUDE_TFHE_BOOTSTRAPROTATE
	    /**
 * multiply the accumulator by X^sum(bara_i.s_i)
 * @param accum the TLWE sample to multiply
 * @param bk An array of n TGSW samples where bk_i encodes s_i
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
EXPORT void tfhe_bootstrapRotate(TLweSample* accum, 
	const TGswSample* bk, 
	const int* bara,
	const int n,
	const TGswParams* bk_params) {
    TGswSample* temp = new_TGswSample(bk_params);
    for (int i=0; i<n; i++) {
	const int barai=bara[i];
	if (barai==0) continue; //indeed, this is an easy case!
	tGswMulByXaiMinusOne(temp, barai, bk+i, bk_params);
	tGswAddH(temp, bk_params);
	tGswExternMulToTLwe(accum, temp, bk_params);
    }
    delete_TGswSample(temp);
}

    };

    /**
     * fills the KeySwitching key array
     * @param result The (n x t x base) array of samples. 
     *        result[i][j][k] encodes k.s[i]/base^(j+1)
     * @param out_key The LWE key to encode all the output samples 
     * @param out_alpha The standard deviation of all output samples
     * @param in_key The (binary) input key
     * @param n The size of the input key
     * @param t The precision of the keyswitch (technically, 1/2.base^t)
     * @param basebit Log_2 of base
     */
    //void lweCreateKeySwitchKey_fromArray(LweSample*** result, 
    TEST_F(LweKeySwitchTest, lweCreateKeySwitchKey_fromArray) {
	//EXPECT_CALL(*this, lweSymEncrypt(_,_,_,_)).WillRepeatedly(Invoke(fake_lweSymEncrypt));
	LweKeySwitchKey* test = new_LweKeySwitchKey(300,14,2,params500_1em5);
	//int n = test->out_params->n;
	double alpha = 1e-5;
	int N = test->n;
	int t = test->t;
	int basebit = test->basebit;
	int base = test->base;
	int* in_key = new int[N];
	for (int i=0; i<N; i++) in_key[i]=(uniformTorus32_distrib(generator)%2==0?1:0);
	lweCreateKeySwitchKey_fromArray(test->ks,key500,alpha,in_key,N,t,basebit);
	for (int i=0; i<N; i++) {
	    for (int j=0; j<t; j++) {
		for (int k=0; k<base; k++) {
		    LweSample* ks_ijk = &test->ks[i][j][k];
		    ASSERT_EQ(alpha*alpha,ks_ijk->current_variance);
		    ASSERT_EQ(k*in_key[i]*1<<(32-(j+1)*basebit),ks_ijk->b);
		}
	    }
	}
	delete[] in_key;
	delete_LweKeySwitchKey(test);
    }

    /**
     * translates the message of the result sample by -sum(a[i].s[i]) where s is the secret
     * embedded in ks.
     * @param result the LWE sample to translate by -sum(ai.si). 
     * @param ks The (n x t x base) key switching key 
     *        ks[i][j][k] encodes k.s[i]/base^(j+1)
     * @param params The common LWE parameters of ks and result
     * @param ai The input torus array
     * @param n The size of the input key
     * @param t The precision of the keyswitch (technically, 1/2.base^t)
     * @param basebit Log_2 of base
     */
    //void lweKeySwitchTranslate_fromArray(LweSample* result, 
    //	    const LweSample*** ks, const LweParams* params, 
    //	    const Torus32* ai, 
    //	    const int n, const int t, const int basebit)
    TEST_F(LweKeySwitchTest, lweKeySwitchTranslate_fromArray) {
	//EXPECT_CALL(*this, lweSymEncrypt(_,_,_,_)).WillRepeatedly(Invoke(fake_lweSymEncrypt));
	LweKeySwitchKey* test = new_LweKeySwitchKey(300,14,2,params500_1em5);
	//int n = test->out_params->n;
	double alpha = 1e-5;
	int N = test->n;
	int t = test->t;
	int basebit = test->basebit;
	int base = test->base;
	const int32_t prec_offset=1<<(32-(1+basebit*t)); //precision
	const uint32_t prec_mask=-(1<<(32-(basebit*t))); //precision
	//printf("prec_offset: %08x\n",prec_offset);
	//printf("prec_mask: %08x\n",prec_mask);
	int* in_key = new int[N];
	Torus32 b = uniformTorus32_distrib(generator);
	Torus32* ai = new int[N];
	uint32_t* aibar = new uint32_t[N];
	for (int i=0; i<N; i++) {
	    in_key[i]=(uniformTorus32_distrib(generator)%2==0?1:0);
	    ai[i]=uniformTorus32_distrib(generator);
	    aibar[i]=(ai[i]+prec_offset) & prec_mask;
	}
	LweSample* res = new_LweSample(params500_1em5);
	lweCreateKeySwitchKey_fromArray(test->ks,key500,alpha,in_key,N,t,basebit);
	//we first try one by one
	lweNoiselessTrivial(res,b,params500_1em5);
	Torus32 barphi = b;
	ASSERT_EQ(barphi,res->b);
	for (int i=0; i<N; i++) {
	    lweKeySwitchTranslate_fromArray(res, (const LweSample***) test->ks+i,params500_1em5,ai+i,1,t,basebit);
	    barphi -= aibar[i]*in_key[i];
	    //verify the decomposition function
	    //printf( "ai:  %08x\n"
	    //	    "aib: %08x si: %d\n",ai[i],aibar[i],in_key[i]);
	    uint32_t dec = 0;
	    for (int j=0;j<t;j++){
		const uint32_t aij=(aibar[i]>>(32-(j+1)*basebit)) & (base-1);
		const uint32_t rec=aij<<(32-(j+1)*basebit);
		//printf("rec: %08x at j=%d\n",rec,j);
		dec += rec;
	    }
	    ASSERT_EQ(dec,aibar[i]);
	    ASSERT_NEAR(alpha*alpha*(i+1)*t,res->current_variance,1e-10);
	    ASSERT_EQ(barphi,res->b);
	}
	//now, test it all at once
	lweNoiselessTrivial(res,b,params500_1em5);
	lweKeySwitchTranslate_fromArray(res, (const LweSample***) test->ks,params500_1em5,ai,N,t,basebit);
	ASSERT_NEAR(alpha*alpha*N*t,res->current_variance,1e-10);
	ASSERT_EQ(barphi,res->b);
	delete[] in_key;
	delete[] ai;
	delete[] aibar;
	delete_LweSample(res);
	delete_LweKeySwitchKey(test);
    }
}
