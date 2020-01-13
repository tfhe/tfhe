#include <complex>
#include <polynomials.h>
#include "lagrangehalfc_impl.h"
#include "fft.h"
#include <cassert>
#include <cmath>

FFT_Processor_nayuki::FFT_Processor_nayuki(const int32_t N): _2N(2*N),N(N),Ns2(N/2) {
    real_inout = (double*) malloc(sizeof(double) * _2N);
    imag_inout = (double*) malloc(sizeof(double) * _2N);
    tables_direct = fft_init(_2N);
    tables_reverse = fft_init_reverse(_2N);
    omegaxminus1 = (cplx*) malloc(sizeof(cplx) * _2N);
    for (int32_t x=0; x<_2N; x++) {
	omegaxminus1[x]=cplx(cos(x*M_PI/N)-1., sin(x*M_PI/N)); // instead of cos(x*M_PI/N)-1. + sin(x*M_PI/N) * 1i 
	//exp(i.x.pi/N)-1
    }
}

void FFT_Processor_nayuki::check_alternate_real() {
#ifndef NDEBUG
    for (int32_t i=0; i<_2N; i++) assert(fabs(imag_inout[i])<1e-8);
    for (int32_t i=0; i<N; i++) assert(fabs(real_inout[i]+real_inout[N+i])<1e-9);
#endif
}

void FFT_Processor_nayuki::check_conjugate_cplx() {
#ifndef NDEBUG
    for (int32_t i=0; i<N; i++) assert(fabs(real_inout[2*i])+fabs(imag_inout[2*i])<1e-20);
    for (int32_t i=0; i<Ns2; i++) assert(fabs(imag_inout[2*i+1]+imag_inout[_2N-1-2*i])<1e-20);
#endif
}

void FFT_Processor_nayuki::execute_reverse_int(cplx* res, const int32_t* a) {
    double* res_dbl=(double*) res;
    for (int32_t i=0; i<N; i++) real_inout[i]=a[i]/2.;
    for (int32_t i=0; i<N; i++) real_inout[N+i]=-real_inout[i];
    for (int32_t i=0; i<_2N; i++) imag_inout[i]=0;
    check_alternate_real();
    fft_transform_reverse(tables_reverse,real_inout,imag_inout);
    for (int32_t i=0; i<N; i+=2) { 
	res_dbl[i]=real_inout[i+1];
	res_dbl[i+1]=imag_inout[i+1];
    }
    for (int32_t i=0; i<Ns2; i++) {
	assert(abs(cplx(real_inout[2*i+1],imag_inout[2*i+1])-res[i])<1e-20);
    }
    check_conjugate_cplx();
}

void FFT_Processor_nayuki::execute_reverse_torus32(cplx* res, const Torus32* a) {
    static const double _2pm33 = 1./double(INT64_C(1)<<33);
    int32_t* aa = (int32_t*) a;
    for (int32_t i=0; i<N; i++) real_inout[i]=aa[i]*_2pm33;
    for (int32_t i=0; i<N; i++) real_inout[N+i]=-real_inout[i];
    for (int32_t i=0; i<_2N; i++) imag_inout[i]=0;
    check_alternate_real();
    fft_transform_reverse(tables_reverse,real_inout,imag_inout);
    for (int32_t i=0; i<Ns2; i++) res[i]=cplx(real_inout[2*i+1],imag_inout[2*i+1]);
    check_conjugate_cplx();
}

void FFT_Processor_nayuki::execute_direct_torus32(Torus32* res, const cplx* a) {
    static const double _2p32 = double(INT64_C(1)<<32);
    static const double _1sN = double(1)/double(N);
    //double* a_dbl=(double*) a;
    for (int32_t i=0; i<N; i++) real_inout[2*i]=0;
    for (int32_t i=0; i<N; i++) imag_inout[2*i]=0;
    for (int32_t i=0; i<Ns2; i++) real_inout[2*i+1]=real(a[i]);
    for (int32_t i=0; i<Ns2; i++) imag_inout[2*i+1]=imag(a[i]);
    for (int32_t i=0; i<Ns2; i++) real_inout[_2N-1-2*i]=real(a[i]);
    for (int32_t i=0; i<Ns2; i++) imag_inout[_2N-1-2*i]=-imag(a[i]);
#ifndef NDEBUG
    for (int32_t i=0; i<N; i++) assert(real_inout[2*i]==0);
    for (int32_t i=0; i<N; i++) assert(imag_inout[2*i]==0);
    for (int32_t i=0; i<Ns2; i++) assert(real_inout[2*i+1]==real(a[i]));
    for (int32_t i=0; i<Ns2; i++) assert(imag_inout[2*i+1]==imag(a[i]));
    for (int32_t i=0; i<Ns2; i++) assert(real_inout[_2N-1-2*i]==real(a[i]));
    for (int32_t i=0; i<Ns2; i++) assert(imag_inout[_2N-1-2*i]==-imag(a[i]));
    check_conjugate_cplx();
#endif
    fft_transform(tables_direct,real_inout,imag_inout);
    for (int32_t i=0; i<N; i++) res[i]=Torus32(int64_t(real_inout[i]*_1sN*_2p32));
    //pas besoin du fmod... Torus32(int64_t(fmod(rev_out[i]*_1sN,1.)*_2p32));
    check_alternate_real();
}

FFT_Processor_nayuki::~FFT_Processor_nayuki() {
    fft_destroy(tables_direct);
    fft_destroy(tables_reverse);
    free(real_inout); 
    free(imag_inout);
    free(omegaxminus1);    
}

thread_local FFT_Processor_nayuki fp1024_nayuki(1024);

/**
 * FFT functions 
 */
EXPORT void IntPolynomial_ifft(LagrangeHalfCPolynomial* result, const IntPolynomial* p) {
    LagrangeHalfCPolynomial_IMPL* r = (LagrangeHalfCPolynomial_IMPL*) result;
    fp1024_nayuki.execute_reverse_int(r->coefsC, p->coefs);
}
EXPORT void TorusPolynomial_ifft(LagrangeHalfCPolynomial* result, const TorusPolynomial* p) {
    LagrangeHalfCPolynomial_IMPL* r = (LagrangeHalfCPolynomial_IMPL*) result;
    fp1024_nayuki.execute_reverse_torus32(r->coefsC, p->coefsT);
}
EXPORT void TorusPolynomial_fft(TorusPolynomial* result, const LagrangeHalfCPolynomial* p) {
    LagrangeHalfCPolynomial_IMPL* r = (LagrangeHalfCPolynomial_IMPL*) p;
    fp1024_nayuki.execute_direct_torus32(result->coefsT, r->coefsC);
}
