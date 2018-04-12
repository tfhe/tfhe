#include "polynomial.h"

/**
 * Instantiate Polynomial class for big torus and int types
 */
template
struct Polynomial<BigTorus, CoefTypeEnum::Torus>;
template
struct Polynomial<BigTorus, CoefTypeEnum::Integer>;

/**
 * Constructor/destructor specialization for BigTorus type
 */
template<>
Polynomial<BigTorus, CoefTypeEnum::Torus>::Polynomial(
        const PolynomialParams<BigTorus> *params,
        TfheThreadContext *context,
        Allocator *alloc) {
    const ZModuleParams<BigTorus> *const zparams =
            params->zmodule_params;
    coefs = alloc->newArray<BigTorus>(params->N, zparams, alloc);
}

template<>
void Polynomial<BigTorus, CoefTypeEnum::Torus>::destroy(
        const PolynomialParams<BigTorus> *params,
        TfheThreadContext *context,
        Allocator *alloc) {
    const ZModuleParams<BigTorus> *const zparams =
            params->zmodule_params;
    alloc->deleteArray<BigTorus>(params->N, coefs, zparams, alloc);
}

/**
 * Constructor/destructor specialization for BigInt type
 */
template<>
Polynomial<BigTorus, CoefTypeEnum::Integer>::Polynomial(
        const PolynomialParams<BigTorus> *params,
        TfheThreadContext *context,
        Allocator *alloc) {
    coefs = alloc->newArray<BigInt>(params->N);
}

template<>
void Polynomial<BigTorus, CoefTypeEnum::Integer>::destroy(
        const PolynomialParams<BigTorus> *params,
        TfheThreadContext *context,
        Allocator *alloc) {
    alloc->deleteArray<BigInt>(params->N, coefs);
}


template<typename TORUS, CoefTypeEnum CoefType>
void Polynomial<TORUS, CoefType>::Clear(
        Polynomial<TORUS, CoefType> *result,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t N = params->N;
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    for (int32_t i = 0; i < N; ++i)
        zero(result->coefs + i, zparams);
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
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    for (int32_t i = 0; i < N; ++i)
        copy(r + i, s + i, zparams);
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
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    for (int32_t i = 0; i < N; ++i)
        add(r + i, a + i, b + i, zparams);
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
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    for (int32_t i = 0; i < N; ++i)
        add(r + i, r + i, s + i, zparams);
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
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    for (int32_t i = 0; i < N; ++i)
        sub(r + i, a + i, b + i, zparams);
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
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    for (int32_t i = 0; i < N; ++i)
        sub(r + i, r + i, s + i, zparams);
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
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    if (a < N) {
        // i-a < 0
        for (int32_t i = 0; i < a; i++) {
            copy(out + i, in + (i - a + N), zparams);
            neg(out + i, out + i, zparams);
            sub(out + i, out + i, in + i, zparams);
        }
        // N > i-a >= 0
        for (int32_t i = a; i < N; i++) {
            copy(out + i, in + (i - a), zparams);
            sub(out + i, out + i, in + i, zparams);
        }
    } else {
        const int32_t aa = a - N;
        //sur que i-a<0
        for (int32_t i = 0; i < aa; i++) {
            copy(out + i, in + (i - aa + N), zparams);
            sub(out + i, out + i, in + i, zparams);
        }
        //sur que N>i-a>=0
        for (int32_t i = aa; i < N; i++) {
            copy(out + i, in + (i - aa), zparams);
            neg(out + i, out + i, zparams);
            sub(out + i, out + i, in + i, zparams);
        }
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
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    if (a < N) {
        //sur que i-a<0
        for (int32_t i = 0; i < a; i++) {
            copy(out + i, in + (i - a + N), zparams);
            neg(out + i, out + i, zparams);
        }
        //sur que N>i-a>=0
        for (int32_t i = a; i < N; i++) {
            copy(out + i, in + (i - a), zparams);
        }
    } else {
        const int32_t aa = a - N;
        //sur que i-a<0
        for (int32_t i = 0; i < aa; i++) {
            copy(out + i, in + (i - aa + N), zparams);
        }
        //sur que N>i-a>=0
        for (int32_t i = aa; i < N; i++) {
            copy(out + i, in + (i - a), zparams);
            neg(out + i, out + i, zparams);
        }
    }
}


