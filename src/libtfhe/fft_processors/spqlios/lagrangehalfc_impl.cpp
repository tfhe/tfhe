#include <polynomials.h>
#include "lagrangehalfc_impl.h"

using namespace std;


LagrangeHalfCPolynomial_IMPL::LagrangeHalfCPolynomial_IMPL(const int32_t N) {
    assert(N == 1024);
    coefsC = new double[N];
    proc = &fftp1024;
}

LagrangeHalfCPolynomial_IMPL::~LagrangeHalfCPolynomial_IMPL() {
    delete[] coefsC;
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial *obj, const int32_t N) {
    new(obj) LagrangeHalfCPolynomial_IMPL(N);
}
EXPORT void init_LagrangeHalfCPolynomial_array(int32_t nbelts, LagrangeHalfCPolynomial *obj, const int32_t N) {
    for (int32_t i = 0; i < nbelts; i++) {
        new(obj + i) LagrangeHalfCPolynomial_IMPL(N);
    }
}

//destroys the LagrangeHalfCPolynomial structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial *obj) {
    LagrangeHalfCPolynomial_IMPL *objbis = (LagrangeHalfCPolynomial_IMPL *) obj;
    objbis->~LagrangeHalfCPolynomial_IMPL();
}
EXPORT void destroy_LagrangeHalfCPolynomial_array(int32_t nbelts, LagrangeHalfCPolynomial *obj) {
    LagrangeHalfCPolynomial_IMPL *objbis = (LagrangeHalfCPolynomial_IMPL *) obj;
    for (int32_t i = 0; i < nbelts; i++) {
        (objbis + i)->~LagrangeHalfCPolynomial_IMPL();
    }
}


//MISC OPERATIONS
/** sets to zero */
EXPORT void LagrangeHalfCPolynomialClear(
        LagrangeHalfCPolynomial *reps) {
    LagrangeHalfCPolynomial_IMPL *reps1 = (LagrangeHalfCPolynomial_IMPL *) reps;
    const int32_t N = reps1->proc->N;
#ifndef __AVX2__
    for (int32_t i = 0; i < N; i++)
        reps1->coefsC[i] = 0;
#else
    {
    double* sit = reps1->coefsC;
    double* send = reps1->coefsC+N;
    __asm__ __volatile__ (
        "vpxor %%ymm0,%%ymm0,%%ymm0\n"
        "0:\n"
        "vmovupd %%ymm0,(%0)\n"
        "addq $32,%0\n"
        "cmpq %1,%0\n"
        "jb 0b\n"
        : "=r"(sit),"=r"(send)
        :  "0"(sit), "1"(send)
        : "%ymm0", "memory"
        );
    }
#endif
}

#ifndef __AVX2__
EXPORT void LagrangeHalfCPolynomialSetTorusConstant(LagrangeHalfCPolynomial *result, const Torus32 mu) {
    LagrangeHalfCPolynomial_IMPL *result1 = (LagrangeHalfCPolynomial_IMPL *) result;
    const int32_t Ns2 = result1->proc->Ns2;
    double *b = result1->coefsC;
    double *c = b + Ns2;
    const double muc = mu; //we do not rescale

    for (int32_t j = 0; j < Ns2; j++) b[j] = muc;
    for (int32_t j = 0; j < Ns2; j++) c[j] = 0;


}
#else
EXPORT void LagrangeHalfCPolynomialSetTorusConstant(LagrangeHalfCPolynomial* result, const Torus32 mu) {
    LagrangeHalfCPolynomial_IMPL* result1 = (LagrangeHalfCPolynomial_IMPL*) result;
    const int32_t Ns2 = result1->proc->Ns2;
    double* b = result1->coefsC;
    double* c = b+Ns2;
    double* d = c+Ns2;

    __asm__ __volatile__ (
        "VCVTSI2SD %%esi,%%xmm0,%%xmm0\n"
        "vbroadcastsd %%xmm0,%%ymm0\n" // (mu,mu,mu,mu)
        "0:\n"
        "vmovupd %%ymm0,(%0)\n"
        "addq $32,%0\n"
        "cmpq %1,%0\n"
        "jb 0b\n"
        "vpxor %%ymm0,%%ymm0,%%ymm0\n" // (0,0,0,0)
        "1:\n"
        "vmovupd %%ymm0,(%1)\n"
        "addq $32,%1\n"
        "cmpq %2,%1\n"
        "jb 1b\n"
        : "=r"(b),"=r"(c),"=r"(d)
        :  "0"(b), "1"(c), "2"(d),"S"(mu)
        : "%ymm0", "memory"
        );
 
}
#endif

#ifndef __AVX2__
EXPORT void LagrangeHalfCPolynomialAddTorusConstant(LagrangeHalfCPolynomial *result, const Torus32 mu) {
    LagrangeHalfCPolynomial_IMPL *result1 = (LagrangeHalfCPolynomial_IMPL *) result;
    const int32_t Ns2 = result1->proc->Ns2;
    double *b = result1->coefsC;
    const double muc = mu; //we do not rescale
    for (int32_t j = 0; j < Ns2; j++) b[j] += muc;
}
#else
EXPORT void LagrangeHalfCPolynomialAddTorusConstant(LagrangeHalfCPolynomial* result, const Torus32 mu) {
    LagrangeHalfCPolynomial_IMPL* result1 = (LagrangeHalfCPolynomial_IMPL*) result;
    const int32_t Ns2 = result1->proc->Ns2;
    double* b = result1->coefsC;
    double* c = b+Ns2;

    __asm__ __volatile__ (
        "VCVTSI2SD %%esi,%%xmm0,%%xmm0\n"
        "vbroadcastsd %%xmm0,%%ymm0\n" // (mu,mu,mu,mu)
        "0:\n"
        "vmovupd (%0),%%ymm1\n"
        "vaddpd %%ymm0,%%ymm1,%%ymm1\n"
        "vmovupd %%ymm1,(%0)\n"
        "addq $32,%0\n"
        "cmpq %1,%0\n"
        "jb 0b\n"
        "vzeroall\n"
        : "=r"(b),"=r"(c)
        :  "0"(b), "1"(c),"S"(mu)
        : "%ymm0","%ymm1","memory"
        );
 
}
#endif

EXPORT void LagrangeHalfCPolynomialSetXaiMinusOne(LagrangeHalfCPolynomial *result, const int32_t ai) {
    LagrangeHalfCPolynomial_IMPL *result1 = (LagrangeHalfCPolynomial_IMPL *) result;
    const int32_t Ns2 = result1->proc->Ns2;
    const int32_t _2Nm1 = result1->proc->_2N - 1;
    const double *cosomegaxminus1 = result1->proc->cosomegaxminus1;
    const double *sinomegaxminus1 = result1->proc->sinomegaxminus1;
    const int32_t *reva = result1->proc->reva;
    double *b = result1->coefsC;
    double *c = b + Ns2;
    for (int32_t i = 0; i < Ns2; i++) {
        int32_t ii = (reva[i] * ai) & _2Nm1;
        b[i] = cosomegaxminus1[ii];
        c[i] = sinomegaxminus1[ii];
    }
}

EXPORT void LagrangeHalfCPolynomialAddTo(
        LagrangeHalfCPolynomial *accum,
        const LagrangeHalfCPolynomial *a) {
    LagrangeHalfCPolynomial_IMPL *result1 = (LagrangeHalfCPolynomial_IMPL *) accum;
    const int32_t N = result1->proc->N;
    double *rr = result1->coefsC;
    double *ar = ((LagrangeHalfCPolynomial_IMPL *) a)->coefsC;
    for (int32_t i = 0; i < N; i++) {
        rr[i] += ar[i];
    }
}    

