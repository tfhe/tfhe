#ifndef LAGRANGEHALFC_IMPL_SPQLIOS_H
#define LAGRANGEHALFC_IMPL_SPQLIOS_H

#include <cassert>
#include <cmath>
#include <tfhe.h>
#include <polynomials.h>

class FFT_Processor_Spqlios {
public:
    const int32_t _2N;
    const int32_t N;
    const int32_t Ns2;

private:
    double *real_inout_direct;
    double *imag_inout_direct;
    double *real_inout_rev;
    double *imag_inout_rev;
    void *tables_direct;
    void *tables_reverse;
public:
    double *cosomegaxminus1;
    double *sinomegaxminus1;
    int32_t *reva; //rev(2i+1,_2N)

    FFT_Processor_Spqlios(const int32_t N);

    void execute_reverse_int(double *res, const int32_t *a);

    void execute_reverse_torus32(double *res, const Torus32 *a);

    void execute_direct_torus32(Torus32 *res, const double *a);

    ~FFT_Processor_Spqlios();
};

extern thread_local FFT_Processor_Spqlios fftp1024;

/**
 * structure that represents a real polynomial P mod X^N+1
 * as the N/2 complex numbers:
 * P(w), P(w^3), ..., P(w^(N-1))
 * where w is exp(i.pi/N)
 */
struct LagrangeHalfCPolynomial_IMPL {
    double *coefsC;
    FFT_Processor_Spqlios *proc;

    LagrangeHalfCPolynomial_IMPL(int32_t N);

    ~LagrangeHalfCPolynomial_IMPL();
};

#endif // LAGRANGEHALFC_IMPL_SPQLIOS_H
