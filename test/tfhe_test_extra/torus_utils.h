#ifndef TORUS_UTILS_EXTRA_H
#define TORUS_UTILS_EXTRA_H

#include <core/arithmetic/torus_utils.h>

/**
 * Wrapper class to TorusUtils<TORUS> providing generic functions for unittests
 */


template<typename TORUS>
struct TorusUtilsExtra : public TorusUtils<TORUS> { };

template<>
struct TorusUtilsExtra<BigTorus> : public TorusUtils<BigTorus> {
    static double to_double(const TORUS &x, const ZModuleParams<TORUS> *params) {
        return TorusUtils<BigTorus>::to_double(&x, params);
    }

    static double normInftyDist(const TORUS &t1, const TORUS &t2, const ZModuleParams<TORUS> *params) {
        return TorusUtils<BigTorus>::normInftyDist(&t1, &t2, params);
    }
};

#endif //TORUS_UTILS_EXTRA_H
