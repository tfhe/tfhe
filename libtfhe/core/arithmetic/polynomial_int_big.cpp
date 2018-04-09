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
    abort(); //not implemented yet
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
    abort(); //not implemented yet
    return 0;
}
