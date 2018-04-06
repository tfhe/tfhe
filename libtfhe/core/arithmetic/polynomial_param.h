#ifndef POLYNOMIAL_PARAM_H
#define POLYNOMIAL_PARAM_H

#include "BigInt.h"
#include "BigTorus.h"

#include <cstdint>

/**
 * @brief Polynomial parameters class
 * 
 * @tparam COEF_TYPE polynomial coefficient type
 */
template<typename COEF_TYPE>
class PolynomialParameters {
public:
    int32_t N;
    ZModuleParams<COEF_TYPE>* zmodule_params;
};

#endif //POLYNOMIAL_PARAM_H
