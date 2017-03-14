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


// result = result + p*sample
EXPORT void tLweFFTAddMulRTo(TLweSampleFFT* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT* sample, const TLweParams* params) {
    const int k = params->k;
    
    for (int i=0; i<=k; i++)
        LagrangeHalfCPolynomialAddMul(result->a+i,p,sample->a+i);
}

    
