#ifndef TFHE_TEST_ENVIRONMENT
#include <iostream>
#include "lwe-functions.h"
#include "lwekeyswitch.h"

using namespace std;
#else
#undef EXPORT
#define EXPORT
#endif

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
void lweCreateKeySwitchKey_fromArray(LweSample*** result, 
	const LweKey* out_key, const double out_alpha, 
	const int* in_key, const int n, const int t, const int basebit){
    const int base=1<<basebit;       // base=2 in [CGGI16]

    for(int i=0;i<n;i++) {
    	for(int j=0;j<t;j++){
    	    for(int k=0;k<base;k++){
		Torus32 x=(in_key[i]*k)*(1<<(32-(j+1)*basebit));
		lweSymEncrypt(&result[i][j][k],x,out_alpha,out_key);
		//printf("i,j,k,ki,x,phase=%d,%d,%d,%d,%d,%d\n",i,j,k,in_key->key[i],x,lwePhase(&result->ks[i][j][k],out_key));
    	    }
    	}
    }
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
void lweKeySwitchTranslate_fromArray(LweSample* result, 
	const LweSample*** ks, const LweParams* params, 
	const Torus32* ai, 
	const int n, const int t, const int basebit){
    const int base=1<<basebit;       // base=2 in [CGGI16]
    const int32_t prec_offset=1<<(32-(1+basebit*t)); //precision
    const int mask=base-1;

    for (int i=0;i<n;i++){
	const uint32_t aibar=ai[i]+prec_offset;
	for (int j=0;j<t;j++){
	    const uint32_t aij=(aibar>>(32-(j+1)*basebit)) & mask;
	    lweSubTo(result,&ks[i][j][aij],params);
	}
    }
}



EXPORT void lweCreateKeySwitchKey(LweKeySwitchKey* result, const LweKey* in_key, const LweKey* out_key){
    const int n=result->n;
    const int basebit=result->basebit;
    const int t=result->t;

    //TODO check the parameters


    lweCreateKeySwitchKey_fromArray(result->ks,
	    out_key, out_key->params->alpha_min,
	    in_key->key, n, t, basebit);
}

//sample=(a',b')
EXPORT void lweKeySwitch(LweSample* result, const LweKeySwitchKey* ks, const LweSample* sample){
    const LweParams* params=ks->out_params;
    const int n=ks->n;
    const int basebit=ks->basebit;
    const int t=ks->t;

    lweNoiselessTrivial(result,sample->b,params);
    lweKeySwitchTranslate_fromArray(result,
	    (const LweSample***) ks->ks, params,
	    sample->a, n, t, basebit);
}

