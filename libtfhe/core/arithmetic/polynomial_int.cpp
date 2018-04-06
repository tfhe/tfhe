#include "polynomial_int.h"

#include <cassert>

using namespace std;

template<typename INT_TYPE>
IntPolynomial<INT_TYPE>::IntPolynomial(
    const PolynomialParameters<INT_TYPE> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    coefs = alloc->newArray<INT_TYPE>(params->N);
}

template<typename INT_TYPE>
void IntPolynomial<INT_TYPE>::destroy(
    const PolynomialParameters<INT_TYPE> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    alloc->deleteArray<INT_TYPE>(params->N, coefs);
}


/**
 * Integer polynomial functions
 */

// Sets to zero
template<typename INT_TYPE>
void IntPolynomial<INT_TYPE>::Clear(
    IntPolynomial<INT_TYPE> *poly,
    const PolynomialParameters<INT_TYPE> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    for (int32_t i = 0; i < N; ++i)
        poly->coefs[i] = 0;
}

// copy
template<typename INT_TYPE>
void IntPolynomial<INT_TYPE>::Copy(
    IntPolynomial<INT_TYPE> *result,
    const IntPolynomial<INT_TYPE> *source,
    const PolynomialParameters<INT_TYPE> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    assert(result != source);

    for (int32_t i = 0; i < N; ++i)
        result->coefs[i] = source->coefs[i];
}

// accum += source
template<typename INT_TYPE>
void IntPolynomial<INT_TYPE>::AddTo(
    IntPolynomial<INT_TYPE> *accum,
    const IntPolynomial<INT_TYPE> *source,
    const PolynomialParameters<INT_TYPE> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;

    for (int32_t i = 0; i < N; ++i)
        accum->coefs[i] += source->coefs[i];
}

// result = (X^ai-1) * source
template<typename INT_TYPE>
void IntPolynomial<INT_TYPE>::MulByXaiMinusOne(
    IntPolynomial<INT_TYPE> *result,
    const INT_TYPE ai,
    const IntPolynomial<INT_TYPE> *source,
    const PolynomialParameters<INT_TYPE> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    INT_TYPE *out = result->coefs;
    INT_TYPE *in = source->coefs;

    assert(ai >= 0 && ai < 2 * N);

    if (ai < N) {
        for (int32_t i = 0; i < ai; i++)//sur que i-a<0
            out[i] = -in[i - ai + N] - in[i];
        for (int32_t i = ai; i < N; i++)//sur que N>i-a>=0
            out[i] = in[i - ai] - in[i];
    } else {
        const int32_t aa = ai - N;
        for (int32_t i = 0; i < aa; i++)//sur que i-a<0
            out[i] = in[i - aa + N] - in[i];
        for (int32_t i = aa; i < N; i++)//sur que N>i-a>=0
            out[i] = -in[i - aa] - in[i];
    }
}

// Euclidean norm of an IntPolynomial
template<typename INT_TYPE>
double IntPolynomial<INT_TYPE>::Norm2sq(
    const IntPolynomial<INT_TYPE> *poly,
    const PolynomialParameters<INT_TYPE> *params,
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
    const PolynomialParameters<INT_TYPE> *params,
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


