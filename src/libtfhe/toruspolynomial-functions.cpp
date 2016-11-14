#include <complex.h>
#define complex _Complex
#include <fftw3.h>
#include <cassert>
#include <cmath>
#include "tfhe_core.h"
#include "numeric_functions.h"
#include "polynomials.h"

using namespace std;

// TorusPolynomial = 0
EXPORT void torusPolynomialClear(TorusPolynomial* result) {
    const int N = result->N;

    for (int i = 0; i < N; ++i) result->coefsT[i] = 0;
}

// TorusPolynomial = random
EXPORT void torusPolynomialUniform(TorusPolynomial* result) {
    const int N = result->N;
    Torus32* x = result->coefsT;

    for (int i = 0; i < N; ++i) 
	x[i] = uniformTorus32_distrib(generator);
}

// TorusPolynomial = TorusPolynomial
EXPORT void torusPolynomialCopy(
	TorusPolynomial* result, 
	const TorusPolynomial* sample) {
    assert(result!=sample);
    const int N = result->N;
    const Torus32* __restrict s = sample->coefsT;
    Torus32* __restrict r = result->coefsT;

    for (int i = 0; i < N; ++i) r[i] = s[i];
}

// TorusPolynomial + TorusPolynomial
EXPORT void torusPolynomialAdd(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2) {
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
EXPORT void torusPolynomialAddTo(TorusPolynomial* result, const TorusPolynomial* poly2) {
    const int N = poly2->N;
    Torus32* r = result->coefsT;
    const Torus32* b = poly2->coefsT;

    for (int i = 0; i < N; ++i) 
	r[i] += b[i];
}


// TorusPolynomial - TorusPolynomial
EXPORT void torusPolynomialSub(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2) {
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
EXPORT void torusPolynomialSubTo(TorusPolynomial* result, const TorusPolynomial* poly2) {
    const int N = poly2->N;
    Torus32* r = result->coefsT;
    const Torus32* b = poly2->coefsT;

    for (int i = 0; i < N; ++i) 
	r[i] -= b[i];
}

// TorusPolynomial + p*TorusPolynomial
EXPORT void torusPolynomialAddMulZ(TorusPolynomial* result, const TorusPolynomial* poly1, int p, const TorusPolynomial* poly2) {
    const int N = poly1->N;
    Torus32* r = result->coefsT;
    const Torus32* a = poly1->coefsT;
    const Torus32* b = poly2->coefsT;

    for (int i = 0; i < N; ++i) 
	r[i] = a[i] + p*b[i];
}

// TorusPolynomial += p*TorusPolynomial
EXPORT void torusPolynomialAddMulZTo(TorusPolynomial* result, const int p, const TorusPolynomial* poly2) {
    const int N = poly2->N;
    Torus32* r = result->coefsT;
    const Torus32* b = poly2->coefsT;

    for (int i = 0; i < N; ++i) r[i] += p*b[i];
}

// TorusPolynomial - p*TorusPolynomial
EXPORT void torusPolynomialSubMulZ(TorusPolynomial* result, const TorusPolynomial* poly1, const int p, const TorusPolynomial* poly2) {
    const int N = poly1->N;
    Torus32* r = result->coefsT;
    const Torus32* a = poly1->coefsT;
    const Torus32* b = poly2->coefsT;

    for (int i = 0; i < N; ++i) r[i] = a[i] - p*b[i];
}

// TorusPolynomial -= p*TorusPolynomial
EXPORT void torusPolynomialSubMulZTo(TorusPolynomial* result, int p, const TorusPolynomial* poly2) {
    const int N = poly2->N;
    Torus32* r = result->coefsT;
    const Torus32* b = poly2->coefsT;

    for (int i = 0; i < N; ++i) r[i] -= p*b[i];
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

// Sets to zero
EXPORT void intPolynomialClear(IntPolynomial* poly){
    const int N = poly->N;
    for (int i = 0; i < N; ++i)
        poly->coefs[i]=0;
}

// Sets to zero
EXPORT void intPolynomialCopy(IntPolynomial* result, const IntPolynomial* source){
    const int N = source->N;
    for (int i = 0; i < N; ++i)
        result->coefs[i]=source->coefs[i];
}

/** accum += source */
EXPORT void intPolynomialAddTo(IntPolynomial* accum, const IntPolynomial* source) {
    const int N = source->N;
    for (int i = 0; i < N; ++i)
        accum->coefs[i]+=source->coefs[i];
}

/**  result = (X^ai-1) * source */
EXPORT void intPolynomialMulByXaiMinusOne(IntPolynomial* result, int ai, const IntPolynomial* source) {
    const int N=source->N;
    int* out=result->coefs;
    int* in =source->coefs; 

    assert(ai>=0 && ai<2*N);

    if (ai<N) {
	for (int i=0;i<ai;i++)//sur que i-a<0
	    out[i]= -in[i-ai+N]-in[i];
	for (int i=ai;i<N;i++)//sur que N>i-a>=0
	    out[i]= in[i-ai]-in[i];
    } else {
	const int aa=ai-N;
	for (int i=0;i<aa;i++)//sur que i-a<0
	    out[i]= in[i-aa+N]-in[i];
	for (int i=aa;i<N;i++)//sur que N>i-a>=0
	    out[i]= -in[i-aa]-in[i];
    }
}



// Norme infini de la distance entre deux TorusPolynomial
EXPORT double torusPolynomialNormInftyDist(const TorusPolynomial* poly1, const TorusPolynomial* poly2) {
    const int N = poly1->N;
    double norm = 0;

    // Max between the coefficients of abs(poly1-poly2)
    for (int i = 0; i < N; ++i){
        double r = abs(t32tod(poly1->coefsT[i] - poly2->coefsT[i]));
        if (r>norm) {norm = r;}
    }
    return norm;
}
