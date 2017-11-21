#ifndef Lwe_FUNCTIONS_H
#define Lwe_FUNCTIONS_H

///@file
///@brief This file contains the operations on Lwe samples

#include "tfhe_core.h"
#include "lwekey.h"
#include "lweparams.h"
#include "lwesamples.h"

/**
 * This function generates a random Lwe key for the given parameters.
 * The Lwe key for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
EXPORT void lweKeyGen(LweKey* result);


/**
 * This function encrypts message by using key, with stdev alpha
 * The Lwe sample for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
EXPORT void lweSymEncrypt(LweSample* result, Torus32 message, double alpha, const LweKey* key);
/* 
 * This function encrypts a message by using key and a given noise value
*/
EXPORT void lweSymEncryptWithExternalNoise(LweSample* result, Torus32 message, double noise, double alpha, const LweKey* key);


/**
 * This function computes the phase of sample by using key : phi = b - a.s
 */
EXPORT Torus32 lwePhase(const LweSample* sample, const LweKey* key);


/**
 * This function computes the decryption of sample by using key
 * The constant Msize indicates the message space and is used to approximate the phase
 */
EXPORT Torus32 lweSymDecrypt(const LweSample* sample, const LweKey* key, const int32_t Msize);


//Arithmetic operations on Lwe samples
/** result = (0,0) */
EXPORT void lweClear(LweSample* result, const LweParams* params);

/** result = sample */
EXPORT void lweCopy(LweSample* result, const LweSample* sample, const LweParams* params);

/** result = -sample */
EXPORT void lweNegate(LweSample* result, const LweSample* sample, const LweParams* params);

/** result = (0,mu) */
EXPORT void lweNoiselessTrivial(LweSample* result, Torus32 mu, const LweParams* params);

/** result = result + sample */
EXPORT void lweAddTo(LweSample* result, const LweSample* sample, const LweParams* params);

/** result = result - sample */
EXPORT void lweSubTo(LweSample* result, const LweSample* sample, const LweParams* params);

/** result = result + p.sample */
EXPORT void lweAddMulTo(LweSample* result, int32_t p, const LweSample* sample, const LweParams* params);

/** result = result - p.sample */
EXPORT void lweSubMulTo(LweSample* result, int32_t p, const LweSample* sample, const LweParams* params);

/** 
 * creates a Key Switching Key between the two keys
 */
EXPORT void lweCreateKeySwitchKey_old(LweKeySwitchKey* result, const LweKey* in_key, const LweKey* out_key);
EXPORT void lweCreateKeySwitchKey(LweKeySwitchKey* result, const LweKey* in_key, const LweKey* out_key);

/**
 * applies keySwitching
 */
EXPORT void lweKeySwitch(LweSample* result, const LweKeySwitchKey* ks, const LweSample* sample);

#endif //Lwe_FUNCTIONS_H
