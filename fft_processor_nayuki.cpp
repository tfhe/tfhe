#include <complex.h>
#include "fft_x86_assembly/fft.h"
#include <cassert>
#include <cmath>

class FFT_Processor {
    const int _2N;
    const int N;    
    const int Ns2;
    double* real;
    double* imag;
    void* tables_direct;
    void* tables_reverse;
    public:

    FFT_Processor(const int N): _2N(2*N),N(N),Ns2(N/2) {
         real = (double*) malloc(sizeof(double) * _2N);
         imag = (double*) malloc(sizeof(double) * _2N);
         tables_direct = fft_init(_2N);
	 tables_reverse = fft_init_reverse(_2N);
    }

    //input: a[0],...,a[N]
    // => conv to a[0],...,a[N],-a[0],...,-a[N]
    void execute_direct(cplx* res, const double* a) {
	for (int i=0; i<N; i++) real[i]=a[i]/2.;
	for (int i=0; i<N; i++) real[N+i]=-real[i];
	for (int i=0; i<_2N; i++) imag[i]=0;
	fft_transform(tables_direct,real,imag);
	for (int i=0; i<Ns2; i++) res[i]=cplx(real[2*i+1],imag[2*i+1]);
	for (int i=0; i<=Ns2; i++) {
	    assert(abs(real[2*i])<1e-20);
	    assert(abs(imag[2*i])<1e-20);
	}
    }
    void execute_direct_int(cplx* res, const int* a) {
	for (int i=0; i<N; i++) real[i]=a[i]/2.;
	for (int i=0; i<N; i++) real[N+i]=-in[i];
	for (int i=0; i<_2N; i++) imag[i]=0;
	fft_transform(tables_direct,real,imag);
	for (int i=0; i<Ns2; i++) res[i]=cplx(real[2*i+1],imag[2*i+1]);
	for (int i=0; i<=Ns2; i++) {
	    assert(abs(real[2*i])<1e-20);
	    assert(abs(imag[2*i])<1e-20);
	}
    }
    void execute_direct_torus32(cplx* res, const Torus32* a) {
	static const double _2pm33 = 1./double(INT64_C(1)<<33);
	int32_t* aa = (int32_t*) a;
	for (int i=0; i<N; i++) real[i]=aa[i]*_2pm33;
	for (int i=0; i<N; i++) real[N+i]=-in[i];
	for (int i=0; i<_2N; i++) imag[i]=0;
	fft_transform(tables_direct,real,imag);
	for (int i=0; i<Ns2; i++) res[i]=cplx(real[2*i+1],imag[2*i+1]);
	for (int i=0; i<=Ns2; i++) {
	    assert(abs(real[2*i])<1e-20);
	    assert(abs(imag[2*i])<1e-20);
	}
    }
    void execute_reverse(double* res, const cplx* a) {
        static const double _1sN = 1./N;
	for (int i=0; i<=Ns2; i++) real[2*i]=0;
	for (int i=0; i<=Ns2; i++) imag[2*i]=0;
	for (int i=0; i<Ns2; i++) real[2*i+1]=Real(a[i]);
	for (int i=0; i<Ns2; i++) imag[2*i+1]=Imag(a[i]);
	for (int i=0; i<Ns2; i++) real[2*i+1]=Real(a[i]);
	for (int i=0; i<Ns2; i++) -imag[2*i+1]=Imag(a[i]);
	fftw_execute(rev_p);
	for (int i=0; i<N; i++) res[i]=rev_out[i]*_1sN;
    }
    void execute_reverse_Torus32(Torus32* res, const cplx* a) {
	static const double _2p32 = double(INT64_C(1)<<32);
	static const double _1sN = double(1)/double(N);
	for (int i=0; i<=Ns2; i++) rev_in[2*i]=0;
	for (int i=0; i<Ns2; i++) rev_in[2*i+1]=a[i];
	fftw_execute(rev_p);
	for (int i=0; i<N; i++) res[i]=Torus32(int64_t(rev_out[i]*_1sN*_2p32));
	//pas besoin du fmod... Torus32(int64_t(fmod(rev_out[i]*_1sN,1.)*_2p32));
	for (int i=0; i<N; i++) assert(abs(rev_out[N+i]+rev_out[i])<1e-20);
    }

    ~FFT_Processor() {
         fftw_destroy_plan(p);
         fftw_destroy_plan(rev_p);
         fftw_free(rev_in); fftw_free(out);	
         free(in); free(rev_out);	
    }
};

FFT_Processor fftp1024(1024);

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

