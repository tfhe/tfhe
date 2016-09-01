#ifndef TLWE_FUNCTIONS_H
#define TLWE_FUNCTIONS_H

///@file
///@brief This file contains the declaration of TLWE related functions

// Ring
EXPORT void tLweKeyGen(TLweKey* result);
EXPORT void tLweSymEncrypt(TLweSample* result, TorusPolynomial* message, double alpha, const TLweKey* key);
EXPORT void tLweSymEncryptT(TLweSample* result, Torus32 message, double alpha, const TLweKey* key);
EXPORT void tLwePhase(TorusPolynomial* phase, const TLweSample* sample, const TLweKey* key);
EXPORT void tLweApproxPhase(TorusPolynomial* message, const TorusPolynomial* phase, int Msize, int N);
EXPORT void tLweSymDecrypt(TorusPolynomial* result, const TLweSample* sample, const TLweKey* key, int Msize);
EXPORT Torus32 tLweSymDecryptT(const TLweSample* sample, const TLweKey* key, int Msize);

//Arithmetic operations on TLwe samples
/** result = (0,0) */
EXPORT void tLweClear(TLweSample* result, const TLweParams* params);
/** result = sample */
EXPORT void tLweCopy(TLweSample* result, const TLweSample* sample, const TLweParams* params);
/** result = (0,mu) */
EXPORT void tLweNoiselessTrivial(TLweSample* result, const TorusPolynomial* mu, const TLweParams* params);
/** result = result + sample */
EXPORT void tLweAddTo(TLweSample* result, const TLweSample* sample, const TLweParams* params);
/** result = result - sample */
EXPORT void tLweSubTo(TLweSample* result, const TLweSample* sample, const TLweParams* params);
/** result = result + p.sample */
EXPORT void tLweAddMulTo(TLweSample* result, int p, const TLweSample* sample, const TLweParams* params);
/** result = result - p.sample */
EXPORT void tLweSubMulTo(TLweSample* result, int p, const TLweSample* sample, const TLweParams* params);

// EXPORT void tLwePolyCombination(TLweSample* result, const int* combi, const TLweSample* samples, const TLweParams* params);

EXPORT void tLweMulByXaiMinusOne(TLweSample* result, int ai, const TLweSample* bk, const TLweParams* params);

EXPORT void tLweExtractLweSample(LweSample* result, const TLweSample* x, const LweParams* params,  const TLweParams* rparams);

//extractions TLwe -> Lwe
EXPORT void tLweExtractKey(LweKey* result, const TLweKey*); //sans doute un param supplémentaire
//EXPORT void tLweExtractSample(LweSample* result, const TLweSample* x);

//FFT operations

EXPORT void tLweToFFTConvert(TLweSampleFFT* result, const TLweSample* source, const TLweParams* params);
EXPORT void tLweFromFFTConvert(TLweSample* result, const TLweSampleFFT* source, const TLweParams* params);
EXPORT void tLweFFTClear(TLweSampleFFT* result, const TLweParams* params);
EXPORT void tLweFFTNoiselessTrivial(TLweSampleFFT* result, const TorusPolynomial* mu, const TLweParams* params);
EXPORT void tLweFFTNoiselessTrivialT(TLweSampleFFT* result, const Torus32 mu, const TLweParams* params);
EXPORT void tLweFFTAddTo(TLweSampleFFT* result, const TLweSampleFFT* sample, const TLweParams* params);
EXPORT void tLweFFTSubTo(TLweSampleFFT* result, const TLweSampleFFT* sample, const TLweParams* params);
EXPORT void tLweFFTAddMulZTo(TLweSampleFFT* result, int p, const TLweSampleFFT* sample, const TLweParams* params);
EXPORT void tLweFFTSubMulZTo(TLweSampleFFT* result, int p, const TLweSampleFFT* sample, const TLweParams* params);
EXPORT void tLweFFTAddMulRTo(TLweSampleFFT* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT* sample, const TLweParams* params);
EXPORT void tLweFFTMulR(TLweSampleFFT* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT* sample, const TLweParams* params);
EXPORT void tLweFFTSubMulRTo(TLweSampleFFT* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT* sample, const TLweParams* params);

#endif// TLWE_FUNCTIONS_H
