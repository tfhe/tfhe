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
//      pour les opérations externes, prévoir int et intPolynomial

//extractions TLwe -> Lwe
EXPORT void tLweExtractKey(LweKey* result, const TLweKey*); //sans doute un param supplémentaire
EXPORT void tLweExtractSample(LweSample* result, const TLweSample* x);

//extraction TGsw -> SemiTGsw
EXPORT void tGswExtractKey(SemiTGswSample* result, const TGswKey* key);
EXPORT void tGswExtractSample(TLweSample* result, const TGswSample* x);


/*//calcule l'arrondi inférieur d'un élément Torus32
  int bar(uint64_t b, uint64_t Nx2){
  uint64_t xx=b*Nx2+(1l<<31);
  return (xx>>32)%Nx2;
  }*/



EXPORT void tLweExtractLweSample(LweSample* result, const TLweSample* x, const LweParams* params,  const TLweParams* rparams) {
    const int N = rparams->N;
    const int k = rparams->k;
    assert(params->n == k*N);
    for (int i=0; i<k; i++) {
	result->a[i*N]=x->a[i].coefsT[0];
	for (int j=1; j<N; j++)
	    result->a[i*N+j]=-x->a[i].coefsT[N-j];
    }
    result->b=x->b->coefsT[0];
}

//leveled functions

//Lwe to SemiRing Bootstrapping
EXPORT void semiRingBootstrap(LweSample* result, const LweBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LweSample* x);



// EXPORT void tGswPolyCombination(LweSample* result, const int* combi, const LweSample* samples, const LweParams* params);

//extractions Ring Lwe -> Lwe
EXPORT void tLweExtractKey(LweKey* result, const TLweKey* key) //sans doute un param supplémentaire
{
    const int N = key->params->N;
    const int k = key->params->k;
    assert(result->params->n == k*N);
    for (int i=0; i<k; i++) {
	for (int j=0; j<N; j++)
	    result->key[i*N+j]=key->key[i].coefs[j];
    }
}



//extraction TGsw -> Gsw
// EXPORT void gswKeyExtract(TLweKey* result, const TGswKey* key); //sans doute un param supplémentaire
// EXPORT void gswSampleExtract(TLweSample* result, const TGswSample* x);

//bootstrapping
// EXPORT void tfhe_bootstrap(LweSample* result, const LweBootstrappingKey* bk, double mu1, double mu0, const LweSample* x);
