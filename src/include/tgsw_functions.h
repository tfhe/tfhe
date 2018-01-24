#ifndef TGSW_FUNCTIONS_H
#define TGSW_FUNCTIONS_H

///@file
///@brief This file contains the declaration of TGSW related functions

#include "tfhe_core.h"
#include "tgsw.h"
#include "tlwe.h"
#include "lwebootstrappingkey.h"

template<typename TORUS>
struct TGswFunctions
{
  // Gsw
  static void KeyGen(TGswKey<TORUS>* result);
  static void SymEncrypt(TGswSample<TORUS>* result, const IntPolynomial* message, double alpha, const TGswKey<TORUS>* key);
  static void SymEncryptInt(TGswSample<TORUS>* result, const int message, double alpha, const TGswKey<TORUS>* key);
  static void SymDecrypt(IntPolynomial* result, const TGswSample<TORUS>* sample, const TGswKey<TORUS>* key, const int Msize);
  static int SymDecryptInt(const TGswSample<TORUS>* sample, const TGswKey<TORUS>* key);
  //do we really decrypt Gsw samples?

  // support Functions for TGsw
  // Result = 0
  static void Clear(TGswSample<TORUS>* result, const TGswParams<TORUS>* params);
  // Result += H
  static void AddH(TGswSample<TORUS>* result, const TGswParams<TORUS>* params);
  // Result += mu*H
  static void AddMuH(TGswSample<TORUS>* result, const IntPolynomial* message, const TGswParams<TORUS>* params);
  // Result += mu*H, mu integer
  static void AddMuIntH(TGswSample<TORUS>* result, const int message, const TGswParams<TORUS>* params);
  // Result = tGsw(0)
  static void EncryptZero(TGswSample<TORUS>* result, double alpha, const TGswKey<TORUS>* key);

  //fonction de decomposition
  static void TLweDecompH(IntPolynomial* result, const TLweSample<TORUS>* sample, const TGswParams<TORUS>* params);
  static void TorusPolynomialDecompH(IntPolynomial* result, const TorusPolynomial<TORUS>* sample, const TGswParams<TORUS>* params);

  //TODO: Ilaria.Theoreme3.5
  static void ExternProduct(TLweSample<TORUS>* result, const TGswSample<TORUS>* a, const TLweSample<TORUS>* b, const TGswParams<TORUS>* params);

  // result=result+ (X^ai-1)*bi (ligne 5 de l'algo)
  static void MulByXaiMinusOne(TGswSample<TORUS>* result, int ai, const TGswSample<TORUS>* bk, const TGswParams<TORUS>* params);

  //ligne 5 algo,mult externe
  static void ExternMulToTLwe(TLweSample<TORUS>* accum, const TGswSample<TORUS>* sample,const TGswParams<TORUS>* params);

  /** result = (0,mu) */
  static void NoiselessTrivial(TGswSample<TORUS>* result, const IntPolynomial* mu, const TGswParams<TORUS>* params);

  /** result = result + sample */
  static void AddTo(TGswSample<TORUS>* result, const TGswSample<TORUS>* sample, const TGswParams<TORUS>* params);

  static void ToFFTConvert(TGswSampleFFT<TORUS>* result, const TGswSample<TORUS>* source, const TGswParams<TORUS>* params);
  static void FromFFTConvert(TGswSample<TORUS>* result, const TGswSampleFFT<TORUS>* source, const TGswParams<TORUS>* params);
  static void FFTAddH(TGswSampleFFT<TORUS>* result, const TGswParams<TORUS>* params);
  static void FFTClear(TGswSampleFFT<TORUS>* result, const TGswParams<TORUS>* params);
  static void FFTExternMulToTLwe(TLweSample<TORUS>* accum, const TGswSampleFFT<TORUS>* gsw, const TGswParams<TORUS>* params);
  static void FFTMulByXaiMinusOne(TGswSampleFFT<TORUS>* result, const int ai, const TGswSampleFFT<TORUS>* bki, const TGswParams<TORUS>* params);
};

template struct TGswFunctions<Torus32>;
template struct TGswFunctions<Torus64>;

// Gsw
template<typename TORUS>
inline void tGswKeyGen(TGswKey<TORUS>* result) {
  TGswFunctions<TORUS>::KeyGen(result);
}
template<typename TORUS>
inline void tGswSymEncrypt(TGswSample<TORUS>* result, const IntPolynomial* message, double alpha, const TGswKey<TORUS>* key) {
  TGswFunctions<TORUS>::SymEncrypt(result, message, alpha, key);
}
template<typename TORUS>
inline void tGswSymEncryptInt(TGswSample<TORUS>* result, const int message, double alpha, const TGswKey<TORUS>* key) {
  TGswFunctions<TORUS>::SymEncryptInt(result, message, alpha, key);
}
template<typename TORUS>
inline void tGswSymDecrypt(IntPolynomial* result, const TGswSample<TORUS>* sample, const TGswKey<TORUS>* key, const int Msize) {
  TGswFunctions<TORUS>::SymDecrypt(result, sample, key, Msize);
}
template<typename TORUS>
inline int tGswSymDecryptInt(const TGswSample<TORUS>* sample, const TGswKey<TORUS>* key) {
  return TGswFunctions<TORUS>::SymDecryptInt(sample, key);
}
//do we really decrypt Gsw samples?

// support Functions for TGsw
// Result = 0
template<typename TORUS>
inline void tGswClear(TGswSample<TORUS>* result, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::Clear(result, params);
}
// Result += H
template<typename TORUS>
inline void tGswAddH(TGswSample<TORUS>* result, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::AddH(result, params);
}
// Result += mu*H
template<typename TORUS>
inline void tGswAddMuH(TGswSample<TORUS>* result, const IntPolynomial* message, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::AddMuH(result, message, params);
}
// Result += mu*H, mu integer
template<typename TORUS>
inline void tGswAddMuIntH(TGswSample<TORUS>* result, const int message, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::AddMuIntH(result, message, params);
}
// Result = tGsw(0)
template<typename TORUS>
inline void tGswEncryptZero(TGswSample<TORUS>* result, double alpha, const TGswKey<TORUS>* key) {
  TGswFunctions<TORUS>::EncryptZero(result, alpha, key);
}

//fonction de decomposition
template<typename TORUS>
inline void tGswTLweDecompH(IntPolynomial* result, const TLweSample<TORUS>* sample, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::TLweDecompH(result, sample, params);
}

template<typename TORUS>
inline void tGswTorusPolynomialDecompH(IntPolynomial* result, const TorusPolynomial<TORUS>* sample, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::TorusPolynomialDecompH(result, sample, params);
}

//TODO: Ilaria.Theoreme3.5
template<typename TORUS>
inline void tGswExternProduct(TLweSample<TORUS>* result, const TGswSample<TORUS>* a, const TLweSample<TORUS>* b, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::ExternProduct(result, a, b, params);
}

// result=result+ (X^ai-1)*bi (ligne 5 de l'algo)
template<typename TORUS>
inline void tGswMulByXaiMinusOne(TGswSample<TORUS>* result, int ai, const TGswSample<TORUS>* bk, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::MulByXaiMinusOne(result, ai, bk, params);
}

//ligne 5 algo,mult externe
template<typename TORUS>
inline void tGswExternMulToTLwe(TLweSample<TORUS>* accum, const TGswSample<TORUS>* sample,const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::ExternMulToTLwe(accum, sample, params);
}

/** result = (0,mu) */
template<typename TORUS>
inline void tGswNoiselessTrivial(TGswSample<TORUS>* result, const IntPolynomial* mu, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::NoiselessTrivial(result, mu, params);
}

/** result = result + sample */
template<typename TORUS>
inline void tGswAddTo(TGswSample<TORUS>* result, const TGswSample<TORUS>* sample, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::AddTo(result, sample, params);
}


template<typename TORUS>
inline void tGswToFFTConvert(TGswSampleFFT<TORUS>* result, const TGswSample<TORUS>* source, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::ToFFTConvert(result, source, params);
}
template<typename TORUS>
inline void tGswFromFFTConvert(TGswSample<TORUS>* result, const TGswSampleFFT<TORUS>* source, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::FromFFTConvert(result, source, params);
}
template<typename TORUS>
inline void tGswFFTAddH(TGswSampleFFT<TORUS>* result, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::FFTAddH(result, params);
}
template<typename TORUS>
inline void tGswFFTClear(TGswSampleFFT<TORUS>* result, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::FFTClear(result, params);
}
template<typename TORUS>
inline void tGswFFTExternMulToTLwe(TLweSample<TORUS>* accum, const TGswSampleFFT<TORUS>* gsw, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::FFTExternMulToTLwe(accum, gsw, params);
}
template<typename TORUS>
inline void tGswFFTMulByXaiMinusOne(TGswSampleFFT<TORUS>* result, const int ai, const TGswSampleFFT<TORUS>* bki, const TGswParams<TORUS>* params) {
  TGswFunctions<TORUS>::FFTMulByXaiMinusOne(result, ai, bki, params);
}


/** result = result - sample */
//EXPORT void tGswSubTo(TLweSample<TORUS>* result, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params);
/** result = result + p.sample */
//EXPORT void tGswAddMulTo(TLweSample<TORUS>* result, int p, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params);
/** result = result - p.sample */
//EXPORT void tGswSubMulTo(TLweSample<TORUS>* result, int p, const TLweSample<TORUS>* sample, const TLweParams<TORUS>* params);


#endif //TGSW_FUNCTIONS_H
