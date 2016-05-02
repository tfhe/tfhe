#ifndef MULTILICATION_H
#define MULTILICATION_H

#include "lwe.h"


struct IntPolynomial {
   const int N;
   int* coefs;

#ifdef __cplusplus   
   IntPolynomial(const int N);
   ~IntPolynomial();
   IntPolynomial(const IntPolynomial&) = delete; //forbidden 
   IntPolynomial* operator=(const IntPolynomial&) = delete; //forbidden
#endif
};


struct TorusPolynomial {
   const int N;
   Torus32* coefsT;

#ifdef __cplusplus   
   TorusPolynomial(const int N);
   ~TorusPolynomial();
   TorusPolynomial(const TorusPolynomial&) = delete; //forbidden 
   TorusPolynomial* operator=(const TorusPolynomial&) = delete; //forbidden
#endif
};




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
EXPORT void addMultKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);
EXPORT void subMultKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);

#define mulRTorusPolynomial torusPolynomialMultKaratsuba
#define addMulRToTorusPolynomial addMultKaratsuba
#define subMulRToTorusPolynomial subMultKaratsuba


#endif //MULTILICATION_H
