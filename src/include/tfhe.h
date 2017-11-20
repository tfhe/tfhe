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

#include "tfhe_gate_bootstrapping_functions.h"

#include "tfhe_io.h"

///////////////////////////////////////////////////
//  TFHE bootstrapping internal functions
//////////////////////////////////////////////////


/** sets the seed of the random number generator to the given values */
EXPORT void tfhe_random_generator_setSeed(uint32_t* values, int32_t size);

EXPORT void tfhe_blindRotate(TLweSample* accum, const TGswSample* bk, const int32_t* bara, const int32_t n, const TGswParams* bk_params);
EXPORT void tfhe_blindRotateAndExtract(LweSample* result, const TorusPolynomial* v, const TGswSample* bk, const int32_t barb, const int32_t* bara, const int32_t n, const TGswParams* bk_params);
EXPORT void tfhe_bootstrap_woKS(LweSample* result, const LweBootstrappingKey* bk, Torus32 mu, const LweSample* x);
EXPORT void tfhe_bootstrap(LweSample* result, const LweBootstrappingKey* bk, Torus32 mu, const LweSample* x);
EXPORT void tfhe_createLweBootstrappingKey(LweBootstrappingKey* bk, const LweKey* key_in, const TGswKey* rgsw_key);

EXPORT void tfhe_blindRotate_FFT(TLweSample* accum, const TGswSampleFFT* bk, const int32_t* bara, const int32_t n, const TGswParams* bk_params);
EXPORT void tfhe_blindRotateAndExtract_FFT(LweSample* result, const TorusPolynomial* v, const TGswSampleFFT* bk, const int32_t barb, const int32_t* bara, const int32_t n, const TGswParams* bk_params);
EXPORT void tfhe_bootstrap_woKS_FFT(LweSample* result, const LweBootstrappingKeyFFT* bk, Torus32 mu, const LweSample* x);
EXPORT void tfhe_bootstrap_FFT(LweSample* result, const LweBootstrappingKeyFFT* bk, Torus32 mu, const LweSample* x);


#endif //TFHE_H
