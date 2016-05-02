#include <complex.h>
#define complex _Complex
#include <fftw3.h>
#include <cassert>
#include <cmath>
#include "lwe.h"
#include "polynomials.h"

using namespace std;

// TorusPolynomial = 0
EXPORT void ClearTorusPolynomial(TorusPolynomial* result) {
    const int N = result->N;

    for (int i = 0; i < N; ++i) result->coefsT[i] = 0;
}

// TorusPolynomial = random
EXPORT void UniformTorusPolynomial(TorusPolynomial* result) {
    const int N = result->N;
    Torus32* x = result->coefsT;

    for (int i = 0; i < N; ++i) 
	x[i] = uniformTorus32_distrib(generator);
}

// TorusPolynomial = TorusPolynomial
EXPORT void CopyTorusPolynomial(
	TorusPolynomial* result, 
	const TorusPolynomial* sample) {
    assert(result!=sample);
    const int N = result->N;
    const Torus32* __restrict s = sample->coefsT;
    Torus32* __restrict r = result->coefsT;

    for (int i = 0; i < N; ++i) r[i] = s[i];
}

// TorusPolynomial + TorusPolynomial
EXPORT void AddTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2) {
    const int N = poly1->N;
    assert(result!=poly1); //if it fails here, please use addTo
    assert(result!=poly2); //if it fails here, please use addTo
    Torus32* __restrict r = result->coefsT;
    const Torus32* __restrict a = poly1->coefsT;
    const Torus32* __restrict b = poly2->coefsT;

    for (int i = 0; i < N; ++i) 
	r[i] = a[i] + b[i];
}

// TorusPolynomial += TorusPolynomial
EXPORT void AddToTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly2) {
    const int N = poly2->N;
    Torus32* r = result->coefsT;
    const Torus32* b = poly2->coefsT;

    for (int i = 0; i < N; ++i) 
	r[i] += b[i];
}


// TorusPolynomial - TorusPolynomial
EXPORT void SubTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2) {
    const int N = poly1->N;
    assert(result!=poly1); //if it fails here, please use subTo
    assert(result!=poly2); //if it fails here, please use subTo
    Torus32* __restrict r = result->coefsT;
    const Torus32* a = poly1->coefsT;
    const Torus32* b = poly2->coefsT;

    for (int i = 0; i < N; ++i) 
	r[i] = a[i] - b[i];
}

// TorusPolynomial -= TorusPolynomial
EXPORT void SubToTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly2) {
    const int N = poly2->N;
    Torus32* r = result->coefsT;
    const Torus32* b = poly2->coefsT;

    for (int i = 0; i < N; ++i) 
	r[i] -= b[i];
}

// TorusPolynomial + p*TorusPolynomial
EXPORT void AddMulZTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, int p, const TorusPolynomial* poly2) {
    const int N = poly1->N;
    Torus32* r = result->coefsT;
    const Torus32* a = poly1->coefsT;
    const Torus32* b = poly2->coefsT;

    for (int i = 0; i < N; ++i) 
	r[i] = a[i] + p*b[i];
}

// TorusPolynomial += p*TorusPolynomial
EXPORT void AddMulZToTorusPolynomial(TorusPolynomial* result, const int p, const TorusPolynomial* poly2) {
    const int N = poly2->N;
    Torus32* r = result->coefsT;
    const Torus32* b = poly2->coefsT;

    for (int i = 0; i < N; ++i) r[i] += p*b[i];
}

// TorusPolynomial - p*TorusPolynomial
EXPORT void SubMulZTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, const int p, const TorusPolynomial* poly2) {
    int N = poly1->N;
    Torus32* r = result->coefsT;
    const Torus32* a = poly1->coefsT;
    const Torus32* b = poly2->coefsT;

    for (int i = 0; i < N; ++i) r[i] = a[i] - p*b[i];
}

// TorusPolynomial -= p*TorusPolynomial
EXPORT void SubMulZToTorusPolynomial(TorusPolynomial* result, int p, const TorusPolynomial* poly2) {
    int N = poly2->N;
    Torus32* r = result->coefsT;
    const Torus32* b = poly2->coefsT;

    for (int i = 0; i < N; ++i) r[i] += p*b[i];
}


// Norme Euclidienne d'un IntPolynomial
EXPORT double intPolynomialNormSq2(const IntPolynomial* poly){
    const int N = poly->N;
    int temp1 = 0;

    for (int i = 0; i < N; ++i)
    {
        int temp0 = poly->coefs[i]*poly->coefs[i];
        temp1 += temp0;
    }
    return temp1;
}
