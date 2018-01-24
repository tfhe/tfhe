#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include "tfhe_core.h"
#include "numeric_functions.h"
#include "lweparams.h"
#include "lwekey.h"
#include "lwesamples.h"
#include "lwe-functions.h"
#include "tlwe_functions.h"
#include "tgsw_functions.h"
#include "lwekeyswitch.h"
#include "lwebootstrappingkey.h"
#include "polynomials_arithmetic.h"
#include "lagrangehalfc_arithmetic.h"

using namespace std;


// #ifndef NDEBUG
// const TLweKey<TORUS>* debug_accum_key;
// const LweKey<TORUS>* debug_extract_key;
// const LweKey<TORUS>* debug_in_key;
// #endif



//TODO: mettre les mêmes fonctions arithmétiques que pour Lwe
//    pour les opérations externes, prévoir int et intPolynomial


template<typename TORUS>
void TLweFunctions<TORUS>::ExtractLweSampleIndex(LweSample<TORUS>* result, const TLweSample<TORUS>* x, const int index, const LweParams<TORUS>* params,  const TLweParams<TORUS>* rparams) {
  const int N = rparams->N;
  const int k = rparams->k;
  assert(params->n == k*N);

  for (int i=0; i<k; i++) {
    for (int j=0; j<=index; j++)
    result->a[i*N+j] = x->a[i].coefsT[index-j];
    for (int j=index+1; j<N; j++)
    result->a[i*N+j] = -x->a[i].coefsT[N+index-j];
  }
  result->b = x->b->coefsT[index];
}

template<typename TORUS>
void TLweFunctions<TORUS>::ExtractLweSample(LweSample<TORUS>* result, const TLweSample<TORUS>* x, const LweParams<TORUS>* params,  const TLweParams<TORUS>* rparams) {
  ExtractLweSampleIndex(result, x, 0, params, rparams);
}


//extractions Ring Lwe -> Lwe
template<typename TORUS>
void TLweFunctions<TORUS>::ExtractKey(LweKey<TORUS>* result, const TLweKey<TORUS>* key) //sans doute un param supplémentaire
{
  const int N = key->params->N;
  const int k = key->params->k;
  assert(result->params->n == k*N);
  for (int i=0; i<k; i++) {
    for (int j=0; j<N; j++)
      result->key[i*N+j]=key->key[i].coefs[j];
  }
}

