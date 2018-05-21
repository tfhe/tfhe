#ifndef TFHE_POLYNOMIAL_PRIMITIVE_H
#define TFHE_POLYNOMIAL_PRIMITIVE_H

#include "polynomial.h"
#include "core/arithmetic/torus.h"

#ifdef TFHE_POLYNOMIAL_BIG_H
#error "polynomial_primitive.h and polynomial_big.h are incompatible"
#endif

template<typename TORUS, CoefTypeEnum CoefType, AsmTypeEnum AsmType>
Polynomial<TORUS, CoefType, AsmType>::Polynomial(
        const PolynomialParams<TORUS, AsmType> *params,
        TfheThreadContext *context,
        Allocator *alloc): ZModuleVector<TORUS, CoefType, AsmType>(params, context, alloc) {
}

template<typename TORUS, CoefTypeEnum CoefType, AsmTypeEnum AsmType>
void Polynomial<TORUS, CoefType, AsmType>::destroy(
        const PolynomialParams<TORUS, AsmType> *params,
        TfheThreadContext *context,
        Allocator *alloc) {
    ZModuleVector<TORUS, CoefType, AsmType>::destroy(params, context, alloc);
}


template<typename TORUS, CoefTypeEnum CoefType, AsmTypeEnum AsmType>
void Polynomial<TORUS, CoefType, AsmType>::MulByXaiMinusOne(
        Polynomial<TORUS, CoefType, AsmType> *result,
        const int32_t a,
        const Polynomial<TORUS, CoefType, AsmType> *source,
        const PolynomialParams<TORUS, AsmType> *params,
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

template<typename TORUS, CoefTypeEnum CoefType, AsmTypeEnum AsmType>
void Polynomial<TORUS, CoefType, AsmType>::MulByXai(
        Polynomial<TORUS, CoefType, AsmType> *result,
        const int32_t a,
        const Polynomial<TORUS, CoefType, AsmType> *source,
        const PolynomialParams<TORUS, AsmType> *params,
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


#endif // TFHE_POLYNOMIAL_PRIMITIVE_H
