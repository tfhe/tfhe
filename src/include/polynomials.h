#ifndef POLYNOMIALS_H
#define POLYNOMIALS_H

#include "tfhe.h"

/** This structure represents an integer polynomial modulo X^N+1 */
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


/** This structure represents an torus polynomial modulo X^N+1 */
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
 * This structure is used for FFT operations, and is a representation
 * over C of a polynomial in R[X]/X^N+1
 * This type is meant to be specialized, and all implementations of the structure must be compatible 
 * (reinterpret_cast) with this one. Namely, they should contain at most 2 pointers 
 */
struct LagrangeHalfCPolynomial
{
   void* data;
   void* precomp;
};

#endif //POLYNOMIALS_H
