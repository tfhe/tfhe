#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include "tfhe_generic_templates.h"
#include "tfhe_core.h"
#include "numeric_functions.h"
#include "lweparams.h"
#include "lwekey.h"
#include "lwesamples.h"
#include "lwekeyswitch.h"

using namespace std;


/**
 * This function generates a random Lwe key for the given parameters.
 * The Lwe key for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
EXPORT void lweKeyGen(LweKey* result) {
  const int32_t n = result->params->n;
  uniform_int_distribution<int32_t> distribution(0,1);

  for (int32_t i=0; i<n; i++) 
    result->key[i]=distribution(generator);
}



/**
 * This function encrypts message by using key, with stdev alpha
 * The Lwe sample for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
EXPORT void lweSymEncrypt(LweSample* result, Torus32 message, double alpha, const LweKey* key){
    const int32_t n = key->params->n;

    result->b = gaussian32(message, alpha); 
    for (int32_t i = 0; i < n; ++i)
    {
        result->a[i] = uniformTorus32_distrib(generator);
        result->b += result->a[i]*key->key[i];
    }

    result->current_variance = alpha*alpha;
}


/* 
 * This function encrypts a message by using key and a given noise value
*/
EXPORT void lweSymEncryptWithExternalNoise(LweSample* result, Torus32 message, double noise, double alpha, const LweKey* key){
    const int32_t n = key->params->n;

    result->b = message + dtot32(noise); 
    for (int32_t i = 0; i < n; ++i)
    {
        result->a[i] = uniformTorus32_distrib(generator);
        result->b += result->a[i]*key->key[i];
    }

    result->current_variance = alpha*alpha;
}




/**
 * This function computes the phase of sample by using key : phi = b - a.s
 */
EXPORT Torus32 lwePhase(const LweSample* sample, const LweKey* key){
    const int32_t n = key->params->n;
    Torus32 axs = 0;
    const Torus32 *__restrict a = sample->a;
    const int32_t * __restrict k = key->key;

    for (int32_t i = 0; i < n; ++i) 
	   axs += a[i]*k[i]; 
    return sample->b - axs;
}


/**
 * This function computes the decryption of sample by using key
 * The constant Msize indicates the message space and is used to approximate the phase
 */
EXPORT Torus32 lweSymDecrypt(const LweSample* sample, const LweKey* key, const int32_t Msize){
    Torus32 phi;

    phi = lwePhase(sample, key);
    return approxPhase(phi, Msize);
}




//Arithmetic operations on Lwe samples
/** result = (0,0) */
EXPORT void lweClear(LweSample* result, const LweParams* params){
    const int32_t n = params->n;

    for (int32_t i = 0; i < n; ++i) result->a[i] = 0;
    result->b = 0;
    result->current_variance = 0.;
}


/** result = sample */
EXPORT void lweCopy(LweSample* result, const LweSample* sample, const LweParams* params){
    const int32_t n = params->n;

    for (int32_t i = 0; i < n; ++i) result->a[i] = sample->a[i];
    result->b = sample->b;
    result->current_variance = sample->current_variance;
}


/** result = -sample */
EXPORT void lweNegate(LweSample* result, const LweSample* sample, const LweParams* params){
    const int32_t n = params->n;

    for (int32_t i = 0; i < n; ++i) result->a[i] = -sample->a[i];
    result->b = -sample->b;
    result->current_variance = sample->current_variance;
}


/** result = (0,mu) */
EXPORT void lweNoiselessTrivial(LweSample* result, Torus32 mu, const LweParams* params){
    const int32_t n = params->n;

    for (int32_t i = 0; i < n; ++i) result->a[i] = 0;
    result->b = mu;
    result->current_variance = 0.;
}

/** result = result + sample */
EXPORT void lweAddTo(LweSample* result, const LweSample* sample, const LweParams* params){
    const int32_t n = params->n;

    for (int32_t i = 0; i < n; ++i) result->a[i] += sample->a[i];
    result->b += sample->b;
    result->current_variance += sample->current_variance; 
}

#ifdef __AVX2__
/** r -= a  using avx instructions (of size n, not necessarily multiple of 8) */
EXPORT void __attribute__ ((noinline)) intVecSubTo_avx(int32_t* r, const int32_t* a, int64_t n) {
    __asm__ __volatile__ (
	    //"pushq %%r8\n"               //save clobbered regs
	    //"pushq %%r9\n" 
	    "movq %%rdx,%%rax\n"
	    "andq $0xFFFFFFFFFFFFFFF8,%%rax\n"  //r8: n0 = n - n%8
	    "leaq (%%rsi,%%rax,4),%%rcx\n"       //r9: aend = a + 4n0
	    "1:\n"
	    "vmovdqu (%%rdi),%%ymm0\n"
	    "vmovdqu (%%rsi),%%ymm1\n"
	    "vpsubd %%ymm1,%%ymm0,%%ymm0\n"
	    "vmovdqu %%ymm0,(%%rdi)\n"
	    "addq $32,%%rdi\n"                 //advance r by 8*4
	    "addq $32,%%rsi\n"                 //advance a by 8*4
	    "cmpq %%rcx,%%rsi\n"                //until aend
	    "jb 1b\n"
	    "vzeroall\n"
	    "subq %%rax,%%rdx\n"                //n = n - n0 (between 0 and 7)
	    "cmpq $4,%%rdx\n"                  //last 4 operands?
	    "jb 2f\n"
	    "vmovdqu (%%rdi),%%xmm0\n"
	    "vmovdqu (%%rsi),%%xmm1\n"
	    "vpsubd %%xmm1,%%xmm0,%%xmm0\n"
	    "vmovdqu %%xmm0,(%%rdi)\n"
	    "addq $16,%%rdi\n"                 //advance r by 4*4
	    "addq $16,%%rsi\n"                 //advance a by 4*4
	    "subq $4,%%rdx\n"                  //n = n - 4
	    "2:"
	    "cmpq $2,%%rdx\n"                  //last 2 operands?
	    "jb 3f\n"
	    "movq (%%rdi),%%xmm0\n"
	    "movq (%%rsi),%%xmm1\n"
	    "psubd %%xmm1,%%xmm0\n"
	    "movq %%xmm0,(%%rdi)\n"
	    "addq $8,%%rdi\n"                 //advance r by 2*4
	    "addq $8,%%rsi\n"                 //advance a by 2*4
	    "subq $2,%%rdx\n"                 //n = n - 2
	    "3:"
	    "cmpq $1,%%rdx\n"                 //last 1 operand?
	    "jb 4f\n"
	    "movl (%%rdi),%%eax\n"
	    "movl (%%rsi),%%ecx\n"
	    "subl %%ecx,%%eax\n"
	    "movl %%eax,(%%rdi)\n"
	    "4:"
	    //"popq %%rcx\n"
	    //"popq %%r8\n"
	    : "=D"(r),"=S"(a),"=d"(n)  //output
	    : "D"(r),"S"(a),"d"(n)     //input
	    : "%rax","%rcx" //clobber list (don't mess up with it)
	 );
}

int32_t intVecSubTo_avx_test() {
    fprintf(stderr,"testint intVecSubTo_avx\n");
    static int32_t tst[1000];
    static int32_t tst2[1000];
    static int32_t tst3[1000];
    for (int32_t i=0; i<1000; i++) tst[i]=rand();
    for (int32_t i=0; i<1000; i++) tst2[i]=rand();
    for (int32_t trial=0; trial<100; trial++) {
	for (int32_t i=0; i<1000; i++) tst3[i]=tst[i];
	int32_t dim = 500 + (rand()%500);
	intVecSubTo_avx(tst,tst2,dim);
	for (int32_t i=0; i<dim; i++) if (tst[i]!=tst3[i]-tst2[i]) abort();
	for (int32_t i=dim; i<1000; i++) if (tst[i]!=tst3[i]) abort();
    }
    return tst[0];
}
//int32_t ooo = intVecSubTo_avx_test();
#endif

/** result = result - sample */
EXPORT void lweSubTo(LweSample* result, const LweSample* sample, const LweParams* params){
    const int32_t n = params->n;
    const Torus32* __restrict sa = sample->a;
    Torus32* __restrict ra = result->a;

#ifdef __AVX2__
    intVecSubTo_avx(ra,sa,n);
#else
    for (int32_t i = 0; i < n; ++i) ra[i] -= sa[i];
#endif
    result->b -= sample->b;
    result->current_variance += sample->current_variance; 
}

/** result = result + p.sample */
EXPORT void lweAddMulTo(LweSample* result, int32_t p, const LweSample* sample, const LweParams* params){
    const int32_t n = params->n;

    for (int32_t i = 0; i < n; ++i) result->a[i] += p*sample->a[i];
    result->b += p*sample->b;
    result->current_variance += (p*p)*sample->current_variance; 
}

/** result = result - p.sample */
EXPORT void lweSubMulTo(LweSample* result, int32_t p, const LweSample* sample, const LweParams* params){
    const int32_t n = params->n;

    for (int32_t i = 0; i < n; ++i) result->a[i] -= p*sample->a[i];
    result->b -= p*sample->b;
    result->current_variance += (p*p)*sample->current_variance; 
}



//autogenerated memory-related functions

//explicit constructor
EXPORT void init_LweSample(LweSample* obj, const LweParams* params) {
    new(obj) LweSample(params);
}

//explicit destructor
EXPORT void destroy_LweSample(LweSample* obj) {
    obj->~LweSample();
}

//explicit constructor
EXPORT void init_LweKey(LweKey* obj, const LweParams* params) {
    new(obj) LweKey(params);
}

//explicit destructor
EXPORT void destroy_LweKey(LweKey* obj) {
    obj->~LweKey();
}


USE_DEFAULT_CONSTRUCTOR_DESTRUCTOR_IMPLEMENTATIONS1(LweSample, LweParams);

USE_DEFAULT_CONSTRUCTOR_DESTRUCTOR_IMPLEMENTATIONS1(LweKey, LweParams);



 


