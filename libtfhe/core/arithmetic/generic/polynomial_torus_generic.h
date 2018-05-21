#ifndef TFHE_POLYNOMIAL_TORUS_GENERIC_H
#define TFHE_POLYNOMIAL_TORUS_GENERIC_H

#include "polynomial_torus.h"
#include "core/arithmetic/random_gen.h"

#include <cassert>

template<typename TORUS, AsmTypeEnum AsmType>
TorusPolynomial<TORUS, AsmType>::TorusPolynomial(const PolynomialParams<TORUS, AsmType> *params,
                                                 TfheThreadContext *context, Allocator *alloc) :
        Polynomial<TORUS, CoefTypeEnum::Torus, AsmType>(params, context, alloc) {}


template<typename TORUS, AsmTypeEnum AsmType>
void
TorusPolynomial<TORUS, AsmType>::destroy(const PolynomialParams<TORUS, AsmType> *params, TfheThreadContext *context,
                                         Allocator *alloc) {
    Polynomial<TORUS, CoefTypeEnum::Torus, AsmType>::destroy(params, context, alloc);
}


template<typename TORUS, AsmTypeEnum AsmType>
void TorusPolynomial<TORUS, AsmType>::Uniform(
        TorusPolynomial<TORUS, AsmType> *result,
        const PolynomialParams<TORUS, AsmType> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t N = params->N;
    TORUS *x = result->coefs;
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    for (int32_t i = 0; i < N; ++i)
        RandomGenTorus<TORUS>::uniform(x + i, zparams);
}


// Infinity norm of the distance between two TorusPolynomial
template<typename TORUS, AsmTypeEnum AsmType>
double TorusPolynomial<TORUS, AsmType>::NormInftyDist(
        const TorusPolynomial<TORUS, AsmType> *poly1,
        const TorusPolynomial<TORUS, AsmType> *poly2,
        const PolynomialParams<TORUS, AsmType> *params,
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

#endif// TFHE_POLYNOMIAL_TORUS_GENERIC_H
