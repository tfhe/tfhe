#include "lagrangehalfc_impl.h"
#include "poc-fft.h"
#include <cassert>
#include <cmath>

using namespace std;

int rev(int x, int M) {
    int reps = 0;
    for (int j=M; j>1; j/=2) {
	reps = 2*reps+(x%2);
	x/=2;
    }
    return reps;   	
}

FFT_Processor_Poc::FFT_Processor_Poc(const int N): _2N(2*N),N(N),Ns2(N/2) {
    tables_direct = new_fft_table(N);
    tables_reverse = new_ifft_table(N);
    real_inout_direct = fft_table_get_buffer(tables_direct);
    imag_inout_direct = real_inout_direct+Ns2;
    real_inout_rev = fft_table_get_buffer(tables_reverse);
    imag_inout_rev = real_inout_rev+Ns2;
    reva = new int[Ns2];
    cosomegaxminus1 = new double[2*_2N];
    sinomegaxminus1 = cosomegaxminus1+_2N;
    int rev1 = rev(1,_2N);
    int rev3 = rev(3,_2N);
    printf("rev-interval: %d, %d\n",rev1,rev3);
    for (int revi=rev1; revi<rev3; revi++) 
	reva[revi-rev1]=rev(revi,_2N);
    for (int j=0; j<_2N; j++) {
       cosomegaxminus1[j]=cos(2*M_PI*j/_2N)-1.;	
       sinomegaxminus1[j]=sin(2*M_PI*j/_2N);
    }      
}

void FFT_Processor_Poc::execute_reverse_int(double* res, const int* a) {
    for (int i=0; i<N; i++) real_inout_rev[i]=a[i];
    ifft(tables_reverse,real_inout_rev);
    for (int i=0; i<N; i++) res[i]=real_inout_rev[i];
}

void FFT_Processor_Poc::execute_reverse_torus32(double* res, const Torus32* a) {
    int32_t* aa = (int32_t*) a;
    for (int i=0; i<N; i++) real_inout_rev[i]=aa[i]; //we do not rescale
    ifft(tables_reverse,real_inout_rev);
    for (int i=0; i<N; i++) res[i]=real_inout_rev[i];
}

void FFT_Processor_Poc::execute_direct_torus32(Torus32* res, const double* a) {
    static const double _2sN = double(2)/double(N);
    for (int i=0; i<N; i++) real_inout_direct[i]=a[i];
    fft(tables_direct,real_inout_direct);
    for (int i=0; i<N; i++) res[i]=Torus32(int64_t(real_inout_direct[i]*_2sN));
}

FFT_Processor_Poc::~FFT_Processor_Poc() {
    //delete (tables_direct);
    //delete (tables_reverse);
    delete[] cosomegaxminus1;
}

FFT_Processor_Poc fftp1024(1024);

/**
 * FFT functions 
 */
EXPORT void IntPolynomial_ifft(LagrangeHalfCPolynomial* result, const IntPolynomial* p) {
    fftp1024.execute_reverse_int(((LagrangeHalfCPolynomial_IMPL*)result)->coefsC, p->coefs);
}
EXPORT void TorusPolynomial_ifft(LagrangeHalfCPolynomial* result, const TorusPolynomial* p) {
    fftp1024.execute_reverse_torus32(((LagrangeHalfCPolynomial_IMPL*)result)->coefsC, p->coefsT);
}
EXPORT void TorusPolynomial_fft(TorusPolynomial* result, const LagrangeHalfCPolynomial* p) {
    fftp1024.execute_direct_torus32(result->coefsT, ((LagrangeHalfCPolynomial_IMPL*)p)->coefsC);
}
