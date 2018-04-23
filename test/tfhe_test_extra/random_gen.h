#ifndef RANDOM_GEN_EXTRA_H
#define RANDOM_GEN_EXTRA_H

#include "core/arithmetic/random_gen.h"

/**
 * Wrapper class to RandomGenTorus<TORUS> providing generic functions for unittests
 */

template<typename TORUS>
struct RandomGenTorusExtra : public RandomGenTorus<TORUS> {
    static void uniform(
            TORUS &dst,
            const ZModuleParams<TORUS> *params) { dst = RandomGenTorus<TORUS>::uniform(params); }

    static void gaussian(
            TORUS &dst,
            const TORUS mean,
            const double sigma,
            const ZModuleParams<TORUS> *params) { dst = RandomGenTorus<TORUS>::gaussian(mean, sigma, params); }
};

template<>
struct RandomGenTorusExtra<BigTorus> : public RandomGenTorus<BigTorus> {
    static void uniform(
            TORUS &dst,
            const ZModuleParams<TORUS> *params) { RandomGenTorus<TORUS>::uniform(&dst, params); }

    static void gaussian(
            TORUS &dst,
            const TORUS &mean,
            const double sigma,
            const ZModuleParams<TORUS> *params) { RandomGenTorus<TORUS>::gaussian(&dst, &mean, sigma, params); }
};


#endif //RANDOM_GEN_EXTRA_H
