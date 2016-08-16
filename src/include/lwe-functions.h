#ifndef LWE_FUNCTIONS_H
#define LWE_FUNCTIONS_H

///@file
///@brief This file contains the operations on LWE samples

#include "lwe.h"

/**
 * This function generates a random LWE key for the given parameters.
 * The LWE key for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
EXPORT void lweKeyGen(LWEKey* result);


/**
 * This function encrypts message by using key, with stdev alpha
 * The LWE sample for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
EXPORT void lweSymEncrypt(LWESample* result, Torus32 message, double alpha, const LWEKey* key);


/**
 * This function computes the phase of sample by using key : phi = b - a.s
 */
EXPORT Torus32 lwePhase(const LWESample* sample, const LWEKey* key);


/**
 * This function computes the decryption of sample by using key
 * The constant Msize indicates the message space and is used to approximate the phase
 */
EXPORT Torus32 lweSymDecrypt(const LWESample* sample, const LWEKey* key, const int Msize);


//Arithmetic operations on LWE samples
/** result = (0,0) */
EXPORT void lweClear(LWESample* result, const LWEParams* params);

/** result = (0,mu) */
EXPORT void lweNoiselessTrivial(LWESample* result, Torus32 mu, const LWEParams* params);

/** result = result + sample */
EXPORT void lweAddTo(LWESample* result, const LWESample* sample, const LWEParams* params);

/** result = result - sample */
EXPORT void lweSubTo(LWESample* result, const LWESample* sample, const LWEParams* params);

/** result = result + p.sample */
EXPORT void lweAddMulTo(LWESample* result, int p, const LWESample* sample, const LWEParams* params);

/** result = result - p.sample */
EXPORT void lweSubMulTo(LWESample* result, int p, const LWESample* sample, const LWEParams* params);

/** 
 * creates a Key Switching Key between the two keys
 */
EXPORT void lweCreateKeySwitchKey(LWEKeySwitchKey* result, const LWEKey* in_key, const LWEKey* out_key);


/**
 * applies keySwitching
 */
EXPORT void lweKeySwitch(LWESample* result, const LWEKeySwitchKey* ks, const LWESample* sample);

#endif //LWE_FUNCTIONS_H
