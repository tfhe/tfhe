#include "polynomial_int.h"

#include <cassert>

/**
 * Instantiate IntPolynomial class for big int type
 */
template struct IntPolynomial<BigInt>;

// Euclidean norm of an IntPolynomial
template<>
double IntPolynomial<BigInt>::Norm2sq(
    const IntPolynomial<BigInt> *poly,
    const PolynomialParams<BigInt> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // double norm = 0;

    // for (int32_t i = 0; i < N; ++i) {
    //     double r = poly->coefs[i];
    //     norm += r * r;
    // }
    // return norm;
    return 0;
}

// Infinity norm of the distance between two IntPolynomial
template<>
double IntPolynomial<BigInt>::NormInftyDist(
    const IntPolynomial<BigInt> *poly1,
    const IntPolynomial<BigInt> *poly2,
    const PolynomialParams<BigInt> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // double norm = 0;

    // // Max between the coefficients of abs(poly1-poly2)
    // for (int32_t i = 0; i < N; ++i) {
    //     double r = abs(poly1->coefs[i] - poly2->coefs[i]);
    //     if (r > norm) { norm = r; }
    // }
    // return norm;
    return 0;
}
