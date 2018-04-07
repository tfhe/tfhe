#include "polynomial.h"

/**
 * Instantiate Polynomial class for big torus and int types
 */
template struct Polynomial<BigTorus>;
template struct Polynomial<BigInt>;


/**
 * Constructor/destructor specialization for BigTorus type
 */
template<>
Polynomial<BigTorus>::Polynomial(
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    coefs = alloc->newArray<BigTorus>(params->N, params->zmodule_params, alloc);
}

template<>
void Polynomial<BigTorus>::destroy(
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    alloc->deleteArray<BigTorus>(params->N, coefs, params->zmodule_params, alloc);
}

/**
 * Constructor/destructor specialization for BigInt type
 */
template<>
Polynomial<BigInt>::Polynomial(
    const PolynomialParams<BigInt> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    coefs = alloc->newArray<BigInt>(params->N);
}

template<>
void Polynomial<BigInt>::destroy(
    const PolynomialParams<BigInt> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    alloc->deleteArray<BigInt>(params->N, coefs);
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
        zero(result->coefs + i, params->zmodule_params);
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
        copy(r+i, s+i, params->zmodule_params);
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
        add(r+i, a+i, b+i, params->zmodule_params);
}

template<typename TYPE>
void Polynomial<TYPE>::AddTo(
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
        add(r+i, r+i, s+i, params->zmodule_params);
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
        sub(r+i, a+i, b+i, params->zmodule_params);
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
        sub(r+i, r+i, s+i, params->zmodule_params);
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
    TYPE *out = result->coefs;
    const TYPE *in = source->coefs;
    const auto *zparams =
        params->zmodule_params;

    assert(a >= 0 && a < 2 * N);

    if (a < N) {
        // i-a < 0
        for (int32_t i = 0; i < a; i++) {
            // out[i] = -in[i - a + N] - in[i];
            copy(out+i, in+(i - a + N), zparams);
            neg(out+i, out+i, zparams);
            sub(out+i, out+i, in+i, zparams);
        }
        // N > i-a >= 0
        for (int32_t i = a; i < N; i++) {
            // out[i] = in[i - a] - in[i];
            copy(out+i, in+(i - a), zparams);
            sub(out+i, out+i, in+i, zparams);
        }
    } else {
        const int32_t aa = a - N;
        //sur que i-a<0
        for (int32_t i = 0; i < aa; i++) {
            // out[i] = in[i - aa + N] - in[i];
            copy(out+i, in+(i - aa + N), zparams);
            sub(out+i, out+i, in+i, zparams);
        }
        //sur que N>i-a>=0
        for (int32_t i = aa; i < N; i++) {
            // out[i] = -in[i - aa] - in[i];
            copy(out+i, in+(i - aa), zparams);
            neg(out+i, out+i, zparams);
            sub(out+i, out+i, in+i, zparams);
        }
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
    TYPE *out = result->coefs;
    const TYPE *in = source->coefs;
    const auto *zparams =
        params->zmodule_params;

    assert(a >= 0 && a < 2 * N);
    assert(result != source);

    if (a < N) {
        //sur que i-a<0
        for (int32_t i = 0; i < a; i++) {
            // out[i] = -in[i - a + N];
            copy(out+i, in+(i - a + N), zparams);
            neg(out+i, out+i, zparams);
        }
        //sur que N>i-a>=0
        for (int32_t i = a; i < N; i++) {
            // out[i] = in[i - a];
            copy(out+i, in+(i - a), zparams);
        }
    } else {
        const int32_t aa = a - N;
        //sur que i-a<0
        for (int32_t i = 0; i < aa; i++) {
            // out[i] = in[i - aa + N];
            copy(out+i, in+(i - aa + N), zparams);
        }
        //sur que N>i-a>=0
        for (int32_t i = aa; i < N; i++) {
            // out[i] = -in[i - aa];
            copy(out+i, in+(i - a), zparams);
            neg(out+i, out+i, zparams);
        }
    }
}


