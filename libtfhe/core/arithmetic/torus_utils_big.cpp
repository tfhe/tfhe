#include "torus_utils_big.h"

void TorusUtils<BigTorus>::from_double(TorusUtils<BigTorus>::TORUS *res, const double d,
                                       const ZModuleParams<BigTorus> *params) {
    tfhe_backend::from_double(res, d, params);
}

double TorusUtils<BigTorus>::to_double(const TorusUtils<BigTorus>::TORUS *x, const ZModuleParams<BigTorus> *params) {
    return tfhe_backend::to_double(x, params);
}

void TorusUtils<BigTorus>::approxPhase(TorusUtils<BigTorus>::TORUS *res, const TorusUtils<BigTorus>::TORUS *phase,
                                       const uint64_t Msize, const ZModuleParams<BigTorus> *params, Allocator alloc) {
    tfhe_backend::approxPhase(res, phase, Msize, params, std::move(alloc));
}

uint64_t TorusUtils<BigTorus>::modSwitchFromTorus(const TorusUtils<BigTorus>::TORUS *phase, const uint64_t Msize,
                                                  const ZModuleParams<BigTorus> *params, Allocator alloc) {
    return tfhe_backend::modSwitchFromTorus(phase, Msize, params, std::move(alloc));
}

void TorusUtils<BigTorus>::modSwitchToTorus(TorusUtils<BigTorus>::TORUS *res, const uint64_t mu, const uint64_t Msize,
                                            const ZModuleParams<BigTorus> *params, Allocator alloc) {
    tfhe_backend::modSwitchToTorus(res, mu, Msize, params, std::move(alloc));
}

double TorusUtils<BigTorus>::distance(const TorusUtils<BigTorus>::TORUS *t1, const TorusUtils<BigTorus>::TORUS *t2,
                                      const ZModuleParams<BigTorus> *params, Allocator alloc) {
    TORUS *t = alloc.newObject<TORUS>(params, &alloc);
    tfhe_backend::sub(t, t1, t2, params);
    double diff = std::abs(TorusUtils<TORUS>::to_double(t, params));
    alloc.deleteObject(t, params, &alloc);
    return diff;
}

EXPLICIT_INSTANTIATE_CLASS(TorusUtils, BigTorus);
