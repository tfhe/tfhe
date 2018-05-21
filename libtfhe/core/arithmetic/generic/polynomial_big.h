#ifndef TFHE_POLYNOMIAL_BIG_H
#define TFHE_POLYNOMIAL_BIG_H

#include "polynomial.h"

#ifdef TFHE_POLYNOMIAL_PRIMITIVE_H
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
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    if (a < N) {
        // i-a < 0
        for (int32_t i = 0; i < a; i++) {
            tfhe_backend::copy(out + i, in + (i - a + N), zparams);
            tfhe_backend::neg(out + i, out + i, zparams);
            tfhe_backend::sub(out + i, out + i, in + i, zparams);
        }
        // N > i-a >= 0
        for (int32_t i = a; i < N; i++) {
            tfhe_backend::copy(out + i, in + (i - a), zparams);
            tfhe_backend::sub(out + i, out + i, in + i, zparams);
        }
    } else {
        const int32_t aa = a - N;
        //sur que i-a<0
        for (int32_t i = 0; i < aa; i++) {
            tfhe_backend::copy(out + i, in + (i - aa + N), zparams);
            tfhe_backend::sub(out + i, out + i, in + i, zparams);
        }
        //sur que N>i-a>=0
        for (int32_t i = aa; i < N; i++) {
            tfhe_backend::copy(out + i, in + (i - aa), zparams);
            tfhe_backend::neg(out + i, out + i, zparams);
            tfhe_backend::sub(out + i, out + i, in + i, zparams);
        }
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
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    if (a < N) {
        //sur que i-a<0
        for (int32_t i = 0; i < a; i++) {
            tfhe_backend::copy(out + i, in + (i - a + N), zparams);
            tfhe_backend::neg(out + i, out + i, zparams);
        }
        //sur que N>i-a>=0
        for (int32_t i = a; i < N; i++) {
            tfhe_backend::copy(out + i, in + (i - a), zparams);
        }
    } else {
        const int32_t aa = a - N;
        //sur que i-a<0
        for (int32_t i = 0; i < aa; i++) {
            tfhe_backend::copy(out + i, in + (i - aa + N), zparams);
        }
        //sur que N>i-a>=0
        for (int32_t i = aa; i < N; i++) {
            tfhe_backend::copy(out + i, in + (i - a), zparams);
            tfhe_backend::neg(out + i, out + i, zparams);
        }
    }
}

#endif // TFHE_POLYNOMIAL_BIG_H
