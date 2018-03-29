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
void TorusPolyFunctions<TORUS>::Clear(TorusPolynomial<TORUS> *result) {
    const int N = result->N;

    for (int i = 0; i < N; ++i)
        result->coefsT[i] = 0;
}

// TorusPolynomial = random
template<typename TORUS>
void TorusPolyFunctions<TORUS>::Uniform(TorusPolynomial<TORUS> *result) {
    const int N = result->N;
    TORUS *x = result->coefsT;

    for (int i = 0; i < N; ++i)
        x[i] = RandomGenTorus<TORUS>::uniform();
}

// TorusPolynomial = TorusPolynomial
template<typename TORUS>
void TorusPolyFunctions<TORUS>::Copy(TorusPolynomial<TORUS> *result,
                                     const TorusPolynomial<TORUS> *sample) {
    assert(result != sample);
    const int N = result->N;
    const TORUS *s = sample->coefsT;
    TORUS *r = result->coefsT;

    for (int i = 0; i < N; ++i)
        r[i] = s[i];
}

// TorusPolynomial + TorusPolynomial
template<typename TORUS>
void TorusPolyFunctions<TORUS>::Add(TorusPolynomial<TORUS> *result,
                                    const TorusPolynomial<TORUS> *poly1,
                                    const TorusPolynomial<TORUS> *poly2) {
    const int N = poly1->N;
    assert(result != poly1); //if it fails here, please use addTo
    assert(result != poly2); //if it fails here, please use addTo
    TORUS *r = result->coefsT;
    const TORUS *a = poly1->coefsT;
    const TORUS *b = poly2->coefsT;

    for (int i = 0; i < N; ++i)
        r[i] = a[i] + b[i];
}

// TorusPolynomial += TorusPolynomial
template<typename TORUS>
void TorusPolyFunctions<TORUS>::AddTo(TorusPolynomial<TORUS> *result,
                                      const TorusPolynomial<TORUS> *poly2) {
    const int N = poly2->N;
    TORUS *r = result->coefsT;
    const TORUS *b = poly2->coefsT;

    for (int i = 0; i < N; ++i)
        r[i] += b[i];
}


// TorusPolynomial - TorusPolynomial
template<typename TORUS>
void TorusPolyFunctions<TORUS>::Sub(TorusPolynomial<TORUS> *result,
                                    const TorusPolynomial<TORUS> *poly1,
                                    const TorusPolynomial<TORUS> *poly2) {
    const int N = poly1->N;
    assert(result != poly1); //if it fails here, please use subTo
    assert(result != poly2); //if it fails here, please use subTo
    TORUS *r = result->coefsT;
    const TORUS *a = poly1->coefsT;
    const TORUS *b = poly2->coefsT;

    for (int i = 0; i < N; ++i)
        r[i] = a[i] - b[i];
}

// TorusPolynomial -= TorusPolynomial
template<typename TORUS>
void TorusPolyFunctions<TORUS>::SubTo(TorusPolynomial<TORUS> *result,
                                      const TorusPolynomial<TORUS> *poly2) {
    const int N = poly2->N;
    TORUS *r = result->coefsT;
    const TORUS *b = poly2->coefsT;

    for (int i = 0; i < N; ++i)
        r[i] -= b[i];
}

// TorusPolynomial + p*TorusPolynomial
template<typename TORUS>
void TorusPolyFunctions<TORUS>::AddMulZ(TorusPolynomial<TORUS> *result,
                                        const TorusPolynomial<TORUS> *poly1,
                                        int p,
                                        const TorusPolynomial<TORUS> *poly2) {
    const int N = poly1->N;
    assert(result != poly1); //if it fails here, please use AddMulZTo
    TORUS *r = result->coefsT;
    const TORUS *a = poly1->coefsT;
    const TORUS *b = poly2->coefsT;

    for (int i = 0; i < N; ++i)
        r[i] = a[i] + p * b[i];
}

// TorusPolynomial += p*TorusPolynomial
template<typename TORUS>
void
TorusPolyFunctions<TORUS>::AddMulZTo(TorusPolynomial<TORUS> *result,
                                     const int p,
                                     const TorusPolynomial<TORUS> *poly2) {
    const int N = poly2->N;
    TORUS *r = result->coefsT;
    const TORUS *b = poly2->coefsT;

    for (int i = 0; i < N; ++i)
        r[i] += p * b[i];
}

// TorusPolynomial - p*TorusPolynomial
template<typename TORUS>
void
TorusPolyFunctions<TORUS>::SubMulZ(TorusPolynomial<TORUS> *result,
                                   const TorusPolynomial<TORUS> *poly1,
                                   const int p,
                                   const TorusPolynomial<TORUS> *poly2) {
    const int N = poly1->N;
    assert(result != poly1); //if it fails here, please use SubMulZTo
    TORUS *r = result->coefsT;
    const TORUS *a = poly1->coefsT;
    const TORUS *b = poly2->coefsT;

    for (int i = 0; i < N; ++i)
        r[i] = a[i] - p * b[i];
}

// TorusPolynomial -= p*TorusPolynomial
template<typename TORUS>
void TorusPolyFunctions<TORUS>::SubMulZTo(TorusPolynomial<TORUS> *result,
                                          int p,
                                          const TorusPolynomial<TORUS> *poly2) {
    const int N = poly2->N;
    TORUS *r = result->coefsT;
    const TORUS *b = poly2->coefsT;

    for (int i = 0; i < N; ++i)
        r[i] -= p * b[i];
}

//result = (X^{a}-1)*source
template<typename TORUS>
void TorusPolyFunctions<TORUS>::MulByXaiMinusOne(TorusPolynomial<TORUS> *result,
                                                 int a,
                                                 const TorusPolynomial<TORUS> *source) {
    const int N = source->N;
    TORUS *out = result->coefsT;
    TORUS *in = source->coefsT;

    assert(a >= 0 && a < 2 * N);

    if (a < N) {
        for (int i = 0; i < a; i++) // i-a < 0
            out[i] = -in[i - a + N] - in[i];
        for (int i = a; i < N; i++) // N > i-a >= 0
            out[i] = in[i - a] - in[i];
    } else {
        const int aa = a - N;
        for (int i = 0; i < aa; i++)//sur que i-a<0
            out[i] = in[i - aa + N] - in[i];
        for (int i = aa; i < N; i++)//sur que N>i-a>=0
            out[i] = -in[i - aa] - in[i];
    }
}


//result= X^{a}*source
template<typename TORUS>
void TorusPolyFunctions<TORUS>::MulByXai(TorusPolynomial<TORUS> *result,
                                         int a,
                                         const TorusPolynomial<TORUS> *source) {
    const int N = source->N;
    TORUS *out = result->coefsT;
    TORUS *in = source->coefsT;

    assert(a >= 0 && a < 2 * N);
    assert(result != source);

    if (a < N) {
        for (int i = 0; i < a; i++)//sur que i-a<0
            out[i] = -in[i - a + N];
        for (int i = a; i < N; i++)//sur que N>i-a>=0
            out[i] = in[i - a];
    } else {
        const int aa = a - N;
        for (int i = 0; i < aa; i++)//sur que i-a<0
            out[i] = in[i - aa + N];
        for (int i = aa; i < N; i++)//sur que N>i-a>=0
            out[i] = -in[i - aa];
    }
}


// Infinity norm of the distance between two TorusPolynomial
template<typename TORUS>
double TorusPolyFunctions<TORUS>::NormInftyDist(const TorusPolynomial<TORUS> *poly1,
                                                const TorusPolynomial<TORUS> *poly2) {
    const int N = poly1->N;
    double norm = 0;

    // Max between the coefficients of abs(poly1-poly2)
    for (int i = 0; i < N; ++i) {
        double r = abs(TorusUtils<TORUS>::to_double(poly1->coefsT[i] - poly2->coefsT[i]));
        if (r > norm) { norm = r; }
    }
    return norm;
}





/*
 * INTEGER POLYNOMIALS
 */


// Sets to zero
void IntPolyFunctions::Clear(IntPolynomial *poly) {
    const int N = poly->N;
    for (int i = 0; i < N; ++i)
        poly->coefs[i] = 0;
}

// Sets to zero
void IntPolyFunctions::Copy(IntPolynomial *result,
                            const IntPolynomial *source) {
    const int N = source->N;
    assert(result != source);

    for (int i = 0; i < N; ++i)
        result->coefs[i] = source->coefs[i];
}

// accum += source
void IntPolyFunctions::AddTo(IntPolynomial *accum,
                             const IntPolynomial *source) {
    const int N = source->N;

    for (int i = 0; i < N; ++i)
        accum->coefs[i] += source->coefs[i];
}

// result = (X^ai-1) * source
void IntPolyFunctions::MulByXaiMinusOne(IntPolynomial *result,
                                        int ai,
                                        const IntPolynomial *source) {
    const int N = source->N;
    int *out = result->coefs;
    int *in = source->coefs;

    assert(ai >= 0 && ai < 2 * N);

    if (ai < N) {
        for (int i = 0; i < ai; i++)//sur que i-a<0
            out[i] = -in[i - ai + N] - in[i];
        for (int i = ai; i < N; i++)//sur que N>i-a>=0
            out[i] = in[i - ai] - in[i];
    } else {
        const int aa = ai - N;
        for (int i = 0; i < aa; i++)//sur que i-a<0
            out[i] = in[i - aa + N] - in[i];
        for (int i = aa; i < N; i++)//sur que N>i-a>=0
            out[i] = -in[i - aa] - in[i];
    }
}

// Euclidean norm of an IntPolynomial
double IntPolyFunctions::Norm2sq(const IntPolynomial *poly) {
    const int N = poly->N;
    double norm = 0;

    for (int i = 0; i < N; ++i) {
        double r = poly->coefs[i];
        norm += r * r;
    }
    return norm;
}

// Infinity norm of the distance between two IntPolynomial
double IntPolyFunctions::NormInftyDist(const IntPolynomial *poly1,
                                       const IntPolynomial *poly2) {
    const int N = poly1->N;
    double norm = 0;

    // Max between the coefficients of abs(poly1-poly2)
    for (int i = 0; i < N; ++i) {
        double r = abs(poly1->coefs[i] - poly2->coefs[i]);
        if (r > norm) { norm = r; }
    }
    return norm;
}


