#ifndef FIXED_POINT_PARAM_H
#define FIXED_POINT_PARAM_H

#include "tfhe_core.h"
#include "zmodule_param.h"

/**
 * @brief params of fixed point real numbers
 */
template<typename TORUS>
class RealParams { };

/**
 * @brief specialization for "big" number classes
 * @details store number of integral and fractional limbs
 */
template<>
class RealParams<BigTorus> {
public:
    const ZModuleParams<BigTorus> *const zparams;

    const int nbLimbsInt;
    const int nbLimbsFrac;
    const int nbLimbs;

    const int nbBitsInt;
    const int nbBitsFrac;
    const int nbBits;

    /**
     * @brief Fix point real number parameters constructor
     *
     * @param zparams Z-module parameters
     * @param nbLimbsInt number of limbs for the integral part
     */
    RealParams(const ZModuleParams<BigTorus> *const zparams, const int nbLimbsInt = 1);

    void destroy();

    PREVENT_STACK_COPY(RealParams);
};

#endif // FIXED_POINT_PARAM_H
