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
 * @brief Polynomial parameters class
 */
class PolynomialParameters {
public:
    int32_t N;
};

/**
 * @brief Generic polynomial class
 * 
 * @tparam COEF_TYPE type of coefficients
 * @tparam tag dummy tag to differentiate between different polynomial types
 */
template<typename COEF_TYPE, int tag>
class Polynomial
{
public:
    COEF_TYPE *coefs;

    /**
     * @brief Constructs a polynomial with given parameters
     * 
     * @param params polynomial parameters
     * @param context thread execution context
     * @param alloc allocator to use
     */
    Polynomial(const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator *alloc);

    /**
     * @brief Destroys inner data of polynomial
     * 
     * @param params polynomial parameters used for construction
     * @param context thread execution context
     * @param alloc allocator to use
     */
    void destroy(const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator *alloc);

    PREVENT_STACK_COPY(Polynomial);
};

/**
 * Polynomial with integer coefficients
 */
template<typename INT_TYPE>
using IntPolynomial = Polynomial<INT_TYPE, 0>;

/**
 * Polynomial with torus coefficients
 */
template<typename TORUS_TYPE>
using TorusPolynomial = Polynomial<TORUS_TYPE, 1>;

INT_CLASS_IMPL_ALL(Polynomial, 0);

TORUS_CLASS_IMPL_ALL(Polynomial, 1);

#endif //POLYNOMIALS_H
