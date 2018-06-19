#ifndef TFHE_POLYNOMIAL_INT_BIG_H
#define TFHE_POLYNOMIAL_INT_BIG_H


#include "polynomial_int.h"

#include <cassert>

#ifdef TFHE_POLYNOMIAL_INT_PRIMITIVE_H
#error "polynomial_int_primitive.h and polynomial_int_big.h are incompatible"
#endif

using namespace tfhe_backend;

// Euclidean norm of an IntPolynomial
template<typename TORUS, AsmTypeEnum AsmType>
double IntPolynomial<TORUS, AsmType>::Norm2sq(const IntPolynomial<TORUS, AsmType> *poly,
                                              const PolynomialParams<TORUS, AsmType> *params,
                                              TfheThreadContext *context,
                                              Allocator alloc) {
    const int32_t N = params->N;
    //typename PolynomialParams<TORUS>::ZModuleParams<TORUS> *zparams = params->zmodule_params;
    double norm_d = 0;
    BigInt *r = alloc.newObject<BigInt>(0);
    BigInt *norm = alloc.newObject<BigInt>(0);


    for (int32_t i = 0; i < N; ++i) {
        tfhe_backend::mul(r, poly->coefs + i, poly->coefs + i, params->zmodule_params);
        tfhe_backend::add(norm, norm, r, params->zmodule_params);
    }

    // get the double from BigInt
    norm_d = mpz_get_d(norm->data);

    // delete
    alloc.deleteObject<BigInt>(norm);
    alloc.deleteObject<BigInt>(r);

    return norm_d;
}

// Infinity norm of the distance between two IntPolynomial
template<typename TORUS, AsmTypeEnum AsmType>
double IntPolynomial<TORUS, AsmType>::NormInftyDist(const IntPolynomial<TORUS, AsmType> *poly1,
                                                    const IntPolynomial<TORUS, AsmType> *poly2,
                                                    const PolynomialParams<TORUS, AsmType> *params,
                                                    TfheThreadContext *context,
                                                    Allocator alloc) {
    const int32_t N = params->N;
    double norm = 0;
    double r;

    // Max between the coefficients of abs(poly1-poly2)
    for (int32_t i = 0; i < N; ++i) {
        r = std::abs(mpz_get_d(poly1->coefs[i].data) - mpz_get_d(poly2->coefs[i].data));
        if (r > norm) { norm = r; }
    }

    return norm;
}

#endif //TFHE_POLYNOMIAL_INT_BIG_H
