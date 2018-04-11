#include "polynomial_int.h"

#include <cassert>

/**
 * Instantiate IntPolynomial class for big int type
 */
template
struct IntPolynomial<BigInt>;

// Euclidean norm of an IntPolynomial
template<>
double IntPolynomial<BigInt>::Norm2sq(const IntPolynomial<BigInt> *poly,
                                      const PolynomialParams<BigInt> *params,
                                      TfheThreadContext *context,
                                      Allocator alloc) {
    const int32_t N = params->N;
    const typename PolynomialParams<BigTorus>::ZModuleType *const zparams = params->zmodule_params;
    double norm_d = 0;
    BigInt *r = alloc.newObject<BigInt>(0);
    BigInt *norm = alloc.newObject<BigInt>(0);


    for (int32_t i = 0; i < N; ++i) {
        mul(r, poly->coefs + i, poly->coefs + i, zparams);
        add(norm, norm, r, zparams);
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
double IntPolynomial<BigInt>::NormInftyDist(const IntPolynomial<BigInt> *poly1,
                                            const IntPolynomial<BigInt> *poly2,
                                            const PolynomialParams<BigInt> *params,
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
