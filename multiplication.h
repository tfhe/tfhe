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




EXPORT void multKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);


#endif //MULTILICATION_H
