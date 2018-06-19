#include "../generic/zmodule_vector_big.h"

//specialization for BigInt, whose constructor has no argument
template<>
ZModuleVector<BigTorus, CoefTypeEnum::Integer, AsmTypeEnum::PORTABLE>::ZModuleVector(
        const ZModuleVectorParams<BigTorus, AsmTypeEnum::PORTABLE> *params,
        TfheThreadContext *context, Allocator *alloc) {
    this->coefs = alloc->newArray<TYPE>(params->N);
}

//specialization for BigInt, whose destroy has no argument
template<>
void ZModuleVector<BigTorus, CoefTypeEnum::Integer, AsmTypeEnum::PORTABLE>::destroy(
        const ZModuleVectorParams<BigTorus, AsmTypeEnum::PORTABLE> *params,
        TfheThreadContext *context, Allocator *alloc) {
    alloc->deleteArray<TYPE>(params->N, this->coefs);
}


EXPLICIT_INSTANTIATE_CLASS(ZModuleVector, BigTorus, CoefTypeEnum::Torus, AsmTypeEnum::PORTABLE);

EXPLICIT_INSTANTIATE_CLASS(ZModuleVector, BigTorus, CoefTypeEnum::Integer, AsmTypeEnum::PORTABLE);
