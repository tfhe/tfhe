#include "polynomial_int.h"

#include <cassert>

using namespace tfhe_backend;

/**
 * Instantiate IntPolynomial class for big int type
 */
template
class IntPolynomial<BigTorus>;

// Euclidean norm of an IntPolynomial
template<>
double IntPolynomial<BigTorus>::Norm2sq(const IntPolynomial<BigTorus> *poly,
                                        const PolynomialParams<BigTorus> *params,
                                        TfheThreadContext *context,
                                        Allocator alloc) {
    const int32_t N = params->N;
    //typename PolynomialParams<BigTorus>::ZModuleParams<BigTorus> *zparams = params->zmodule_params;
    double norm_d = 0;
    BigInt *r = alloc.newObject<BigInt>(0);
    BigInt *norm = alloc.newObject<BigInt>(0);


    for (int32_t i = 0; i < N; ++i) {
        mul(r, poly->coefs + i, poly->coefs + i, params->zmodule_params);
        add(norm, norm, r, params->zmodule_params);
    }

    // get the double from BigInt
    norm_d = mpz_get_d(norm->data);

    // delete
    alloc.deleteObject<BigInt>(norm);
    alloc.deleteObject<BigInt>(r);

    return norm_d;
}

// Infinity norm of the distance between two IntPolynomial
template<>
double IntPolynomial<BigTorus>::NormInftyDist(const IntPolynomial<BigTorus> *poly1,
                                              const IntPolynomial<BigTorus> *poly2,
                                              const PolynomialParams<BigTorus> *params,
                                              TfheThreadContext *context,
                                              Allocator alloc) {
    const int32_t N = params->N;
    double norm = 0;
    double r;

    // Max between the coefficients of abs(poly1-poly2)
    for (int32_t i = 0; i < N; ++i) {
        r = abs(mpz_get_d(poly1->coefs[i].data) - mpz_get_d(poly2->coefs[i].data));
        if (r > norm) { norm = r; }
    }

    return norm;
}
