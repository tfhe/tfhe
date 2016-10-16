#ifndef TFHE_H
#define TFHE_H

///@file
///@brief This file declares almost everything

#include "tfhe_core.h"

#include "numeric_functions.h"

#include "polynomials_arithmetic.h"
#include "lagrangehalfc_arithmetic.h"

#include "lwe-functions.h"

#include "tlwe_functions.h"

#include "tgsw_functions.h"

//extraction TGsw -> SemiTGsw
EXPORT void tGswExtractKey(SemiTGswSample* result, const TGswKey* key);
EXPORT void tGswExtractSample(TLweSample* result, const TGswSample* x);

//Lwe to Lwe Single gate bootstrapping
EXPORT void lweToLweBootstrap(LweSample* result, const LweBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LweSample* x);


//these functions call the bootstrapping, assuming that the message space is {0,1/4} 
// TODO syncronize names with new files
EXPORT void lweNand(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b);
EXPORT void lweOr(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b);
EXPORT void lweAnd(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b);
EXPORT void lweXor(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b);
// mux(a,b,c) = a?b:c = a et b + not(a) et c 
EXPORT void lweMux(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b, const LweSample* c);
EXPORT void lweNot(LweSample* result, LweSample* a);


//leveled functions

//Lwe to SemiRing Bootstrapping
EXPORT void semiRingBootstrap(LweSample* result, const LweBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LweSample* x);


#endif //TFHE_H
