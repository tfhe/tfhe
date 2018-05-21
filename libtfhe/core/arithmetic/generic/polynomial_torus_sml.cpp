#include "polynomial_torus.h"

#include <cassert>

using namespace std;

/**
 * Instantiate TorusPolynomial class for available torus types
 */
EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_TORUS(TorusPolynomial, AsmTypeEnum::PORTABLE);

// TorusPolynomial + p*TorusPolynomial
template<typename TORUS, AsmTypeEnum AsmType>
void TorusPolynomial<TORUS, AsmType>::AddMulZ(
        TorusPolynomial<TORUS, AsmType> *result,
        const TorusPolynomial<TORUS, AsmType> *poly1,
        const INT_TYPE *p,
        const TorusPolynomial<TORUS, AsmType> *poly2,
        const PolynomialParams<TORUS, AsmType> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t N = params->N;
    assert(result != poly1); //if it fails here, please use AddMulZTo
    TORUS *r = result->coefs;
    const TORUS *a = poly1->coefs;
    const TORUS *b = poly2->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] = a[i] + *p * b[i];
}

// TorusPolynomial += p*TorusPolynomial
template<typename TORUS, AsmTypeEnum AsmType>
void TorusPolynomial<TORUS, AsmType>::AddMulZTo(
        TorusPolynomial<TORUS, AsmType> *result,
        const INT_TYPE *p,
        const TorusPolynomial<TORUS, AsmType> *poly2,
        const PolynomialParams<TORUS, AsmType> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t N = params->N;
    TORUS *r = result->coefs;
    const TORUS *b = poly2->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] += *p * b[i];
}

// TorusPolynomial - p*TorusPolynomial
template<typename TORUS, AsmTypeEnum AsmType>
void TorusPolynomial<TORUS, AsmType>::SubMulZ(
        TorusPolynomial<TORUS, AsmType> *result,
        const TorusPolynomial<TORUS, AsmType> *poly1,
        const INT_TYPE *p,
        const TorusPolynomial<TORUS, AsmType> *poly2,
        const PolynomialParams<TORUS, AsmType> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t N = params->N;
    assert(result != poly1); //if it fails here, please use SubMulZTo
    TORUS *r = result->coefs;
    const TORUS *a = poly1->coefs;
    const TORUS *b = poly2->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] = a[i] - *p * b[i];
}

// TorusPolynomial -= p*TorusPolynomial
template<typename TORUS, AsmTypeEnum AsmType>
void TorusPolynomial<TORUS, AsmType>::SubMulZTo(
        TorusPolynomial<TORUS, AsmType> *result,
        const INT_TYPE *p,
        const TorusPolynomial<TORUS, AsmType> *poly2,
        const PolynomialParams<TORUS, AsmType> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t N = params->N;
    TORUS *r = result->coefs;
    const TORUS *b = poly2->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] -= *p * b[i];
}

template<typename TORUS, AsmTypeEnum AsmType>
void TorusPolynomial<TORUS, AsmType>::MultNaive_plain_aux_old(
        TORUS *__restrict result,
        const INT_TYPE *__restrict poly1,
        const TORUS *__restrict poly2,
        const int32_t N,
        const ZModuleParams<TORUS> *const zparams,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t _2Nm1 = 2 * N - 1;
    TORUS ri;

    for (int32_t i = 0; i < N; i++) {
        ri = 0;
        for (int32_t j = 0; j <= i; j++) {
            ri += poly1[j] * poly2[i - j];
        }
        result[i] = ri;
    }

    for (int32_t i = N; i < _2Nm1; i++) {
        ri = 0;
        for (int32_t j = i - N + 1; j < N; j++) {
            ri += poly1[j] * poly2[i - j];
        }
        result[i] = ri;
    }
}

template<typename TORUS, AsmTypeEnum AsmType>
void TorusPolynomial<TORUS, AsmType>::MultNaive_aux(
        TORUS *__restrict result,
        const INT_TYPE *__restrict poly1,
        const TORUS *__restrict poly2,
        const int32_t N,
        const ZModuleParams<TORUS> *const zparams,
        TfheThreadContext *context,
        Allocator alloc) {
    TORUS ri;

    for (int32_t i = 0; i < N; i++) {
        ri = 0;
        for (int32_t j = 0; j <= i; j++) {
            ri += poly1[j] * poly2[i - j];
        }
        for (int32_t j = i + 1; j < N; j++) {
            ri -= poly1[j] * poly2[N + i - j];
        }
        result[i] = ri;
    }
}

/**
 * This is the naive external multiplication of an integer polynomial
 * with a torus polynomial. (this function should yield exactly the same
 * result as the karatsuba or fft version)
 */
template<typename TORUS, AsmTypeEnum AsmType>
void TorusPolynomial<TORUS, AsmType>::MultNaive(
        TorusPolynomial<TORUS, AsmType> *result,
        const IntPolynomial<TORUS, AsmType> *poly1,
        const TorusPolynomial<TORUS, AsmType> *poly2,
        const PolynomialParams<TORUS, AsmType> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    assert(result != poly2);

    const int32_t N = params->N;
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    TorusPolynomial<TORUS, AsmType>::MultNaive_aux(result->coefs, poly1->coefs,
                                                   poly2->coefs, N, zparams, context, alloc.createStackChildAllocator());
}

/**
 * This function multiplies 2 polynomials (an integer poly and a torus poly) by using Karatsuba
 * The karatsuba function is torusPolynomialMultKaratsuba: it takes in input two polynomials and multiplies them
 * To do that, it uses the auxiliary function Karatsuba_aux, which is recursive ad which works with
 * the vectors containing the coefficients of the polynomials (primitive types)
 */

// A and B of size = size
// R of size = 2*size-1
template<typename TORUS, AsmTypeEnum AsmType>
void TorusPolynomial<TORUS, AsmType>::Karatsuba_aux_old(
        TORUS *R,
        const INT_TYPE *A,
        const TORUS *B,
        const int32_t size,
        const char *buf,
        const ZModuleParams<TORUS> *const zparams,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t h = size / 2;
    const int32_t sm1 = size - 1;

    //we stop the karatsuba recursion at h=4, because on my machine,
    //it seems to be optimal
    if (h <= 4) {
        TorusPolynomial<TORUS, AsmType>::MultNaive_plain_aux_old(R, A, B, size, zparams, context,
                                                                 alloc.createStackChildAllocator());
        return;
    }

    //we split the polynomials in 2
    INT_TYPE *Atemp = (INT_TYPE *) buf;
    buf += h * sizeof(INT_TYPE);
    TORUS *Btemp = (TORUS *) buf;
    buf += h * sizeof(TORUS);
    TORUS *Rtemp = (TORUS *) buf;
    buf += size * sizeof(TORUS);
    //Note: in the above line, I have put size instead of sm1 so that buf remains aligned on a power of 2

    for (int32_t i = 0; i < h; ++i)
        Atemp[i] = A[i] + A[h + i];
    for (int32_t i = 0; i < h; ++i)
        Btemp[i] = B[i] + B[h + i];

    // Karatsuba recursivly
    Karatsuba_aux_old(R, A, B, h, buf, zparams, context,
                      alloc.createStackChildAllocator()); // (R[0],R[2*h-2]), (A[0],A[h-1]), (B[0],B[h-1])
    Karatsuba_aux_old(R + size, A + h, B + h, h, buf, zparams, context,
                      alloc.createStackChildAllocator()); // (R[2*h],R[4*h-2]), (A[h],A[2*h-1]), (B[h],B[2*h-1])
    Karatsuba_aux_old(Rtemp, Atemp, Btemp, h, buf, zparams, context, alloc.createStackChildAllocator());
    R[sm1] = 0; //this one needs to be copy manually
    for (int32_t i = 0; i < sm1; ++i)
        Rtemp[i] -= R[i] + R[size + i];
    for (int32_t i = 0; i < sm1; ++i)
        R[h + i] += Rtemp[i];

}

// poly1, poly2 and result are polynomials mod X^N+1
template<typename TORUS, AsmTypeEnum AsmType>
void TorusPolynomial<TORUS, AsmType>::MultKaratsuba_old(
        TorusPolynomial<TORUS, AsmType> *result,
        const IntPolynomial<TORUS, AsmType> *poly1,
        const TorusPolynomial<TORUS, AsmType> *poly2,
        const PolynomialParams<TORUS, AsmType> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t N = params->N;
    TORUS *R = new TORUS[2 * N - 1];
    char *buf = new char[4 * N *
                         sizeof(TORUS)]; //that's large enough to store every tmp variables (2*2*N*4) TODO: see if there is unused memory (before generic torus byte cnt was 16*N)
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    // Karatsuba
    Karatsuba_aux_old(R, poly1->coefs, poly2->coefs, N, buf, zparams, context, alloc.createStackChildAllocator());

    // reduction mod X^N+1
    for (int32_t i = 0; i < N - 1; ++i)
        result->coefs[i] = R[i] - R[N + i];
    result->coefs[N - 1] = R[N - 1];

    delete[] buf;
    delete[] R;
}

// poly1, poly2 and result are polynomials mod X^N+1
template<typename TORUS, AsmTypeEnum AsmType>
void TorusPolynomial<TORUS, AsmType>::AddMulRKaratsuba_old(
        TorusPolynomial<TORUS, AsmType> *result,
        const IntPolynomial<TORUS, AsmType> *poly1,
        const TorusPolynomial<TORUS, AsmType> *poly2,
        const PolynomialParams<TORUS, AsmType> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t N = params->N;
    TORUS *R = new TORUS[2 * N - 1];
    char *buf = new char[16 * N]; //that's large enough to store every tmp variables (2*2*N*4)
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    // Karatsuba
    Karatsuba_aux_old(R, poly1->coefs, poly2->coefs, N, buf, zparams, context, alloc.createStackChildAllocator());

    // reduction mod X^N+1
    for (int32_t i = 0; i < N - 1; ++i)
        result->coefs[i] += R[i] - R[N + i];
    result->coefs[N - 1] += R[N - 1];

    delete[] R;
    delete[] buf;
}

// poly1, poly2 and result are polynomials mod X^N+1
template<typename TORUS, AsmTypeEnum AsmType>
void TorusPolynomial<TORUS, AsmType>::SubMulRKaratsuba_old(
        TorusPolynomial<TORUS, AsmType> *result,
        const IntPolynomial<TORUS, AsmType> *poly1,
        const TorusPolynomial<TORUS, AsmType> *poly2,
        const PolynomialParams<TORUS, AsmType> *params,
        TfheThreadContext *context,
        Allocator alloc) {
    const int32_t N = params->N;
    TORUS *R = new TORUS[2 * N - 1];
    char *buf = new char[16 * N]; //that's large enough to store every tmp variables (2*2*N*4)
    const ZModuleParams<TORUS> *const zparams =
            params->zmodule_params;

    // Karatsuba
    Karatsuba_aux_old(R, poly1->coefs, poly2->coefs, N, buf, zparams, context, alloc.createStackChildAllocator());

    // reduction mod X^N+1
    for (int32_t i = 0; i < N - 1; ++i)
        result->coefs[i] -= R[i] - R[N + i];
    result->coefs[N - 1] -= R[N - 1];

    delete[] R;
    delete[] buf;
}
























/**
 * Multiplication of two polynomials, one integer (INT_TYPE) and the other
 * with torus (TORUS) coefficients via Karatsuba
 * The auxiliary functions (_aux) are:
 * * MultNaive_plain_aux: computes a naive multiplication
 * * Karatsuba_aux: recursive Karatsuba function
 * The main functions (calling Karatsuba_aux) perform the Karatsuba
 * multiplication of the two polynomials modulo X^N+1:
 * MultKaratsuba: result = poly1 * poly 2
 * AddMultKaratsuba: result += poly1 * poly 2
 * SubMultKaratsuba: result -= poly1 * poly 2
 */


// naive multiplication
template<typename TORUS, typename INT_TYPE>
static void MultNaive_plain_aux(TORUS *__restrict result,
                                const INT_TYPE *__restrict poly1,
                                const TORUS *__restrict poly2,
                                const int32_t N) {
    const int32_t _2Nm1 = 2 * N - 1;
    TORUS ri;

    for (int32_t i = 0; i < N; i++) {
        ri = 0;
        for (int32_t j = 0; j <= i; j++) {
            ri += poly1[j] * poly2[i - j];
        }
        result[i] = ri;
    }

    for (int32_t i = N; i < _2Nm1; i++) {
        ri = 0;
        for (int32_t j = i - N + 1; j < N; j++) {
            ri += poly1[j] * poly2[i - j];
        }
        result[i] = ri;
    }
}


// recursive Karatsuba
// A and B of size = size
// R of size = 2*size-1
template<typename TORUS, typename INT_TYPE>
void Karatsuba_aux(TORUS *R,
                   INT_TYPE *bufInt,
                   TORUS *bufTorus,
                   const INT_TYPE *A,
                   const TORUS *B,
                   const int32_t size,
                   const ZModuleParams<TORUS> *const zparams) {
    const int32_t h = size / 2;
    const int32_t sm1 = size - 1;

    // h<=4 experimentally chosen
    // it seems to be optimal
    if (h <= 4) {
        MultNaive_plain_aux(R, A, B, size);
        return;
    }


    //we split the polynomials in 2
    // A = A0 + A1*X^h
    // Atemp = A0 + A1
    INT_TYPE *Atemp = bufInt;
    for (int32_t i = 0; i < h; ++i) {
        Atemp[i] = A[i] + A[h + i];
    }

    // B = B0 + B1*X^h
    // Btemp = B0 + B1
    TORUS *Btemp = bufTorus;
    for (int32_t i = 0; i < h; ++i) {
        Btemp[i] = B[i] + B[h + i];
    }

    // R
    TORUS *Rtemp = bufTorus + h;


    // Karatsuba recursivly
    // R0 = A0 * B0
    // (R[0],R[2*h-2]), (A[0],A[h-1]), (B[0],B[h-1])
    Karatsuba_aux(R, bufInt, bufTorus, A, B, h, zparams);
    // R1 = A1 * B1
    // (R[2*h],R[4*h-2]), (A[h],A[2*h-1]), (B[h],B[2*h-1])
    Karatsuba_aux(R + size, bufInt, bufTorus, A + h, B + h, h, zparams);
    // Rtemp = Atemp * Btemp = (A0 + A1)*(B0 + B1)
    Karatsuba_aux(Rtemp, bufInt, bufTorus, Atemp, Btemp, h, zparams);
    // R[2*h-1] = 0
    R[sm1] = 0; //this one needs to be copy manually


    // R = R0 + Rtemp * X^h + R1 * X^size
    // with Rtemp = Rtemp - R0 - R1
    for (int32_t i = 0; i < sm1; ++i)
        Rtemp[i] -= R[i] + R[size + i];
    for (int32_t i = 0; i < sm1; ++i)
        R[h + i] += Rtemp[i];

}


// Karatsuba: result = poly1 * poly2
// poly1, poly2 and result are polynomials mod X^N+1
template<typename TORUS, AsmTypeEnum AsmType>
void TorusPolynomial<TORUS, AsmType>::MultKaratsuba(TorusPolynomial<TORUS, AsmType> *result,
                                                    const IntPolynomial<TORUS, AsmType> *poly1,
                                                    const TorusPolynomial<TORUS, AsmType> *poly2,
                                                    const PolynomialParams<TORUS, AsmType> *params,
                                           TfheThreadContext *context,
                                           Allocator alloc) {
    const int32_t N = params->N;
    // zmodule_params
    const ZModuleParams<TORUS> *const zparams = params->zmodule_params;


    // Buffer: that's large enough to store every tmp INT variable (1*N*sizeof(INT_TYPE))
    INT_TYPE *bufInt = alloc.newArray<INT_TYPE>(N);
    // Buffer: that's large enough to store every tmp TORUS variable (3*N*sizeof(TORUS))
    TORUS *bufTorus = alloc.newArray<TORUS>(3 * N);
    // Result in Karatsuba_aux
    TORUS *R = alloc.newArray<TORUS>(2 * N - 1);


    // Karatsuba
    Karatsuba_aux(R, bufInt, bufTorus, poly1->coefs, poly2->coefs, N, zparams);

    // reduction mod X^N+1
    for (int32_t i = 0; i < N - 1; ++i) {
        result->coefs[i] = R[i] - R[N + i];
    }
    result->coefs[N - 1] = R[N - 1];


    // delete
    alloc.deleteArray<TORUS>(2 * N - 1, R);
    alloc.deleteArray<TORUS>(3 * N, bufTorus);
    alloc.deleteArray<INT_TYPE>(N, bufInt);
}


// Karatsuba: result += poly1 * poly2
// poly1, poly2 and result are polynomials mod X^N+1
template<typename TORUS, AsmTypeEnum AsmType>
void TorusPolynomial<TORUS, AsmType>::AddMulRKaratsuba(TorusPolynomial<TORUS, AsmType> *result,
                                                       const IntPolynomial<TORUS, AsmType> *poly1,
                                                       const TorusPolynomial<TORUS, AsmType> *poly2,
                                                       const PolynomialParams<TORUS, AsmType> *params,
                                              TfheThreadContext *context,
                                              Allocator alloc) {
    const int32_t N = params->N;
    // zmodule_params
    const ZModuleParams<TORUS> *const zparams = params->zmodule_params;


    // Buffer: that's large enough to store every tmp INT variable (1*N*sizeof(INT_TYPE))
    INT_TYPE *bufInt = alloc.newArray<INT_TYPE>(N);
    // Buffer: that's large enough to store every tmp TORUS variable (3*N*sizeof(TORUS))
    TORUS *bufTorus = alloc.newArray<TORUS>(3 * N);
    // Result in Karatsuba_aux
    TORUS *R = alloc.newArray<TORUS>(2 * N - 1);


    // Karatsuba
    Karatsuba_aux(R, bufInt, bufTorus, poly1->coefs, poly2->coefs, N, zparams);

    // reduction mod X^N+1
    for (int32_t i = 0; i < N - 1; ++i) {
        result->coefs[i] += R[i] - R[N + i];
    }
    result->coefs[N - 1] += R[N - 1];


    // delete
    alloc.deleteArray<TORUS>(2 * N - 1, R);
    alloc.deleteArray<TORUS>(3 * N, bufTorus);
    alloc.deleteArray<INT_TYPE>(N, bufInt);
}


// Karatsuba: result -= poly1 * poly2
// poly1, poly2 and result are polynomials mod X^N+1
template<typename TORUS, AsmTypeEnum AsmType>
void TorusPolynomial<TORUS, AsmType>::SubMulRKaratsuba(TorusPolynomial<TORUS, AsmType> *result,
                                                       const IntPolynomial<TORUS, AsmType> *poly1,
                                                       const TorusPolynomial<TORUS, AsmType> *poly2,
                                                       const PolynomialParams<TORUS, AsmType> *params,
                                              TfheThreadContext *context,
                                              Allocator alloc) {
    const int32_t N = params->N;
    // zmodule_params
    const ZModuleParams<TORUS> *const zparams = params->zmodule_params;


    // Buffer: that's large enough to store every tmp INT variable (1*N*sizeof(INT_TYPE))
    INT_TYPE *bufInt = alloc.newArray<INT_TYPE>(N);
    // Buffer: that's large enough to store every tmp TORUS variable (3*N*sizeof(TORUS))
    TORUS *bufTorus = alloc.newArray<TORUS>(3 * N);
    // Result in Karatsuba_aux
    TORUS *R = alloc.newArray<TORUS>(2 * N - 1);


    // Karatsuba
    Karatsuba_aux(R, bufInt, bufTorus, poly1->coefs, poly2->coefs, N, zparams);

    // reduction mod X^N+1
    for (int32_t i = 0; i < N - 1; ++i) {
        result->coefs[i] -= R[i] - R[N + i];
    }
    result->coefs[N - 1] -= R[N - 1];


    // delete
    alloc.deleteArray<TORUS>(2 * N - 1, R);
    alloc.deleteArray<TORUS>(3 * N, bufTorus);
    alloc.deleteArray<INT_TYPE>(N, bufInt);
}



