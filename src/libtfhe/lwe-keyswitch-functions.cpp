#ifndef TFHE_TEST_ENVIRONMENT
#include <iostream>
#include "lwe-functions.h"
#include "lwekeyswitch.h"
#include "numeric_functions.h"
#include <random>


using namespace std;
#else
#undef EXPORT
#define EXPORT
#endif





/*
Renormalization of KS
 * compute the error of the KS that has been generated and translate the ks to recenter the gaussian in 0
*/
void renormalizeKSkey(LweKeySwitchKey* ks, const LweKey* out_key, const int32_t* in_key){
    const int32_t n = ks->n;
    const int32_t basebit = ks->basebit;
    const int32_t t = ks->t;
    const int32_t base = 1<<basebit; 

    Torus32 phase;
    Torus32 temp_err; 
    Torus32 error = 0;
    // double err_norm = 0; 

    // compute the average error
    for (int32_t i = 0; i < n; ++i) {
        for (int32_t j = 0; j < t; ++j) {
            for (int32_t h = 1; h < base; ++h) { // pas le terme en 0
                // compute the phase 
                phase = lwePhase(&ks->ks[i][j][h], out_key);
                // compute the error 
                Torus32 x = (in_key[i]*h)*(1<<(32-(j+1)*basebit));
                temp_err = phase - x;
                // sum all errors 
                error += temp_err;
            }
        }
    }
    int32_t nb = n*t*(base-1); 
    error = dtot32(t32tod(error)/nb);

    // relinearize
    for (int32_t i = 0; i < n; ++i) {
        for (int32_t j = 0; j < t; ++j) {
            for (int32_t h = 1; h < base; ++h) { // pas le terme en 0
                ks->ks[i][j][h].b -= error;                
            }
        }
    }

}













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
	const int32_t* in_key, const int32_t n, const int32_t t, const int32_t basebit){
    const int32_t base=1<<basebit;       // base=2 in [CGGI16]

    for(int32_t i=0;i<n;i++) {
    	for(int32_t j=0;j<t;j++){
    	    for(int32_t k=0;k<base;k++){
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
	const int32_t n, const int32_t t, const int32_t basebit){
    const int32_t base=1<<basebit;       // base=2 in [CGGI16]
    const int32_t prec_offset=1<<(32-(1+basebit*t)); //precision
    const int32_t mask=base-1;

    for (int32_t i=0;i<n;i++){
	const uint32_t aibar=ai[i]+prec_offset;
	for (int32_t j=0;j<t;j++){
	    const uint32_t aij=(aibar>>(32-(j+1)*basebit)) & mask;
	    if(aij != 0) {lweSubTo(result,&ks[i][j][aij],params);}
	}
    }
}



EXPORT void lweCreateKeySwitchKey_old(LweKeySwitchKey* result, const LweKey* in_key, const LweKey* out_key){
    const int32_t n=result->n;
    const int32_t basebit=result->basebit;
    const int32_t t=result->t;

    //TODO check the parameters


    lweCreateKeySwitchKey_fromArray(result->ks,
	    out_key, out_key->params->alpha_min,
	    in_key->key, n, t, basebit);

    // renormalize
    renormalizeKSkey(result, out_key, in_key->key); // ILA: reverifier 
}









/*
Create the key switching key: normalize the error in the beginning
 * chose a random vector of gaussian noises (same size as ks) 
 * recenter the noises 
 * generate the ks by creating noiseless encryprions and then add the noise
*/
EXPORT void lweCreateKeySwitchKey(LweKeySwitchKey* result, const LweKey* in_key, const LweKey* out_key){
    const int32_t n = result->n;
    const int32_t t = result->t;
    const int32_t basebit = result->basebit;
    const int32_t base = 1<<basebit;
    const double alpha = out_key->params->alpha_min;
    const int32_t sizeks = n*t*(base-1);
    //const int32_t n_out = out_key->params->n;

    double err = 0;

    // chose a random vector of gaussian noises
    double* noise = new double[sizeks];
    for (int32_t i = 0; i < sizeks; ++i){
        normal_distribution<double> distribution(0.,alpha); 
        noise[i] = distribution(generator);
        err += noise[i];
    }
    // recenter the noises
    err = err/sizeks;
    for (int32_t i = 0; i < sizeks; ++i) noise[i] -= err;


    // generate the ks
    int32_t index = 0; 
    for (int32_t i = 0; i < n; ++i) {
        for (int32_t j = 0; j < t; ++j) {

            // term h=0 as trivial encryption of 0 (it will not be used in the KeySwitching)
            lweNoiselessTrivial(&result->ks[i][j][0], 0, out_key->params);
            //lweSymEncrypt(&result->ks[i][j][0],0,alpha,out_key);

            for (int32_t h = 1; h < base; ++h) { // pas le terme en 0
                /*
                // noiseless encryption
                result->ks[i][j][h].b = (in_key->key[i]*h)*(1<<(32-(j+1)*basebit));
                for (int32_t p = 0; p < n_out; ++p) {
                    result->ks[i][j][h].a[p] = uniformTorus32_distrib(generator);
                    result->ks[i][j][h].b += result->ks[i][j][h].a[p] * out_key->key[p];
                }
                // add the noise 
                result->ks[i][j][h].b += dtot32(noise[index]);
                */
                Torus32 mess = (in_key->key[i]*h)*(1<<(32-(j+1)*basebit));
                lweSymEncryptWithExternalNoise(&result->ks[i][j][h], mess, noise[index], alpha, out_key);
                index += 1;
            }
        }
    }


    delete[] noise; 
}











//sample=(a',b')
EXPORT void lweKeySwitch(LweSample* result, const LweKeySwitchKey* ks, const LweSample* sample){
    const LweParams* params=ks->out_params;
    const int32_t n=ks->n;
    const int32_t basebit=ks->basebit;
    const int32_t t=ks->t;

    lweNoiselessTrivial(result,sample->b,params);
    lweKeySwitchTranslate_fromArray(result,
	    (const LweSample***) ks->ks, params,
	    sample->a, n, t, basebit);
}

/**
 * LweKeySwitchKey constructor function
 */
EXPORT void init_LweKeySwitchKey(LweKeySwitchKey* obj, int32_t n, int32_t t, int32_t basebit, const LweParams* out_params) {
    const int32_t base=1<<basebit;
    LweSample* ks0_raw = new_LweSample_array(n*t*base, out_params);

    new(obj) LweKeySwitchKey(n,t,basebit,out_params, ks0_raw);
}

/**
 * LweKeySwitchKey destructor
 */
EXPORT void destroy_LweKeySwitchKey(LweKeySwitchKey* obj) {
    const int32_t n = obj->n;
    const int32_t t = obj->t;
    const int32_t base = obj->base;
    delete_LweSample_array(n*t*base,obj->ks0_raw);

    obj->~LweKeySwitchKey();
}

//------------------------------------------------
//  autogenerated constructor/destructors/allocators
//------------------------------------------------

EXPORT LweKeySwitchKey* alloc_LweKeySwitchKey() {
    return (LweKeySwitchKey*) malloc(sizeof(LweKeySwitchKey));
}
EXPORT LweKeySwitchKey* alloc_LweKeySwitchKey_array(int32_t nbelts) {
    return (LweKeySwitchKey*) malloc(nbelts*sizeof(LweKeySwitchKey));
}

//free memory space for a LweKey
EXPORT void free_LweKeySwitchKey(LweKeySwitchKey* ptr) {
    free(ptr);
}
EXPORT void free_LweKeySwitchKey_array(int32_t nbelts, LweKeySwitchKey* ptr) {
    free(ptr);
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_LweKeySwitchKey_array(int32_t nbelts, LweKeySwitchKey* obj, int32_t n, int32_t t, int32_t basebit, const LweParams* out_params) {
    for (int32_t i=0; i<nbelts; i++) {
	init_LweKeySwitchKey(obj+i, n,t,basebit,out_params);
    }
}

//destroys the LweKeySwitchKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweKeySwitchKey_array(int32_t nbelts, LweKeySwitchKey* obj) {
    for (int32_t i=0; i<nbelts; i++) {
	destroy_LweKeySwitchKey(obj+i);
    }
}
 
//allocates and initialize the LweKeySwitchKey structure
//(equivalent of the C++ new)
EXPORT LweKeySwitchKey* new_LweKeySwitchKey(int32_t n, int32_t t, int32_t basebit, const LweParams* out_params) {
    LweKeySwitchKey* obj = alloc_LweKeySwitchKey();
    init_LweKeySwitchKey(obj, n,t,basebit,out_params);
    return obj;
}
EXPORT LweKeySwitchKey* new_LweKeySwitchKey_array(int32_t nbelts, int32_t n, int32_t t, int32_t basebit, const LweParams* out_params) {
    LweKeySwitchKey* obj = alloc_LweKeySwitchKey_array(nbelts);
    init_LweKeySwitchKey_array(nbelts, obj, n,t,basebit,out_params);
    return obj;
}

//destroys and frees the LweKeySwitchKey structure
//(equivalent of the C++ delete)
EXPORT void delete_LweKeySwitchKey(LweKeySwitchKey* obj) {
    destroy_LweKeySwitchKey(obj);
    free_LweKeySwitchKey(obj);
}
EXPORT void delete_LweKeySwitchKey_array(int32_t nbelts, LweKeySwitchKey* obj) {
    destroy_LweKeySwitchKey_array(nbelts,obj);
    free_LweKeySwitchKey_array(nbelts,obj);
}




