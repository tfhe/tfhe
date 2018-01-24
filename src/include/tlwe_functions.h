#ifndef TLWE_FUNCTIONS_H
#define TLWE_FUNCTIONS_H
///@file
///@brief This file contains the declaration of TLWE related functions

#include "tlwe.h"

template<typename TORUS>
struct TLweFunctions
{
  // Ring
  static void KeyGen(TLweKey<TORUS>* result);
  static void SymEncrypt(TLweSample<TORUS>* result, TorusPolynomial<TORUS>* message, double alpha, const TLweKey<TORUS>* key);
  static void SymEncryptT(TLweSample<TORUS>* result, TORUS message, double alpha, const TLweKey<TORUS>* key);
  static void Phase(TorusPolynomial<TORUS>* phase, const TLweSample<TORUS>* sample, const TLweKey<TORUS>* key);
  static void ApproxPhase(TorusPolynomial<TORUS>* message, const TorusPolynomial<TORUS>* phase, int Msize, int N);
  static void SymDecrypt(TorusPolynomial<TORUS>* result, const TLweSample<TORUS>* sample, const TLweKey<TORUS>* key, int Msize);
  static TORUS SymDecryptT(const TLweSample<TORUS>* sample, const TLweKey<TORUS>* key, int Msize);

  //Arithmetic operations on TLwe samples
  /** result = (0,0) */
  static void Clear(TLweSample<TORUS>* result, const TLweParams<TORUS>* params);
  /** result = sample */
  static void Copy(TLweSample<TORUS>* result, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params);
  /** result = (0,mu) */
  static void NoiselessTrivial(TLweSample<TORUS>* result, const TorusPolynomial<TORUS>* mu, const TLweParams<TORUS>* params);
  /** result = result + sample */
  static void AddTo(TLweSample<TORUS>* result, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params);
  /** result = result - sample */
  static void SubTo(TLweSample<TORUS>* result, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params);
  /** result = result + p.sample */
  static void AddMulTo(TLweSample<TORUS>* result, int p, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params);
  /** result = result - p.sample */
  static void SubMulTo(TLweSample<TORUS>* result, int p, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params);

  /*create an homogeneous tlwe sample*/
  static void SymEncryptZero(TLweSample<TORUS>* result, double alpha, const TLweKey<TORUS>* key);

  /** result = result + p.sample */
  static void AddMulRTo(TLweSample<TORUS>* result, const IntPolynomial* p, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params);

  /** result += (0...,0,x,0,...,0) */
  static void AddTTo(TLweSample<TORUS>* result, const int pos, const TORUS x, const TLweParams<TORUS>* params);

  /** result += p*(0...,0,x,0,...,0) */
  static void AddRTTo(TLweSample<TORUS>* result, const int pos, const IntPolynomial* p, const TORUS x, const TLweParams<TORUS>* params);

  // static void PolyCombination(TLweSample<TORUS>* result, const int* combi, const TLweSample<TORUS>* samples, const TLweParams<TORUS>* params);

  static void MulByXaiMinusOne(TLweSample<TORUS>* result, int ai, const TLweSample<TORUS>* bk, const TLweParams<TORUS>* params);

  static void ExtractLweSampleIndex(LweSample<TORUS>* result, const TLweSample<TORUS>* x, const int index, const LweParams<TORUS>* params,  const TLweParams<TORUS>* rparams);
  static void ExtractLweSample(LweSample<TORUS>* result, const TLweSample<TORUS>* x, const LweParams<TORUS>* params,  const TLweParams<TORUS>* rparams);


  //extractions TLwe -> Lwe
  static void ExtractKey(LweKey<TORUS>* result, const TLweKey<TORUS>*); //sans doute un param supplémentaire
  //static void ExtractSample(LweSample<TORUS>* result, const TLweSample<TORUS>* x);

  //FFT operations
  static void ToFFTConvert(TLweSampleFFT<TORUS>* result, const TLweSample<TORUS>* source, const TLweParams<TORUS>* params);
  static void FromFFTConvert(TLweSample<TORUS>* result, const TLweSampleFFT<TORUS>* source, const TLweParams<TORUS>* params);
  static void FFTClear(TLweSampleFFT<TORUS>* result, const TLweParams<TORUS>* params);
  static void FFTAddMulRTo(TLweSampleFFT<TORUS>* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT<TORUS>* sample, const TLweParams<TORUS>* params);
};

template struct TLweFunctions<Torus32>;
template struct TLweFunctions<Torus64>;


// Ring
template<typename TORUS>
inline void tLweKeyGen(TLweKey<TORUS>* result) {
  TLweFunctions<TORUS>::KeyGen(result);
}
template<typename TORUS>
inline void tLweSymEncrypt(TLweSample<TORUS>* result, TorusPolynomial<TORUS>* message, double alpha, const TLweKey<TORUS>* key) {
  TLweFunctions<TORUS>::SymEncrypt(result, message, alpha, key);
}
template<typename TORUS>
inline void tLweSymEncryptT(TLweSample<TORUS>* result, TORUS message, double alpha, const TLweKey<TORUS>* key) {
  TLweFunctions<TORUS>::SymEncryptT(result, message, alpha, key);
}
template<typename TORUS>
inline void tLwePhase(TorusPolynomial<TORUS>* phase, const TLweSample<TORUS>* sample, const TLweKey<TORUS>* key) {
  TLweFunctions<TORUS>::Phase(phase, sample, key);
}
template<typename TORUS>
inline void tLweApproxPhase(TorusPolynomial<TORUS>* message, const TorusPolynomial<TORUS>* phase, int Msize, int N) {
  TLweFunctions<TORUS>::ApproxPhase(message, phase, Msize, N);
}
template<typename TORUS>
inline void tLweSymDecrypt(TorusPolynomial<TORUS>* result, const TLweSample<TORUS>* sample, const TLweKey<TORUS>* key, int Msize) {
  TLweFunctions<TORUS>::SymDecrypt(result, sample, key, Msize);
}
template<typename TORUS>
inline TORUS tLweSymDecryptT(const TLweSample<TORUS>* sample, const TLweKey<TORUS>* key, int Msize) {
  return TLweFunctions<TORUS>::SymDecryptT(sample, key, Msize);
}

//Arithmetic operations on TLwe samples
/** result = (0,0) */
template<typename TORUS>
inline void tLweClear(TLweSample<TORUS>* result, const TLweParams<TORUS>* params) {
  TLweFunctions<TORUS>::Clear(result, params);
}
/** result = sample */
template<typename TORUS>
inline void tLweCopy(TLweSample<TORUS>* result, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params) {
  TLweFunctions<TORUS>::Copy(result, sample, params);
}
/** result = (0,mu) */
template<typename TORUS>
inline void tLweNoiselessTrivial(TLweSample<TORUS>* result, const TorusPolynomial<TORUS>* mu, const TLweParams<TORUS>* params) {
  TLweFunctions<TORUS>::NoiselessTrivial(result, mu, params);
}
/** result = result + sample */
template<typename TORUS>
inline void tLweAddTo(TLweSample<TORUS>* result, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params) {
  TLweFunctions<TORUS>::AddTo(result, sample, params);
}
/** result = result - sample */
template<typename TORUS>
inline void tLweSubTo(TLweSample<TORUS>* result, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params) {
  TLweFunctions<TORUS>::SubTo(result, sample, params);
}
/** result = result + p.sample */
template<typename TORUS>
inline void tLweAddMulTo(TLweSample<TORUS>* result, int p, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params) {
  TLweFunctions<TORUS>::AddMulTo(result, p, sample, params);
}
/** result = result - p.sample */
template<typename TORUS>
inline void tLweSubMulTo(TLweSample<TORUS>* result, int p, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params) {
  TLweFunctions<TORUS>::SubMulTo(result, p, sample, params);
}

/*create an homogeneous tlwe sample*/
template<typename TORUS>
inline void tLweSymEncryptZero(TLweSample<TORUS>* result, double alpha, const TLweKey<TORUS>* key) {
  TLweFunctions<TORUS>::SymEncryptZero(result, alpha, key);
}


/** result = result + p.sample */
template<typename TORUS>
inline void tLweAddMulRTo(TLweSample<TORUS>* result, const IntPolynomial* p, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params) {
  TLweFunctions<TORUS>::AddMulRTo(result, p, sample, params);
}

/** result += (0...,0,x,0,...,0) */
template<typename TORUS>
inline void tLweAddTTo(TLweSample<TORUS>* result, const int pos, const TORUS x, const TLweParams<TORUS>* params) {
  TLweFunctions<TORUS>::AddTTo(result, pos, x, params);
}

/** result += p*(0...,0,x,0,...,0) */
template<typename TORUS>
inline void tLweAddRTTo(TLweSample<TORUS>* result, const int pos, const IntPolynomial* p, const TORUS x, const TLweParams<TORUS>* params) {
  TLweFunctions<TORUS>::AddRTTo(result, pos, p, x, params);
}

// EXPORT void PolyCombination(TLweSample<TORUS>* result, const int* combi, const TLweSample<TORUS>* samples, const TLweParams<TORUS>* params);

template<typename TORUS>
inline void tLweMulByXaiMinusOne(TLweSample<TORUS>* result, int ai, const TLweSample<TORUS>* bk, const TLweParams<TORUS>* params) {
  TLweFunctions<TORUS>::MulByXaiMinusOne(result, ai, bk, params);
}

template<typename TORUS>
inline void tLweExtractLweSampleIndex(LweSample<TORUS>* result, const TLweSample<TORUS>* x, const int index, const LweParams<TORUS>* params,  const TLweParams<TORUS>* rparams) {
  TLweFunctions<TORUS>::ExtractLweSampleIndex(result, x, index, params, rparams);
}
template<typename TORUS>
inline void tLweExtractLweSample(LweSample<TORUS>* result, const TLweSample<TORUS>* x, const LweParams<TORUS>* params,  const TLweParams<TORUS>* rparams) {
  TLweFunctions<TORUS>::ExtractLweSample(result, x, params, rparams);
}


//extractions TLwe -> Lwe
template<typename TORUS>
inline void tLweExtractKey(LweKey<TORUS>* result, const TLweKey<TORUS>* p) {  //sans doute un param supplémentair
  TLweFunctions<TORUS>::ExtractKey(result, p);
}
//EXPORT void ExtractSample(LweSample<TORUS>* result, const TLweSample<TORUS>* x);

//FFT operations

template<typename TORUS>
inline void tLweToFFTConvert(TLweSampleFFT<TORUS>* result, const TLweSample<TORUS>* source, const TLweParams<TORUS>* params) {
  TLweFunctions<TORUS>::ToFFTConvert(result, source, params);
}
template<typename TORUS>
inline void tLweFromFFTConvert(TLweSample<TORUS>* result, const TLweSampleFFT<TORUS>* source, const TLweParams<TORUS>* params) {
  TLweFunctions<TORUS>::FromFFTConvert(result, source, params);
}
template<typename TORUS>
inline void tLweFFTClear(TLweSampleFFT<TORUS>* result, const TLweParams<TORUS>* params) {
  TLweFunctions<TORUS>::FFTClear(result, params);
}
template<typename TORUS>
inline void tLweFFTAddMulRTo(TLweSampleFFT<TORUS>* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT<TORUS>* sample, const TLweParams<TORUS>* params) {
  TLweFunctions<TORUS>::FFTAddMulRTo(result, p, sample, params);
}

#endif// TLWE_FUNCTIONS_H
