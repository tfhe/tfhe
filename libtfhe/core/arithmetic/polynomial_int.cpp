#include "polynomial_int.h"

#include <cassert>

/**
 * Instantiate IntPolynomial class for available torus types
 */
EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_TORUS(IntPolynomial);

/**
 * Integer polynomial functions
 */

// Euclidean norm of an IntPolynomial
template<typename TORUS>
double IntPolynomial<TORUS>::Norm2sq(
        const IntPolynomial<TORUS> *poly,
        const PolynomialParams<TORUS> *params,
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
template<typename TORUS>
double IntPolynomial<TORUS>::NormInftyDist(
        const IntPolynomial<TORUS> *poly1,
        const IntPolynomial<TORUS> *poly2,
        const PolynomialParams<TORUS> *params,
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
