#include "polynomial_int.h"

#include <cassert>

/**
 * Instantiate IntPolynomial class for big int type
 */
template struct IntPolynomial<BigTorus>;

// Euclidean norm of an IntPolynomial
template<>
double IntPolynomial<BigTorus>::Norm2sq(
    const IntPolynomial<BigTorus> *poly,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    abort(); //not implemented yet
    return 0;
}

// Infinity norm of the distance between two IntPolynomial
template<>
double IntPolynomial<BigTorus>::NormInftyDist(
    const IntPolynomial<BigTorus> *poly1,
    const IntPolynomial<BigTorus> *poly2,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    abort(); //not implemented yet
    return 0;
}
