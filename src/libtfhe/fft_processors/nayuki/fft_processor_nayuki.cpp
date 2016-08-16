#include <complex.h>
#include "polynomials.h"
#include "fft.h"
#include <cassert>
#include <cmath>

class FFT_Processor_Nayuki {
    const int _2N;
    const int N;    
    const int Ns2;
    double* real_inout;
    double* imag_inout;
    void* tables_direct;
    void* tables_reverse;
    public:

    FFT_Processor_Nayuki(const int N): _2N(2*N),N(N),Ns2(N/2) {
         real_inout = (double*) malloc(sizeof(double) * _2N);
         imag_inout = (double*) malloc(sizeof(double) * _2N);
         tables_direct = fft_init(_2N);
	 tables_reverse = fft_init_reverse(_2N);
    }

    void check_alternate_real() {
#ifndef NDEBUG
	for (int i=0; i<_2N; i++) assert(abs(imag_inout[i])<1e-20);
	for (int i=0; i<N; i++) assert(abs(real_inout[i]+real_inout[N+i])<1e-20);
#endif
    }

    void check_conjugate_cplx() {
#ifndef NDEBUG
	for (int i=0; i<N; i++) assert(abs(real_inout[2*i])+abs(imag_inout[2*i])<1e-20);
	for (int i=0; i<Ns2; i++) assert(abs(imag_inout[2*i+1]+imag_inout[_2N-1-2*i])<1e-20);
#endif
    }

    //input: a[0],...,a[N]
    // => conv to a[0],...,a[N],-a[0],...,-a[N]
    void execute_direct(cplx* res, const double* a) {
	double* res_dbl=(double*) res;
	for (int i=0; i<N; i++) real_inout[i]=a[i]/2.;
	for (int i=0; i<N; i++) real_inout[N+i]=-real_inout[i];
	for (int i=0; i<_2N; i++) imag_inout[i]=0;
	check_alternate_real();
	fft_transform(tables_direct,real_inout,imag_inout);
	for (int i=0; i<_2N; i+=2) { 
	    res_dbl[i]=real_inout[i+1];
	    res_dbl[i+1]=imag_inout[i+1];
	}
	check_conjugate_cplx();
    }
    void execute_direct_int(cplx* res, const int* a) {
	double* res_dbl=(double*) res;
	for (int i=0; i<N; i++) real_inout[i]=a[i]/2.;
	for (int i=0; i<N; i++) real_inout[N+i]=-real_inout[i];
	for (int i=0; i<_2N; i++) imag_inout[i]=0;
	check_alternate_real();
	fft_transform(tables_direct,real_inout,imag_inout);
	for (int i=0; i<N; i+=2) { 
	    res_dbl[i]=real_inout[i+1];
	    res_dbl[i+1]=imag_inout[i+1];
	}
	for (int i=0; i<Ns2; i++) {
	    assert(cabs(real_inout[2*i+1]+I*imag_inout[2*i+1]-res[i])<1e-20);
	}
	check_conjugate_cplx();
    }
    void execute_direct_torus32(cplx* res, const Torus32* a) {
	static const double _2pm33 = 1./double(INT64_C(1)<<33);
	int32_t* aa = (int32_t*) a;
	for (int i=0; i<N; i++) real_inout[i]=aa[i]*_2pm33;
	for (int i=0; i<N; i++) real_inout[N+i]=-real_inout[i];
	for (int i=0; i<_2N; i++) imag_inout[i]=0;
	check_alternate_real();
	fft_transform(tables_direct,real_inout,imag_inout);
	for (int i=0; i<Ns2; i++) res[i]=real_inout[2*i+1]+I*imag_inout[2*i+1];
	check_conjugate_cplx();
    }
    void execute_reverse(double* res, const cplx* a) {
        static const double _1sN = 1./N;
	for (int i=0; i<N; i++) real_inout[2*i]=0;
	for (int i=0; i<N; i++) imag_inout[2*i]=0;
	for (int i=0; i<Ns2; i++) real_inout[2*i+1]=creal(a[i]);
	for (int i=0; i<Ns2; i++) imag_inout[2*i+1]=cimag(a[i]);
	for (int i=0; i<Ns2; i++) real_inout[_2N-2*i]=creal(a[i]);
	for (int i=0; i<Ns2; i++) imag_inout[_2N-2*i]=-cimag(a[i]);
	check_conjugate_cplx();
	fft_transform_reverse(tables_reverse,real_inout,imag_inout);
	for (int i=0; i<N; i++) res[i]=real_inout[i]*_1sN;
	check_alternate_real();
    }
    void execute_reverse_Torus32(Torus32* res, const cplx* a) {
	static const double _2p32 = double(INT64_C(1)<<32);
	static const double _1sN = double(1)/double(N);
	//double* a_dbl=(double*) a;
	for (int i=0; i<N; i++) real_inout[2*i]=0;
	for (int i=0; i<N; i++) imag_inout[2*i]=0;
	for (int i=0; i<Ns2; i++) real_inout[2*i+1]=creal(a[i]);
	for (int i=0; i<Ns2; i++) imag_inout[2*i+1]=cimag(a[i]);
	for (int i=0; i<Ns2; i++) real_inout[_2N-1-2*i]=creal(a[i]);
	for (int i=0; i<Ns2; i++) imag_inout[_2N-1-2*i]=-cimag(a[i]);
	/*
	for (int i=0; i<N; i+=2) {
	    real_inout[i]=0;
	    real_inout[i+1]=a_dbl[i];
	    imag_inout[i]=0;
	    imag_inout[i+1]=a_dbl[i+1];
	}
	for (int i=0; i<N; i+=2) {
	    real_inout[_2N-2-i]=0;
	    real_inout[_2N-1-i]=a_dbl[i];
	    imag_inout[_2N-2-i]=0;
	    imag_inout[_2N-1-i]=-a_dbl[i+1];
	}
	*/
	/*
	double* rr=real_inout;
	double* ii=imag_inout;
	double* aa=a_dbl;
	for (int i=0; i<N; i+=2) {
	    *(rr++)=0;
	    *(rr++)=*(aa++);
	    *(ii++)=0;
	    *(ii++)=*(aa++);
	}
	for (int i=0; i<N; i+=2) {
	    *(ii++)=0;
	    *(ii++)= - *(--aa);
	    *(rr++)=0;
	    *(rr++)= *(--aa);
	}
	*/
#ifndef NDEBUG
	for (int i=0; i<N; i++) assert(real_inout[2*i]==0);
	for (int i=0; i<N; i++) assert(imag_inout[2*i]==0);
	for (int i=0; i<Ns2; i++) assert(real_inout[2*i+1]==creal(a[i]));
	for (int i=0; i<Ns2; i++) assert(imag_inout[2*i+1]==cimag(a[i]));
	for (int i=0; i<Ns2; i++) assert(real_inout[_2N-1-2*i]==creal(a[i]));
	for (int i=0; i<Ns2; i++) assert(imag_inout[_2N-1-2*i]==-cimag(a[i]));
	check_conjugate_cplx();
#endif
	fft_transform_reverse(tables_reverse,real_inout,imag_inout);
	for (int i=0; i<N; i++) res[i]=Torus32(int64_t(real_inout[i]*_1sN*_2p32));
	//pas besoin du fmod... Torus32(int64_t(fmod(rev_out[i]*_1sN,1.)*_2p32));
	check_alternate_real();
    }

    ~FFT_Processor_Nayuki() {
         fft_destroy(tables_direct);
         fft_destroy(tables_reverse);
         free(real_inout); 
	 free(imag_inout);	
    }
};

static FFT_Processor_Nayuki fftp1024(1024);

#ifdef NAYUKI_FFT_PROCESSOR
/**
 * FFT functions 
 */
EXPORT void IntPolynomial_fft(LagrangeHalfCPolynomial* result, const IntPolynomial* p) {
    fftp1024.execute_direct_int(result->coefsC, p->coefs);
}
EXPORT void TorusPolynomial_fft(LagrangeHalfCPolynomial* result, const TorusPolynomial* p) {
    fftp1024.execute_direct_torus32(result->coefsC, p->coefsT);
}
EXPORT void TorusPolynomial_ifft(TorusPolynomial* result, const LagrangeHalfCPolynomial* p) {
    fftp1024.execute_reverse_Torus32(result->coefsT, p->coefsC);
}
#endif
