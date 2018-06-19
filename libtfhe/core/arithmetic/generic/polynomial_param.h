#ifndef POLYNOMIAL_PARAM_H
#define POLYNOMIAL_PARAM_H

#include "core/arithmetic/big_int.h"
#include "core/arithmetic/big_torus.h"
#include "core/arithmetic/zmodule_param.h"
#include "asm_mode.h"
#include "zmodule_vector_params.h"

#include <cstdint>
#include <type_traits>

/**
 * @brief Polynomial parameters class
 *
 * @tparam TORUS torus type
 */
template<typename TORUS, AsmTypeEnum AsmType>
class PolynomialParams : public ZModuleVectorParams<TORUS, AsmType> {
public:
};

#endif //POLYNOMIAL_PARAM_H
