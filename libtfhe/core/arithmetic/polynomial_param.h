#ifndef POLYNOMIAL_PARAM_H
#define POLYNOMIAL_PARAM_H

#include "BigInt.h"

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
};

/**
 * @brief PolynomialParameters specialization for big numbers
 */
template<>
class PolynomialParameters<BigInt> {
public:
    int32_t N;
    BigIntParams* big_int_params;
};

#endif //POLYNOMIAL_PARAM_H
