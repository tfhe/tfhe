#include "random_gen.h"
#include "core/arithmetic/torus_utils.h"

using namespace std;

default_random_engine RandomGen::generator;

uniform_int_distribution<int32_t> RandomGen::uniform_bool_distrib(0, 1);
normal_distribution<double> RandomGen::std_normal_distrib;
uniform_int_distribution<uint64_t> RandomGen::uniform_uint64_distrib(std::numeric_limits<uint64_t>::min(),
                                                                     std::numeric_limits<uint64_t>::max());

/**
 * Static functions for native torus types
 */
template<typename TORUS>
void RandomGenTorus<TORUS>::uniform(
        TORUS *dst,
        const ZModuleParams<TORUS> *params) {
    *dst = (TORUS) RandomGen::uniform();
}

template<typename TORUS>
void RandomGenTorus<TORUS>::gaussian(
        TORUS *dst,
        const TORUS *mean,
        const double sigma,
        const ZModuleParams<TORUS> *params) {
    *dst = *mean + TorusUtils<TORUS>::from_double(RandomGen::gaussian(sigma), params);
}


/**
 * Static functions specializations for big torus types
 */
template<>
void RandomGenTorus<BigTorus>::uniform(
        BigTorus *dst,
        const ZModuleParams<BigTorus> *params) {

    mp_limb_t *const data = dst->data;
    for (int i = 0; i < params->max_nbLimbs; ++i)
        data[i] = RandomGen::uniform();
}

template<>
void RandomGenTorus<BigTorus>::gaussian(
        BigTorus *dst,
        const BigTorus *mean,
        const double sigma,
        const ZModuleParams<BigTorus> *params) {

    // double precision normal should be fine for big torus
    TorusUtils<BigTorus>::from_double(dst, RandomGen::gaussian(sigma), params);
    tfhe_backend::add(dst, mean, dst, params);
}

/**
 * Instantiate RandomGenTorus class for available torus types
 */
EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_TORUS(RandomGenTorus);

