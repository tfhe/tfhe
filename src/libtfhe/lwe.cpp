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


#ifndef NDEBUG
const TLweKey* debug_accum_key;
const LweKey* debug_extract_key;
const LweKey* debug_in_key;
#endif



//TODO: mettre les mêmes fonctions arithmétiques que pour Lwe
//      pour les opérations externes, prévoir int32_t et intPolynomial


/*//calcule l'arrondi inférieur d'un élément Torus32
  int32_t bar(uint64_t b, uint64_t Nx2){
  uint64_t xx=b*Nx2+(1l<<31);
  return (xx>>32)%Nx2;
  }*/



EXPORT void tLweExtractLweSampleIndex(LweSample* result, const TLweSample* x, const int32_t index, const LweParams* params,  const TLweParams* rparams) {
    const int32_t N = rparams->N;
    const int32_t k = rparams->k;
    assert(params->n == k*N);

    for (int32_t i=0; i<k; i++) {
      for (int32_t j=0; j<=index; j++)
        result->a[i*N+j] = x->a[i].coefsT[index-j];
      for (int32_t j=index+1; j<N; j++)
        result->a[i*N+j] = -x->a[i].coefsT[N+index-j];
    }
    result->b = x->b->coefsT[index];
}




EXPORT void tLweExtractLweSample(LweSample* result, const TLweSample* x, const LweParams* params,  const TLweParams* rparams) {
    tLweExtractLweSampleIndex(result, x, 0, params, rparams);
}



//extractions Ring Lwe -> Lwe
EXPORT void tLweExtractKey(LweKey* result, const TLweKey* key) //sans doute un param supplémentaire
{
    const int32_t N = key->params->N;
    const int32_t k = key->params->k;
    assert(result->params->n == k*N);
    for (int32_t i=0; i<k; i++) {
	for (int32_t j=0; j<N; j++)
	    result->key[i*N+j]=key->key[i].coefs[j];
    }
}

