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

using namespace std;


// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************
double approxEquals(Torus32 a, Torus32 b) { return abs(a - b) < 10; }

int32_t main(int32_t argc, char **argv) {
    for (int32_t i = 0; i < 20000; i++) uniformTorus32_distrib(generator); // Ila ?????

    const int32_t N = 1024;
    const int32_t k = 2;
    const double alpha_min = 0.01;
    const double alpha_max = 0.071;
    const int32_t Msize = 7; // taille de l'espace des coeffs du polynome du message
    const double alpha = 0.02;
    //TODO: parallelization
    static uniform_int_distribution<int32_t> distribution(0, Msize - 1);

    // PARAMETERS
    TLweParams *params = new_TLweParams(N, k, alpha_min, alpha_max); //les deux alpha mis un peu au hasard
    // KEY
    TLweKey *key = new_TLweKey(params);
    // CIPHERTEXTS
    TLweSample *cipher = new_TLweSample(params);
    TLweSample *cipherT = new_TLweSample(params);


    //the probability that a sample with stdev alpha decrypts wrongly on
    //the a Msize message space.
    double expected_error_proba = 1. - erf(1. / (sqrt(2.) * 2. * Msize * alpha));


    cout << "-------------" << endl;
    cout << "WARNING:" << endl;
    cout << "All the tests below are supposed to fail with proba: " << expected_error_proba << endl;
    cout << "It is normal and it is part of the test!" << endl;
    cout << "-------------" << endl;

    //MESSAGE
    TorusPolynomial *mu = new_TorusPolynomial(N);
    for (int32_t i = 0; i < N; ++i) {
        int32_t temp = distribution(generator);
        mu->coefsT[i] = modSwitchToTorus32(temp, Msize);
        //cout << mu->coefsT[i] << endl;
    }
    // PHASE, DECRYPTION
    TorusPolynomial *phi = new_TorusPolynomial(N);
    TorusPolynomial *dechif = new_TorusPolynomial(N);


    tLweKeyGen(key); // KEY GENERATION
    tLweSymEncrypt(cipher, mu, alpha, key); // ENCRYPTION
    tLwePhase(phi, cipher, key); // PHASE COMUPTATION
    tLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION

    cout << "Test LweSymDecrypt :" << endl;
    for (int32_t i = 0; i < N; ++i) {
        if (dechif->coefsT[i] != mu->coefsT[i])
            cout << i << " - " << dechif->coefsT[i] << " =? " << mu->coefsT[i] << " error!!!" << endl;
    }
    cout << "----------------------" << endl;


    TorusPolynomial *phiT = new_TorusPolynomial(N);

    for (int32_t trial = 1; trial < 1000; trial++) {
        Torus32 muT = modSwitchToTorus32(distribution(generator), Msize);
        Torus32 dechifT = 0;

        tLweSymEncryptT(cipherT, muT, alpha, key);
        tLwePhase(phiT, cipherT, key);
        dechifT = tLweSymDecryptT(cipherT, key, Msize);

        if (dechifT != muT) {
            cout << "Test LweSymDecryptT: trial " << trial << endl;
            cout << dechifT << " =? " << muT << " Error!!!" << endl;
            cout << "----------------------" << endl;
        }
    }



















    // TEST ADD, SUB, LINEAR COMBINATION, POLYNOMIAL COMBINATIONS 

    cout << endl;
    cout << endl;
    cout << "-------------------------" << endl;
    cout << "TEST Operations TLwe :" << endl;
    cout << "-------------------------" << endl;

    // CIPHERTEXTS
    TLweSample *cipher0 = new_TLweSample(params);
    TLweSample *cipher1 = new_TLweSample(params);

    // MESSAGES
    TorusPolynomial *mu0 = new_TorusPolynomial(N);
    for (int32_t i = 0; i < N; ++i) {
        int32_t temp = distribution(generator);
        mu0->coefsT[i] = modSwitchToTorus32(temp, Msize);
    }
    TorusPolynomial *mu1 = new_TorusPolynomial(N);
    for (int32_t i = 0; i < N; ++i) {
        int32_t temp = distribution(generator);
        mu1->coefsT[i] = modSwitchToTorus32(temp, Msize);
    }


    int32_t p = 1;
    IntPolynomial *poly = new_IntPolynomial(N);
    for (int32_t i = 0; i < N; ++i) {
        poly->coefs[i] = distribution(generator);
    }


    int32_t decInt = 0;
    int32_t muInt = 0;


    for (int32_t trial = 1; trial < 2; trial++) {

        tLweSymEncrypt(cipher0, mu0, alpha, key); // ENCRYPTION
        tLweSymEncrypt(cipher1, mu1, alpha, key); // ENCRYPTION


        // cipher = cipher0 + cipher1 
        tLweCopy(cipher, cipher0, params);
        tLweAddTo(cipher, cipher1, params);
        torusPolynomialAdd(mu, mu0, mu1); // mu = mu0 + mu1
        tLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION

        cout << "Test tLweAddTo Trial : " << trial << endl;
        for (int32_t i = 0; i < N; ++i) {
            decInt = modSwitchFromTorus32(dechif->coefsT[i], Msize);
            muInt = modSwitchFromTorus32(mu->coefsT[i], Msize);
            if (decInt != muInt)
                cout << decInt << " =? " << muInt << " error!!!" << endl;
        }
        cout << cipher->current_variance << endl;
        cout << "----------------------" << endl;



        // cipher = cipher0 - cipher1 
        tLweCopy(cipher, cipher0, params);
        tLweSubTo(cipher, cipher1, params);
        torusPolynomialSub(mu, mu0, mu1); // mu = mu0 - mu1
        tLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION

        cout << "Test tLweSubTo Trial : " << trial << endl;
        for (int32_t i = 0; i < N; ++i) {
            decInt = modSwitchFromTorus32(dechif->coefsT[i], Msize);
            muInt = modSwitchFromTorus32(mu->coefsT[i], Msize);
            if (decInt != muInt)
                cout << decInt << " =? " << muInt << " error!!!" << endl;
        }
        cout << cipher->current_variance << endl;
        cout << "----------------------" << endl;



        // cipher = cipher0 + p.cipher1 
        tLweCopy(cipher, cipher0, params);
        tLweAddMulTo(cipher, p, cipher1, params);
        torusPolynomialAddMulZ(mu, mu0, p, mu1); // mu = mu0 + p.mu1
        tLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION

        cout << "Test tLweAddMulTo Trial :" << trial << endl;
        for (int32_t i = 0; i < N; ++i) {
            decInt = modSwitchFromTorus32(dechif->coefsT[i], Msize);
            muInt = modSwitchFromTorus32(mu->coefsT[i], Msize);
            if (decInt != muInt)
                cout << decInt << " =? " << muInt << " error!!!" << endl;
        }
        cout << cipher->current_variance << endl;
        cout << "----------------------" << endl;




        // cipher = cipher0 - p.cipher1 
        tLweCopy(cipher, cipher0, params);
        tLweSubMulTo(cipher, p, cipher1, params);
        torusPolynomialSubMulZ(mu, mu0, p, mu1); // mu = mu0 - p.mu1
        tLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION

        cout << "Test tLweSubMulTo Trial :" << trial << endl;
        for (int32_t i = 0; i < N; ++i) {
            decInt = modSwitchFromTorus32(dechif->coefsT[i], Msize);
            muInt = modSwitchFromTorus32(mu->coefsT[i], Msize);
            if (decInt != muInt)
                cout << decInt << " =? " << muInt << " error!!!" << endl;
        }
        cout << cipher->current_variance << endl;
        cout << "----------------------" << endl;


        /*
        // result = result + poly.sample 
        tLweCopy(cipher, cipher0, params);
        tLweAddMulRTo(cipher, poly, cipher1, params);
        // mu = mu0 + poly.mu1
        torusPolynomialCopy(mu, mu0);
        torusPolynomialAddMulRKaratsuba(mu, poly, mu1);

        tLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION
        
        cout << "Test tLweAddMulRTo Trial:" << trial << endl;
        for (int32_t i = 0; i < N; ++i)
        {
            decInt = modSwitchFromTorus32(dechif->coefsT[i], Msize);
            muInt = modSwitchFromTorus32(mu->coefsT[i], Msize);
            if (decInt != muInt)
                cout << decInt << " =? " << muInt << " error!!!" << endl;     
        }
        cout << cipher->current_variance << endl;
        cout << "----------------------" << endl;
        */
    }


    delete_TLweSample(cipher0);
    delete_TLweSample(cipher1);
    delete_TorusPolynomial(mu0);
    delete_TorusPolynomial(mu1);
    delete_IntPolynomial(poly);











    // TEST ADD, SUB, LINEAR COMBINATION, POLYNOMIAL COMBINATIONS 

    cout << endl;
    cout << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "TEST Operations TLwe with Torus32 messages :" << endl;
    cout << "-----------------------------------------------" << endl;

    // CIPHERTEXTS
    TLweSample *cipherT0 = new_TLweSample(params);
    TLweSample *cipherT1 = new_TLweSample(params);


    int32_t pT = 1;

    for (int32_t trial = 1; trial < 1000; trial++) {

        // MESSAGES
        Torus32 muT0 = modSwitchToTorus32(distribution(generator), Msize);
        Torus32 muT1 = modSwitchToTorus32(distribution(generator), Msize);

        Torus32 muT = 0;
        Torus32 dechifT = 0;

        tLweSymEncryptT(cipherT0, muT0, alpha, key); // ENCRYPTION
        tLweSymEncryptT(cipherT1, muT1, alpha, key); // ENCRYPTION




        // cipher = cipher0 + cipher1 
        tLweCopy(cipherT, cipherT0, params);
        tLweAddTo(cipherT, cipherT1, params);
        muT = muT0 + muT1;
        dechifT = tLweSymDecryptT(cipherT, key, Msize); // DECRYPTION

        decInt = modSwitchFromTorus32(dechifT, Msize);
        muInt = modSwitchFromTorus32(muT, Msize);
        if (decInt != muInt) {
            cout << "Test tLweAddTo Trial : " << trial << endl;
            cout << decInt << " =? " << muInt << " Error!!!" << endl;
            cout << cipherT->current_variance << endl;
            cout << "----------------------" << endl;
        }



        // cipher = cipher0 - cipher1 
        tLweCopy(cipherT, cipherT0, params);
        tLweSubTo(cipherT, cipherT1, params);
        muT = muT0 - muT1;
        dechifT = tLweSymDecryptT(cipherT, key, Msize); // DECRYPTION

        decInt = modSwitchFromTorus32(dechifT, Msize);
        muInt = modSwitchFromTorus32(muT, Msize);
        if (decInt != muInt) {
            cout << "Test tLweSubTo Trial : " << trial << endl;
            cout << decInt << " =? " << muInt << " Error!!!" << endl;
            cout << cipherT->current_variance << endl;
            cout << "----------------------" << endl;
        }




        // cipher = cipher0 + p.cipher1 
        tLweCopy(cipherT, cipherT0, params);
        tLweAddMulTo(cipherT, pT, cipherT1, params);
        muT = muT0 + pT * muT1;
        dechifT = tLweSymDecryptT(cipherT, key, Msize); // DECRYPTION

        decInt = modSwitchFromTorus32(dechifT, Msize);
        muInt = modSwitchFromTorus32(muT, Msize);
        if (decInt != muInt) {
            cout << "Test tLweAddMulTo Trial : " << trial << endl;
            cout << decInt << " =? " << muInt << " Error!!!" << endl;
            cout << cipherT->current_variance << endl;
            cout << "----------------------" << endl;
        }




        // result = result - p.sample 
        tLweCopy(cipherT, cipherT0, params);
        tLweSubMulTo(cipherT, pT, cipherT1, params);
        muT = muT0 - pT * muT1;
        dechifT = tLweSymDecryptT(cipherT, key, Msize); // DECRYPTION

        decInt = modSwitchFromTorus32(dechifT, Msize);
        muInt = modSwitchFromTorus32(muT, Msize);
        if (decInt != muInt) {
            cout << "Test tLweSubMulTo Trial : " << trial << endl;
            cout << decInt << " =? " << muInt << " Error!!!" << endl;
            cout << cipherT->current_variance << endl;
            cout << "----------------------" << endl;
        }
    }


    delete_TLweSample(cipherT0);
    delete_TLweSample(cipherT1);


    delete_TLweKey(key);
    delete_TLweSample(cipher);
    delete_TLweSample(cipherT);

    delete_TorusPolynomial(mu);
    delete_TorusPolynomial(phi);
    delete_TorusPolynomial(phiT);
    delete_TorusPolynomial(dechif);

    //ATTENTION, le params est utilisé dans divers destructeurs, il faut l'effacer en dernier 
    delete_TLweParams(params);

    return 0;
}
