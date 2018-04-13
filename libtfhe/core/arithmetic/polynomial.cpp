#include "polynomial.h"
#include "torus.h"

/**
 * Instantiate Polynomial class for native torus and int types
 */
EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_TORUS(Polynomial, CoefTypeEnum::Torus);

EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_TORUS(Polynomial, CoefTypeEnum::Integer);

template<typename TORUS, CoefTypeEnum CoefType>
Polynomial<TORUS, CoefType>::Polynomial(
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator *alloc) {
    coefs = alloc->newArray<TYPE>(params->N);
}

template<typename TORUS, CoefTypeEnum CoefType>
void Polynomial<TORUS, CoefType>::destroy(
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator *alloc) {
    alloc->deleteArray<TYPE>(params->N, coefs);
}

template<typename TORUS, CoefTypeEnum CoefType>
void Polynomial<TORUS, CoefType>::Clear(
        Polynomial<TORUS, CoefType> *result,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t N = params->N;
    for (int32_t i = 0; i < N; ++i)
        result->coefs[i] = 0;
}

template<typename TORUS, CoefTypeEnum CoefType>
void Polynomial<TORUS, CoefType>::Copy(
        Polynomial<TORUS, CoefType> *result,
        const Polynomial<TORUS, CoefType> *source,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    assert(result != source);

    const int32_t N = params->N;
    const TYPE *s = source->coefs;
    TYPE *r = result->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] = s[i];
}

template<typename TORUS, CoefTypeEnum CoefType>
void Polynomial<TORUS, CoefType>::Add(
        Polynomial<TORUS, CoefType> *result,
        const Polynomial<TORUS, CoefType> *poly1,
        const Polynomial<TORUS, CoefType> *poly2,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    assert(result != poly1); //if it fails here, please use addTo
    assert(result != poly2); //if it fails here, please use addTo

    const int32_t N = params->N;
    TYPE *r = result->coefs;
    const TYPE *a = poly1->coefs;
    const TYPE *b = poly1->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] = a[i] + b[i];
}

template<typename TORUS, CoefTypeEnum CoefType>
void Polynomial<TORUS, CoefType>::AddTo(
        Polynomial<TORUS, CoefType> *result,
        const Polynomial<TORUS, CoefType> *poly,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t N = params->N;
    const TYPE *s = poly->coefs;
    TYPE *r = result->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] += s[i];
}

template<typename TORUS, CoefTypeEnum CoefType>
void Polynomial<TORUS, CoefType>::Sub(
        Polynomial<TORUS, CoefType> *result,
        const Polynomial<TORUS, CoefType> *poly1,
        const Polynomial<TORUS, CoefType> *poly2,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    assert(result != poly1); //if it fails here, please use addTo
    assert(result != poly2); //if it fails here, please use addTo

    const int32_t N = params->N;
    TYPE *r = result->coefs;
    const TYPE *a = poly1->coefs;
    const TYPE *b = poly1->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] = a[i] - b[i];
}

template<typename TORUS, CoefTypeEnum CoefType>
void Polynomial<TORUS, CoefType>::SubTo(
        Polynomial<TORUS, CoefType> *result,
        const Polynomial<TORUS, CoefType> *poly,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    assert(result != poly);

    const int32_t N = params->N;
    const TYPE *s = poly->coefs;
    TYPE *r = result->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] -= s[i];
}

template<typename TORUS, CoefTypeEnum CoefType>
void Polynomial<TORUS, CoefType>::MulByXaiMinusOne(
        Polynomial<TORUS, CoefType> *result,
        const int32_t a,
        const Polynomial<TORUS, CoefType> *source,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc) {
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

template<typename TORUS, CoefTypeEnum CoefType>
void Polynomial<TORUS, CoefType>::MulByXai(
        Polynomial<TORUS, CoefType> *result,
        const int32_t a,
        const Polynomial<TORUS, CoefType> *source,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc) {
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
