#ifndef TFHE_POLYNOMIAL_INT_PRIMITIVE_H
#define TFHE_POLYNOMIAL_INT_PRIMITIVE_H

#include "polynomial_int.h"

#ifdef TFHE_POLYNOMIAL_INT_BIG_H
#error "polynomial_int_primitive.h and polynomial_int_big.h are incompatible"
#endif

#include <cassert>

template<typename TORUS, AsmTypeEnum AsmType>
IntPolynomial<TORUS, AsmType>::IntPolynomial(const PolynomialParams<TORUS, AsmType> *params, TfheThreadContext *context,
                                             Allocator *alloc) :
        Polynomial<TORUS, CoefTypeEnum::Integer, AsmType>(params, context, alloc) {}

template<typename TORUS, AsmTypeEnum AsmType>
void IntPolynomial<TORUS, AsmType>::destroy(const PolynomialParams<TORUS, AsmType> *params, TfheThreadContext *context,
                                            Allocator *alloc) {
    Polynomial<TORUS, CoefTypeEnum::Integer, AsmType>::destroy(params, context, alloc);
}

/**
 * Integer polynomial functions
 */

// Euclidean norm of an IntPolynomial
template<typename TORUS, AsmTypeEnum AsmType>
double IntPolynomial<TORUS, AsmType>::Norm2sq(
        const IntPolynomial<TORUS, AsmType> *poly,
        const PolynomialParams<TORUS, AsmType> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t N = params->N;
    double norm = 0;

    for (int32_t i = 0; i < N; ++i) {
        double r = poly->coefs[i];
        norm += r * r;
    }
    return norm;
}

// Infinity norm of the distance between two IntPolynomial
template<typename TORUS, AsmTypeEnum AsmType>
double IntPolynomial<TORUS, AsmType>::NormInftyDist(
        const IntPolynomial<TORUS, AsmType> *poly1,
        const IntPolynomial<TORUS, AsmType> *poly2,
        const PolynomialParams<TORUS, AsmType> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t N = params->N;
    double norm = 0;

    // Max between the coefficients of abs(poly1-poly2)
    for (int32_t i = 0; i < N; ++i) {
        double r = abs(poly1->coefs[i] - poly2->coefs[i]);
        if (r > norm) { norm = r; }
    }
    return norm;
}

#endif //TFHE_POLYNOMIAL_INT_PRIMITIVE_H
