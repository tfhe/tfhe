#include "lagrangehalfc_impl.h"
#include "lagrangehalfc_arithmetic.h"
#include "spqlios-fft.h"
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

FFT_Processor_Spqlios::FFT_Processor_Spqlios(const int N): _2N(2*N),N(N),Ns2(N/2) {
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
    //printf("rev-interval: %d, %d\n",rev1,rev3);
    for (int revi=rev1; revi<rev3; revi++)
	reva[revi-rev1]=rev(revi,_2N);
    for (int j=0; j<_2N; j++) {
       cosomegaxminus1[j]=cos(2*M_PI*j/_2N)-1.;
       sinomegaxminus1[j]=sin(2*M_PI*j/_2N);
    }
}

void FFT_Processor_Spqlios::execute_reverse_int(double* res, const int* a) {
    //for (int i=0; i<N; i++) real_inout_rev[i]=(double)a[i];
    {
    	double* dst = real_inout_rev;
	const int* ait = a;
	const int* aend = a+N;
	__asm__ __volatile__ (
		"0:\n"
		"vmovupd (%1),%%xmm0\n"
		"vcvtdq2pd %%xmm0,%%ymm1\n"
		"vmovapd %%ymm1,(%0)\n"
		"addq $16,%1\n"
		"addq $32,%0\n"
		"cmpq %2,%1\n"
		"jb 0b\n"
		: "=r"(dst),"=r"(ait),"=r"(aend)
		: "0"(dst),"1"(ait),"2"(aend)
		: "%xmm0","%ymm1","memory"
		);
    }
    ifft(tables_reverse,real_inout_rev);
    //for (int i=0; i<N; i++) res[i]=real_inout_rev[i];
    {
    	double* dst = res;
	double* sit = real_inout_rev;
	double* send = real_inout_rev+N;
	__asm__ __volatile__ (
		"1:\n"
		"vmovapd (%1),%%ymm0\n"
		"vmovupd %%ymm0,(%0)\n"
		"addq $32,%1\n"
		"addq $32,%0\n"
		"cmpq %2,%1\n"
		"jb 1b\n"
		"vzeroall\n"
		: "=r"(dst),"=r"(sit),"=r"(send)
		: "0"(dst),"1"(sit),"2"(send)
		: "%ymm0","memory"
		);
    }
}

void FFT_Processor_Spqlios::execute_reverse_torus32(double* res, const Torus32* a) {
    int32_t* aa = (int32_t*) a;
    //for (int i=0; i<N; i++) real_inout_rev[i]=aa[i]; //we do not rescale
    //ifft(tables_reverse,real_inout_rev);
    //for (int i=0; i<N; i++) res[i]=real_inout_rev[i];
    execute_reverse_int(res,aa);
}

void FFT_Processor_Spqlios::execute_reverse_torus64(double* res, const int64_t* a) {
    for (int i=0; i<N; i++) real_inout_rev[i]=(double)a[i];
    ifft(tables_reverse,real_inout_rev);
    for (int i=0; i<N; i++) res[i]=real_inout_rev[i];
}

void FFT_Processor_Spqlios::execute_direct_torus32(Torus32* res, const double* a) {
    static const double _2sN = double(2)/double(N);
    //for (int i=0; i<N; i++) real_inout_direct[i]=a[i]*_2sn;
    {
    	double* dst = real_inout_direct;
	const double* sit = a;
	const double* send = a+N;
	//double __2sN = 2./N;
	const double* bla = &_2sN;
	__asm__ __volatile__ (
		"vbroadcastsd (%3),%%ymm2\n"
		"1:\n"
		"vmovupd (%1),%%ymm0\n"
		"vmulpd	%%ymm2,%%ymm0,%%ymm0\n"
		"vmovapd %%ymm0,(%0)\n"
		"addq $32,%1\n"
		"addq $32,%0\n"
		"cmpq %2,%1\n"
		"jb 1b\n"
		: "=r"(dst),"=r"(sit),"=r"(send),"=r"(bla)
		: "0"(dst),"1"(sit),"2"(send),"3"(bla)
		: "%ymm0","%ymm2","memory"
		);
    }
    fft(tables_direct,real_inout_direct);
    for (int i=0; i<N; i++) res[i]=Torus32(int64_t(real_inout_direct[i]));
}

void FFT_Processor_Spqlios::execute_direct_torus64(int64_t* res, const double* a) {
  static const double _2sN = double(2)/double(N);
  //static const double _2p64 = pow(2.,64);
  // for (int i=0; i<N; i++) real_inout_direct[i]=a[i]*_2sN;
  {
    double* dst = real_inout_direct;
    const double* sit = a;
    const double* send = a+N;
    //double __2sN = 2./N;
    const double* bla = &_2sN;
    __asm__ __volatile__ (
      "vbroadcastsd (%3),%%ymm2\n"
      "1:\n"
      "vmovupd (%1),%%ymm0\n"
      "vmulpd %%ymm2,%%ymm0,%%ymm0\n"
      "vmovapd %%ymm0,(%0)\n"
      "addq $32,%1\n"
      "addq $32,%0\n"
      "cmpq %2,%1\n"
      "jb 1b\n"
      : "=r"(dst),"=r"(sit),"=r"(send),"=r"(bla)
      : "0"(dst),"1"(sit),"2"(send),"3"(bla)
      : "%ymm0","%ymm2","memory"
    );
  }
  fft(tables_direct,real_inout_direct);
  const uint64_t* const vals = (const uint64_t*) real_inout_direct;
  static const uint64_t valmask0 = 0x000FFFFFFFFFFFFFul;
  static const uint64_t valmask1 = 0x0010000000000000ul;
  static const uint16_t expmask0 = 0x07FFu;
  for (int i=0; i<N; i++) {
      uint64_t val = (vals[i]&valmask0)|valmask1; //mantissa on 53 bits
      uint16_t expo = (vals[i]>>52)&expmask0; //exponent 11 bits
      // 1023 -> 52th pos -> 0th pos
      // 1075 -> 52th pos -> 52th pos
      int16_t trans = expo-1075;
      uint64_t val2 = trans>0?(val<<trans):(val>>-trans);
      res[i]=(vals[i]>>63)?-val2:val2;
/*
      double aa = real_inout_direct[i];
      double absaa = fabs(aa);
      int bb = int(absaa/_2p64);
      uint64_t vv = uint64_t(rint(absaa-bb*_2p64));
      //uint64_t vv = bb==0?uint64_t(rint(absaa)):uint64_t(rint(absaa-bb*_2p64));
      uint64_t ores=aa>0?vv:-vv;

      if (abs(ores-res[i])>4) printf("discrepancy: %lf -> %ld -> %ld\n",real_inout_direct[i], res[i], ores);
*/
      //printf("%lf %lf %d %lu %ld\n",aa,absaa,bb,vv,res[i]);

  }
}


FFT_Processor_Spqlios::~FFT_Processor_Spqlios() {
    //delete (tables_direct);
    //delete (tables_reverse);
    delete[] cosomegaxminus1;
}

thread_local FFT_Processor_Spqlios fftp1024(1024);

/**
 * FFT functions
 */
void IntPolynomial_ifft(LagrangeHalfCPolynomial* result, const IntPolynomial* p) {
    fftp1024.execute_reverse_int(((LagrangeHalfCPolynomial_IMPL*)result)->coefsC, p->coefs);
}

template<>
void TorusPolynomial_ifft<Torus32>(LagrangeHalfCPolynomial* result, const TorusPolynomial<Torus32>* p) {
    fftp1024.execute_reverse_torus32(((LagrangeHalfCPolynomial_IMPL*)result)->coefsC, p->coefsT);
}

template<>
void TorusPolynomial_fft<Torus32>(TorusPolynomial<Torus32>* result, const LagrangeHalfCPolynomial* p) {
    fftp1024.execute_direct_torus32(result->coefsT, ((LagrangeHalfCPolynomial_IMPL*)p)->coefsC);
}

template<>
void TorusPolynomial_ifft<Torus64>(LagrangeHalfCPolynomial* result, const TorusPolynomial<Torus64>* p) {
    fftp1024.execute_reverse_torus64(((LagrangeHalfCPolynomial_IMPL*)result)->coefsC, p->coefsT);
}

template<>
void TorusPolynomial_fft<Torus64>(TorusPolynomial<Torus64>* result, const LagrangeHalfCPolynomial* p) {
    fftp1024.execute_direct_torus64(result->coefsT, ((LagrangeHalfCPolynomial_IMPL*)p)->coefsC);
}
