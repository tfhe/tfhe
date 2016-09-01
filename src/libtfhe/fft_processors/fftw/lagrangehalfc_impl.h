#ifndef LAGRANGEHALFC_IMPL_H
#define LAGRANGEHALFC_IMPL_H

#include <cassert>
#include <cmath>
#include <complex.h>
typedef double _Complex cplx;
#include <fftw3.h>
#include "tfhe.h"
#include "polynomials.h"

class FFT_Processor_fftw {
    public:
    const int _2N;
    const int N;    
    const int Ns2;
    private:
    double* rev_in;
    fftw_complex* rev_out;
    fftw_complex* in;
    double* out;
    fftw_plan p;
    fftw_plan rev_p;
    public:
    cplx* omegaxminus1;

    FFT_Processor_fftw(const int N);
    void execute_reverse_int(cplx* res, const int* a);
    void execute_reverse_torus32(cplx* res, const Torus32* a);
    void execute_direct_Torus32(Torus32* res, const cplx* a);
    ~FFT_Processor_fftw();
};

extern FFT_Processor_fftw fp1024_fftw;

/**
 * structure that represents a real polynomial P mod X^N+1
 * as the N/2 complex numbers:
 * P(w), P(w^3), ..., P(w^(N-1))
 * where w is exp(i.pi/N)
 */
struct LagrangeHalfCPolynomial_IMPL
{
   cplx* coefsC;
   FFT_Processor_fftw* proc;

   LagrangeHalfCPolynomial_IMPL(int N);
   ~LagrangeHalfCPolynomial_IMPL();
};

#endif // LAGRANGEHALFC_IMPL_H
