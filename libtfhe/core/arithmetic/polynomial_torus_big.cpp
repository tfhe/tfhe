#include "polynomial_torus.h"

#include <cassert>
#include "big_torus.h"

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
    const ZModuleType *const zparams =
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
    const ZModuleType *const zparams =
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
    const ZModuleType *const zparams =
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
    const ZModuleType *const zparams,
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
    const ZModuleType *const zparams,
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
    const IntPolynomial<INT_TYPE> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    assert(result != poly2);

    const int32_t N = params->N;
    const ZModuleType *const zparams =
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
    const ZModuleType *const zparams,
    TfheThreadContext *context,
    Allocator alloc)
{

    const int32_t h = size/2;
    const int32_t sm1 = size-1;
    // temp value
    BigTorus *temp = alloc.newObject<BigTorus>(zparams, &alloc);

    // h<=4 experimentally chosen
    if (h <= 4){
        TorusPolynomial<BigTorus>::MultNaive_aux(R, A, B, size,
                                                 zparams, context,
                                                 alloc.createStackChildAllocator());
        return; // the algorithm finishes
    }

    // We split the polynomials in 2
    // A = A0 + A1*X^h
    INT_TYPE *Atemp = (INT_TYPE *) buf;
    buf += h * sizeof(INT_TYPE);
    // B = B0 + B1*X^h
    BigTorus *Btemp = (BigTorus *) buf;
    buf += h * sizeof(BigTorus);
    // R
    BigTorus *Rtemp = (BigTorus *) buf;
    buf += size * sizeof(BigTorus);
    // Note: in the above line, I have put size instead of sm1 so
    // that buf remains aligned on a power of 2

    // Atemp = A0 + A1
    for (int32_t i = 0; i < h; ++i){
        add(Atemp + i, A + i, A + (h + i), zparams);
    }
    // Btemp = B0 + B1
    for (int32_t i = 0; i < h; ++i){
        add(Btemp + i, B + i, B + (h + i), zparams);
    }


    // Karatsuba recursively
    // R0 = A0 * B0
    // (R[0],R[2*h-2]), (A[0],A[h-1]), (B[0],B[h-1])
    Karatsuba_aux(R, A, B, h, buf, zparams, context, alloc);
    // R[2*h-1] = 0
    zero(R + sm1, zparams);
    // R1 = A1 * B1
    // (R[2*h],R[4*h-2]), (A[h],A[2*h-1]), (B[h],B[2*h-1])
    Karatsuba_aux(R + size, A + h, B + h, h, buf, zparams, context, alloc);
    // Rtemp = Atemp * Btemp = (A0 + A1)*(B0 + B1)
    Karatsuba_aux(Rtemp, Atemp, Btemp, h, buf, zparams, context, alloc);

    // R = R0 + Rtemp * X^h + R1 * X^size
    // with Rtemp = Rtemp - R0 - R1
    for (int32_t i = 0; i < sm1; ++i){
        add(temp, R + i, R + (size + i), zparams);
        sub(Rtemp + i, Rtemp + i, temp, zparams);
    }
    for (int32_t i = 0; i < sm1; ++i){
        add(R + (h + i), R + (h + i), Rtemp + i, zparams);
    }


    // delete
    alloc.deleteObject<BigTorus>(temp, zparams, &alloc);
}

// poly1, poly2 and result are polynomials mod X^N+1
template<>
void TorusPolynomial<BigTorus>::MultKaratsuba(
    TorusPolynomial<BigTorus> *result,
    const IntPolynomial<INT_TYPE> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;

    // Buffer: that's large enough to store every tmp variables (2*2*N*4)
    char *buf = new char[4 * N * sizeof(BigTorus)];
    // zmodule_params
    const typename PolynomialParams<BigTorus>::ZModuleType *const zparams = params->zmodule_params;
    // result in Karatsuba_aux
    BigTorus *R = alloc.newArray<BigTorus>(2*N-1, zparams, &alloc);

    // Karatsuba
    Karatsuba_aux(R, poly1->coefs, poly2->coefs, N, buf, zparams, context, alloc);

    // Reduction mod X^N+1
    for (int32_t i = 0; i < N - 1; ++i){
        sub(result->coefs + i, R + i, R + (N + i), zparams);
    }
    mpn_copyi(result->coefs[N - 1].data, R[N-1].data, zparams->max_nbLimbs);
    //copy(result->coefs + (N - 1), R + (N - 1), zparams);

    // delete
    delete[] buf;
    alloc.deleteArray<BigTorus>(2*N-1, R, zparams, &alloc);
}

// poly1, poly2 and result are polynomials mod X^N+1
template<>
void TorusPolynomial<BigTorus>::AddMulRKaratsuba(
    TorusPolynomial<BigTorus> *result,
    const IntPolynomial<INT_TYPE> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;

    // Buffer: that's large enough to store every tmp variables (2*2*N*4)
    char *buf = new char[4 * N * sizeof(BigTorus)];
    // zmodule_params
    const typename PolynomialParams<BigTorus>::ZModuleType *const zparams = params->zmodule_params;
    // result in Karatsuba_aux
    BigTorus *R = alloc.newArray<BigTorus>(2*N-1, zparams, &alloc);
    // temp value
    BigTorus *temp = alloc.newObject<BigTorus>(zparams, &alloc);

    // Karatsuba
    Karatsuba_aux(R, poly1->coefs, poly2->coefs, N, buf, zparams, context, alloc);

    // Reduction mod X^N+1
    for (int32_t i = 0; i < N - 1; ++i){
        sub(temp, R + i, R + (N + i), zparams);
        add(result->coefs + i, result->coefs + i, temp, zparams);
    }
    add(result->coefs + (N - 1), result->coefs + (N - 1), R + (N - 1), zparams);

    // delete
    delete[] buf;
    alloc.deleteObject<BigTorus>(temp, zparams, &alloc);
    alloc.deleteArray<BigTorus>(2*N-1, R, zparams, &alloc);
}

// poly1, poly2 and result are polynomials mod X^N+1
template<>
void TorusPolynomial<BigTorus>::SubMulRKaratsuba(
    TorusPolynomial<BigTorus> *result,
    const IntPolynomial<INT_TYPE> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParams<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;

    // Buffer: that's large enough to store every tmp variables (2*2*N*4)
    char *buf = new char[4 * N * sizeof(BigTorus)];
    // zmodule_params
    const typename PolynomialParams<BigTorus>::ZModuleType *const zparams = params->zmodule_params;
    // result in Karatsuba_aux
    BigTorus *R = alloc.newArray<BigTorus>(2*N-1, zparams, &alloc);
    // temp value
    BigTorus *temp = alloc.newObject<BigTorus>(zparams, &alloc);

    // Karatsuba
    Karatsuba_aux(R, poly1->coefs, poly2->coefs, N, buf, zparams, context, alloc);

    // Reduction mod X^N+1
    for (int32_t i = 0; i < N - 1; ++i){
        sub(temp, R + i, R + (N + i), zparams);
        sub(result->coefs + i, result->coefs + i, temp, zparams);
    }
    sub(result->coefs + (N - 1), result->coefs + (N - 1), R + (N - 1), zparams);

    // delete
    delete[] buf;
    alloc.deleteObject<BigTorus>(temp, zparams, &alloc);
    alloc.deleteArray<BigTorus>(2*N-1, R, zparams, &alloc);
}
