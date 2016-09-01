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
EXPORT void torusPolynomialAddMulZ(TorusPolynomial* result, const TorusPolynomial* poly1, int p, const TorusPolynomial* poly2);

/**  TorusPolynomial += p*TorusPolynomial */
EXPORT void torusPolynomialAddMulZTo(TorusPolynomial* result, const int p, const TorusPolynomial* poly2);

/**  TorusPolynomial - p*TorusPolynomial */
EXPORT void torusPolynomialSubMulZ(TorusPolynomial* result, const TorusPolynomial* poly1, const int p, const TorusPolynomial* poly2);

/**  TorusPolynomial -= p*TorusPolynomial */
EXPORT void torusPolynomialSubMulZTo(TorusPolynomial* result, int p, const TorusPolynomial* poly2);

/**  TorusPolynomial = X^a * TorusPolynomial */
EXPORT void torusPolynomialMulByXai(TorusPolynomial* result, int a, const TorusPolynomial* bk);

/**  Norme Euclidienne d'un IntPolynomial */
EXPORT double intPolynomialNormSq2(const IntPolynomial* poly);

/** multiplication Karatsuba */
EXPORT void torusPolynomialMultKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);

/** multiplication naive  */
EXPORT void torusPolynomialMultNaive(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);



/**
 * This is the naive external multiplication of an integer polynomial
 * with a torus polynomial. (this function should yield exactly the same
 * result as the karatsuba or fft version, but should be slower) 
 */
EXPORT void torusPolynomialMultNaive(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);



/**
 * This function multiplies 2 polynomials (an integer poly and a torus poly)
 * by using Karatsuba
 */
EXPORT void torusPolynomialMultKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);
EXPORT void torusPolynomialAddMulRKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);
EXPORT void torusPolynomialSubMulRKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);

//#define torusPolynomialMulR torusPolynomialMultKaratsuba
//#define torusPolynomialAddMulR torusPolynomialAddMulRKaratsuba
//#define torusPolynomialSubMulR torusPolynomialSubMulRKaratsuba

#define torusPolynomialMulR torusPolynomialMultFFT
#define torusPolynomialAddMulR torusPolynomialAddMulRFFT
#define torusPolynomialSubMulR torusPolynomialSubMulRFFT


#endif //POLYNOMIALS_ARITHMETIC_H
