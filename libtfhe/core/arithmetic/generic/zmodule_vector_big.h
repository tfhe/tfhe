#ifndef TFHE_ZMODULE_VECTOR_BIG_H
#define TFHE_ZMODULE_VECTOR_BIG_H

#include "zmodule_vector.h"

#ifdef TFHE_ZMODULE_VECTOR_PRIMITIVE_H
#error "zmodule_vector_primitive.h and zmodule_vector_big.h are incompatible and cannot be included at the same time"
#endif

template<typename TORUS, CoefTypeEnum CoefType, AsmTypeEnum AsmType>
ZModuleVector<TORUS, CoefType, AsmType>::ZModuleVector(const ZModuleVectorParams<TORUS, AsmType> *params,
                                                       TfheThreadContext *context, Allocator *alloc) {
    this->coefs = alloc->newArray<TYPE>(params->N, params->zmodule_params, alloc);
}

template<typename TORUS, CoefTypeEnum CoefType, AsmTypeEnum AsmType>
void ZModuleVector<TORUS, CoefType, AsmType>::destroy(const ZModuleVectorParams<TORUS, AsmType> *params,
                                                      TfheThreadContext *context, Allocator *alloc) {
    alloc->deleteArray<TYPE>(params->N, this->coefs, params->zmodule_params, alloc);
}

template<typename TORUS, CoefTypeEnum CoefType, AsmTypeEnum AsmType>
void ZModuleVector<TORUS, CoefType, AsmType>::Clear(ZModuleVector<TORUS, CoefType, AsmType> *result,
                                                    const ZModuleVectorParams<TORUS, AsmType> *params,
                                                    TfheThreadContext *context, Allocator alloc) {
    const int32_t N = params->N;
    const ZModuleParams<TORUS> *const zmodule_params = params->zmodule_params;
    TYPE *const reps = result->coefs;
    for (int32_t i = 0; i < N; i++) {
        tfhe_backend::zero(reps + i, zmodule_params);
    }
}

template<typename TORUS, CoefTypeEnum CoefType, AsmTypeEnum AsmType>
void ZModuleVector<TORUS, CoefType, AsmType>::Copy(ZModuleVector<TORUS, CoefType, AsmType> *result,
                                                   const ZModuleVector<TORUS, CoefType, AsmType> *source,
                                                   const ZModuleVectorParams<TORUS, AsmType> *params,
                                                   TfheThreadContext *context, Allocator alloc) {
    const int32_t N = params->N;
    const ZModuleParams<TORUS> *const zmodule_params = params->zmodule_params;
    TYPE *const reps = result->coefs;
    const TYPE *const src = source->coefs;
    for (int32_t i = 0; i < N; i++) {
        tfhe_backend::copy(reps + i, src + i, zmodule_params);
    }
}

template<typename TORUS, CoefTypeEnum CoefType, AsmTypeEnum AsmType>
void ZModuleVector<TORUS, CoefType, AsmType>::Add(ZModuleVector<TORUS, CoefType, AsmType> *result,
                                                  const ZModuleVector<TORUS, CoefType, AsmType> *vec1,
                                                  const ZModuleVector<TORUS, CoefType, AsmType> *vec2,
                                                  const ZModuleVectorParams<TORUS, AsmType> *params,
                                                  TfheThreadContext *context, Allocator alloc) {
    const int32_t N = params->N;
    const ZModuleParams<TORUS> *const zmodule_params = params->zmodule_params;
    TYPE *const reps = result->coefs;
    const TYPE *const a = vec1->coefs;
    const TYPE *const b = vec2->coefs;
    for (int32_t i = 0; i < N; i++) {
        tfhe_backend::add(reps + i, a + i, b + i, zmodule_params);
    }
}

template<typename TORUS, CoefTypeEnum CoefType, AsmTypeEnum AsmType>
void ZModuleVector<TORUS, CoefType, AsmType>::Sub(ZModuleVector<TORUS, CoefType, AsmType> *result,
                                                  const ZModuleVector<TORUS, CoefType, AsmType> *vec1,
                                                  const ZModuleVector<TORUS, CoefType, AsmType> *vec2,
                                                  const ZModuleVectorParams<TORUS, AsmType> *params,
                                                  TfheThreadContext *context, Allocator alloc) {
    const int32_t N = params->N;
    const ZModuleParams<TORUS> *const zmodule_params = params->zmodule_params;
    TYPE *const reps = result->coefs;
    const TYPE *const a = vec1->coefs;
    const TYPE *const b = vec2->coefs;
    for (int32_t i = 0; i < N; i++) {
        tfhe_backend::sub(reps + i, a + i, b + i, zmodule_params);
    }
}

template<typename TORUS, CoefTypeEnum CoefType, AsmTypeEnum AsmType>
void ZModuleVector<TORUS, CoefType, AsmType>::AddTo(ZModuleVector<TORUS, CoefType, AsmType> *result,
                                                    const ZModuleVector<TORUS, CoefType, AsmType> *vec2,
                                                    const ZModuleVectorParams<TORUS, AsmType> *params,
                                                    TfheThreadContext *context, Allocator alloc) {
    const int32_t N = params->N;
    const ZModuleParams<TORUS> *const zmodule_params = params->zmodule_params;
    TYPE *const reps = result->coefs;
    const TYPE *const b = vec2->coefs;
    for (int32_t i = 0; i < N; i++) {
        tfhe_backend::add(reps + i, reps + i, b + i, zmodule_params);
    }

}

template<typename TORUS, CoefTypeEnum CoefType, AsmTypeEnum AsmType>
void ZModuleVector<TORUS, CoefType, AsmType>::SubTo(ZModuleVector<TORUS, CoefType, AsmType> *result,
                                                    const ZModuleVector<TORUS, CoefType, AsmType> *vec2,
                                                    const ZModuleVectorParams<TORUS, AsmType> *params,
                                                    TfheThreadContext *context, Allocator alloc) {
    const int32_t N = params->N;
    const ZModuleParams<TORUS> *const zmodule_params = params->zmodule_params;
    TYPE *const reps = result->coefs;
    const TYPE *const b = vec2->coefs;
    for (int32_t i = 0; i < N; i++) {
        tfhe_backend::sub(reps + i, reps + i, b + i, zmodule_params);
    }
}


#endif // TFHE_ZMODULE_VECTOR_BIG_H
