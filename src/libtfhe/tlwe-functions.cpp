#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include "tlwe_functions.h"
#include "numeric_functions.h"
#include "polynomials_arithmetic.h"
#include "lagrangehalfc_arithmetic.h"
#include "tfhe_generic_templates.h"

using namespace std;


// TLwe
EXPORT void tLweKeyGen(TLweKey *result) {
    const int32_t N = result->params->N;
    const int32_t k = result->params->k;
    uniform_int_distribution<int32_t> distribution(0, 1);

    for (int32_t i = 0; i < k; ++i)
        for (int32_t j = 0; j < N; ++j)
            result->key[i].coefs[j] = distribution(generator);
}

/*create an homogeneous tlwe sample*/
EXPORT void tLweSymEncryptZero(TLweSample *result, double alpha, const TLweKey *key) {
    const int32_t N = key->params->N;
    const int32_t k = key->params->k;

    for (int32_t j = 0; j < N; ++j)
        result->b->coefsT[j] = gaussian32(0, alpha);

    for (int32_t i = 0; i < k; ++i) {
        torusPolynomialUniform(&result->a[i]);
        torusPolynomialAddMulR(result->b, &key->key[i], &result->a[i]);
    }

    result->current_variance = alpha * alpha;
}

EXPORT void tLweSymEncrypt(TLweSample *result, TorusPolynomial *message, double alpha, const TLweKey *key) {
    const int32_t N = key->params->N;

    tLweSymEncryptZero(result, alpha, key);

    for (int32_t j = 0; j < N; ++j)
        result->b->coefsT[j] += message->coefsT[j];
}

/**
 * encrypts a constant message
 */
EXPORT void tLweSymEncryptT(TLweSample *result, Torus32 message, double alpha, const TLweKey *key) {
    tLweSymEncryptZero(result, alpha, key);

    result->b->coefsT[0] += message;
}



/**
 * This function computes the phase of sample by using key : phi = b - a.s
 */
EXPORT void tLwePhase(TorusPolynomial *phase, const TLweSample *sample, const TLweKey *key) {
    const int32_t k = key->params->k;

    torusPolynomialCopy(phase, sample->b); // phi = b

    for (int32_t i = 0; i < k; ++i)
        torusPolynomialSubMulR(phase, &key->key[i], &sample->a[i]);
}


/**
 * This function computes the approximation of the phase 
 * à revoir, surtout le Msize
 */
EXPORT void tLweApproxPhase(TorusPolynomial *message, const TorusPolynomial *phase, int32_t Msize, int32_t N) {
    for (int32_t i = 0; i < N; ++i) message->coefsT[i] = approxPhase(phase->coefsT[i], Msize);
}




EXPORT void tLweSymDecrypt(TorusPolynomial *result, const TLweSample *sample, const TLweKey *key, int32_t Msize) {
    tLwePhase(result, sample, key);
    tLweApproxPhase(result, result, Msize, key->params->N);
}


EXPORT Torus32 tLweSymDecryptT(const TLweSample *sample, const TLweKey *key, int32_t Msize) {
    TorusPolynomial *phase = new_TorusPolynomial(key->params->N);

    tLwePhase(phase, sample, key);
    Torus32 result = approxPhase(phase->coefsT[0], Msize);

    delete_TorusPolynomial(phase);
    return result;
}




//Arithmetic operations on TLwe samples
/** result = (0,0) */
EXPORT void tLweClear(TLweSample *result, const TLweParams *params) {
    const int32_t k = params->k;

    for (int32_t i = 0; i < k; ++i) torusPolynomialClear(&result->a[i]);
    torusPolynomialClear(result->b);
    result->current_variance = 0.;
}


/** result = sample */
EXPORT void tLweCopy(TLweSample *result, const TLweSample *sample, const TLweParams *params) {
    const int32_t k = params->k;
    const int32_t N = params->N;

    for (int32_t i = 0; i <= k; ++i)
        for (int32_t j = 0; j < N; ++j)
            result->a[i].coefsT[j] = sample->a[i].coefsT[j];

    result->current_variance = sample->current_variance;
}



/** result = (0,mu) */
EXPORT void tLweNoiselessTrivial(TLweSample *result, const TorusPolynomial *mu, const TLweParams *params) {
    const int32_t k = params->k;

    for (int32_t i = 0; i < k; ++i) torusPolynomialClear(&result->a[i]);
    torusPolynomialCopy(result->b, mu);
    result->current_variance = 0.;
}

/** result = (0,mu) where mu is constant*/
EXPORT void tLweNoiselessTrivialT(TLweSample *result, const Torus32 mu, const TLweParams *params) {
    const int32_t k = params->k;

    for (int32_t i = 0; i < k; ++i) torusPolynomialClear(&result->a[i]);
    torusPolynomialClear(result->b);
    result->b->coefsT[0] = mu;
    result->current_variance = 0.;
}

/** result = result + sample */
EXPORT void tLweAddTo(TLweSample *result, const TLweSample *sample, const TLweParams *params) {
    const int32_t k = params->k;

    for (int32_t i = 0; i < k; ++i)
        torusPolynomialAddTo(&result->a[i], &sample->a[i]);
    torusPolynomialAddTo(result->b, sample->b);
    result->current_variance += sample->current_variance;
}

/** result = result - sample */
EXPORT void tLweSubTo(TLweSample *result, const TLweSample *sample, const TLweParams *params) {
    const int32_t k = params->k;

    for (int32_t i = 0; i < k; ++i)
        torusPolynomialSubTo(&result->a[i], &sample->a[i]);
    torusPolynomialSubTo(result->b, sample->b);
    result->current_variance += sample->current_variance;
}

/** result = result + p.sample */
EXPORT void tLweAddMulTo(TLweSample *result, int32_t p, const TLweSample *sample, const TLweParams *params) {
    const int32_t k = params->k;

    for (int32_t i = 0; i < k; ++i)
        torusPolynomialAddMulZTo(&result->a[i], p, &sample->a[i]);
    torusPolynomialAddMulZTo(result->b, p, sample->b);
    result->current_variance += (p * p) * sample->current_variance;
}

/** result = result - p.sample */
EXPORT void tLweSubMulTo(TLweSample *result, int32_t p, const TLweSample *sample, const TLweParams *params) {
    const int32_t k = params->k;

    for (int32_t i = 0; i < k; ++i)
        torusPolynomialSubMulZTo(&result->a[i], p, &sample->a[i]);
    torusPolynomialSubMulZTo(result->b, p, sample->b);
    result->current_variance += (p * p) * sample->current_variance;
}


/** result = result + p.sample */
EXPORT void
tLweAddMulRTo(TLweSample *result, const IntPolynomial *p, const TLweSample *sample, const TLweParams *params) {
    const int32_t k = params->k;

    for (int32_t i = 0; i <= k; ++i)
        torusPolynomialAddMulR(result->a + i, p, sample->a + i);
    result->current_variance += intPolynomialNormSq2(p) * sample->current_variance;
}



//mult externe de X^ai-1 par bki
EXPORT void tLweMulByXaiMinusOne(TLweSample *result, int32_t ai, const TLweSample *bk, const TLweParams *params) {
    const int32_t k = params->k;
    for (int32_t i = 0; i <= k; i++)
        torusPolynomialMulByXaiMinusOne(&result->a[i], ai, &bk->a[i]);
}

/** result += (0,x) */
EXPORT void tLweAddTTo(TLweSample *result, const int32_t pos, const Torus32 x, const TLweParams *params) {
    result->a[pos].coefsT[0] += x;
}

/** result += p*(0,x) */
EXPORT void
tLweAddRTTo(TLweSample *result, const int32_t pos, const IntPolynomial *p, const Torus32 x, const TLweParams *params) {
    const int32_t N = params->N;

    for (int32_t i = 0; i < N; i++)
        result->a[pos].coefsT[i] += p->coefs[i] * x;
}


EXPORT void init_TLweKey(TLweKey *obj, const TLweParams *params) {
    new(obj) TLweKey(params);
}
EXPORT void destroy_TLweKey(TLweKey *obj) {
    (obj)->~TLweKey();
}

EXPORT void init_TLweSample(TLweSample *obj, const TLweParams *params) {
    new(obj) TLweSample(params);
}
EXPORT void destroy_TLweSample(TLweSample *obj) {
    (obj)->~TLweSample();
}

USE_DEFAULT_CONSTRUCTOR_DESTRUCTOR_IMPLEMENTATIONS1(TLweKey, TLweParams);
USE_DEFAULT_CONSTRUCTOR_DESTRUCTOR_IMPLEMENTATIONS1(TLweSample, TLweParams);










