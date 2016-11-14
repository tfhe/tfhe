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

#include "lwekeyswitch.h"

#include "lwebootstrappingkey.h"

//extraction TGsw -> SemiTGsw
EXPORT void tGswExtractKey(SemiTGswSample* result, const TGswKey* key);
EXPORT void tGswExtractSample(TLweSample* result, const TGswSample* x);

//Lwe to Lwe Single gate bootstrapping
EXPORT void lweToLweBootstrap(LweSample* result, const LweBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LweSample* x);


//these functions call the bootstrapping, assuming that the message space is {0,1/4} 
EXPORT void bootsNAND(LweSample* result, const LweSample* c1, const LweSample* c2, const LweBootstrappingKey* BK);
EXPORT void bootsOR(LweSample* result, const LweSample* c1, const LweSample* c2, const LweBootstrappingKey* BK);
EXPORT void bootsAND(LweSample* result, const LweSample* c1, const LweSample* c2, const LweBootstrappingKey* BK);
EXPORT void bootsXOR(LweSample* result, const LweSample* c1, const LweSample* c2, const LweBootstrappingKey* BK);
//the homNOT gate doesn't need to be bootstrapped
EXPORT void homNOT(LweSample* result, const LweSample* c1, const LweParams* params);

// TODO syncronize names with new files
// mux(a,b,c) = a?b:c = a et b + not(a) et c 
EXPORT void lweMux(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b, const LweSample* c);



//leveled functions

//Lwe to SemiRing Bootstrapping
EXPORT void semiRingBootstrap(LweSample* result, const LweBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LweSample* x);


#endif //TFHE_H
