#include "torus_utils_primitive.h"

template<typename TORUS>
TORUS TorusUtils<TORUS>::from_double(const double d, const ZModuleParams<TORUS> *params) {
    static double intpart; //this value is unused, so it can be static
    double temp = modf(d, &intpart) / 2.0; //divide by 2 in order to avoid overflow in next line
    return TORUS(temp * dtot_factor) << 1;
}

template<typename TORUS>
void TorusUtils<TORUS>::from_double(TORUS *reps, const double d, const ZModuleParams<TORUS> *params) {
    *reps = from_double(d, params);
}

template<typename TORUS>
double TorusUtils<TORUS>::to_double(const TORUS *x, const ZModuleParams<TORUS> *params) {
    return double(*x) / dtot_factor;
}

template<typename TORUS>
void
TorusUtils<TORUS>::approxPhase(TORUS &res, const TORUS phase, const uint64_t Msize, const ZModuleParams<TORUS> *params,
                               Allocator alloc) {
    UTORUS half_interv = (half_utorus / UTORUS(Msize)); // half width of each interval
    UTORUS interv = half_interv << 1; // width of each interval
    UTORUS phase_temp = UTORUS(phase) + half_interv;
    phase_temp -= phase_temp % interv;
    res = (TORUS) phase_temp;
}

template<typename TORUS>
uint64_t
TorusUtils<TORUS>::modSwitchFromTorus(const TORUS phase, const uint64_t Msize, const ZModuleParams<TORUS> *params,
                                      Allocator alloc) {
    UTORUS half_interv = (half_utorus / UTORUS(Msize));
    UTORUS interv = half_interv << 1; // width of each interval
    UTORUS phase_temp = UTORUS(phase) + half_interv;
    return uint64_t(phase_temp / interv);
}

template<typename TORUS>
void TorusUtils<TORUS>::modSwitchToTorus(TORUS &res, const uint64_t mu, const uint64_t Msize,
                                         const ZModuleParams<TORUS> *params, Allocator alloc) {
    UTORUS half_interv = (half_utorus / UTORUS(Msize));
    UTORUS phase_temp = UTORUS((mu % Msize) * 2) * half_interv;
    res = (TORUS)phase_temp;
}

template<typename TORUS>
double
TorusUtils<TORUS>::distance(const TORUS *t1, const TORUS *t2, const ZModuleParams<TORUS> *params, Allocator alloc) {
    TORUS t = *t1 - *t2;
    return std::abs(TorusUtils<TORUS>::to_double(&t, params));
}

EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_TORUS(TorusUtils);
