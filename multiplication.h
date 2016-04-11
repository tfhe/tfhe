#ifndef MULTILICATION_H
#define MULTILICATION_H



struct IntPolynomial {
   const int* coefs;

#ifdef __cplusplus   
   IntPolynomial(const int* coefs, const int N);
   ~IntPolynomial();
   IntPolynomial(const IntPolynomial&) = delete; //forbidden 
   IntPolynomial* operator=(const IntPolynomial&) = delete; //forbidden
#endif
};


struct TorusPolynomial {
   const Torus* coefs;

#ifdef __cplusplus   
   TorusPolynomial(const Torus32* coefs, const int N);
   ~TorusPolynomial();
   TorusPolynomial(const TorusPolynomial&) = delete; //forbidden 
   TorusPolynomial* operator=(const TorusPolynomial&) = delete; //forbidden
#endif
};




EXPORT void multKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);


#endif //MULTILICATION_H
