#include "random_gen.h"

using namespace std;

/**
 * Instantiate RandomGenTorus class for big torus type
 */
template
class RandomGenTorus<BigTorus>;

void RandomGenTorus<BigTorus>::uniform(
        TORUS *dst,
        const ZModuleParams<TORUS> *params) {

    mp_limb_t *const data = dst->data;
    for (int i = 0; i < params->max_nbLimbs; ++i) {
        data[i] = RandomGenTorus<Torus64>::uniform(nullptr);
    }
}

void RandomGenTorus<BigTorus>::gaussian(
        TORUS *dst,
        const TORUS *mean,
        const double sigma,
        const ZModuleParams<TORUS> *params) {

    // double precision normal should be fine for big torus
    TorusUtils<TORUS>::from_double(dst, RandomGen::gaussian(sigma), params);
    tfhe_backend::add(dst, mean, dst, params);
}
