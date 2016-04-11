#ifndef MULTILICATION_H
#define MULTILICATION_H



struct IntPolynomial {
   const int* coefs;

#ifdef __cplusplus   
   IntPolynomial(const int* coefs);
   ~IntPolynomial();
   IntPolynomial(const IntPolynomial&) = delete; //forbidden 
   IntPolynomial* operator=(const IntPolynomial&) = delete; //forbidden
#endif
};


struct TorusPolynomial {
   const double* coefs;

#ifdef __cplusplus   
   TorusPolynomial(const double* coefs);
   ~TorusPolynomial();
   TorusPolynomial(const TorusPolynomial&) = delete; //forbidden 
   TorusPolynomial* operator=(const TorusPolynomial&) = delete; //forbidden
#endif
};




EXPORT void multKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);


#endif //MULTILICATION_H
