#ifndef TFHE_ZMODULE_VECTOR_PARAMS_H
#define TFHE_ZMODULE_VECTOR_PARAMS_H

#include <cstdint>
#include <core/arithmetic/zmodule_param.h>
#include <core/arithmetic/torus.h>
#include "asm_mode.h"

/**
 * @brief Z-module vector parameters class
 *
 * @tparam TORUS torus type
 */
template<typename TORUS, AsmTypeEnum AsmType>
class ZModuleVectorParams {
public:
    const int32_t N; ///< size of the vector
    const ZModuleParams<TORUS> *const zmodule_params;
};

/**
 * @brief Z-module vector parameters class, specialization for Torus32
 */
template<>
class ZModuleVectorParams<Torus32, AsmTypeEnum::AVX2> {
public:
    const int32_t N; ///< size of the vector
    const int32_t numAVX256; ///< number of AVX256blocks
    const ZModuleParams<Torus32> *const zmodule_params;
};

/**
 * @brief Z-module vector parameters class, specialization for Torus64
 */
template<>
class ZModuleVectorParams<Torus64, AsmTypeEnum::AVX2> {
public:
    const int32_t N; ///< size of the vector
    const int32_t numAVX256; ///< number of AVX256blocks
    const ZModuleParams<Torus64> *const zmodule_params;
};


#endif //TFHE_ZMODULE_VECTOR_PARAMS_H
