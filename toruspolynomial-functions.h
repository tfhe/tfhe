#ifndef TORUSPOLYNOMIAL_FUNCTIONS_H
#define TORUSPOLYNOMIAL_FUNCTIONS_H

#include <cassert>
#include <cmath>
#include "lwe.h"
#include "polynomials.h"

// TorusPolynomial = 0
EXPORT void ClearTorusPolynomial(TorusPolynomial* result);

// TorusPolynomial = random
EXPORT void UniformTorusPolynomial(TorusPolynomial* result);

// TorusPolynomial = TorusPolynomial
EXPORT void CopyTorusPolynomial(TorusPolynomial* result, const TorusPolynomial*  sample);

// TorusPolynomial + TorusPolynomial
EXPORT void AddTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2);

// TorusPolynomial += TorusPolynomial
EXPORT void AddToTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly2);


// TorusPolynomial - TorusPolynomial
EXPORT void SubTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2);

// TorusPolynomial -= TorusPolynomial
EXPORT void SubToTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly2);

// TorusPolynomial + p*TorusPolynomial
EXPORT void AddMulZTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, int p, const TorusPolynomial* poly2);

// TorusPolynomial += p*TorusPolynomial
EXPORT void AddMulZToTorusPolynomial(TorusPolynomial* result, const int p, const TorusPolynomial* poly2);

// TorusPolynomial - p*TorusPolynomial
EXPORT void SubMulZTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, const int p, const TorusPolynomial* poly2);

// TorusPolynomial -= p*TorusPolynomial
EXPORT void SubMulZToTorusPolynomial(TorusPolynomial* result, int p, const TorusPolynomial* poly2);

// Norme Euclidienne d'un IntPolynomial
EXPORT double intPolynomialNormSq2(const IntPolynomial* poly);


#endif //TORUSPOLYNOMIAL_FUNCTIONS_H
