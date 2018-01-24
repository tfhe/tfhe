#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include "tlwe_functions.h"
#include "numeric_functions.h"
#include "polynomials_arithmetic.h"
#include "lagrangehalfc_arithmetic.h"
#include "random_gen.h"

using namespace std;


// TLwe
template<typename TORUS>
void TLweFunctions<TORUS>::KeyGen(TLweKey<TORUS>* result){
  const int N = result->params->N;
  const int k = result->params->k;

  for (int i = 0; i < k; ++i)
    for (int j = 0; j < N; ++j)
      result->key[i].coefs[j] = RandomGen::uniform_bool();
}

/*create an homogeneous tlwe sample*/
template<typename TORUS>
void TLweFunctions<TORUS>::SymEncryptZero(TLweSample<TORUS>* result, double alpha, const TLweKey<TORUS>* key){
  const int N = key->params->N;
  const int k = key->params->k;

  for (int j = 0; j < N; ++j)
    result->b->coefsT[j] = RandomGenTorus<TORUS>::gaussian(0, alpha);

  for (int i = 0; i < k; ++i)
  {
    torusPolynomialUniform(&result->a[i]);
    torusPolynomialAddMulR(result->b, &key->key[i], &result->a[i]);
  }

  result->current_variance = alpha*alpha;
}

template<typename TORUS>
void TLweFunctions<TORUS>::SymEncrypt(TLweSample<TORUS>* result, TorusPolynomial<TORUS>* message, double alpha, const TLweKey<TORUS>* key){
  const int N = key->params->N;

  tLweSymEncryptZero(result, alpha, key);

  for (int j = 0; j < N; ++j)
    result->b->coefsT[j] += message->coefsT[j];
}

/**
 * encrypts a constant message
 */
template<typename TORUS>
void TLweFunctions<TORUS>::SymEncryptT(TLweSample<TORUS>* result, TORUS message, double alpha, const TLweKey<TORUS>* key){
  tLweSymEncryptZero(result, alpha, key);

  result->b->coefsT[0] += message;
}



/**
 * This function computes the phase of sample by using key : phi = b - a.s
 */
template<typename TORUS>
void TLweFunctions<TORUS>::Phase(TorusPolynomial<TORUS>* phase, const TLweSample<TORUS>* sample, const TLweKey<TORUS>* key){
  const int k = key->params->k;

  torusPolynomialCopy(phase, sample->b); // phi = b

  for (int i = 0; i < k; ++i)
    torusPolynomialSubMulR(phase, &key->key[i], &sample->a[i]);
}


/**
 * This function computes the approximation of the phase
 * à revoir, surtout le Msize
 */
template<typename TORUS>
void TLweFunctions<TORUS>::ApproxPhase(TorusPolynomial<TORUS>* message, const TorusPolynomial<TORUS>* phase, int Msize, int N){
  for (int i = 0; i < N; ++i) message->coefsT[i] = TorusUtils<TORUS>::approxPhase(phase->coefsT[i], Msize);
}




template<typename TORUS>
void TLweFunctions<TORUS>::SymDecrypt(TorusPolynomial<TORUS>* result, const TLweSample<TORUS>* sample, const TLweKey<TORUS>* key, int Msize){
  tLwePhase(result, sample, key);
  tLweApproxPhase(result, result, Msize, key->params->N);
}


template<typename TORUS>
TORUS TLweFunctions<TORUS>::SymDecryptT(const TLweSample<TORUS>* sample, const TLweKey<TORUS>* key, int Msize){
  TorusPolynomial<TORUS>* phase = new_TorusPolynomial<TORUS>(key->params->N);

  tLwePhase(phase, sample, key);
  TORUS result = TorusUtils<TORUS>::approxPhase(phase->coefsT[0], Msize);

  delete_TorusPolynomial(phase);
  return result;
}

//Arithmetic operations on TLwe samples
/** result = (0,0) */
template<typename TORUS>
void TLweFunctions<TORUS>::Clear(TLweSample<TORUS>* result, const TLweParams<TORUS>* params){
  const int k = params->k;

  for (int i = 0; i < k; ++i) torusPolynomialClear(&result->a[i]);
  torusPolynomialClear(result->b);
  result->current_variance = 0.;
}

/** result = sample */
template<typename TORUS>
void TLweFunctions<TORUS>::Copy(TLweSample<TORUS>* result, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params){
  const int k = params->k;
  const int N = params->N;

  for (int i = 0; i <= k; ++i)
    for (int j = 0; j < N; ++j)
      result->a[i].coefsT[j] = sample->a[i].coefsT[j];

  result->current_variance = sample->current_variance;
}

/** result = (0,mu) */
template<typename TORUS>
void TLweFunctions<TORUS>::NoiselessTrivial(TLweSample<TORUS>* result, const TorusPolynomial<TORUS>* mu, const TLweParams<TORUS>* params){
  const int k = params->k;

  for (int i = 0; i < k; ++i) torusPolynomialClear(&result->a[i]);
  torusPolynomialCopy(result->b, mu);
  result->current_variance = 0.;
}

// /** result = (0,mu) where mu is constant*/
// template<typename TORUS>
// void TLweFunctions<TORUS>::NoiselessTrivialT(TLweSample<TORUS>* result, const TORUS mu, const TLweParams<TORUS>* params){
//   const int k = params->k;

//   for (int i = 0; i < k; ++i) torusPolynomialClear(&result->a[i]);
//   torusPolynomialClear(result->b);
//   result->b->coefsT[0]=mu;
//   result->current_variance = 0.;
// }

/** result = result + sample */
template<typename TORUS>
void TLweFunctions<TORUS>::AddTo(TLweSample<TORUS>* result, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params){
  const int k = params->k;

  for (int i = 0; i < k; ++i)
  torusPolynomialAddTo(&result->a[i], &sample->a[i]);
  torusPolynomialAddTo(result->b, sample->b);
  result->current_variance += sample->current_variance;
}

/** result = result - sample */
template<typename TORUS>
void TLweFunctions<TORUS>::SubTo(TLweSample<TORUS>* result, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params){
  const int k = params->k;

  for (int i = 0; i < k; ++i)
  torusPolynomialSubTo(&result->a[i], &sample->a[i]);
  torusPolynomialSubTo(result->b, sample->b);
  result->current_variance += sample->current_variance;
}

/** result = result + p.sample */
template<typename TORUS>
void TLweFunctions<TORUS>::AddMulTo(TLweSample<TORUS>* result, int p, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params){
  const int k = params->k;

  for (int i = 0; i < k; ++i)
  torusPolynomialAddMulZTo(&result->a[i], p, &sample->a[i]);
  torusPolynomialAddMulZTo(result->b, p, sample->b);
  result->current_variance += (p*p)*sample->current_variance;
}

/** result = result - p.sample */
template<typename TORUS>
void TLweFunctions<TORUS>::SubMulTo(TLweSample<TORUS>* result, int p, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params){
  const int k = params->k;

  for (int i = 0; i < k; ++i)
  torusPolynomialSubMulZTo(&result->a[i], p, &sample->a[i]);
  torusPolynomialSubMulZTo(result->b, p, sample->b);
  result->current_variance += (p*p)*sample->current_variance;
}


/** result = result + p.sample */
template<typename TORUS>
void TLweFunctions<TORUS>::AddMulRTo(TLweSample<TORUS>* result, const IntPolynomial* p, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params){
  const int k = params->k;

  for (int i = 0; i <= k; ++i)
     torusPolynomialAddMulR(result->a+i, p, sample->a+i);
  result->current_variance += intPolynomialNormSq2(p)*sample->current_variance;
}



//mult externe de X^ai-1 par bki
template<typename TORUS>
void TLweFunctions<TORUS>::MulByXaiMinusOne(TLweSample<TORUS>* result, int ai, const TLweSample<TORUS>* bk, const TLweParams<TORUS>* params){
  const int k=params->k;
  for(int i=0;i<=k;i++)
    torusPolynomialMulByXaiMinusOne(&result->a[i],ai,&bk->a[i]);
}

/** result += (0,x) */
template<typename TORUS>
void TLweFunctions<TORUS>::AddTTo(TLweSample<TORUS>* result, const int pos, const TORUS x, const TLweParams<TORUS>* params){
  result->a[pos].coefsT[0]+=x;
}

/** result += p*(0,x) */
template<typename TORUS>
void TLweFunctions<TORUS>::AddRTTo(TLweSample<TORUS>* result, const int pos, const IntPolynomial* p, const TORUS x, const TLweParams<TORUS>* params){
  const int N = params->N;

  for (int i=0; i<N; i++)
  result->a[pos].coefsT[i]+=p->coefs[i]*x;
}

