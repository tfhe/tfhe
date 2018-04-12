#include "polynomial_torus.h"

#include <cassert>

/**
 * Instantiate TorusPolynomial class for big torus type
 */
template struct TorusPolynomial<BigTorus>;

// TorusPolynomial = random
template<>
void TorusPolynomial<BigTorus>::Uniform(
    TorusPolynomial<BigTorus> *result,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    abort(); //not implemented yet
}

// TorusPolynomial + p*TorusPolynomial
template<>
void TorusPolynomial<BigTorus>::AddMulZ(
    TorusPolynomial<BigTorus> *result,
    const TorusPolynomial<BigTorus> *poly1,
    const INT_TYPE *p,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    assert(result != poly1); //if it fails here, please use AddMulZTo
    BigTorus *r = result->coefs;
    const BigTorus *a = poly1->coefs;
    const BigTorus *b = poly2->coefs;
    const ZModuleParams<BigTorus> *const zparams =
        params->zmodule_params;

    BigTorus *t = alloc.newObject<BigTorus>(zparams, &alloc);
    for (int32_t i = 0; i < N; ++i) {
        copy(t, b+i, zparams);
        mul(t, p, t, zparams, alloc);
        add(r+i, a+i, t, zparams);
    }
}

// TorusPolynomial += p*TorusPolynomial
template<>
void TorusPolynomial<BigTorus>::AddMulZTo(
    TorusPolynomial<BigTorus> *result,
    const INT_TYPE *p,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    TorusPolynomial<BigTorus>::AddMulZ(result, result, p, poly2, params, context, alloc);
}

// TorusPolynomial - p*TorusPolynomial
template<>
void TorusPolynomial<BigTorus>::SubMulZ(
    TorusPolynomial<BigTorus> *result,
    const TorusPolynomial<BigTorus> *poly1,
    const INT_TYPE *p,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    assert(result != poly1); //if it fails here, please use AddMulZTo
    BigTorus *r = result->coefs;
    const BigTorus *a = poly1->coefs;
    const BigTorus *b = poly2->coefs;
    const ZModuleParams<BigTorus> *const zparams =
        params->zmodule_params;

    BigTorus *t = alloc.newObject<BigTorus>(zparams, &alloc);
    for (int32_t i = 0; i < N; ++i) {
        copy(t, b+i, zparams);
        mul(t, p, t, zparams, alloc);
        sub(r+i, a+i, t, zparams);
    }
}

// TorusPolynomial -= p*TorusPolynomial
template<>
void TorusPolynomial<BigTorus>::SubMulZTo(
    TorusPolynomial<BigTorus> *result,
    const INT_TYPE *p,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    TorusPolynomial<BigTorus>::SubMulZ(result, result, p, poly2, params, context, alloc);
}

// Infinity norm of the distance between two TorusPolynomial
template<>
double TorusPolynomial<BigTorus>::NormInftyDist(
    const TorusPolynomial<BigTorus> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    double norm = 0;
    const ZModuleParams<BigTorus> *const zparams =
        params->zmodule_params;

    // Max between the coefficients of abs(poly1-poly2)
    for (int32_t i = 0; i < N; ++i) {
        double r = TorusUtils<BigTorus>::normInftyDist(poly1->coefs+i, poly2->coefs+i, zparams);
        if (r > norm) { norm = r; }
    }
    return norm;
}


template<>
void TorusPolynomial<BigTorus>::MultNaive_plain_aux(
    BigTorus *__restrict result,
    const INT_TYPE *__restrict poly1,
    const BigTorus *__restrict poly2,
    const int32_t N,
    const ZModuleParams<BigTorus> *const zparams,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t _2Nm1 = 2 * N - 1;
    BigTorus *ti = alloc.newObject<BigTorus>(zparams, &alloc);

    for (int32_t i = 0; i < N; i++) {
        BigTorus *ri = result+i;
        zero(ri, zparams);
        for (int32_t j = 0; j <= i; j++) {
            mul(ti, poly1+j, poly2+(i-j), zparams, alloc);
            add(ri, ri, ti, zparams);
        }
    }

    for (int32_t i = N; i < _2Nm1; i++) {
        BigTorus *ri = result+i;
        zero(ri, zparams);
        for (int32_t j = i - N + 1; j < N; j++) {
            mul(ti, poly1+j, poly2+(i-j), zparams, alloc);
            add(ri, ri, ti, zparams);
        }
    }
}

template<>
void TorusPolynomial<BigTorus>::MultNaive_aux(
    BigTorus *__restrict result,
    const INT_TYPE *__restrict poly1,
    const BigTorus *__restrict poly2,
    const int32_t N,
    const ZModuleParams<BigTorus> *const zparams,
    TfheThreadContext *context,
    Allocator alloc)
{
    BigTorus *ti = alloc.newObject<BigTorus>(zparams, &alloc);

    for (int32_t i = 0; i < N; i++) {
        BigTorus *ri = result+i;
        zero(ri, zparams);

        for (int32_t j = 0; j <= i; j++) {
            mul(ti, poly1+j, poly2+(i-j), zparams, alloc);
            add(ri, ri, ti, zparams);
        }
        for (int32_t j = i + 1; j < N; j++) {
            mul(ti, poly1+j, poly2+(N + i - j), zparams, alloc);
            sub(ri, ri, ti, zparams);
        }
    }
}

/**
 * This is the naive external multiplication of an integer polynomial
 * with a torus polynomial. (this function should yield exactly the same
 * result as the karatsuba or fft version)
 */
template<>
void TorusPolynomial<BigTorus>::MultNaive(
    TorusPolynomial<BigTorus> *result,
    const IntPolynomial<BigTorus> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    assert(result != poly2);

    const int32_t N = params->N;
    const ZModuleParams<BigTorus> *const zparams =
        params->zmodule_params;

    TorusPolynomial<BigTorus>::MultNaive_aux(result->coefs, poly1->coefs,
        poly2->coefs, N, zparams, context, alloc.createStackChildAllocator());
}
//
/**
 * This function multiplies 2 polynomials (an integer poly and a torus poly) by using Karatsuba
 * The karatsuba function is torusPolynomialMultKaratsuba: it takes in input two polynomials and multiplies them
 * To do that, it uses the auxiliary function Karatsuba_aux, which is recursive ad which works with
 * the vectors containing the coefficients of the polynomials (primitive types)
 */

// A and B of size = size
// R of size = 2*size-1
template<>
void TorusPolynomial<BigTorus>::Karatsuba_aux(
    BigTorus *R,
    const INT_TYPE *A,
    const BigTorus *B,
    const int32_t size,
    const char *buf,
    const ZModuleParams<BigTorus> *const zparams,
    TfheThreadContext *context,
    Allocator alloc)
{
    abort(); //not implemented yet
}

// poly1, poly2 and result are polynomials mod X^N+1
template<>
void TorusPolynomial<BigTorus>::MultKaratsuba(
    TorusPolynomial<BigTorus> *result,
    const IntPolynomial<BigTorus> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    abort(); //not implemented yet
}

// poly1, poly2 and result are polynomials mod X^N+1
template<>
void TorusPolynomial<BigTorus>::AddMulRKaratsuba(
    TorusPolynomial<BigTorus> *result,
    const IntPolynomial<BigTorus> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    abort(); //not implemented yet
}

// poly1, poly2 and result are polynomials mod X^N+1
template<>
void TorusPolynomial<BigTorus>::SubMulRKaratsuba(
    TorusPolynomial<BigTorus> *result,
    const IntPolynomial<BigTorus> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    abort(); //not implemented yet
}
