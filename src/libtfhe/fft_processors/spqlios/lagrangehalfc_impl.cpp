#include <polynomials.h>
#include "lagrangehalfc_impl.h"

using namespace std;


LagrangeHalfCPolynomial_IMPL::LagrangeHalfCPolynomial_IMPL(const int N) {
    assert(N==1024);
    coefsC = new double[N];
    proc = &fftp1024;
}

LagrangeHalfCPolynomial_IMPL::~LagrangeHalfCPolynomial_IMPL() {
    delete[] coefsC;
}

//initialize the key structure
//(equivalent of the C++ constructor)
EXPORT void init_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial* obj, const int N) {
    new(obj) LagrangeHalfCPolynomial_IMPL(N);
}
EXPORT void init_LagrangeHalfCPolynomial_array(int nbelts, LagrangeHalfCPolynomial* obj, const int N) {
    for (int i=0; i<nbelts; i++) {
	new(obj+i) LagrangeHalfCPolynomial_IMPL(N);
    }
}

//destroys the LagrangeHalfCPolynomial structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial* obj) {
    LagrangeHalfCPolynomial_IMPL* objbis = (LagrangeHalfCPolynomial_IMPL*) obj;
    objbis->~LagrangeHalfCPolynomial_IMPL();
}
EXPORT void destroy_LagrangeHalfCPolynomial_array(int nbelts, LagrangeHalfCPolynomial* obj) {
    LagrangeHalfCPolynomial_IMPL* objbis = (LagrangeHalfCPolynomial_IMPL*) obj;
    for (int i=0; i<nbelts; i++) {
	(objbis+i)->~LagrangeHalfCPolynomial_IMPL();
    }
}
 

//MISC OPERATIONS
/** sets to zero */
EXPORT void LagrangeHalfCPolynomial_clear(
	LagrangeHalfCPolynomial* reps) {
    LagrangeHalfCPolynomial_IMPL* reps1 = (LagrangeHalfCPolynomial_IMPL*) reps;
    const int N = reps1->proc->N;
    for (int i=0; i<N; i++) 
	reps1->coefsC[i] = 0;
}

EXPORT void LagrangeHalfCPolynomial_setTorusConstant(LagrangeHalfCPolynomial* result, const Torus32 mu) {
    LagrangeHalfCPolynomial_IMPL* result1 = (LagrangeHalfCPolynomial_IMPL*) result;
    const int Ns2 = result1->proc->Ns2;
    double* b = result1->coefsC;
    double* c = b+Ns2;
    const double muc = mu; //we do not rescale
    for (int j=0; j<Ns2; j++) b[j]=muc;
    for (int j=0; j<Ns2; j++) c[j]=0;
}

EXPORT void LagrangeHalfCPolynomial_addTorusConstant(LagrangeHalfCPolynomial* result, const Torus32 mu) {
    LagrangeHalfCPolynomial_IMPL* result1 = (LagrangeHalfCPolynomial_IMPL*) result;
    const int Ns2 = result1->proc->Ns2;
    double* b = result1->coefsC;
    const double muc = mu; //we do not rescale
    for (int j=0; j<Ns2; j++) b[j]+=muc;
}

EXPORT void LagrangeHalfCPolynomial_setXaiMinusOne(LagrangeHalfCPolynomial* result, const int ai) {
    LagrangeHalfCPolynomial_IMPL* result1 = (LagrangeHalfCPolynomial_IMPL*) result;
    const int Ns2 = result1->proc->Ns2;
    const int _2Nm1 = result1->proc->_2N-1;
    const double* cosomegaxminus1 = result1->proc->cosomegaxminus1;
    const double* sinomegaxminus1 = result1->proc->sinomegaxminus1;
    const int* reva = result1->proc->reva;
    double* b = result1->coefsC;
    double* c = b+Ns2;
    for (int i=0; i<Ns2; i++) 
	b[i]=cosomegaxminus1[(reva[i]*ai)&_2Nm1];
    for (int i=0; i<Ns2; i++) 
	c[i]=sinomegaxminus1[(reva[i]*ai)&_2Nm1];
}

EXPORT void LagrangeHalfCPolynomial_addto(
	LagrangeHalfCPolynomial* accum, 
	const LagrangeHalfCPolynomial* a) {
    LagrangeHalfCPolynomial_IMPL* result1 = (LagrangeHalfCPolynomial_IMPL*) accum;
    const int N = result1->proc->N;
    double* rr = result1->coefsC;
    double* ar = ((LagrangeHalfCPolynomial_IMPL*) a)->coefsC;
    for (int i=0; i<N; i++) {
	rr[i]+=ar[i];
    }
}    

