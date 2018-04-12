#include "polynomial.h"

/**
 * Instantiate Polynomial class for big torus and int types
 */
template struct Polynomial<BigTorus,BigTorus>;
template struct Polynomial<BigTorus,BigInt>;

/**
 * Constructor/destructor specialization for BigTorus type
 */
template<>
Polynomial<BigTorus,BigTorus>::Polynomial(
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    const ZModuleParams<BigTorus> *const zparams =
        params->zmodule_params;
    coefs = alloc->newArray<BigTorus>(params->N, zparams, alloc);
}

template<>
void Polynomial<BigTorus,BigTorus>::destroy(
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    const ZModuleParams<BigTorus> *const zparams =
        params->zmodule_params;
    alloc->deleteArray<BigTorus>(params->N, coefs, zparams, alloc);
}

/**
 * Constructor/destructor specialization for BigInt type
 */
template<>
Polynomial<BigTorus,BigInt>::Polynomial(
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    coefs = alloc->newArray<BigInt>(params->N);
}

template<>
void Polynomial<BigTorus,BigInt>::destroy(
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    alloc->deleteArray<BigInt>(params->N, coefs);
}


template<typename TORUS, typename COEF_TYPE>
void Polynomial<TORUS,COEF_TYPE>::Clear(
    Polynomial<TORUS,COEF_TYPE> *result,
    const PolynomialParams<TORUS> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    const ZModuleParams<TORUS> *const zparams =
        params->zmodule_params;

    for (int32_t i = 0; i < N; ++i)
        zero(result->coefs + i, zparams);
}

template<typename TORUS, typename COEF_TYPE>
void Polynomial<TORUS,COEF_TYPE>::Copy(
    Polynomial<TORUS,COEF_TYPE> *result,
    const Polynomial<TORUS,COEF_TYPE> *source,
    const PolynomialParams<TORUS> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    assert(result != source);

    const int32_t N = params->N;
    const COEF_TYPE *s = source->coefs;
    COEF_TYPE *r = result->coefs;
    const ZModuleParams<TORUS> *const zparams =
        params->zmodule_params;

    for (int32_t i = 0; i < N; ++i)
        copy(r+i, s+i, zparams);
}

template<typename TORUS, typename COEF_TYPE>
void Polynomial<TORUS,COEF_TYPE>::Add(
    Polynomial<TORUS,COEF_TYPE> *result,
    const Polynomial<TORUS,COEF_TYPE> *poly1,
    const Polynomial<TORUS,COEF_TYPE> *poly2,
    const PolynomialParams<TORUS> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    assert(result != poly1); //if it fails here, please use addTo
    assert(result != poly2); //if it fails here, please use addTo

    const int32_t N = params->N;
    COEF_TYPE *r = result->coefs;
    const COEF_TYPE *a = poly1->coefs;
    const COEF_TYPE *b = poly1->coefs;
    const ZModuleParams<TORUS> *const zparams =
        params->zmodule_params;

    for (int32_t i = 0; i < N; ++i)
        add(r+i, a+i, b+i, zparams);
}

template<typename TORUS, typename COEF_TYPE>
void Polynomial<TORUS,COEF_TYPE>::AddTo(
    Polynomial<TORUS,COEF_TYPE> *result,
    const Polynomial<TORUS,COEF_TYPE> *poly,
    const PolynomialParams<TORUS> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    const COEF_TYPE *s = poly->coefs;
    COEF_TYPE *r = result->coefs;
    const ZModuleParams<TORUS> *const zparams =
        params->zmodule_params;

    for (int32_t i = 0; i < N; ++i)
        add(r+i, r+i, s+i, zparams);
}

template<typename TORUS, typename COEF_TYPE>
void Polynomial<TORUS,COEF_TYPE>::Sub(
    Polynomial<TORUS,COEF_TYPE> *result,
    const Polynomial<TORUS,COEF_TYPE> *poly1,
    const Polynomial<TORUS,COEF_TYPE> *poly2,
    const PolynomialParams<TORUS> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    assert(result != poly1); //if it fails here, please use addTo
    assert(result != poly2); //if it fails here, please use addTo

    const int32_t N = params->N;
    COEF_TYPE *r = result->coefs;
    const COEF_TYPE *a = poly1->coefs;
    const COEF_TYPE *b = poly1->coefs;
    const ZModuleParams<TORUS> *const zparams =
        params->zmodule_params;

    for (int32_t i = 0; i < N; ++i)
        sub(r+i, a+i, b+i, zparams);
}

template<typename TORUS, typename COEF_TYPE>
void Polynomial<TORUS,COEF_TYPE>::SubTo(
    Polynomial<TORUS,COEF_TYPE> *result,
    const Polynomial<TORUS,COEF_TYPE> *poly,
    const PolynomialParams<TORUS> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    assert(result != poly);

    const int32_t N = params->N;
    const COEF_TYPE *s = poly->coefs;
    COEF_TYPE *r = result->coefs;
    const ZModuleParams<TORUS> *const zparams =
        params->zmodule_params;

    for (int32_t i = 0; i < N; ++i)
        sub(r+i, r+i, s+i, zparams);
}

template<typename TORUS, typename COEF_TYPE>
void Polynomial<TORUS,COEF_TYPE>::MulByXaiMinusOne(
    Polynomial<TORUS,COEF_TYPE> *result,
    const int32_t a,
    const Polynomial<TORUS,COEF_TYPE> *source,
    const PolynomialParams<TORUS> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    assert(result != source);
    assert(a >= 0 && a < 2 * N);

    COEF_TYPE *out = result->coefs;
    const COEF_TYPE *in = source->coefs;
    const ZModuleParams<TORUS> *const zparams =
        params->zmodule_params;

    if (a < N) {
        // i-a < 0
        for (int32_t i = 0; i < a; i++) {
            copy(out+i, in+(i - a + N), zparams);
            neg(out+i, out+i, zparams);
            sub(out+i, out+i, in+i, zparams);
        }
        // N > i-a >= 0
        for (int32_t i = a; i < N; i++) {
            copy(out+i, in+(i - a), zparams);
            sub(out+i, out+i, in+i, zparams);
        }
    } else {
        const int32_t aa = a - N;
        //sur que i-a<0
        for (int32_t i = 0; i < aa; i++) {
            copy(out+i, in+(i - aa + N), zparams);
            sub(out+i, out+i, in+i, zparams);
        }
        //sur que N>i-a>=0
        for (int32_t i = aa; i < N; i++) {
            copy(out+i, in+(i - aa), zparams);
            neg(out+i, out+i, zparams);
            sub(out+i, out+i, in+i, zparams);
        }
    }
}

template<typename TORUS, typename COEF_TYPE>
void Polynomial<TORUS,COEF_TYPE>::MulByXai(
    Polynomial<TORUS,COEF_TYPE> *result,
    const int32_t a,
    const Polynomial<TORUS,COEF_TYPE> *source,
    const PolynomialParams<TORUS> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    assert(result != source);
    assert(a >= 0 && a < 2 * N);

    COEF_TYPE *out = result->coefs;
    const COEF_TYPE *in = source->coefs;
    const ZModuleParams<TORUS> *const zparams =
        params->zmodule_params;

    if (a < N) {
        //sur que i-a<0
        for (int32_t i = 0; i < a; i++) {
            copy(out+i, in+(i - a + N), zparams);
            neg(out+i, out+i, zparams);
        }
        //sur que N>i-a>=0
        for (int32_t i = a; i < N; i++) {
            copy(out+i, in+(i - a), zparams);
        }
    } else {
        const int32_t aa = a - N;
        //sur que i-a<0
        for (int32_t i = 0; i < aa; i++) {
            copy(out+i, in+(i - aa + N), zparams);
        }
        //sur que N>i-a>=0
        for (int32_t i = aa; i < N; i++) {
            copy(out+i, in+(i - a), zparams);
            neg(out+i, out+i, zparams);
        }
    }
}


