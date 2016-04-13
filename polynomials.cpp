#include <complex.h>
#define complex _Complex
#include <fftw3.h>
#include <cassert>
#include <cmath>
#include "lwe.h"
#include "polynomials.h"

using namespace std;


LagrangeHalfCPolynomial::LagrangeHalfCPolynomial(const int N): N(N) {
    coefsC = new cplx[N];
}

LagrangeHalfCPolynomial::~LagrangeHalfCPolynomial() {
    delete[] coefsC;
}

class FFT_Processor {
    const int _2N;
    const int N;    
    const int Ns2;
    double* in;
    fftw_complex* out;
    fftw_complex* rev_in;
    double* rev_out;
    fftw_plan p;
    fftw_plan rev_p;
    public:

    FFT_Processor(const int N): _2N(2*N),N(N),Ns2(N/2) {
         in = (double*) malloc(sizeof(double) * _2N);
         rev_out = (double*) malloc(sizeof(double) * _2N);
         out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N+1));
         rev_in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N+1));
         p = fftw_plan_dft_r2c_1d(_2N, in, out, FFTW_ESTIMATE);	
         rev_p = fftw_plan_dft_c2r_1d(_2N, rev_in, rev_out, FFTW_ESTIMATE);	
    }

    void execute_direct(cplx* res, const double* a) {
	for (int i=0; i<N; i++) in[i]=a[i]/2.;
	for (int i=0; i<N; i++) in[N+i]=-in[i];
	fftw_execute(p);
	for (int i=0; i<Ns2; i++) res[i]=out[2*i+1];
	for (int i=0; i<=Ns2; i++) assert(cabs(out[2*i])<1e-20);
    }
    void execute_direct_int(cplx* res, const int* a) {
	for (int i=0; i<N; i++) in[i]=a[i]/2.;
	for (int i=0; i<N; i++) in[N+i]=-in[i];
	fftw_execute(p);
	for (int i=0; i<Ns2; i++) res[i]=out[2*i+1];
	for (int i=0; i<=Ns2; i++) assert(cabs(out[2*i])<1e-20);
    }
    void execute_direct_torus32(cplx* res, const Torus32* a) {
	static const double _2pm33 = 1./double(1l<<33);
	int32_t* aa = (int32_t*) a;
	for (int i=0; i<N; i++) in[i]=aa[i]*_2pm33;
	for (int i=0; i<N; i++) in[N+i]=-in[i];
	fftw_execute(p);
	for (int i=0; i<Ns2; i++) res[i]=out[2*i+1];
	for (int i=0; i<=Ns2; i++) assert(cabs(out[2*i])<1e-20);
    }
    void execute_reverse(double* res, const cplx* a) {
        static const double _1sN = 1./N;
	for (int i=0; i<=Ns2; i++) rev_in[2*i]=0;
	for (int i=0; i<Ns2; i++) rev_in[2*i+1]=a[i];
	fftw_execute(rev_p);
	for (int i=0; i<N; i++) res[i]=rev_out[i]*_1sN;
    }
    void execute_reverse_Torus32(Torus32* res, const cplx* a) {
	static const double _2p32 = double(1l<<32);
	static const double _1sN = double(1)/double(N);
	for (int i=0; i<=Ns2; i++) rev_in[2*i]=0;
	for (int i=0; i<Ns2; i++) rev_in[2*i+1]=a[i];
	fftw_execute(rev_p);
	for (int i=0; i<N; i++) res[i]=Torus32(long(fmod(rev_out[i]*_1sN,1.)*_2p32));
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

//MISC OPERATIONS
/** sets to zero */
EXPORT void clear(LagrangeHalfCPolynomial* result) {
    const int N = result->N;
    for (int i=0; i<N; i++) result->coefsC[i]=0;
}

/** multiplication via direct FFT */
EXPORT void multFFT(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2) {
    const int N = poly1->N;
    LagrangeHalfCPolynomial* tmp = new_LagrangeHalfCPolynomial_array(3,N);
    IntPolynomial_fft(tmp+0,poly1);
    TorusPolynomial_fft(tmp+1,poly2);
    LagrangeHalfCPolynomial_mul(tmp+2,tmp+0,tmp+1);
    TorusPolynomial_ifft(result, tmp+2);
    delete_LagrangeHalfCPolynomial_array(3,tmp);
}
/** termwise multiplication in Lagrange space */
EXPORT void LagrangeHalfCPolynomial_mul(
	LagrangeHalfCPolynomial* result, 
	const LagrangeHalfCPolynomial* a, 
	const LagrangeHalfCPolynomial* b) {
    const int Ns2 = a->N/2;
    for (int i=0; i<Ns2; i++) 
	result->coefsC[i] = a->coefsC[i]*b->coefsC[i];
}

/** termwise multiplication and addTo in Lagrange space */
EXPORT void LagrangeHalfCPolynomial_addmul(
	LagrangeHalfCPolynomial* accum, 
	const LagrangeHalfCPolynomial* a, 
	const LagrangeHalfCPolynomial* b);

