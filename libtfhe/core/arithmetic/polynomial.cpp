#include "polynomial.h"
#include "torus.h"

/**
 * Instantiate Polynomial class for native torus and int types
 */
EXPLICIT_INSTANTIATE_CLASS(Polynomial, Torus32, TorusUtils<Torus32>::INT_TYPE);
EXPLICIT_INSTANTIATE_CLASS(Polynomial, Torus64, TorusUtils<Torus64>::INT_TYPE);
// EXPLICIT_INSTANTIATE_CLASS(Polynomial, Torus32, Torus32);   // native torus and int types are the same => don't need both
// EXPLICIT_INSTANTIATE_CLASS(Polynomial, Torus64, Torus64);

template<typename TORUS, typename TYPE>
Polynomial<TORUS,TYPE>::Polynomial(
    const PolynomialParams<TORUS> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    coefs = alloc->newArray<TYPE>(params->N);
}

template<typename TORUS, typename TYPE>
void Polynomial<TORUS,TYPE>::destroy(
    const PolynomialParams<TORUS> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    alloc->deleteArray<TYPE>(params->N, coefs);
}

template<typename TORUS, typename TYPE>
void Polynomial<TORUS,TYPE>::Clear(
    Polynomial<TORUS,TYPE> *result,
    const PolynomialParams<TORUS> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    for (int32_t i = 0; i < N; ++i)
        result->coefs[i] = 0;
}

template<typename TORUS, typename TYPE>
void Polynomial<TORUS,TYPE>::Copy(
    Polynomial<TORUS,TYPE> *result,
    const Polynomial<TORUS,TYPE> *source,
    const PolynomialParams<TORUS> *params,
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

template<typename TORUS, typename TYPE>
void Polynomial<TORUS,TYPE>::Add(
    Polynomial<TORUS,TYPE> *result,
    const Polynomial<TORUS,TYPE> *poly1,
    const Polynomial<TORUS,TYPE> *poly2,
    const PolynomialParams<TORUS> *params,
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

template<typename TORUS, typename TYPE>
void Polynomial<TORUS,TYPE>::AddTo(
    Polynomial<TORUS,TYPE> *result,
    const Polynomial<TORUS,TYPE> *poly,
    const PolynomialParams<TORUS> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    const TYPE *s = poly->coefs;
    TYPE *r = result->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] += s[i];
}

template<typename TORUS, typename TYPE>
void Polynomial<TORUS,TYPE>::Sub(
    Polynomial<TORUS,TYPE> *result,
    const Polynomial<TORUS,TYPE> *poly1,
    const Polynomial<TORUS,TYPE> *poly2,
    const PolynomialParams<TORUS> *params,
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

template<typename TORUS, typename TYPE>
void Polynomial<TORUS,TYPE>::SubTo(
    Polynomial<TORUS,TYPE> *result,
    const Polynomial<TORUS,TYPE> *poly,
    const PolynomialParams<TORUS> *params,
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

template<typename TORUS, typename TYPE>
void Polynomial<TORUS,TYPE>::MulByXaiMinusOne(
    Polynomial<TORUS,TYPE> *result,
    const int32_t a,
    const Polynomial<TORUS,TYPE> *source,
    const PolynomialParams<TORUS> *params,
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

template<typename TORUS, typename TYPE>
void Polynomial<TORUS,TYPE>::MulByXai(
    Polynomial<TORUS,TYPE> *result,
    const int32_t a,
    const Polynomial<TORUS,TYPE> *source,
    const PolynomialParams<TORUS> *params,
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
