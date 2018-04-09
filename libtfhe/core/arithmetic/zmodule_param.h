#ifndef ZMODULE_PARAM_H
#define ZMODULE_PARAM_H

#include "tfhe_core.h"

/**
 * @brief params of the Z-module
 */
template<typename TORUS>
class ZModuleParams { };

/**
 * @brief specialization for "big" number classes
 * @details Namely, the maximum number of limbs, and the number of bits p of the modulus.
 * (every operation is modulo 2^p)
 */
template<>
class ZModuleParams<BigTorus> {
public:
    const int max_nbLimbs;
    const int p;

    ZModuleParams(const int max_nbLimbs);

    void destroy();

    PREVENT_STACK_COPY(ZModuleParams);
};

#endif // ZMODULE_PARAM_H
