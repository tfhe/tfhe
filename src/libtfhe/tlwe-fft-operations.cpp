/* ***************************************************
TLWE fft operations
*************************************************** */

#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include <ccomplex>
#include "tfhe_core.h"
#include "numeric_functions.h"
#include "lweparams.h"
#include "lwekey.h"
#include "lwesamples.h"
#include "lwe-functions.h"
#include "tlwe_functions.h"
#include "tgsw_functions.h"
#include "polynomials_arithmetic.h"
#include "lagrangehalfc_arithmetic.h"
#include "lwebootstrappingkey.h"

using namespace std;

#ifndef NDEBUG
extern const TLweKey* debug_accum_key;
extern const TLweKey* debug_extract_key;
extern const TLweKey* debug_in_key;
#endif




// Computes the inverse FFT of the coefficients of the TLWE sample
EXPORT void tLweToFFTConvert(TLweSampleFFT* result, const TLweSample* source, const TLweParams* params){
    const int k = params->k;
    
    for (int i = 0; i <= k; ++i)
    TorusPolynomial_ifft(result->a+i,source->a+i);
}

// Computes the FFT of the coefficients of the TLWEfft sample
EXPORT void tLweFromFFTConvert(TLweSample* result, const TLweSampleFFT* source, const TLweParams* params){
    const int k = params->k;
    
    for (int i = 0; i <= k; ++i)
    TorusPolynomial_fft(result->a+i,source->a+i);
}



//Arithmetic operations on TLwe samples
/** result = (0,0) */
EXPORT void tLweFFTClear(TLweSampleFFT* result, const TLweParams* params){
    int k = params->k;

    for (int i = 0; i <= k; ++i) 
        LagrangeHalfCPolynomialClear(&result->a[i]);
    result->current_variance = 0.;
}

/** result = (0,mu) where mu is a torus polynomial */
EXPORT void tLweFFTNoiselessTrivial(TLweSampleFFT* result, const TorusPolynomial* mu, const TLweParams* params){
    int k = params->k;

    for (int i = 0; i < k; ++i) 
        LagrangeHalfCPolynomialClear(&result->a[i]);
    TorusPolynomial_ifft(result->b, mu);
    result->current_variance = 0.;
}

/** result = (0,mu) where mu is constant*/
EXPORT void tLweFFTNoiselessTrivialT(TLweSampleFFT* result, const Torus32 mu, const TLweParams* params){
    const int k = params->k;
    
    for (int i = 0; i < k; ++i) 
        LagrangeHalfCPolynomialClear(&result->a[i]);
    LagrangeHalfCPolynomialSetTorusConstant(result->b,mu);
    result->current_variance = 0.;
}

/** result = result + sample */
EXPORT void tLweFFTAddTo(TLweSampleFFT* result, const TLweSampleFFT* sample, const TLweParams* params);
//Let's postpone this to make sure we actually need it
//{
//    int k = params->k;
//
//    for (int i = 0; i < k; ++i) 
//  AddToLagrangeHalfCPolynomial(&result->a[i], &sample->a[i]);
//    AddToLagrangeHalfCPolynomial(result->b, sample->b);
//    result->current_variance += sample->current_variance; //à revoir//OK si c'est la variance
//}

/** result = result - sample */
EXPORT void tLweFFTSubTo(TLweSampleFFT* result, const TLweSampleFFT* sample, const TLweParams* params);

/** result = result + p.sample */
EXPORT void tLweFFTAddMulZTo(TLweSampleFFT* result, int p, const TLweSampleFFT* sample, const TLweParams* params);
//Let's postpone this to make sure we actually need it
//{
//    int k = params->k;
//
//    for (int i = 0; i < k; ++i) 
//  torusPolynomialAddMulZTo(&result->a[i], p, &sample->a[i]);
//    torusPolynomialAddMulZTo(result->b, p, sample->b);
//    result->current_variance += (p*p)*sample->current_variance;
//}

/** result = result - p.sample */
EXPORT void tLweFFTSubMulZTo(TLweSampleFFT* result, int p, const TLweSampleFFT* sample, const TLweParams* params);



// result = result + p*sample
EXPORT void tLweFFTAddMulRTo(TLweSampleFFT* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT* sample, const TLweParams* params) {
    const int k = params->k;
    
    for (int i=0; i<=k; i++)
        LagrangeHalfCPolynomialAddMul(result->a+i,p,sample->a+i);
}

// result = p*sample
EXPORT void tLweFFTMulR(TLweSampleFFT* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT* sample, const TLweParams* params) {
    const int k = params->k;
    
    for (int i=0; i<=k; i++)
    LagrangeHalfCPolynomialMul(result->a+i,p,sample->a+i);
}

// result = result - p*sample
EXPORT void tLweFFTSubMulRTo(TLweSampleFFT* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT* sample, const TLweParams* params) {
    const int k = params->k;
    
    for (int i=0; i<=k; i++)
    LagrangeHalfCPolynomialSubMul(result->a+i,p,sample->a+i);
}

    
