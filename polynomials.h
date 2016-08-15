#ifndef POLYNOMIALS_H
#define POLYNOMIALS_H

#include <complex.h>
#include "lwe.h"
#include "multiplication.h"

typedef double _Complex cplx;
/**
 * structure that represents a real polynomial P mod X^N+1
 * as the N/2 complex numbers:
 * P(w), P(w^3), ..., P(w^(N-1))
 * where w is exp(i.pi/N)
 */
struct LagrangeHalfCPolynomial {
   const int N;
   cplx* coefsC;

#ifdef __cplusplus   
   LagrangeHalfCPolynomial(const int N);
   ~LagrangeHalfCPolynomial();
   LagrangeHalfCPolynomial(const LagrangeHalfCPolynomial&) = delete;
   LagrangeHalfCPolynomial* operator=(const LagrangeHalfCPolynomial&) = delete;
#endif
};


/**
 * FFT functions 
 */
EXPORT void IntPolynomial_fft(LagrangeHalfCPolynomial* result, const IntPolynomial* p);
EXPORT void TorusPolynomial_fft(LagrangeHalfCPolynomial* result, const TorusPolynomial* p);
EXPORT void TorusPolynomial_ifft(TorusPolynomial* result, const LagrangeHalfCPolynomial* p);

//MISC OPERATIONS
/** sets to zero */
EXPORT void clearLagrangeHalfCPolynomial(LagrangeHalfCPolynomial* result);

/** multiplication via direct FFT */
EXPORT void multFFT(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);
EXPORT void addMultToFFT(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);
EXPORT void subMultToFFT(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);

/** termwise multiplication in Lagrange space */
EXPORT void LagrangeHalfCPolynomial_mul(
	LagrangeHalfCPolynomial* result, 
	const LagrangeHalfCPolynomial* a, 
	const LagrangeHalfCPolynomial* b);

/** termwise multiplication and addTo in Lagrange space */
EXPORT void LagrangeHalfCPolynomial_clear(
	LagrangeHalfCPolynomial* reps);

EXPORT void LagrangeHalfCPolynomial_addto(
	LagrangeHalfCPolynomial* accum, 
	const LagrangeHalfCPolynomial* a);

EXPORT void LagrangeHalfCPolynomial_addmul(
	LagrangeHalfCPolynomial* accum, 
	const LagrangeHalfCPolynomial* a, 
	const LagrangeHalfCPolynomial* b);

EXPORT void LagrangeHalfCPolynomial_submul(
	LagrangeHalfCPolynomial* accum, 
	const LagrangeHalfCPolynomial* a, 
	const LagrangeHalfCPolynomial* b);

#endif //POLYNOMIALS_H
