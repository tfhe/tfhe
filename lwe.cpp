#include <cstdlib>
#include "lwe.h"
#include "lweparams.h"
#include "lwekey.h"

using namespace std;

/**
 * This function generates a random LWE key for the given parameters.
 * The LWE key for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
EXPORT void lweKeyGen(LWEKey* result) {
  int n = result->params->n;
  for (int i=0; i<n; i++) {
    result->key[i]=rand()%2;
    // à completer je crois
  }
}



/**
 * This function generates a random LWE key for the given parameters.
 * The LWE key for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
EXPORT void lweSymEncrypt(LWESample* result, Torus32 message, double alpha, const LWEKey* key){
    
/*
    mpfr_t temp;
    mpfr_init(temp);

    gaussian(c[N], mu, sigma); // rajouter un condition de verification 
    if (mpfr_cmp_d(c[N],0.0)<0) mpfr_add_d(c[N], c[N], 1.0, MPFR_RNDN); //on travaille mod 1

    for (int i = 0; i < N; ++i)
    {
        uniform_01(c[i]); // uniforme en [0,1]
        mpfr_mul(temp, c[i], sk[i], MPFR_RNDN);
        mpfr_add(c[N], c[N], temp, MPFR_RNDN);
    }
    mpfr_frac(c[N], c[N], MPFR_RNDN); //on travaille mod 1, donc on elimine la partie entiere
    
    mpfr_clear(temp);*/
}



EXPORT Torus32 lwePhase(const LWESample* sample, const LWEKey* key);



EXPORT double lweSymDecrypt(const LWESample* sample, const LWEKey* key);

EXPORT void lweLinearCombination(LWESample* result, const int* combi, const LWESample* samples, const LWEParams* params);

EXPORT void lweKeySwitch(LWESample* result, const LWEKeySwitchKey* ks, const LWESample sample);

// Ring
EXPORT void ringLweKeyGen(LWEKey* result);
EXPORT void ringLweSymEncrypt(LWESample* result, double message, const LWEKey* key);
EXPORT double ringLweSymDecrypt(const LWESample* sample, const LWEKey* key);

EXPORT void ringLwePolyCombination(LWESample* result, const int* combi, const LWESample* samples, const LWEParams* params);

// GSW
EXPORT void ringGswKeyGen(LWEKey* result);
EXPORT void ringGswSymEncrypt(LWESample* result, double message, const LWEKey* key);
EXPORT double ringGswSymDecrypt(const LWESample* sample, const LWEKey* key);

EXPORT void ringGswPolyCombination(LWESample* result, const int* combi, const LWESample* samples, const LWEParams* params);

//extractions Ring LWE -> LWE
EXPORT void keyExtract(LWEKey* result, const RingLWEKey*); //sans doute un param supplémentaire
EXPORT void sampleExtract(LWESample* result, const RingLWESample* x);

//extraction RingGSW -> GSW
EXPORT void gswKeyExtract(RingLWEKey* result, const RingGSWKey* key); //sans doute un param supplémentaire
EXPORT void gswSampleExtract(RingLWESample* result, const RingGSWSample* x);

//bootstrapping
EXPORT void bootstrap(LWESample* result, const LWEBootstrappingKey* bk, double mu1, double mu0, const LWESample* x);
