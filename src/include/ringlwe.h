#ifndef RINGLWE_H
#define RINGLWE_H

#include "lwe.h"
#include "multiplication.h"
#include "lweparams.h"

struct RingLWEParams {
    const int N; ///< a power of 2: degree of the polynomials
    const int k; ///< number of polynomials in the mask
    const double alpha_min; ///< minimal noise s.t. the sample is secure
    const double alpha_max; ///< maximal noise s.t. we can decrypt
    const LWEParams extracted_lweparams; ///< lwe params if one extracts

#ifdef __cplusplus
    RingLWEParams(int N, int k, double alpha_min, double alpha_max);
    ~RingLWEParams();
    RingLWEParams(const RingLWEParams&) = delete;
    void operator=(const RingLWEParams&) = delete;
#endif
};

struct RingLWEKey {
    const RingLWEParams* params; ///< the parameters of the key
    IntPolynomial* key; ///< the key (i.e k binary polynomials)
#ifdef __cplusplus
    RingLWEKey(const RingLWEParams* params);
    ~RingLWEKey();
    RingLWEKey(const RingLWEKey&) = delete;
    void operator=(const RingLWEKey&) = delete;
#endif
};


struct RingLWESample {
    TorusPolynomial* a; ///< array of length k+1: mask + right term
    TorusPolynomial* b; ///< alias of a[k] to get the right term
    double current_variance; ///< avg variance of the sample
    const int k; 
#ifdef __cplusplus
    RingLWESample(const RingLWEParams* params);
    ~RingLWESample();
    RingLWESample(const RingLWESample&) = delete;
    void operator=(const RingLWESample&) = delete;
#endif
};

struct RingLWESampleFFT {
    LagrangeHalfCPolynomial* a; ///< array of length k+1: mask + right term
    LagrangeHalfCPolynomial* b; ///< alias of a[k] to get the right term
    double current_variance; ///< avg variance of the sample
    const int k; //required during the destructor call...
#ifdef __cplusplus
    RingLWESampleFFT(const RingLWEParams* params);
    ~RingLWESampleFFT();
    RingLWESampleFFT(const RingLWESampleFFT&) = delete;
    void operator=(const RingLWESampleFFT&) = delete;
#endif
};

#endif // RINGLWE_H
