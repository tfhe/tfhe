#include "polynomial.h"
#include "torus.h"

/**
 * Instantiate Polynomial class for native torus and int types
 */
EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_TORUS(Polynomial);
// EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_INT(Polynomial); // native torus and int types are the same => don't need both

template<typename TYPE>
Polynomial<TYPE>::Polynomial(
    const PolynomialParams<TYPE> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    coefs = alloc->newArray<TYPE>(params->N);
}

template<typename TYPE>
void Polynomial<TYPE>::destroy(
    const PolynomialParams<TYPE> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    alloc->deleteArray<TYPE>(params->N, coefs);
}

template<typename TYPE>
void Polynomial<TYPE>::Clear(
    Polynomial<TYPE> *result,
    const PolynomialParams<TYPE> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    for (int32_t i = 0; i < N; ++i)
        result->coefs[i] = 0;
}

template<typename TYPE>
void Polynomial<TYPE>::Copy(
    Polynomial<TYPE> *result,
    const Polynomial<TYPE> *source,
    const PolynomialParams<TYPE> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    assert(result != source);

    const int32_t N = params->N;
    const TYPE *s = source->coefs;
    TYPE *r = result->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] = s[i];
}

template<typename TYPE>
void Polynomial<TYPE>::Add(
    Polynomial<TYPE> *result,
    const Polynomial<TYPE> *poly1,
    const Polynomial<TYPE> *poly2,
    const PolynomialParams<TYPE> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    assert(result != poly1); //if it fails here, please use addTo
    assert(result != poly2); //if it fails here, please use addTo

    const int32_t N = params->N;
    TYPE *r = result->coefs;
    const TYPE *a = poly1->coefs;
    const TYPE *b = poly1->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] = a[i] + b[i];
}

template<typename TYPE>
void Polynomial<TYPE>::AddTo(
    Polynomial<TYPE> *result,
    const Polynomial<TYPE> *poly,
    const PolynomialParams<TYPE> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    const TYPE *s = poly->coefs;
    TYPE *r = result->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] += s[i];
}

template<typename TYPE>
void Polynomial<TYPE>::Sub(
    Polynomial<TYPE> *result,
    const Polynomial<TYPE> *poly1,
    const Polynomial<TYPE> *poly2,
    const PolynomialParams<TYPE> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    assert(result != poly1); //if it fails here, please use addTo
    assert(result != poly2); //if it fails here, please use addTo

    const int32_t N = params->N;
    TYPE *r = result->coefs;
    const TYPE *a = poly1->coefs;
    const TYPE *b = poly1->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] = a[i] - b[i];
}

template<typename TYPE>
void Polynomial<TYPE>::SubTo(
    Polynomial<TYPE> *result,
    const Polynomial<TYPE> *poly,
    const PolynomialParams<TYPE> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    assert(result != poly);

    const int32_t N = params->N;
    const TYPE *s = poly->coefs;
    TYPE *r = result->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] -= s[i];
}

template<typename TYPE>
void Polynomial<TYPE>::MulByXaiMinusOne(
    Polynomial<TYPE> *result,
    const int32_t a,
    const Polynomial<TYPE> *source,
    const PolynomialParams<TYPE> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    assert(result != source);
    assert(a >= 0 && a < 2 * N);

    TYPE *out = result->coefs;
    const TYPE *in = source->coefs;

    if (a < N) {
        for (int32_t i = 0; i < a; i++) // i-a < 0
            out[i] = -in[i - a + N] - in[i];
        for (int32_t i = a; i < N; i++) // N > i-a >= 0
            out[i] = in[i - a] - in[i];
    } else {
        const int32_t aa = a - N;
        for (int32_t i = 0; i < aa; i++)//sur que i-a<0
            out[i] = in[i - aa + N] - in[i];
        for (int32_t i = aa; i < N; i++)//sur que N>i-a>=0
            out[i] = -in[i - aa] - in[i];
    }
}

template<typename TYPE>
void Polynomial<TYPE>::MulByXai(
    Polynomial<TYPE> *result,
    const int32_t a,
    const Polynomial<TYPE> *source,
    const PolynomialParams<TYPE> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    assert(result != source);
    assert(a >= 0 && a < 2 * N);

    TYPE *out = result->coefs;
    const TYPE *in = source->coefs;

    if (a < N) {
        for (int32_t i = 0; i < a; i++)//sur que i-a<0
            out[i] = -in[i - a + N];
        for (int32_t i = a; i < N; i++)//sur que N>i-a>=0
            out[i] = in[i - a];
    } else {
        const int32_t aa = a - N;
        for (int32_t i = 0; i < aa; i++)//sur que i-a<0
            out[i] = in[i - aa + N];
        for (int32_t i = aa; i < N; i++)//sur que N>i-a>=0
            out[i] = -in[i - aa];
    }
}
