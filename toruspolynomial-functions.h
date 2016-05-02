#ifndef TORUSPOLYNOMIAL_FUNCTIONS_H
#define TORUSPOLYNOMIAL_FUNCTIONS_H

///@file
///@brief This file contains the operations on polynomials


#include "lwe.h"
#include "polynomials.h"

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

/**  Norme Euclidienne d'un IntPolynomial */
EXPORT double intPolynomialNormSq2(const IntPolynomial* poly);

/** multiplication Karatsuba */
EXPORT void torusPolynomialMultKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);

/** multiplication naive  */
EXPORT void torusPolynomialMultNaive(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);


#endif //TORUSPOLYNOMIAL_FUNCTIONS_H
