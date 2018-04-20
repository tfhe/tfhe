#include "random_gen.h"
#include "torus_utils.h"

using namespace std;

default_random_engine RandomGen::generator;

uniform_int_distribution <int32_t> RandomGen::uniform_bool_distrib(0, 1);
normal_distribution<double> RandomGen::std_normal_distrib;

/**
 * Instantiate RandomGenTorus class for available torus types
 */
EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_TORUS(RandomGenTorus);

/**
 * Static members
 */
template<>
std::uniform_int_distribution <Torus32> RandomGenTorus<Torus32>::uniform_distrib =
        std::uniform_int_distribution<Torus32>(TorusUtils<Torus32>::min_val, TorusUtils<Torus32>::max_val);
template<>
std::uniform_int_distribution <Torus64> RandomGenTorus<Torus64>::uniform_distrib =
        std::uniform_int_distribution<Torus64>(TorusUtils<Torus64>::min_val, TorusUtils<Torus64>::max_val);

/**
 * Static functions
 */
template<typename TORUS>
TORUS RandomGenTorus<TORUS>::uniform(
        const ZModuleParams<TORUS> *params) {
    return RandomGenTorus::uniform_distrib(generator);
}

template<typename TORUS>
TORUS RandomGenTorus<TORUS>::gaussian(
        const TORUS mean,
        const double sigma,
        const ZModuleParams<TORUS> *params) {
    return mean + TorusUtils<TORUS>::from_double(RandomGen::gaussian(sigma), params);
}
