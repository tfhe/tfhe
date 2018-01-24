#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include "tfhe_core.h"
#include "numeric_functions.h"
#include "lweparams.h"
#include "lwekey.h"
#include "lwesamples.h"
#include "lwekeyswitch.h"
#include "lwe-functions.h"
#include "random_gen.h"

using namespace std;


/**
 * This function generates a random Lwe key for the given parameters.
 * The Lwe key for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
template<typename TORUS>
void LweFunctions<TORUS>::KeyGen(LweKey<TORUS>* result) {
  const int n = result->params->n;
  for (int i=0; i<n; i++)
    result->key[i] = RandomGen::uniform_bool();
}


/**
 * This function encrypts message by using key, with stdev alpha
 * The Lwe sample for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
template<typename TORUS>
void LweFunctions<TORUS>::SymEncrypt(LweSample<TORUS>* result, TORUS message, double alpha, const LweKey<TORUS>* key){
  const int n = key->params->n;

  result->b = RandomGenTorus<TORUS>::gaussian(message, alpha);
  for (int i = 0; i < n; ++i)
  {
    result->a[i] = RandomGenTorus<TORUS>::uniform();
    result->b += result->a[i]*key->key[i];
  }

  result->current_variance = alpha*alpha;
}



/**
 * This function computes the phase of sample by using key : phi = b - a.s
 */
template<typename TORUS>
TORUS LweFunctions<TORUS>::Phase(const LweSample<TORUS>* sample, const LweKey<TORUS>* key){
  const int n = key->params->n;
  TORUS axs = 0;
  const TORUS *__restrict a = sample->a;
  const int * __restrict k = key->key;

  for (int i = 0; i < n; ++i)
	   axs += a[i]*k[i];
  return sample->b - axs;
}

/**
 * This function computes the decryption of sample by using key
 * The constant Msize indicates the message space and is used to approximate the phase
 */
template<typename TORUS>
TORUS LweFunctions<TORUS>::SymDecrypt(const LweSample<TORUS>* sample, const LweKey<TORUS>* key, const int Msize){
  TORUS phi;

  phi = lwePhase(sample, key);
  return TorusUtils<TORUS>::approxPhase(phi, Msize);
}

//Arithmetic operations on Lwe samples

/** result = (0,0) */
template<typename TORUS>
void LweFunctions<TORUS>::Clear(LweSample<TORUS>* result, const LweParams<TORUS>* params){
  const int n = params->n;

  for (int i = 0; i < n; ++i) result->a[i] = 0;
  result->b = 0;
  result->current_variance = 0.;
}

/** result = sample */
template<typename TORUS>
void LweFunctions<TORUS>::Copy(LweSample<TORUS>* result, const LweSample<TORUS>* sample, const LweParams<TORUS>* params){
  const int n = params->n;

  for (int i = 0; i < n; ++i) result->a[i] = sample->a[i];
  result->b = sample->b;
  result->current_variance = sample->current_variance;
}


/** result = -sample */
template<typename TORUS>
void LweFunctions<TORUS>::Negate(LweSample<TORUS>* result, const LweSample<TORUS>* sample, const LweParams<TORUS>* params){
  const int n = params->n;

  for (int i = 0; i < n; ++i) result->a[i] = -sample->a[i];
  result->b = -sample->b;
  result->current_variance = sample->current_variance;
}


/** result = (0,mu) */
template<typename TORUS>
void LweFunctions<TORUS>::NoiselessTrivial(LweSample<TORUS>* result, TORUS mu, const LweParams<TORUS>* params){
  const int n = params->n;

  for (int i = 0; i < n; ++i) result->a[i] = 0;
  result->b = mu;
  result->current_variance = 0.;
}

/** result = result + sample */
template<typename TORUS>
void LweFunctions<TORUS>::AddTo(LweSample<TORUS>* result, const LweSample<TORUS>* sample, const LweParams<TORUS>* params){
  const int n = params->n;

  for (int i = 0; i < n; ++i) result->a[i] += sample->a[i];
  result->b += sample->b;
  result->current_variance += sample->current_variance;
}

#ifdef __AVX2__
/** r -= a  using avx instructions (of size n, not necessarily multiple of 8) */
EXPORT void __attribute__ ((noinline)) intVecSubTo_avx(int* r, const int* a, long n) {
  __asm__ __volatile__ (
	  //"pushq %%r8\n"         //save clobbered regs
	  //"pushq %%r9\n"
	  "movq %%rdx,%%rax\n"
	  "andq $0xFFFFFFFFFFFFFFF8,%%rax\n"  //r8: n0 = n - n%8
	  "leaq (%%rsi,%%rax,4),%%rcx\n"     //r9: aend = a + 4n0
	  "1:\n"
	  "vmovdqu (%%rdi),%%ymm0\n"
	  "vmovdqu (%%rsi),%%ymm1\n"
	  "vpsubd %%ymm1,%%ymm0,%%ymm0\n"
	  "vmovdqu %%ymm0,(%%rdi)\n"
	  "addq $32,%%rdi\n"         //advance r by 8*4
	  "addq $32,%%rsi\n"         //advance a by 8*4
	  "cmpq %%rcx,%%rsi\n"        //until aend
	  "jb 1b\n"
	  "vzeroall\n"
	  "subq %%rax,%%rdx\n"        //n = n - n0 (between 0 and 7)
	  "cmpq $4,%%rdx\n"          //last 4 operands?
	  "jb 2f\n"
	  "vmovdqu (%%rdi),%%xmm0\n"
	  "vmovdqu (%%rsi),%%xmm1\n"
	  "vpsubd %%xmm1,%%xmm0,%%xmm0\n"
	  "vmovdqu %%xmm0,(%%rdi)\n"
	  "addq $16,%%rdi\n"         //advance r by 4*4
	  "addq $16,%%rsi\n"         //advance a by 4*4
	  "subq $4,%%rdx\n"          //n = n - 4
	  "2:"
	  "cmpq $2,%%rdx\n"          //last 2 operands?
	  "jb 3f\n"
	  "movq (%%rdi),%%xmm0\n"
	  "movq (%%rsi),%%xmm1\n"
	  "psubd %%xmm1,%%xmm0\n"
	  "movq %%xmm0,(%%rdi)\n"
	  "addq $8,%%rdi\n"         //advance r by 2*4
	  "addq $8,%%rsi\n"         //advance a by 2*4
	  "subq $2,%%rdx\n"         //n = n - 2
	  "3:"
	  "cmpq $1,%%rdx\n"         //last 1 operand?
	  "jb 4f\n"
	  "movl (%%rdi),%%eax\n"
	  "movl (%%rsi),%%ecx\n"
	  "subl %%ecx,%%eax\n"
	  "movl %%eax,(%%rdi)\n"
	  "4:"
	  //"popq %%rcx\n"
	  //"popq %%r8\n"
	  : "=D"(r),"=S"(a),"=d"(n)  //output
	  : "D"(r),"S"(a),"d"(n)   //input
	  : "%rax","%rcx" //clobber list (don't mess up with it)
	 );
}

int intVecSubTo_avx_test() {
  fprintf(stderr,"testint intVecSubTo_avx\n");
  static int tst[1000];
  static int tst2[1000];
  static int tst3[1000];
  for (int i=0; i<1000; i++) tst[i]=rand();
  for (int i=0; i<1000; i++) tst2[i]=rand();
  for (int trial=0; trial<100; trial++) {
	for (int i=0; i<1000; i++) tst3[i]=tst[i];
	int dim = 500 + (rand()%500);
	intVecSubTo_avx(tst,tst2,dim);
	for (int i=0; i<dim; i++) if (tst[i]!=tst3[i]-tst2[i]) abort();
	for (int i=dim; i<1000; i++) if (tst[i]!=tst3[i]) abort();
  }
  return tst[0];
}
//int ooo = intVecSubTo_avx_test();
#endif

/** result = result - sample */
template<typename TORUS>
void LweFunctions<TORUS>::SubTo(LweSample<TORUS>* result, const LweSample<TORUS>* sample, const LweParams<TORUS>* params){
  const int n = params->n;
  const TORUS* __restrict sa = sample->a;
  TORUS* __restrict ra = result->a;

  for (int i = 0; i < n; ++i) ra[i] -= sa[i];
  result->b -= sample->b;
  result->current_variance += sample->current_variance;
}

/** result = result - sample */
#ifdef __AVX2__
template<>
void LweFunctions<Torus32>::SubTo(LweSample<Torus32>* result, const LweSample<Torus32>* sample, const LweParams<Torus32>* params){
  const int n = params->n;
  const Torus32* __restrict sa = sample->a;
  Torus32* __restrict ra = result->a;

  intVecSubTo_avx(ra,sa,n);
  result->b -= sample->b;
  result->current_variance += sample->current_variance;
}
#endif

/** result = result + p.sample */
template<typename TORUS>
void LweFunctions<TORUS>::AddMulTo(LweSample<TORUS>* result, int p, const LweSample<TORUS>* sample, const LweParams<TORUS>* params){
  const int n = params->n;

  for (int i = 0; i < n; ++i) result->a[i] += p*sample->a[i];
  result->b += p*sample->b;
  result->current_variance += (p*p)*sample->current_variance;
}

/** result = result - p.sample */
template<typename TORUS>
void LweFunctions<TORUS>::SubMulTo(LweSample<TORUS>* result, int p, const LweSample<TORUS>* sample, const LweParams<TORUS>* params){
  const int n = params->n;

  for (int i = 0; i < n; ++i) result->a[i] -= p*sample->a[i];
  result->b -= p*sample->b;
  result->current_variance += (p*p)*sample->current_variance;
}

