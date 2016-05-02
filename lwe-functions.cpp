#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include "lwe.h"
#include "lweparams.h"
#include "lwekey.h"
#include "lwesamples.h"
#include "lwekeyswitch.h"

using namespace std;


/**
 * This function generates a random LWE key for the given parameters.
 * The LWE key for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
EXPORT void lweKeyGen(LWEKey* result) {
  const int n = result->params->n;
  uniform_int_distribution<int> distribution(0,1);

  for (int i=0; i<n; i++) 
    result->key[i]=distribution(generator);
}



/**
 * This function encrypts message by using key, with stdev alpha
 * The LWE sample for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
EXPORT void lweSymEncrypt(LWESample* result, Torus32 message, double alpha, const LWEKey* key){
    const int n = key->params->n;

    result->b = gaussian32(message, alpha); 
    for (int i = 0; i < n; ++i)
    {
        result->a[i] = uniformTorus32_distrib(generator);
        result->b += result->a[i]*key->key[i];
    }

    result->current_variance = alpha*alpha;
}



/**
 * This function computes the phase of sample by using key : phi = b - a.s
 */
EXPORT Torus32 lwePhase(const LWESample* sample, const LWEKey* key){
    const int n = key->params->n;
    Torus32 axs = 0;
    const Torus32 *__restrict a = sample->a;
    const int * __restrict k = key->key;

    for (int i = 0; i < n; ++i) 
	   axs += a[i]*k[i]; 
    return sample->b - axs;
}


/**
 * This function computes the decryption of sample by using key
 * The constant Msize indicates the message space and is used to approximate the phase
 */
EXPORT Torus32 lweSymDecrypt(const LWESample* sample, const LWEKey* key, const int Msize){
    Torus32 phi;

    phi = lwePhase(sample, key);
    return approxPhase(phi, Msize);
}




//Arithmetic operations on LWE samples
/** result = (0,0) */
EXPORT void lweClear(LWESample* result, const LWEParams* params){
    const int n = params->n;

    for (int i = 0; i < n; ++i) result->a[i] = 0;
    result->b = 0;
    result->current_variance = 0.;
}

/** result = (0,mu) */
EXPORT void lweNoiselessTrivial(LWESample* result, Torus32 mu, const LWEParams* params){
    const int n = params->n;

    for (int i = 0; i < n; ++i) result->a[i] = 0;
    result->b = mu;
    result->current_variance = 0.;
}

/** result = result + sample */
EXPORT void lweAddTo(LWESample* result, const LWESample* sample, const LWEParams* params){
    const int n = params->n;

    for (int i = 0; i < n; ++i) result->a[i] += sample->a[i];
    result->b += sample->b;
    result->current_variance += sample->current_variance; 
}

/** result = result - sample */
EXPORT void lweSubTo(LWESample* result, const LWESample* sample, const LWEParams* params){
    const int n = params->n;

    for (int i = 0; i < n; ++i) result->a[i] -= sample->a[i];
    result->b -= sample->b;
    result->current_variance += sample->current_variance; 
}

/** result = result + p.sample */
EXPORT void lweAddMulTo(LWESample* result, int p, const LWESample* sample, const LWEParams* params){
    const int n = params->n;

    for (int i = 0; i < n; ++i) result->a[i] += p*sample->a[i];
    result->b += p*sample->b;
    result->current_variance += (p*p)*sample->current_variance; 
}

/** result = result - p.sample */
EXPORT void lweSubMulTo(LWESample* result, int p, const LWESample* sample, const LWEParams* params){
    const int n = params->n;

    for (int i = 0; i < n; ++i) result->a[i] -= p*sample->a[i];
    result->b -= p*sample->b;
    result->current_variance += (p*p)*sample->current_variance; 
}


EXPORT void lweCreateKeySwitchKey(LWEKeySwitchKey* result, const LWEKey* in_key, const LWEKey* out_key);
//voir si on le garde ou on fait lweAdd (laisser en suspense)

// EXPORT void lweLinearCombination(LWESample* result, const int* combi, const LWESample** samples, const LWEParams* params);

EXPORT void lweKeySwitch(LWESample* result, const LWEKeySwitchKey* ks, const LWESample* sample);


