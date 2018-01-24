#ifndef LWE_FUNCTIONS_H
#define LWE_FUNCTIONS_H

///@file
///@brief This file contains the operations on Lwe samples

#include "lwekey.h"
#include "lweparams.h"
#include "lwesamples.h"

template<typename TORUS>
struct LweFunctions
{
private:
  static void CreateKeySwitchKey_fromArray(LweSample<TORUS>*** result,
    const LweKey<TORUS>* out_key, const double out_alpha,
    const int* in_key, const int n, const int t, const int basebit);

  static void KeySwitchTranslate_fromArray(LweSample<TORUS>* result,
    const LweSample<TORUS>*** ks, const LweParams<TORUS>* params,
    const TORUS* ai,
    const int n, const int t, const int basebit);

public:
  /**
   * This function generates a random Lwe key for the given parameters.
   * The Lwe key for the result must be allocated and initialized
   * (this means that the parameters are already in the result)
   */
  static void KeyGen(LweKey<TORUS>* result);

  /**
   * This function encrypts message by using key, with stdev alpha
   * The Lwe sample for the result must be allocated and initialized
   * (this means that the parameters are already in the result)
   */
  static void SymEncrypt(LweSample<TORUS>* result, TORUS message, double alpha, const LweKey<TORUS>* key);

  /**
   * This function computes the phase of sample by using key : phi = b - a.s
   */
  static TORUS Phase(const LweSample<TORUS>* sample, const LweKey<TORUS>* key);

  /**
   * This function computes the decryption of sample by using key
   * The constant Msize indicates the message space and is used to approximate the phase
   */
  static TORUS SymDecrypt(const LweSample<TORUS>* sample, const LweKey<TORUS>* key, const int Msize);

  //Arithmetic operations on Lwe samples
  /** result = (0,0) */
  static void Clear(LweSample<TORUS>* result, const LweParams<TORUS>* params);

  /** result = sample */
  static void Copy(LweSample<TORUS>* result, const LweSample<TORUS>* sample, const LweParams<TORUS>* params);

  /** result = -sample */
  static void Negate(LweSample<TORUS>* result, const LweSample<TORUS>* sample, const LweParams<TORUS>* params);

  /** result = (0,mu) */
  static void NoiselessTrivial(LweSample<TORUS>* result, TORUS mu, const LweParams<TORUS>* params);

  /** result = result + sample */
  static void AddTo(LweSample<TORUS>* result, const LweSample<TORUS>* sample, const LweParams<TORUS>* params);

  /** result = result - sample */
  static void SubTo(LweSample<TORUS>* result, const LweSample<TORUS>* sample, const LweParams<TORUS>* params);

  /** result = result + p.sample */
  static void AddMulTo(LweSample<TORUS>* result, int p, const LweSample<TORUS>* sample, const LweParams<TORUS>* params);

  /** result = result - p.sample */
  static void SubMulTo(LweSample<TORUS>* result, int p, const LweSample<TORUS>* sample, const LweParams<TORUS>* params);

  /**
   * creates a Key Switching Key between the two keys
   */
  static void CreateKeySwitchKey(LweKeySwitchKey<TORUS>* result, const LweKey<TORUS>* in_key, const LweKey<TORUS>* out_key);

  /**
   * applies keySwitching
   */
  static void KeySwitch(LweSample<TORUS>* result, const LweKeySwitchKey<TORUS>* ks, const LweSample<TORUS>* sample);
};

template struct LweFunctions<Torus32>;
template struct LweFunctions<Torus64>;

/**
 * This function generates a random Lwe key for the given parameters.
 * The Lwe key for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
template<typename TORUS>
inline void lweKeyGen(LweKey<TORUS>* result) {
  LweFunctions<TORUS>::KeyGen(result);
}


/**
 * This function encrypts message by using key, with stdev alpha
 * The Lwe sample for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
template<typename TORUS>
inline void lweSymEncrypt(LweSample<TORUS>* result, TORUS message, double alpha, const LweKey<TORUS>* key) {
  LweFunctions<TORUS>::SymEncrypt(result, message, alpha, key);
}


/**
 * This function computes the phase of sample by using key : phi = b - a.s
 */
template<typename TORUS>
inline TORUS lwePhase(const LweSample<TORUS>* sample, const LweKey<TORUS>* key) {
  return LweFunctions<TORUS>::Phase(sample, key);
}


/**
 * This function computes the decryption of sample by using key
 * The constant Msize indicates the message space and is used to approximate the phase
 */
template<typename TORUS>
inline TORUS lweSymDecrypt(const LweSample<TORUS>* sample, const LweKey<TORUS>* key, const int Msize) {
  return LweFunctions<TORUS>::SymDecrypt(sample, key, Msize);
}


//Arithmetic operations on Lwe samples
/** result = (0,0) */
template<typename TORUS>
inline void lweClear(LweSample<TORUS>* result, const LweParams<TORUS>* params) {
  LweFunctions<TORUS>::Clear(result, params);
}

/** result = sample */
template<typename TORUS>
inline void lweCopy(LweSample<TORUS>* result, const LweSample<TORUS>* sample, const LweParams<TORUS>* params) {
  LweFunctions<TORUS>::Copy(result, sample, params);
}

/** result = -sample */
template<typename TORUS>
inline void lweNegate(LweSample<TORUS>* result, const LweSample<TORUS>* sample, const LweParams<TORUS>* params) {
  LweFunctions<TORUS>::Negate(result, sample, params);
}

/** result = (0,mu) */
template<typename TORUS>
inline void lweNoiselessTrivial(LweSample<TORUS>* result, TORUS mu, const LweParams<TORUS>* params) {
  LweFunctions<TORUS>::NoiselessTrivial(result, mu, params);
}

/** result = result + sample */
template<typename TORUS>
inline void lweAddTo(LweSample<TORUS>* result, const LweSample<TORUS>* sample, const LweParams<TORUS>* params) {
  LweFunctions<TORUS>::AddTo(result, sample, params);
}

/** result = result - sample */
template<typename TORUS>
inline void lweSubTo(LweSample<TORUS>* result, const LweSample<TORUS>* sample, const LweParams<TORUS>* params) {
  LweFunctions<TORUS>::SubTo(result, sample, params);
}

/** result = result + p.sample */
template<typename TORUS>
inline void lweAddMulTo(LweSample<TORUS>* result, int p, const LweSample<TORUS>* sample, const LweParams<TORUS>* params) {
  LweFunctions<TORUS>::AddMulTo(result, p, sample, params);
}

/** result = result - p.sample */
template<typename TORUS>
inline void lweSubMulTo(LweSample<TORUS>* result, int p, const LweSample<TORUS>* sample, const LweParams<TORUS>* params) {
  LweFunctions<TORUS>::SubMulTo(result, p, sample, params);
}

/**
 * creates a Key Switching Key between the two keys
 */
template<typename TORUS>
inline void lweCreateKeySwitchKey(LweKeySwitchKey<TORUS>* result, const LweKey<TORUS>* in_key, const LweKey<TORUS>* out_key) {
  LweFunctions<TORUS>::CreateKeySwitchKey(result, in_key, out_key);
}

/**
 * applies keySwitching
 */
template<typename TORUS>
inline void lweKeySwitch(LweSample<TORUS>* result, const LweKeySwitchKey<TORUS>* ks, const LweSample<TORUS>* sample) {
  LweFunctions<TORUS>::KeySwitch(result, ks, sample);
}

#endif //LWE_FUNCTIONS_H
