#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include "lwe.h"
#include "lweparams.h"
#include "lwekey.h"
#include "lwesamples.h"
#include "ringlwe.h"
#include "ringgsw.h"

using namespace std;

static default_random_engine generator;
static const int64_t _two31 = INT64_C(1) << 31; // 2^31
static const int64_t _two32 = INT64_C(1) << 32; // 2^32
static const double _two32_double = _two32;
static const double _two31_double = _two31;


// from double to Torus32
EXPORT Torus32 dtot32(double d) {
    return int32_t(int64_t((d - int64_t(d))*_two32));
}
// from Torus32 to double
EXPORT double t32tod(Torus32 x) {
    return double(x)/_two32_double;
}





// TorusPolynomial = 0
EXPORT void ClearTorusPolynomial(TorusPolynomial* result) {
    int N = result->N;

    for (int i = 0; i < N; ++i) result->coefsT[i] = 0;
}

// TorusPolynomial = TorusPolynomial
EXPORT void CopyTorusPolynomial(TorusPolynomial* __restrict result, const TorusPolynomial* __restrict sample) {
    assert(result!=sample);
    const int N = result->N;

    for (int i = 0; i < N; ++i) result->coefsT[i] = sample->coefsT[i];
}

// TorusPolynomial + TorusPolynomial
EXPORT void AddTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2) {
    int N = poly1->N;

    for (int i = 0; i < N; ++i) result->coefsT[i] = poly1->coefsT[i] + poly2->coefsT[i];
}

// TorusPolynomial += TorusPolynomial
EXPORT void AddToTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly2) {
    int N = poly2->N;

    for (int i = 0; i < N; ++i) result->coefsT[i] += poly2->coefsT[i];
}


// TorusPolynomial - TorusPolynomial
EXPORT void SubTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2) {
    int N = poly1->N;

    for (int i = 0; i < N; ++i) result->coefsT[i] = poly1->coefsT[i] - poly2->coefsT[i];
}

// TorusPolynomial -= TorusPolynomial
EXPORT void SubToTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly2) {
    int N = poly2->N;

    for (int i = 0; i < N; ++i) result->coefsT[i] -= poly2->coefsT[i];
}

// TorusPolynomial + p*TorusPolynomial
EXPORT void AddMulZTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, int p, const TorusPolynomial* poly2) {
    int N = poly1->N;

    for (int i = 0; i < N; ++i) result->coefsT[i] = poly1->coefsT[i] + p*poly2->coefsT[i];
}

// TorusPolynomial += p*TorusPolynomial
EXPORT void AddMulZToTorusPolynomial(TorusPolynomial* result, int p, const TorusPolynomial* poly2) {
    int N = poly2->N;

    for (int i = 0; i < N; ++i) result->coefsT[i] += p*poly2->coefsT[i];
}

// TorusPolynomial - p*TorusPolynomial
EXPORT void SubMulZTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, int p, const TorusPolynomial* poly2) {
    int N = poly1->N;

    for (int i = 0; i < N; ++i) result->coefsT[i] = poly1->coefsT[i] - p*poly2->coefsT[i];
}

// TorusPolynomial -= p*TorusPolynomial
EXPORT void SubMulZToTorusPolynomial(TorusPolynomial* result, int p, const TorusPolynomial* poly2) {
    int N = poly2->N;

    for (int i = 0; i < N; ++i) result->coefsT[i] += p*poly2->coefsT[i];
}



// Gaussian sample centered in message, with standard deviation sigma
Torus32 gaussian32(Torus32 message, double sigma){
    //Attention: all the implementation will use the stdev instead of the gaussian fourier param
    normal_distribution<double> distribution(0.,sigma); //TODO: can we create a global distrib of param 1 and multiply by sigma?
    double err = distribution(generator);
    return message + dtot32(err);
}


// Used to approximate the phase to the nearest message possible in the message space
// The constant Msize will indicate on which message space we are working (how many messages possible)
Torus32 approxPhase(Torus32 phase, int Msize){
    uint64_t interv = UINT64_C(-1)/Msize; // width of each intervall
    uint64_t half_interval = interv/2; // begin of the first intervall
    uint64_t phase64 = (uint64_t(phase)<<32) + half_interval;
    //floor to the nearest multiples of interv
    phase64 -= phase64%interv;
    //rescale to torus32
    return int32_t(phase64>>32); 
}







/**
 * This function generates a random LWE key for the given parameters.
 * The LWE key for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
EXPORT void lweKeyGen(LWEKey* result) {
  int n = result->params->n;
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
    int n = key->params->n;
    uniform_real_distribution<double> distribution(-0.5,0.5);

    result->b = gaussian32(message, alpha); 
    for (int i = 0; i < n; ++i)
    {
        result->a[i] = dtot32(distribution(generator));
        result->b += result->a[i]*key->key[i];
    }
}



/**
 * This function computes the phase of sample by using key : phi = b - a.s
 */
EXPORT Torus32 lwePhase(const LWESample* sample, const LWEKey* key){
    const int n = key->params->n;
    Torus32 phi = 0;
    const Torus32 *__restrict a = sample->a;
    const int * __restrict k = key->key;

    for (int i = 0; i < n; ++i) 
	   phi += a[i]*k[i]; 
    return sample->b - phi;
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
    int n = params->n;

    for (int i = 0; i < n; ++i) result->a[i] = 0;
    result->b = 0;
    result->alpha = 0.;
}

/** result = (0,mu) */
EXPORT void lweNoiselessTrivial(LWESample* result, Torus32 mu, const LWEParams* params){
    int n = params->n;

    for (int i = 0; i < n; ++i) result->a[i] = 0;
    result->b = mu;
    result->alpha = 0.;
}

/** result = result + sample */
EXPORT void lweAddTo(LWESample* result, const LWESample* sample, const LWEParams* params){
    int n = params->n;

    for (int i = 0; i < n; ++i) result->a[i] += sample->a[i];
    result->b += sample->b;
    result->alpha += sample->alpha; //à revoir
}

/** result = result - sample */
EXPORT void lweSubTo(LWESample* result, const LWESample* sample, const LWEParams* params){
    int n = params->n;

    for (int i = 0; i < n; ++i) result->a[i] -= sample->a[i];
    result->b -= sample->b;
    result->alpha += sample->alpha; //à revoir
}

/** result = result + p.sample */
EXPORT void lweAddMulTo(LWESample* result, int p, const LWESample* sample, const LWEParams* params){
    int n = params->n;

    for (int i = 0; i < n; ++i) result->a[i] += p*sample->a[i];
    result->b += p*sample->b;
    result->alpha += p*sample->alpha; //à revoir
}

/** result = result - p.sample */
EXPORT void lweSubMulTo(LWESample* result, int p, const LWESample* sample, const LWEParams* params){
    int n = params->n;

    for (int i = 0; i < n; ++i) result->a[i] -= p*sample->a[i];
    result->b -= p*sample->b;
    result->alpha += p*sample->alpha; //à revoir
}


EXPORT void createKeySwitchKey(LWEKeySwitchKey* result, const LWEKey* in_key, const LWEKey* out_key);
//voir si on le garde ou on fait lweAdd (laisser en suspense)

// EXPORT void lweLinearCombination(LWESample* result, const int* combi, const LWESample** samples, const LWEParams* params);

EXPORT void lweKeySwitch(LWESample* result, const LWEKeySwitchKey* ks, const LWESample* sample);







// RingLWE
EXPORT void ringLweKeyGen(RingLWEKey* result){
  int N = result->params->N;
  int k = result->params->k;
  uniform_int_distribution<int> distribution(0,1);

  for (int i = 0; i < k; ++i)
      for (int j = 0; j < N; ++j)
          result->key[i].coefs[j] = distribution(generator);
}




EXPORT void ringLweSymEncrypt(RingLWESample* result, TorusPolynomial* message, double alpha, const RingLWEKey* key){
    int N = key->params->N;
    int k = key->params->k;
    uniform_real_distribution<double> distribution(-0.5,0.5);
    TorusPolynomial* temp0 = new_TorusPolynomial(N); // temporary value for Karatsuba
    
    for (int j = 0; j < N; ++j)
        result->b->coefsT[j] = gaussian32(0, alpha) + message->coefsT[j];   
    
    for (int i = 0; i < k; ++i)
    {
        for (int j = 0; j < N; ++j)
            result->a[i].coefsT[j] = dtot32(distribution(generator));
        multKaratsuba(temp0, &key->key[i], &result->a[i]);
        AddTorusPolynomial(result->b, result->b, temp0); 
    }
    delete_TorusPolynomial(temp0);
}






/**
 * This function computes the phase of sample by using key : phi = b - a.s
 */
EXPORT void ringLwePhase(TorusPolynomial* phase, const RingLWESample* sample, const RingLWEKey* key){
    int N = key->params->N;
    int k = key->params->k;
    TorusPolynomial* temp = new_TorusPolynomial(N);

    CopyTorusPolynomial(phase, sample->b); // phi = b

    for (int i = 0; i < k; ++i)
    {
        multKaratsuba(temp, &key->key[i], &sample->a[i]); // temp = a_i*s_i
        SubTorusPolynomial(phase, phase, temp); 
    }
    delete_TorusPolynomial(temp);
}


/**
 * This function computes the approximation of the phase 
 * à revoir, surtout le Msize
 */
EXPORT void ringLweApproxPhase(TorusPolynomial* message, TorusPolynomial* phase, int Msize, int N){
    for (int i = 0; i < N; ++i) message->coefsT[i] = approxPhase(phase->coefsT[i], Msize);
}




EXPORT void ringLweSymDecrypt(TorusPolynomial* result, const RingLWESample* sample, const RingLWEKey* key, int Msize){
    ringLwePhase(result, sample, key);
    ringLweApproxPhase(result, result, Msize, key->params->N);
}











//Arithmetic operations on RingLWE samples
/** result = (0,0) */
EXPORT void RingLweClear(RingLWESample* result, const RingLWEParams* params){
    int k = params->k;

    for (int i = 0; i < k; ++i) ClearTorusPolynomial(&result->a[i]);
    ClearTorusPolynomial(result->b);
    result->current_alpha = 0.;
}

/** result = (0,mu) */
EXPORT void RingLweNoiselessTrivial(RingLWESample* result, const TorusPolynomial* mu, const RingLWEParams* params){
    int k = params->k;

    for (int i = 0; i < k; ++i) ClearTorusPolynomial(&result->a[i]);
    CopyTorusPolynomial(result->b, mu);
    result->current_alpha = 0.;
}

/** result = result + sample */
EXPORT void RingLweAddTo(RingLWESample* result, const RingLWESample* sample, const RingLWEParams* params){
    int k = params->k;

    for (int i = 0; i < k; ++i) AddTorusPolynomial(&result->a[i], &result->a[i], &sample->a[i]);
    AddTorusPolynomial(result->b, result->b, sample->b);
    result->current_alpha += sample->current_alpha; //à revoir
}

/** result = result - sample */
EXPORT void RingLweSubTo(RingLWESample* result, const RingLWESample* sample, const RingLWEParams* params){
    int k = params->k;

    for (int i = 0; i < k; ++i) SubToTorusPolynomial(&result->a[i], &sample->a[i]);
    SubToTorusPolynomial(result->b, sample->b);
    result->current_alpha += sample->current_alpha; //à revoir
}

/** result = result + p.sample */
EXPORT void RingLweAddMulTo(RingLWESample* result, int p, const RingLWESample* sample, const RingLWEParams* params){
    int k = params->k;

    for (int i = 0; i < k; ++i) AddMulTorusPolynomial(&result->a[i], &result->a[i], &sample->a[i], p);
    AddMulTorusPolynomial(result->b, result->b, sample->b, p);
    result->current_alpha += sample->current_alpha; //à revoir
}

/** result = result - p.sample */
EXPORT void RingLweSubMulTo(RingLWESample* result, int p, const RingLWESample* sample, const RingLWEParams* params){
    int k = params->k;

    for (int i = 0; i < k; ++i) SubMulTorusPolynomial(&result->a[i], &result->a[i], &sample->a[i], p);
    SubMulTorusPolynomial(result->b, result->b, sample->b, p);
    result->current_alpha += sample->current_alpha; //à revoir
}

// EXPORT void ringLwePolyCombination(RingLWESample* result, const int* combi, const RingLWESample* samples, const RingLWEParams* params);









// RingGSW
EXPORT void ringGswKeyGen(LWEKey* result);
EXPORT void ringGswSymEncrypt(RingGSWSample* result, const IntPolynomial* message, const RingGSWKey* key);
EXPORT void ringGswSymDecrypt(IntPolynomial* result, const RingGSWSample* sample, const RingGSWKey* key); 

EXPORT void ringGswSymEncryptInt(RingGSWSample* result, const int message, const RingGSWKey* key);
EXPORT int ringGswSymDecryptInt(const RingGSWSample* sample, const RingGSWKey* key); 
//do we really decrypt GSW samples?



//TODO: Ilaria.
EXPORT void ringGSWExternProduct(RingLWESample* result, const RingGSWSample* a, const RingLWESample* b, const RingLWEParams* rlweParams, const RingGSWParams* rgswParams);

//TODO: mettre les mêmes fonctions arithmétiques que pour LWE
//      pour les opérations externes, prévoir int et intPolynomial

//extractions RingLWE -> LWE
EXPORT void ringLweExtractKey(LWEKey* result, const RingLWEKey*); //sans doute un param supplémentaire
EXPORT void ringLweExtractSample(LWESample* result, const RingLWESample* x);

//extraction RingGSW -> SemiRingGSW
EXPORT void ringGswExtractKey(SemiRingGSWSample* result, const RingGSWKey* key);
EXPORT void ringGswExtractSample(RingLWESample* result, const RingGSWSample* x);

//LWE to LWE Single gate bootstrapping
//TODO: Malika
EXPORT void bootstrap(LWESample* result, const LWEBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LWESample* x);
//these functions call the bootstrapping, assuming that the message space is {0,1/4} 
EXPORT void lweNand(LWESample* result, const LWEBootstrappingKey* bk, const LWESample* a, const LWESample* b);
EXPORT void lweOr(LWESample* result, const LWEBootstrappingKey* bk, const LWESample* a, const LWESample* b);
EXPORT void lweAnd(LWESample* result, const LWEBootstrappingKey* bk, const LWESample* a, const LWESample* b);
EXPORT void lweXor(LWESample* result, const LWEBootstrappingKey* bk, const LWESample* a, const LWESample* b);
// mux(a,b,c) = a?b:c = a et b + not(a) et c 
EXPORT void lweMux(LWESample* result, const LWEBootstrappingKey* bk, const LWESample* a, const LWESample* b, const LWESample* c);
EXPORT void lweNot(LWESample* result, LWESample* a);


//leveled functions

//LWE to SemiRing Bootstrapping
EXPORT void semiRingBootstrap(LWESample* result, const LWEBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LWESample* x);



// EXPORT void ringGswPolyCombination(LWESample* result, const int* combi, const LWESample* samples, const LWEParams* params);

//extractions Ring LWE -> LWE
// EXPORT void keyExtract(LWEKey* result, const RingLWEKey*); //sans doute un param supplémentaire
// EXPORT void sampleExtract(LWESample* result, const RingLWESample* x);

//extraction RingGSW -> GSW
// EXPORT void gswKeyExtract(RingLWEKey* result, const RingGSWKey* key); //sans doute un param supplémentaire
// EXPORT void gswSampleExtract(RingLWESample* result, const RingGSWSample* x);

//bootstrapping
// EXPORT void bootstrap(LWESample* result, const LWEBootstrappingKey* bk, double mu1, double mu0, const LWESample* x);
