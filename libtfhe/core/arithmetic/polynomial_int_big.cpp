#include "polynomial_int_big.h"


template<>
IntPolynomial<BigInt>::IntPolynomial(
    const PolynomialParameters<BigInt> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    // coefs = alloc->newArray<BigInt>(params->N);
}

template<>
void IntPolynomial<BigInt>::destroy(
    const PolynomialParameters<BigInt> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    // alloc->deleteArray<BigInt>(params->N, coefs);
}


// Sets to zero
template<>
void IntPolynomial<BigInt>::Clear(
    IntPolynomial<BigInt> *poly,
    const PolynomialParameters<BigInt> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // for (int32_t i = 0; i < N; ++i)
    //     poly->coefs[i] = 0;
}



// copy
template<>
void IntPolynomial<BigInt>::Copy(
    IntPolynomial<BigInt> *result,
    const IntPolynomial<BigInt> *source,
    const PolynomialParameters<BigInt> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // assert(result != source);

    // for (int32_t i = 0; i < N; ++i)
    //     result->coefs[i] = source->coefs[i];
}

// accum += source
template<>
void IntPolynomial<BigInt>::AddTo(
    IntPolynomial<BigInt> *accum,
    const IntPolynomial<BigInt> *source,
    const PolynomialParameters<BigInt> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;

    // for (int32_t i = 0; i < N; ++i)
    //     accum->coefs[i] += source->coefs[i];
}

// result = (X^ai-1) * source
template<>
void IntPolynomial<BigInt>::MulByXaiMinusOne(
    IntPolynomial<BigInt> *result,
    const BigInt ai,
    const IntPolynomial<BigInt> *source,
    const PolynomialParameters<BigInt> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // BigInt *out = result->coefs;
    // BigInt *in = source->coefs;

    // assert(ai >= 0 && ai < 2 * N);

    // if (ai < N) {
    //     for (int32_t i = 0; i < ai; i++)//sur que i-a<0
    //         out[i] = -in[i - ai + N] - in[i];
    //     for (int32_t i = ai; i < N; i++)//sur que N>i-a>=0
    //         out[i] = in[i - ai] - in[i];
    // } else {
    //     const int32_t aa = ai - N;
    //     for (int32_t i = 0; i < aa; i++)//sur que i-a<0
    //         out[i] = in[i - aa + N] - in[i];
    //     for (int32_t i = aa; i < N; i++)//sur que N>i-a>=0
    //         out[i] = -in[i - aa] - in[i];
    // }
}

// Euclidean norm of an IntPolynomial
template<>
double IntPolynomial<BigInt>::Norm2sq(
    const IntPolynomial<BigInt> *poly,
    const PolynomialParameters<BigInt> *params,
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
    const PolynomialParameters<BigInt> *params,
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


