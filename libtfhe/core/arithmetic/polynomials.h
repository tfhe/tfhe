#ifndef POLYNOMIALS_H
#define POLYNOMIALS_H

///@file
///@brief This file contains the declaration of polynomials structures

#include <cmath>
#include "tfhe_core.h"
#include "torus.h"
#include "../allocator/allocator.h"
#include "threadcontext.h"


/**
 * CLASSES
 */
class PolynomialParameters {
public:
    int N;
};


/**
 * Class of integer polynomials modulo X^N+1
 */
class IntPolynomial {
public:
    int *coefs;


    // constructor
    IntPolynomial(const PolynomialParameters *params,
                  TfheThreadContext *context,
                  Allocator *alloc);

    void destroy(const PolynomialParameters *params,
                 TfheThreadContext *context,
                 Allocator *alloc);

    // destructors
    IntPolynomial(const IntPolynomial &) = delete;

    IntPolynomial *operator=(const IntPolynomial &) = delete;

    ~IntPolynomial() = delete;
};


/**
 * Class of torus polynomials modulo X^N+1
 */
template<typename TORUS>
class TorusPolynomial {
public:
    TORUS *coefsT;


    // constructor
    TorusPolynomial(const PolynomialParameters *params,
                    TfheThreadContext *context,
                    Allocator *alloc);

    void destroy(const PolynomialParameters *params,
                 TfheThreadContext *context,
                 Allocator *alloc);


    // destructors
    TorusPolynomial(const TorusPolynomial<TORUS> &) = delete;

    TorusPolynomial<TORUS> *operator=(const TorusPolynomial<TORUS> &) = delete;

    ~TorusPolynomial() = delete;
};

TORUS_CLASS_IMPL_ALL(TorusPolynomial);

#endif //POLYNOMIALS_H
