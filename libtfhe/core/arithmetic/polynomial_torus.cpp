#include "polynomial_torus.h"
#include "random_gen.h"

#include <cassert>


/**
 * Instantiate TorusPolynomial class for all torus types
 */
EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_TORUS(TorusPolynomial);
template
class TorusPolynomial<BigTorus>;

template<typename TORUS>
void TorusPolynomial<TORUS>::Uniform(
        TorusPolynomial<TORUS> *result,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t N = params->N;
    TORUS *x = result->coefs;
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    for (int32_t i = 0; i < N; ++i)
        RandomGenTorus<TORUS>::uniform(x+i, zparams);
}



// Infinity norm of the distance between two TorusPolynomial
template<typename TORUS>
double TorusPolynomial<TORUS>::NormInftyDist(
        const TorusPolynomial<TORUS> *poly1,
        const TorusPolynomial<TORUS> *poly2,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t N = params->N;
    double norm = 0;
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    // Max between the coefficients of abs(poly1-poly2)
    for (int32_t i = 0; i < N; ++i) {
        double r = TorusUtils<TORUS>::distance(poly1->coefs + i, poly2->coefs + i, zparams, alloc.createStackChildAllocator());
        if (r > norm) { norm = r; }
    }
    return norm;
}
