#include "polynomial_int.h"

#include <cassert>

/**
 * Instantiate IntPolynomial class for available int types
 */
INT_CLASS_IMPL_ALL(IntPolynomial);


/**
 * Integer polynomial functions
 */

// Euclidean norm of an IntPolynomial
template<typename INT_TYPE>
double IntPolynomial<INT_TYPE>::Norm2sq(
    const IntPolynomial<INT_TYPE> *poly,
    const PolynomialParams<INT_TYPE> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    double norm = 0;

    for (int32_t i = 0; i < N; ++i) {
        double r = poly->coefs[i];
        norm += r * r;
    }
    return norm;
}

// Infinity norm of the distance between two IntPolynomial
template<typename INT_TYPE>
double IntPolynomial<INT_TYPE>::NormInftyDist(
    const IntPolynomial<INT_TYPE> *poly1,
    const IntPolynomial<INT_TYPE> *poly2,
    const PolynomialParams<INT_TYPE> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    double norm = 0;

    // Max between the coefficients of abs(poly1-poly2)
    for (int32_t i = 0; i < N; ++i) {
        double r = abs(poly1->coefs[i] - poly2->coefs[i]);
        if (r > norm) { norm = r; }
    }
    return norm;
}
