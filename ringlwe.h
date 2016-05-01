#ifndef RINGLWE_H
#define RINGLWE_H

#include "lwe.h"
#include "multiplication.h"

struct RingLWEParams {
    const int N; //a power of 2: degree of the polynomials
    const int k; //number of polynomials in the mask
    const double alpha_min;
    const double alpha_max;

#ifdef __cplusplus
    RingLWEParams(int N, int k, double alpha_min, double alpha_max);
    ~RingLWEParams();
    RingLWEParams(const RingLWEParams&) = delete;
    void operator=(const RingLWEParams&) = delete;
#endif
};

struct RingLWEKey {
    const RingLWEParams* params;
    IntPolynomial* key;
#ifdef __cplusplus
    RingLWEKey(const RingLWEParams* params);
    ~RingLWEKey();
    RingLWEKey(const RingLWEKey&) = delete;
    void operator=(const RingLWEKey&) = delete;
#endif
};


struct RingLWESample {
    const int k; //required during the destructor call...
    TorusPolynomial* a;
    TorusPolynomial* b;
    double current_variance;
#ifdef __cplusplus
    RingLWESample(const RingLWEParams* params);
    ~RingLWESample();
    RingLWESample(const RingLWESample&) = delete;
    void operator=(const RingLWESample&) = delete;
#endif
};

struct RingLWESampleFFT {
    const int k; //required during the destructor call...
    LagrangeHalfCPolynomial* a;
    LagrangeHalfCPolynomial* b;
    double current_variance;
#ifdef __cplusplus
    RingLWESampleFFT(const RingLWEParams* params);
    ~RingLWESampleFFT();
    RingLWESampleFFT(const RingLWESampleFFT&) = delete;
    void operator=(const RingLWESampleFFT&) = delete;
#endif
};

#endif // RINGLWE_H
