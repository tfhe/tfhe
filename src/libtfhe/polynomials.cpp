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

//MISC OPERATIONS
/** sets to zero */
EXPORT void clearLagrangeHalfCPolynomial(LagrangeHalfCPolynomial* result) {
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
EXPORT void addMultToFFT(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2) {
    const int N = poly1->N;
    LagrangeHalfCPolynomial* tmp = new_LagrangeHalfCPolynomial_array(3,N);
    TorusPolynomial* tmpr = new_TorusPolynomial(N);
    IntPolynomial_fft(tmp+0,poly1);
    TorusPolynomial_fft(tmp+1,poly2);
    LagrangeHalfCPolynomial_mul(tmp+2,tmp+0,tmp+1);
    TorusPolynomial_ifft(tmpr, tmp+2);
    torusPolynomialAddTo(result, tmpr);
    delete_TorusPolynomial(tmpr);
    delete_LagrangeHalfCPolynomial_array(3,tmp);
}
EXPORT void subMultToFFT(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2) {
    const int N = poly1->N;
    LagrangeHalfCPolynomial* tmp = new_LagrangeHalfCPolynomial_array(3,N);
    TorusPolynomial* tmpr = new_TorusPolynomial(N);
    IntPolynomial_fft(tmp+0,poly1);
    TorusPolynomial_fft(tmp+1,poly2);
    LagrangeHalfCPolynomial_mul(tmp+2,tmp+0,tmp+1);
    TorusPolynomial_ifft(tmpr, tmp+2);
    torusPolynomialSubTo(result, tmpr);
    delete_TorusPolynomial(tmpr);
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
	const LagrangeHalfCPolynomial* b) 
{
    const int Ns2 = a->N/2;
    for (int i=0; i<Ns2; i++) 
	accum->coefsC[i] += a->coefsC[i]*b->coefsC[i];
}


/** termwise multiplication and addTo in Lagrange space */
EXPORT void LagrangeHalfCPolynomial_submul(
	LagrangeHalfCPolynomial* accum, 
	const LagrangeHalfCPolynomial* a, 
	const LagrangeHalfCPolynomial* b) 
{
    const int Ns2 = a->N/2;
    for (int i=0; i<Ns2; i++) 
	accum->coefsC[i] -= a->coefsC[i]*b->coefsC[i];
}

EXPORT void LagrangeHalfCPolynomial_clear(
	LagrangeHalfCPolynomial* reps) {
    const int Ns2 = reps->N/2;
    for (int i=0; i<Ns2; i++) 
	reps->coefsC[i] = 0;
}

EXPORT void LagrangeHalfCPolynomial_addto(
	LagrangeHalfCPolynomial* accum, 
	const LagrangeHalfCPolynomial* a) {
    const int Ns2 = accum->N/2;
    for (int i=0; i<Ns2; i++) 
	accum->coefsC[i] += a->coefsC[i];
}    

