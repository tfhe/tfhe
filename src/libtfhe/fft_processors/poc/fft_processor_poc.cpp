#include <complex.h>
#define complex _Complex
#include <polynomials.h>
#include "poc-fft.h"
#include <cassert>
#include <cmath>

using namespace std;

class FFT_Processor_Poc {
    const int _2N;
    const int N;    
    const int Ns2;
    double* real_inout_direct;
    double* imag_inout_direct;
    double* real_inout_rev;
    double* imag_inout_rev;
    void* tables_direct;
    void* tables_reverse;
    public:

    FFT_Processor_Poc(const int N): _2N(2*N),N(N),Ns2(N/2) {
         tables_direct = new_fft_table(N);
	 tables_reverse = new_ifft_table(N);
	 real_inout_direct = fft_table_get_buffer(tables_direct);
	 imag_inout_direct = real_inout_direct+Ns2;
	 real_inout_rev = fft_table_get_buffer(tables_reverse);
	 imag_inout_rev = real_inout_rev+Ns2;
    }

    void execute_direct(cplx* res, const double* a) {
	//double* res_dbl=(double*) res;
	for (int i=0; i<N; i++) real_inout_rev[i]=a[i];
	ifft(tables_reverse,real_inout_rev);
	for (int i=0; i<Ns2; i++) {
	    res[i]=real_inout_rev[i] + I*imag_inout_rev[i];
	    //res_dbl[2*i]=real_inout[i];
	    //res_dbl[2*i+1]=imag_inout[i];
	}
    }

    void execute_direct_int(cplx* res, const int* a) {
	//double* res_dbl=(double*) res;
	//cplx* res_cplx=(cplx*) res;
	for (int i=0; i<N; i++) real_inout_rev[i]=a[i];
	ifft(tables_reverse,real_inout_rev);
	for (int i=0; i<Ns2; i++) {
	    //res[i]=res_cplx[i];
	    res[i]=real_inout_rev[i] + I*imag_inout_rev[i];
	    //res_dbl[2*i]=real_inout[i];
	    //res_dbl[2*i+1]=imag_inout[i];
	}
    }
    void execute_direct_torus32(cplx* res, const Torus32* a) {
	//double* res_dbl=(double*) res;
	//cplx* res_cplx=(cplx*) res;
	static const double _2pm32 = 1./double(INT64_C(1)<<32);
	int32_t* aa = (int32_t*) a;
	for (int i=0; i<N; i++) real_inout_rev[i]=aa[i]*_2pm32;
	ifft(tables_reverse,real_inout_rev);
	for (int i=0; i<Ns2; i++) {
	    //res[i]=res_cplx[i];
	    res[i]=real_inout_rev[i] + I*imag_inout_rev[i];
	    //res_dbl[2*i]=real_inout[i];
	    //res_dbl[2*i+1]=imag_inout[i];
	}
    }

    void execute_reverse(double* res, const cplx* a) {
        static const double _2sN = 2./N;
	for (int i=0; i<Ns2; i++) real_inout_direct[i]=creal(a[i]);
	for (int i=0; i<Ns2; i++) imag_inout_direct[i]=cimag(a[i]);
	fft(tables_direct,real_inout_direct);
	for (int i=0; i<N; i++) res[i]=real_inout_direct[i]*_2sN;
    }
    void execute_reverse_Torus32(Torus32* res, const cplx* a) {
	static const double _2p32 = double(INT64_C(1)<<32);
	static const double _2sN = double(2)/double(N);
	static const double _2p32_2sN = _2p32*_2sN;
	//double* a_dbl=(double*) a;
	for (int i=0; i<Ns2; i++) real_inout_direct[i]=creal(a[i]);
	for (int i=0; i<Ns2; i++) imag_inout_direct[i]=cimag(a[i]);
	fft(tables_direct,real_inout_direct);
	for (int i=0; i<N; i++) res[i]=Torus32(int64_t(real_inout_direct[i]*_2p32_2sN));
	//pas besoin du fmod... Torus32(int64_t(fmod(rev_out[i]*_1sN,1.)*_2p32));
    }

    ~FFT_Processor_Poc() {
         //delete (tables_direct);
         //delete (tables_reverse);
    }
};

static FFT_Processor_Poc fftp1024(1024);

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
