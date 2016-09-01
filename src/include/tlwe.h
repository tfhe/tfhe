#ifndef RINGLWE_H
#define RINGLWE_H

#include "tfhe.h"
#include "polynomials.h"
#include "lweparams.h"

struct TLweParams {
    const int N; ///< a power of 2: degree of the polynomials
    const int k; ///< number of polynomials in the mask
    const double alpha_min; ///< minimal noise s.t. the sample is secure
    const double alpha_max; ///< maximal noise s.t. we can decrypt
    const LweParams extracted_lweparams; ///< lwe params if one extracts

#ifdef __cplusplus
    TLweParams(int N, int k, double alpha_min, double alpha_max);
    ~TLweParams();
    TLweParams(const TLweParams&) = delete;
    void operator=(const TLweParams&) = delete;
#endif
};

struct TLweKey {
    const TLweParams* params; ///< the parameters of the key
    IntPolynomial* key; ///< the key (i.e k binary polynomials)
#ifdef __cplusplus
    TLweKey(const TLweParams* params);
    ~TLweKey();
    TLweKey(const TLweKey&) = delete;
    void operator=(const TLweKey&) = delete;
#endif
};


struct TLweSample {
    TorusPolynomial* a; ///< array of length k+1: mask + right term
    TorusPolynomial* b; ///< alias of a[k] to get the right term
    double current_variance; ///< avg variance of the sample
    const int k; 
#ifdef __cplusplus
    TLweSample(const TLweParams* params);
    ~TLweSample();
    TLweSample(const TLweSample&) = delete;
    void operator=(const TLweSample&) = delete;
#endif
};

struct TLweSampleFFT {
    LagrangeHalfCPolynomial* a; ///< array of length k+1: mask + right term
    LagrangeHalfCPolynomial* b; ///< alias of a[k] to get the right term
    double current_variance; ///< avg variance of the sample
    const int k; //required during the destructor call...
#ifdef __cplusplus
    TLweSampleFFT(const TLweParams* params);
    ~TLweSampleFFT();
    TLweSampleFFT(const TLweSampleFFT&) = delete;
    void operator=(const TLweSampleFFT&) = delete;
#endif
};

#endif // RINGLWE_H
