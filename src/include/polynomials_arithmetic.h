#ifndef POLYNOMIALS_ARITHMETIC_H
#define POLYNOMIALS_ARITHMETIC_H

#include "tfhe_core.h"
#include "polynomials.h"

///@file
///@brief This file contains the operations on polynomials


/**  TorusPolynomial = 0 */
EXPORT void torusPolynomialClear(TorusPolynomial* result);

/**  TorusPolynomial = random */
EXPORT void torusPolynomialUniform(TorusPolynomial* result);

/**  TorusPolynomial = TorusPolynomial */
EXPORT void torusPolynomialCopy(TorusPolynomial* result, const TorusPolynomial*  sample);

/**  TorusPolynomial + TorusPolynomial */
EXPORT void torusPolynomialAdd(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2);

/**  TorusPolynomial += TorusPolynomial */
EXPORT void torusPolynomialAddTo(TorusPolynomial* result, const TorusPolynomial* poly2);


/**  TorusPolynomial - TorusPolynomial */
EXPORT void torusPolynomialSub(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2);

/**  TorusPolynomial -= TorusPolynomial */
EXPORT void torusPolynomialSubTo(TorusPolynomial* result, const TorusPolynomial* poly2);

/**  TorusPolynomial + p*TorusPolynomial */
EXPORT void torusPolynomialAddMulZ(TorusPolynomial* result, const TorusPolynomial* poly1, int32_t p, const TorusPolynomial* poly2);

/**  TorusPolynomial += p*TorusPolynomial */
EXPORT void torusPolynomialAddMulZTo(TorusPolynomial* result, const int32_t p, const TorusPolynomial* poly2);

/**  TorusPolynomial - p*TorusPolynomial */
EXPORT void torusPolynomialSubMulZ(TorusPolynomial* result, const TorusPolynomial* poly1, const int32_t p, const TorusPolynomial* poly2);

/**  TorusPolynomial -= p*TorusPolynomial */
EXPORT void torusPolynomialSubMulZTo(TorusPolynomial* result, int32_t p, const TorusPolynomial* poly2);

/**  TorusPolynomial = (X^a-1)* TorusPolynomial */
EXPORT void torusPolynomialMulByXaiMinusOne(TorusPolynomial* result, int32_t a, const TorusPolynomial* source);

/** result= X^{a}*source */
EXPORT void torusPolynomialMulByXai(TorusPolynomial* result, int32_t a, const TorusPolynomial* source);

/**  Norme Euclidienne d'un IntPolynomial */
EXPORT double intPolynomialNormSq2(const IntPolynomial* poly);

/**  IntPolynomial = 0 */
EXPORT void intPolynomialClear(IntPolynomial* result);

/**  result = source */
EXPORT void intPolynomialCopy(IntPolynomial* result, const IntPolynomial* source);

/**  result += poly1 */
EXPORT void intPolynomialAddTo(IntPolynomial* result, const IntPolynomial* poly1);

/**  result = (X^ai-1) * source */
EXPORT void intPolynomialMulByXaiMinusOne(IntPolynomial* result, int32_t ai, const IntPolynomial* source);

/**  ILA: Norme infini de la distance entre deux TorusPolynomial */
EXPORT double torusPolynomialNormInftyDist(const TorusPolynomial* poly1, const TorusPolynomial* poly2);

// Norme 2 d'un IntPolynomial
EXPORT double intPolynomialNorm2sq(const IntPolynomial* poly);

// Norme infini de la distance entre deux IntPolynomial
EXPORT double intPolynomialNormInftyDist(const IntPolynomial* poly1, const IntPolynomial* poly2);




/**
 * This is the naive external multiplication of an integer polynomial
 * with a torus polynomial. (this function should yield exactly the same
 * result as the karatsuba or fft version, but should be slower) 
 */
EXPORT void torusPolynomialMultNaive(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);


/**
 * This function multiplies 2 polynomials (an integer poly and a torus poly)
 * by using Karatsuba
 * WARNING: N must be a power of 2 to use this function. Else, the
 * behaviour is unpredictable
 */
EXPORT void torusPolynomialMultKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);

/**
 * result += poly1 * poly2 (via karatsuba)
 * WARNING: N must be a power of 2 to use this function. Else, the
 * behaviour is unpredictable
 */
EXPORT void torusPolynomialAddMulRKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);

/**
 * result -= poly1 * poly2 (via karatsuba)
 * WARNING: N must be a power of 2 to use this function. Else, the
 * behaviour is unpredictable
 */
EXPORT void torusPolynomialSubMulRKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);

//#define torusPolynomialMulR torusPolynomialMultKaratsuba
//#define torusPolynomialAddMulR torusPolynomialAddMulRKaratsuba
//#define torusPolynomialSubMulR torusPolynomialSubMulRKaratsuba

#define torusPolynomialMulR torusPolynomialMultFFT
#define torusPolynomialAddMulR torusPolynomialAddMulRFFT
#define torusPolynomialSubMulR torusPolynomialSubMulRFFT


#endif //POLYNOMIALS_ARITHMETIC_H
