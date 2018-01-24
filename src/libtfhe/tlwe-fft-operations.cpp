#ifndef TFHE_TEST_ENVIRONMENT
/* ***************************************************
   TLWE fft operations
 *************************************************** */

#include <random>
#include <cassert>
#include "tfhe_core.h"
#include "tfhe_alloc.h"
#include "numeric_functions.h"
#include "lweparams.h"
#include "lwekey.h"
#include "lwesamples.h"
#include "lwe-functions.h"
#include "tlwe_functions.h"
#include "tgsw_functions.h"
#include "polynomials_arithmetic.h"
#include "lagrangehalfc_arithmetic.h"

using namespace std;
#define INCLUDE_ALL

#else
#undef EXPORT
#define EXPORT
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TLWE_TO_FFT_CONVERT
#undef INCLUDE_TLWE_TO_FFT_CONVERT
// Computes the inverse FFT of the coefficients of the TLWE sample
template<typename TORUS>
void TLweFunctions<TORUS>::ToFFTConvert(TLweSampleFFT<TORUS>* result, const TLweSample<TORUS>* source, const TLweParams<TORUS>* params){
  const int k = params->k;

  for (int i = 0; i <= k; ++i)
  TorusPolynomial_ifft(result->a+i,source->a+i);
  result->current_variance=source->current_variance;
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TLWE_FROM_FFT_CONVERT
#undef INCLUDE_TLWE_FROM_FFT_CONVERT
// Computes the FFT of the coefficients of the TLWEfft sample
template<typename TORUS>
void TLweFunctions<TORUS>::FromFFTConvert(TLweSample<TORUS>* result, const TLweSampleFFT<TORUS>* source, const TLweParams<TORUS>* params){
  const int k = params->k;

  for (int i = 0; i <= k; ++i)
  TorusPolynomial_fft(result->a+i,source->a+i);
  result->current_variance=source->current_variance;
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TLWE_FFT_CLEAR
#undef INCLUDE_TLWE_FFT_CLEAR
//Arithmetic operations on TLwe samples
/** result = (0,0) */
template<typename TORUS>
void TLweFunctions<TORUS>::FFTClear(TLweSampleFFT<TORUS>* result, const TLweParams<TORUS>* params){
  int k = params->k;

  for (int i = 0; i <= k; ++i)
  LagrangeHalfCPolynomialClear(&result->a[i]);
  result->current_variance = 0.;
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TLWE_FFT_ADDMULRTO
#undef INCLUDE_TLWE_FFT_ADDMULRTO
// result = result + p*sample
template<typename TORUS>
void TLweFunctions<TORUS>::FFTAddMulRTo(TLweSampleFFT<TORUS>* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT<TORUS>* sample, const TLweParams<TORUS>* params) {
  const int k = params->k;

  for (int i=0; i<=k; i++)
  LagrangeHalfCPolynomialAddMul(result->a+i,p,sample->a+i);
  //result->current_variance += sample->current_variance;
  //TODO: how to compute the variance correctly?
}
#endif

#undef INCLUDE_ALL
