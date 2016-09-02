#ifndef LAGRANGEHALFC_IMPL_H
#define LAGRANGEHALFC_IMPL_H

#include <cassert>
#include <cmath>
#include <complex.h>
typedef double _Complex cplx;
#include <fftw3.h>
#include "tfhe.h"
#include "polynomials.h"

class FFT_Processor_nayuki {
    public:
    const int _2N;
    const int N;    
    const int Ns2;
    private:
    double* real_inout;
    double* imag_inout;
    void* tables_direct;
    void* tables_reverse;
    public:
    cplx* omegaxminus1;

    FFT_Processor_nayuki(const int N);
    void check_alternate_real();
    void check_conjugate_cplx();
    void execute_reverse_int(cplx* res, const int* a);
    void execute_reverse_torus32(cplx* res, const Torus32* a);
    void execute_direct_torus32(Torus32* res, const cplx* a);
    ~FFT_Processor_nayuki();
};

extern FFT_Processor_nayuki fp1024_nayuki;

/**
 * structure that represents a real polynomial P mod X^N+1
 * as the N/2 complex numbers:
 * P(w), P(w^3), ..., P(w^(N-1))
 * where w is exp(i.pi/N)
 */
struct LagrangeHalfCPolynomial_IMPL
{
   cplx* coefsC;
   FFT_Processor_nayuki* proc;

   LagrangeHalfCPolynomial_IMPL(int N);
   ~LagrangeHalfCPolynomial_IMPL();
};

#endif // LAGRANGEHALFC_IMPL_H
