#ifndef LAGRANGEHALFC_ARITHMETIC_H
#define LAGRANGEHALFC_ARITHMETIC_H

///@file
///@brief This file contains the declaration of operations on LagrangeHalfC polynomials
/// these functions are defined in an fft processor class

#include "polynomials.h"

/**
 * FFT functions
 */
extern void IntPolynomial_ifft(LagrangeHalfCPolynomial *result,
                               const IntPolynomial *p);

template<typename TORUS>
extern void TorusPolynomial_ifft(LagrangeHalfCPolynomial *result,
                                 const TorusPolynomial<TORUS> *p);

template<typename TORUS>
extern void TorusPolynomial_fft(TorusPolynomial<TORUS> *result,
                                const LagrangeHalfCPolynomial *p);

//MISC OPERATIONS
/** sets to zero */
extern void LagrangeHalfCPolynomialClear(LagrangeHalfCPolynomial *result);

/** sets to this torus constant */
template<typename TORUS>
extern void LagrangeHalfCPolynomialSetTorusConstant(LagrangeHalfCPolynomial *result,
                                                    const TORUS mu);

/** adds torus constant */
template<typename TORUS>
extern void LagrangeHalfCPolynomialAddTorusConstant(LagrangeHalfCPolynomial *result,
                                                    const TORUS cst);

/** sets to X^ai-1 */
extern void LagrangeHalfCPolynomialSetXaiMinusOne(LagrangeHalfCPolynomial *result,
                                                  const int ai);

/** termwise multiplication in Lagrange space
 * result = a*b
 */
extern void LagrangeHalfCPolynomialMul(LagrangeHalfCPolynomial *result,
                                       const LagrangeHalfCPolynomial *a,
                                       const LagrangeHalfCPolynomial *b);

/** termwise multiplication and addTo in Lagrange space
 *
 */
extern void LagrangeHalfCPolynomialAddTo(LagrangeHalfCPolynomial *accum,
                                         const LagrangeHalfCPolynomial *a);

/**  */
extern void LagrangeHalfCPolynomialAddMul(LagrangeHalfCPolynomial *accum,
                                          const LagrangeHalfCPolynomial *a,
                                          const LagrangeHalfCPolynomial *b);

extern void LagrangeHalfCPolynomialSubMul(LagrangeHalfCPolynomial *accum,
                                          const LagrangeHalfCPolynomial *a,
                                          const LagrangeHalfCPolynomial *b);

#endif // LAGRANGEHALFC_ARITHMETIC_H
