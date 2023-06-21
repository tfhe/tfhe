#include "spqlios-lib-impl.h"
#include "/home/gama/perso/libfft-spqlios/spqlios/spqlios-cplx-fft.h"
#include <cassert>
#include <cmath>
#include <cstring>

using namespace std;

int32_t rev(int32_t x, int32_t M) {
    int32_t reps = 0;
    for (int32_t j = M; j > 1; j /= 2) {
        reps = 2 * reps + (x % 2);
        x /= 2;
    }
    return reps;
}

FFT_Processor_Spqlios_Lib::FFT_Processor_Spqlios_Lib(const int32_t N) : _2N(2 * N), N(N), Ns2(N / 2) {
    fft_tables = new_cplx_fft_tables(Ns2, 1);
    ifft_tables = new_cplx_ifft_tables(Ns2, 1);
    inout_fft_buffer = cplx_fft_tables_get_buffer(fft_tables, 0);
    inout_ifft_buffer = cplx_ifft_tables_get_buffer(ifft_tables, 0);
    from_tnx32_tables = new_cplx_from_tnx32_tables(Ns2);
    from_znx32_tables = new_cplx_from_znx32_tables(Ns2, 32);
    to_tnx32_tables = new_cplx_to_tnx32_tables(Ns2, Ns2, 18);
    vecfft_mul_tables = new_cplx_fftvec_mul_tables(Ns2);
    vecfft_addmul_tables = new_cplx_fftvec_addmul_tables(Ns2);
    reva = new int32_t[Ns2];
    cosomegaxminus1 = new double[2 * _2N];
    sinomegaxminus1 = cosomegaxminus1 + _2N;
    int32_t rev1 = rev(1, _2N);
    int32_t rev3 = rev(3, _2N);
    //printf("rev-interval: %d, %d\n",rev1,rev3);
    for (int32_t revi = rev1; revi < rev3; revi++)
        reva[revi - rev1] = rev(revi, _2N);
    for (int32_t j = 0; j < _2N; j++) {
        cosomegaxminus1[j] = cos(2 * M_PI * j / _2N) - 1.;
        sinomegaxminus1[j] = sin(2 * M_PI * j / _2N);
    }
}

void FFT_Processor_Spqlios_Lib::execute_reverse_int(double *res, const int32_t *a) {
    //cplx_from_znx32(from_znx32_tables, res, a);
    //cplx_fft(fft_tables, res);
    cplx_from_znx32(from_znx32_tables, inout_fft_buffer, a);
    cplx_fft(fft_tables, inout_fft_buffer);
    memcpy(res, inout_fft_buffer, N*sizeof(double));
}

void FFT_Processor_Spqlios_Lib::execute_reverse_torus32(double *res, const Torus32 *a) {
    //cplx_from_tnx32(from_tnx32_tables, res, a);
    //cplx_fft(fft_tables, res);
    cplx_from_tnx32(from_tnx32_tables, inout_fft_buffer, a);
    cplx_fft(fft_tables, inout_fft_buffer);
    memcpy(res, inout_fft_buffer, N*sizeof(double));
}

void FFT_Processor_Spqlios_Lib::execute_direct_torus32(Torus32 *res, const double *a) {
    memcpy(inout_ifft_buffer, a, N*sizeof(double));
    cplx_ifft(ifft_tables, inout_ifft_buffer);
    cplx_to_tnx32(to_tnx32_tables, res, inout_ifft_buffer);
    //cplx_ifft(ifft_tables, (void*) a);
    //cplx_to_tnx32(to_tnx32_tables, res, a);
}

FFT_Processor_Spqlios_Lib::~FFT_Processor_Spqlios_Lib() {
    delete[] cosomegaxminus1;
}

thread_local FFT_Processor_Spqlios_Lib fftp1024(1024);

/**
 * FFT functions
 */
EXPORT void IntPolynomial_ifft(LagrangeHalfCPolynomial *result, const IntPolynomial *p) {
    fftp1024.execute_reverse_int(((LagrangeHalfCPolynomial_IMPL *) result)->coefsC, p->coefs);
}
EXPORT void TorusPolynomial_ifft(LagrangeHalfCPolynomial *result, const TorusPolynomial *p) {
    fftp1024.execute_reverse_torus32(((LagrangeHalfCPolynomial_IMPL *) result)->coefsC, p->coefsT);
}
EXPORT void TorusPolynomial_fft(TorusPolynomial *result, const LagrangeHalfCPolynomial *p) {
    fftp1024.execute_direct_torus32(result->coefsT, ((LagrangeHalfCPolynomial_IMPL *) p)->coefsC);
}

EXPORT void LagrangeHalfCPolynomialMul(LagrangeHalfCPolynomial *r, const LagrangeHalfCPolynomial *a, const LagrangeHalfCPolynomial *b) {
    cplx_fftvec_mul(fftp1024.vecfft_mul_tables, r->data,a->data,b->data);
}
EXPORT void LagrangeHalfCPolynomialAddMul(LagrangeHalfCPolynomial *r, const LagrangeHalfCPolynomial *a, const LagrangeHalfCPolynomial *b) {
    cplx_fftvec_addmul(fftp1024.vecfft_addmul_tables, r->data,a->data,b->data);
}
