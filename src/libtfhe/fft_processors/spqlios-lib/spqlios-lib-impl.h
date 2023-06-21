#ifndef TFHE_SPQLIOS_LIB_IMPL_H
#define TFHE_SPQLIOS_LIB_IMPL_H

#include <cassert>
#include <cmath>
#include <tfhe.h>
#include <polynomials.h>
#include <spqlios/spqlios-cplx-fft.h>
#include <spqlios/reim4/reim4-vecfft.h>

class FFT_Processor_Spqlios_Lib {
public:
    const int32_t _2N;
    const int32_t N;
    const int32_t Ns2;

private:
    void *inout_fft_buffer;
    void *inout_ifft_buffer;
    CPLX_IFFT_PRECOMP* ifft_tables;
    CPLX_FFT_PRECOMP* fft_tables;
public:
    CPLX_FROM_TNX32_PRECOMP* from_tnx32_tables;
    CPLX_FROM_ZNX32_PRECOMP* from_znx32_tables;
    CPLX_TO_TNX32_PRECOMP* to_tnx32_tables;
    REIM4_VECFFT_MUL_PRECOMP* vecfft_mul_tables;
    REIM4_VECFFT_ADDMUL_PRECOMP* vecfft_addmul_tables;
    REIM4_FROM_CPLX_PRECOMP* reim4_from_cplx_tables;
    REIM4_TO_CPLX_PRECOMP* reim4_to_cplx_tables;
public:
    double *cosomegaxminus1;
    double *sinomegaxminus1;
    int32_t *reva; //rev(2i+1,_2N)

    FFT_Processor_Spqlios_Lib(const int32_t N);

    void execute_reverse_int(double *res, const int32_t *a);

    void execute_reverse_torus32(double *res, const Torus32 *a);

    void execute_direct_torus32(Torus32 *res, const double *a);

    ~FFT_Processor_Spqlios_Lib();
};

extern thread_local FFT_Processor_Spqlios_Lib fftp1024;

/**
 * structure that represents a real polynomial P mod X^N+1
 * as the N/2 complex numbers:
 * P(w), P(w^3), ..., P(w^(N-1))
 * where w is exp(i.pi/N)
 */
struct LagrangeHalfCPolynomial_IMPL {
    double *coefsC;
    FFT_Processor_Spqlios_Lib *proc;

    LagrangeHalfCPolynomial_IMPL(int32_t N);

    ~LagrangeHalfCPolynomial_IMPL();
};


#endif //TFHE_SPQLIOS_LIB_IMPL_H
