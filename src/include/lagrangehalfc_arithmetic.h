#ifndef LAGRANGEHALFC_ARITHMETIC_H
#define LAGRANGEHALFC_ARITHMETIC_H

#include "polynomials.h"

//allocate memory space for a LagrangeHalfCPolynomial
EXPORT LagrangeHalfCPolynomial* alloc_LagrangeHalfCPolynomial();
EXPORT LagrangeHalfCPolynomial* alloc_LagrangeHalfCPolynomial_array(int nbelts);

//free memory space for a LagrangeHalfCPolynomial
EXPORT void free_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial* ptr);
EXPORT void free_LagrangeHalfCPolynomial_array(int nbelts, LagrangeHalfCPolynomial* ptr);

//initialize the LagrangeHalfCPolynomial structure
//(equivalent of the C++ constructor)
EXPORT void init_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial* obj, const int N);
EXPORT void init_LagrangeHalfCPolynomial_array(int nbelts, LagrangeHalfCPolynomial* obj, const int N);

//destroys the LagrangeHalfCPolynomial structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial* obj);
EXPORT void destroy_LagrangeHalfCPolynomial_array(int nbelts, LagrangeHalfCPolynomial* obj);
 
//allocates and initialize the LagrangeHalfCPolynomial structure
//(equivalent of the C++ new)
EXPORT LagrangeHalfCPolynomial* new_LagrangeHalfCPolynomial(const int N);
EXPORT LagrangeHalfCPolynomial* new_LagrangeHalfCPolynomial_array(int nbelts, const int N);

//destroys and frees the LagrangeHalfCPolynomial structure
//(equivalent of the C++ delete)
EXPORT void delete_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial* obj);
EXPORT void delete_LagrangeHalfCPolynomial_array(int nbelts, LagrangeHalfCPolynomial* obj);


/**
 * FFT functions 
 */
EXPORT void IntPolynomial_ifft(LagrangeHalfCPolynomial* result, const IntPolynomial* p);
EXPORT void TorusPolynomial_ifft(LagrangeHalfCPolynomial* result, const TorusPolynomial* p);
EXPORT void TorusPolynomial_fft(TorusPolynomial* result, const LagrangeHalfCPolynomial* p);

//MISC OPERATIONS
/** sets to zero */
EXPORT void LagrangeHalfCPolynomial_clear(LagrangeHalfCPolynomial* result);

/** sets to this torus32 constant */
EXPORT void LagrangeHalfCPolynomial_setTorusConstant(LagrangeHalfCPolynomial* result, const Torus32 mu);
EXPORT void LagrangeHalfCPolynomial_addTorusConstant(LagrangeHalfCPolynomial* result, const Torus32 cst);

/** sets to X^ai-1 */
EXPORT void LagrangeHalfCPolynomial_setXaiMinusOne(LagrangeHalfCPolynomial* result, const int ai);


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

#endif // LAGRANGEHALFC_ARITHMETIC_H
