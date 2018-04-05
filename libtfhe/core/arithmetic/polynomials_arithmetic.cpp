#include <cassert>
#include <cmath>
#include "tfhe_core.h"
#include "polynomials.h"
#include "polynomials_arithmetic.h"
#include "random_gen.h"

using namespace std;


/*
 * TORUS POLYNOMIALS
 */


// TorusPolynomial = 0
template<typename TORUS>
void TorusPolyFunctions<TORUS>::Clear(TorusPolynomial<TORUS> *result,
                                      const PolynomialParameters *params,
                                      TfheThreadContext *context,
                                      Allocator alloc) {
    const int32_t N = params->N;

    for (int32_t i = 0; i < N; ++i)
        result->coefs[i] = 0;
}

// TorusPolynomial = random
template<typename TORUS>
void TorusPolyFunctions<TORUS>::Uniform(TorusPolynomial<TORUS> *result,
                                        const PolynomialParameters *params,
                                        TfheThreadContext *context,
                                        Allocator alloc) {
    const int32_t N = params->N;
    TORUS *x = result->coefs;

    for (int32_t i = 0; i < N; ++i)
        x[i] = RandomGenTorus<TORUS>::uniform();
}

// TorusPolynomial = TorusPolynomial
template<typename TORUS>
void TorusPolyFunctions<TORUS>::Copy(TorusPolynomial<TORUS> *result,
                                     const TorusPolynomial<TORUS> *sample,
                                     const PolynomialParameters *params,
                                     TfheThreadContext *context,
                                     Allocator alloc) {
    assert(result != sample);
    const int32_t N = params->N;
    const TORUS *s = sample->coefs;
    TORUS *r = result->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] = s[i];
}

// TorusPolynomial + TorusPolynomial
template<typename TORUS>
void TorusPolyFunctions<TORUS>::Add(TorusPolynomial<TORUS> *result,
                                    const TorusPolynomial<TORUS> *poly1,
                                    const TorusPolynomial<TORUS> *poly2,
                                    const PolynomialParameters *params,
                                    TfheThreadContext *context,
                                    Allocator alloc) {
    const int32_t N = params->N;
    assert(result != poly1); //if it fails here, please use addTo
    assert(result != poly2); //if it fails here, please use addTo
    TORUS *r = result->coefs;
    const TORUS *a = poly1->coefs;
    const TORUS *b = poly2->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] = a[i] + b[i];
}

// TorusPolynomial += TorusPolynomial
template<typename TORUS>
void TorusPolyFunctions<TORUS>::AddTo(TorusPolynomial<TORUS> *result,
                                      const TorusPolynomial<TORUS> *poly2,
                                      const PolynomialParameters *params,
                                      TfheThreadContext *context,
                                      Allocator alloc) {
    const int32_t N = params->N;
    TORUS *r = result->coefs;
    const TORUS *b = poly2->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] += b[i];
}


// TorusPolynomial - TorusPolynomial
template<typename TORUS>
void TorusPolyFunctions<TORUS>::Sub(TorusPolynomial<TORUS> *result,
                                    const TorusPolynomial<TORUS> *poly1,
                                    const TorusPolynomial<TORUS> *poly2,
                                    const PolynomialParameters *params,
                                    TfheThreadContext *context,
                                    Allocator alloc) {
    const int32_t N = params->N;
    assert(result != poly1); //if it fails here, please use subTo
    assert(result != poly2); //if it fails here, please use subTo
    TORUS *r = result->coefs;
    const TORUS *a = poly1->coefs;
    const TORUS *b = poly2->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] = a[i] - b[i];
}

// TorusPolynomial -= TorusPolynomial
template<typename TORUS>
void TorusPolyFunctions<TORUS>::SubTo(TorusPolynomial<TORUS> *result,
                                      const TorusPolynomial<TORUS> *poly2,
                                      const PolynomialParameters *params,
                                      TfheThreadContext *context,
                                      Allocator alloc) {
    const int32_t N = params->N;
    TORUS *r = result->coefs;
    const TORUS *b = poly2->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] -= b[i];
}

// TorusPolynomial + p*TorusPolynomial
template<typename TORUS>
void TorusPolyFunctions<TORUS>::AddMulZ(TorusPolynomial<TORUS> *result,
                                        const TorusPolynomial<TORUS> *poly1,
                                        const INT_TYPE p,
                                        const TorusPolynomial<TORUS> *poly2,
                                        const PolynomialParameters *params,
                                        TfheThreadContext *context,
                                        Allocator alloc) {
    const int32_t N = params->N;
    assert(result != poly1); //if it fails here, please use AddMulZTo
    TORUS *r = result->coefs;
    const TORUS *a = poly1->coefs;
    const TORUS *b = poly2->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] = a[i] + p * b[i];
}

// TorusPolynomial += p*TorusPolynomial
template<typename TORUS>
void
TorusPolyFunctions<TORUS>::AddMulZTo(TorusPolynomial<TORUS> *result,
                                     const INT_TYPE p,
                                     const TorusPolynomial<TORUS> *poly2,
                                     const PolynomialParameters *params,
                                     TfheThreadContext *context,
                                     Allocator alloc) {
    const int32_t N = params->N;
    TORUS *r = result->coefs;
    const TORUS *b = poly2->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] += p * b[i];
}

// TorusPolynomial - p*TorusPolynomial
template<typename TORUS>
void
TorusPolyFunctions<TORUS>::SubMulZ(TorusPolynomial<TORUS> *result,
                                   const TorusPolynomial<TORUS> *poly1,
                                   const INT_TYPE p,
                                   const TorusPolynomial<TORUS> *poly2,
                                   const PolynomialParameters *params,
                                   TfheThreadContext *context,
                                   Allocator alloc) {
    const int32_t N = params->N;
    assert(result != poly1); //if it fails here, please use SubMulZTo
    TORUS *r = result->coefs;
    const TORUS *a = poly1->coefs;
    const TORUS *b = poly2->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] = a[i] - p * b[i];
}

// TorusPolynomial -= p*TorusPolynomial
template<typename TORUS>
void TorusPolyFunctions<TORUS>::SubMulZTo(TorusPolynomial<TORUS> *result,
                                          const INT_TYPE p,
                                          const TorusPolynomial<TORUS> *poly2,
                                          const PolynomialParameters *params,
                                          TfheThreadContext *context,
                                          Allocator alloc) {
    const int32_t N = params->N;
    TORUS *r = result->coefs;
    const TORUS *b = poly2->coefs;

    for (int32_t i = 0; i < N; ++i)
        r[i] -= p * b[i];
}

//result = (X^{a}-1)*source
template<typename TORUS>
void TorusPolyFunctions<TORUS>::MulByXaiMinusOne(TorusPolynomial<TORUS> *result,
                                                 const INT_TYPE a,
                                                 const TorusPolynomial<TORUS> *source,
                                                 const PolynomialParameters *params,
                                                 TfheThreadContext *context,
                                                 Allocator alloc) {
    const int32_t N = params->N;
    TORUS *out = result->coefs;
    TORUS *in = source->coefs;

    assert(a >= 0 && a < 2 * N);

    if (a < N) {
        for (int32_t i = 0; i < a; i++) // i-a < 0
            out[i] = -in[i - a + N] - in[i];
        for (int32_t i = a; i < N; i++) // N > i-a >= 0
            out[i] = in[i - a] - in[i];
    } else {
        const int32_t aa = a - N;
        for (int32_t i = 0; i < aa; i++)//sur que i-a<0
            out[i] = in[i - aa + N] - in[i];
        for (int32_t i = aa; i < N; i++)//sur que N>i-a>=0
            out[i] = -in[i - aa] - in[i];
    }
}


//result= X^{a}*source
template<typename TORUS>
void TorusPolyFunctions<TORUS>::MulByXai(TorusPolynomial<TORUS> *result,
                                         const INT_TYPE a,
                                         const TorusPolynomial<TORUS> *source,
                                         const PolynomialParameters *params,
                                         TfheThreadContext *context,
                                         Allocator alloc) {
    const int32_t N = params->N;
    TORUS *out = result->coefs;
    TORUS *in = source->coefs;

    assert(a >= 0 && a < 2 * N);
    assert(result != source);

    if (a < N) {
        for (int32_t i = 0; i < a; i++)//sur que i-a<0
            out[i] = -in[i - a + N];
        for (int32_t i = a; i < N; i++)//sur que N>i-a>=0
            out[i] = in[i - a];
    } else {
        const int32_t aa = a - N;
        for (int32_t i = 0; i < aa; i++)//sur que i-a<0
            out[i] = in[i - aa + N];
        for (int32_t i = aa; i < N; i++)//sur que N>i-a>=0
            out[i] = -in[i - aa];
    }
}


// Infinity norm of the distance between two TorusPolynomial
template<typename TORUS>
double TorusPolyFunctions<TORUS>::NormInftyDist(const TorusPolynomial<TORUS> *poly1,
                                                const TorusPolynomial<TORUS> *poly2,
                                                const PolynomialParameters *params,
                                                TfheThreadContext *context,
                                                Allocator alloc) {
    const int32_t N = params->N;
    double norm = 0;

    // Max between the coefficients of abs(poly1-poly2)
    for (int32_t i = 0; i < N; ++i) {
        double r = abs(TorusUtils<TORUS>::to_double(poly1->coefs[i] - poly2->coefs[i]));
        if (r > norm) { norm = r; }
    }
    return norm;
}



/**
 * Integer polynomial functions
 */

// Sets to zero
template<typename INT_TYPE>
void IntPolyFunctions<INT_TYPE>::Clear(
    IntPolynomial<INT_TYPE> *poly,
    const PolynomialParameters *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    for (int32_t i = 0; i < N; ++i)
        poly->coefs[i] = 0;
}

// copy
template<typename INT_TYPE>
void IntPolyFunctions<INT_TYPE>::Copy(
    IntPolynomial<INT_TYPE> *result,
    const IntPolynomial<INT_TYPE> *source,
    const PolynomialParameters *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    assert(result != source);

    for (int32_t i = 0; i < N; ++i)
        result->coefs[i] = source->coefs[i];
}

// accum += source
template<typename INT_TYPE>
void IntPolyFunctions<INT_TYPE>::AddTo(
    IntPolynomial<INT_TYPE> *accum,
    const IntPolynomial<INT_TYPE> *source,
    const PolynomialParameters *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;

    for (int32_t i = 0; i < N; ++i)
        accum->coefs[i] += source->coefs[i];
}

// result = (X^ai-1) * source
template<typename INT_TYPE>
void IntPolyFunctions<INT_TYPE>::MulByXaiMinusOne(
    IntPolynomial<INT_TYPE> *result,
    const INT_TYPE ai,
    const IntPolynomial<INT_TYPE> *source,
    const PolynomialParameters *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    INT_TYPE *out = result->coefs;
    INT_TYPE *in = source->coefs;

    assert(ai >= 0 && ai < 2 * N);

    if (ai < N) {
        for (int32_t i = 0; i < ai; i++)//sur que i-a<0
            out[i] = -in[i - ai + N] - in[i];
        for (int32_t i = ai; i < N; i++)//sur que N>i-a>=0
            out[i] = in[i - ai] - in[i];
    } else {
        const int32_t aa = ai - N;
        for (int32_t i = 0; i < aa; i++)//sur que i-a<0
            out[i] = in[i - aa + N] - in[i];
        for (int32_t i = aa; i < N; i++)//sur que N>i-a>=0
            out[i] = -in[i - aa] - in[i];
    }
}

// Euclidean norm of an IntPolynomial
template<typename INT_TYPE>
double IntPolyFunctions<INT_TYPE>::Norm2sq(
    const IntPolynomial<INT_TYPE> *poly,
    const PolynomialParameters *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    double norm = 0;

    for (int32_t i = 0; i < N; ++i) {
        double r = poly->coefs[i];
        norm += r * r;
    }
    return norm;
}

// Infinity norm of the distance between two IntPolynomial
template<typename INT_TYPE>
double IntPolyFunctions<INT_TYPE>::NormInftyDist(
    const IntPolynomial<INT_TYPE> *poly1,
    const IntPolynomial<INT_TYPE> *poly2,
    const PolynomialParameters *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    const int32_t N = params->N;
    double norm = 0;

    // Max between the coefficients of abs(poly1-poly2)
    for (int32_t i = 0; i < N; ++i) {
        double r = abs(poly1->coefs[i] - poly2->coefs[i]);
        if (r > norm) { norm = r; }
    }
    return norm;
}


