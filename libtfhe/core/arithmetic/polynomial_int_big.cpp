#include "polynomial_int.h"
#include "polynomial_common.h"

#include <cassert>

/**
 * Instantiate IntPolynomial class for big int type
 */
template class IntPolynomial<BigInt>;

template<>
IntPolynomial<BigInt>::IntPolynomial(
    const PolynomialParams<BigInt> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    coefs = alloc->newArray<BigInt>(params->N);
}

template<>
void IntPolynomial<BigInt>::destroy(
    const PolynomialParams<BigInt> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    alloc->deleteArray<BigInt>(params->N, coefs);
}

// Sets to zero
template<>
void IntPolynomial<BigInt>::Clear(
    IntPolynomial<BigInt> *result,
    const PolynomialParams<BigInt> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    PolynomialCommon<BigInt, IntPolynomial>::Clear(
        result, params, context, alloc);
}

// copy
template<>
void IntPolynomial<BigInt>::Copy(
    IntPolynomial<BigInt> *result,
    const IntPolynomial<BigInt> *source,
    const PolynomialParams<BigInt> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    PolynomialCommon<BigInt, IntPolynomial>::Copy(
        result, source, params, context, alloc);
}

// result += poly
template<>
void IntPolynomial<BigInt>::AddTo(
    IntPolynomial<BigInt> *result,
    const IntPolynomial<BigInt> *poly,
    const PolynomialParams<BigInt> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    PolynomialCommon<BigInt, IntPolynomial>::AddTo(
        result, poly, params, context, alloc);
}

// result = (X^ai-1) * source
template<>
void IntPolynomial<BigInt>::MulByXaiMinusOne(
    IntPolynomial<BigInt> *result,
    const int32_t a,
    const IntPolynomial<BigInt> *source,
    const PolynomialParams<BigInt> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    PolynomialCommon<BigInt, IntPolynomial>::MulByXaiMinusOne(
        result, a, source, params, context, alloc);
}

// result = (X^ai) * source
template<>
void IntPolynomial<BigInt>::MulByXai(
    IntPolynomial<BigInt> *result,
    const int32_t a,
    const IntPolynomial<BigInt> *source,
    const PolynomialParams<BigInt> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    PolynomialCommon<BigInt, IntPolynomial>::MulByXai(
        result, a, source, params, context, alloc);
}

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
