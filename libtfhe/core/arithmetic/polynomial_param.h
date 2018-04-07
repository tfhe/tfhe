#ifndef POLYNOMIAL_PARAM_H
#define POLYNOMIAL_PARAM_H

#include "big_int.h"
#include "big_torus.h"
#include "zmodule_param.h"

#include <cstdint>
#include <type_traits>

/**
 * @brief Polynomial parameters class
 *
 * @tparam COEF_TYPE polynomial coefficient type
 */
template<typename COEF_TYPE>
class PolynomialParams {
private:
    using TORUS = typename std::conditional<std::is_same<COEF_TYPE, BigInt>::value, BigTorus, COEF_TYPE>::type;

public:
    int32_t N;
    ZModuleParams<TORUS>* zmodule_params;
};

#endif //POLYNOMIAL_PARAM_H
