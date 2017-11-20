#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include <random>
#include "tfhe.h"
#include "polynomials.h"
#include "tlwe.h"
#include "tgsw.h"

using namespace std;


EXPORT void
tGswTorus32PolynomialDecompH(IntPolynomial *result, const TorusPolynomial *sample, const TGswParams *params);

// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************
double approxEquals(Torus32 a, Torus32 b) { return abs(a - b) < 10; }

int32_t main(int32_t argc, char **argv) {
    for (int32_t i = 0; i < 20000; i++) uniformTorus32_distrib(generator);

    const int32_t N = 1024;
    const int32_t k = 1;
    const double alpha_min_gsw = 0.;
    const double alpha_max_gsw = 0.071;
    const int32_t Msize = 2;
    const double alpha = 1e-6;
    const int32_t l = 3;
    const int32_t Bgbits = 10;
//TODO: parallelization
    static uniform_int_distribution<int32_t> unift(0, Msize - 1);

    // PARAMETERS
    TLweParams *rlwe_params = new_TLweParams(N, k, alpha_min_gsw, alpha_max_gsw); //les deux alpha mis un peu au hasard
    TGswParams *rgsw_params = new_TGswParams(l, Bgbits, rlwe_params);
    // KEY
    TGswKey *rgsw_key = new_TGswKey(rgsw_params);
    TLweKey *rlwe_key = &rgsw_key->tlwe_key;
    // CIPHERTEXTS
    TGswSample *cipherA = new_TGswSample(rgsw_params);
    TLweSample *cipherB = new_TLweSample(rlwe_params);
    TLweSample *cipherAB = new_TLweSample(rlwe_params);


    //the probability that a sample with stdev alpha decrypts wrongly on
    //the a Msize message space.
    double expected_error_proba = 1. - erf(1. / (sqrt(2.) * 2. * Msize * alpha));


    cout << "-------------" << endl;
    cout << "WARNING:" << endl;
    cout << "All the tests below are supposed to fail with proba: " << expected_error_proba << endl;
    cout << "It is normal and it is part of the test!" << endl;
    cout << "-------------" << endl;


    //MESSAGE RLwe
    TorusPolynomial *muB = new_TorusPolynomial(N);

    //test decompH
    cout << "Test decompH on TorusPolynomial" << endl;
    IntPolynomial *muBDecH = new_IntPolynomial_array(l, N);
    for (int32_t i = 0; i < N; ++i) {
        muB->coefsT[i] = uniformTorus32_distrib(generator);
    }
    tGswTorus32PolynomialDecompH(muBDecH, muB, rgsw_params);
    for (int32_t i = 0; i < N; ++i) {
        Torus32 expected = muB->coefsT[i];
        Torus32 actual = 0;
        for (int32_t j = 0; j < l; j++)
            actual += muBDecH[j].coefs[i] * rgsw_params->h[j];
        if (abs(expected - actual) > 3)
            printf("decompH error %d: %d != %d\n", i, actual, expected);
    }


    for (int32_t i = 0; i < N; ++i) {
        int32_t temp = unift(generator);
        muB->coefsT[i] = modSwitchToTorus32(temp, Msize);
        //cout << mu->coefsT[i] << endl;
    }
    //MESSAGE RLwe
    IntPolynomial *muA = new_IntPolynomial(N);
    for (int32_t i = 0; i < N; ++i) {
        int32_t temp = unift(generator);
        muA->coefs[i] = 1 - (temp % 3);
        //cout << mu->coefsT[i] << endl;
    }
    // PHASE, DECRYPTION
    IntPolynomial *dechifA = new_IntPolynomial(N);
    TorusPolynomial *dechifB = new_TorusPolynomial(N);
    TorusPolynomial *dechifAB = new_TorusPolynomial(N);
    TorusPolynomial *muAB = new_TorusPolynomial(N);


    tGswKeyGen(rgsw_key); // KEY GENERATION
    tLweSymEncrypt(cipherB, muB, alpha, rlwe_key); // ENCRYPTION

    //decryption test tlwe
    cout << "Test TLweSymDecrypt on muB:" << endl;
    cout << " variance: " << cipherB->current_variance << endl;
    tLweSymDecrypt(dechifB, cipherB, rlwe_key, Msize); // DECRYPTION
    for (int32_t i = 0; i < N; i++) {
        int32_t expected = modSwitchFromTorus32(muB->coefsT[i], Msize);
        int32_t actual = modSwitchFromTorus32(dechifB->coefsT[i], Msize);
        if (expected != actual)
            printf("tlwe decryption error %d: %d != %d\n", i, actual, expected);
    }

    //test decompH on tLwe
    cout << "Test decompH on TLwe(muB)" << endl;
    IntPolynomial *cipherBDecH = new_IntPolynomial_array(l * (k + 1), N);
    tGswTLweDecompH(cipherBDecH, cipherB, rgsw_params);
    for (int32_t p = 0; p <= k; ++p) {
        for (int32_t i = 0; i < N; ++i) {
            Torus32 expected = cipherB->a[p].coefsT[i];
            Torus32 actual = 0;
            for (int32_t j = 0; j < l; j++)
                actual += cipherBDecH[l * p + j].coefs[i] * rgsw_params->h[j];
            if (abs(expected - actual) > 3)
                printf("decompH error (p,i)=(%d,%d): %d != %d\n", p, i, actual, expected);
            int32_t expected2 = modSwitchFromTorus32(expected, Msize);
            int32_t actual2 = modSwitchFromTorus32(actual, Msize);
            if (expected2 != actual2)
                printf("modswitch error %d: %d != %d\n", i, actual2, expected2);
        }
    }


    //test externProduct with H
    tGswClear(cipherA, rgsw_params);
    tGswAddH(cipherA, rgsw_params);
    tGswExternProduct(cipherAB, cipherA, cipherB, rgsw_params);
    cout << "Test cipher after product 3.5 H*muB:" << endl;
    for (int32_t p = 0; p <= k; ++p) {
        for (int32_t i = 0; i < N; ++i) {
            Torus32 expected = cipherB->a[p].coefsT[i];
            Torus32 actual = cipherAB->a[p].coefsT[i];
            if (abs(expected - actual) > 10)
                printf("decompH error (p,i)=(%d,%d): %d != %d\n", p, i, actual, expected);
            int32_t expected2 = modSwitchFromTorus32(expected, Msize);
            int32_t actual2 = modSwitchFromTorus32(actual, Msize);
            if (expected2 != actual2)
                printf("modswitch error %d: %d != %d\n", i, actual2, expected2);
        }
    }
    tLweSymDecrypt(dechifAB, cipherAB, rlwe_key, Msize); // DECRYPTION
    cout << "Test LweSymDecrypt after product 3.5 H*muB:" << endl;
    cout << " variance: " << cipherAB->current_variance << endl;
    for (int32_t i = 0; i < N; i++) {
        int32_t expected = modSwitchFromTorus32(muB->coefsT[i], Msize);
        int32_t actual = modSwitchFromTorus32(dechifAB->coefsT[i], Msize);
        if (expected != actual)
            printf("tlwe decryption error %d: %d != %d\n", i, actual, expected);
    }
    cout << "----------------------" << endl;


    //decryption test tgsw
    cout << "decryption test tgsw:" << endl;
    tGswSymEncrypt(cipherA, muA, alpha, rgsw_key); // ENCRYPTION
    tLwePhase(dechifB, &cipherA->bloc_sample[k][0], rlwe_key);
    cout << "manual decryption test:" << endl;
    for (int32_t i = 0; i < N; i++) {
        int32_t expected = muA->coefs[i];
        int32_t actual = modSwitchFromTorus32(-512 * dechifB->coefsT[i], 2);
        if (expected != actual)
            printf("tgsw encryption error %d: %d != %d\n", i, actual, expected);
    }

    tGswSymDecrypt(dechifA, cipherA, rgsw_key, Msize);
    cout << "automatic decryption test:" << endl;
    for (int32_t i = 0; i < N; i++) {
        int32_t expected = muA->coefs[i];
        int32_t actual = dechifA->coefs[i];
        if (expected != actual)
            printf("tgsw decryption error %d: %d != %d\n", i, actual, expected);
    }

    torusPolynomialMulR(muAB, muA, muB);
    tGswExternProduct(cipherAB, cipherA, cipherB, rgsw_params);
    tLweSymDecrypt(dechifAB, cipherAB, rlwe_key, Msize); // DECRYPTION

    cout << "Test LweSymDecrypt after product 3.5:" << endl;
    cout << " variance: " << cipherAB->current_variance << endl;
    for (int32_t i = 0; i < N; i++) {
        int32_t expected = modSwitchFromTorus32(muAB->coefsT[i], Msize);
        int32_t actual = modSwitchFromTorus32(dechifAB->coefsT[i], Msize);
        if (expected != actual)
            printf("tlwe decryption error %d: %d != %d\n", i, actual, expected);
    }
    cout << "----------------------" << endl;


    delete_TGswKey(rgsw_key);
    delete_TGswSample(cipherA);
    delete_TLweSample(cipherB);
    delete_TLweSample(cipherAB);
    delete_TorusPolynomial(muB);
    delete_IntPolynomial(muA);
    delete_IntPolynomial(dechifA);
    delete_TorusPolynomial(dechifB);
    delete_TorusPolynomial(dechifAB);
    delete_TorusPolynomial(muAB);

    //ATTENTION, le params est utilisé dans divers destructeurs, il faut l'effacer en dernier 
    delete_TLweParams(rlwe_params); //les deux alpha mis un peu au hasard
    delete_TGswParams(rgsw_params);

    return 0;
}
