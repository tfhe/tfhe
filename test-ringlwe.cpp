#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include <random>
#include "lwe.h"
#include "multiplication.h"
#include "polynomials.h"
#include "ringlwe.h"

using namespace std;



// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************
double approxEquals(Torus32 a, Torus32 b) { return abs(a-b)<10; }

int main(int argc, char** argv) {
    for (int i=0; i<20000; i++) uniformTorus32_distrib(generator);
    
    const int N = 512;
    const int k = 2;
    const int alpha_min = 0.01;
    const int alpha_max = 0.071;
    const int Msize = 7;
    const double alpha = 0.02;
    static uniform_int_distribution<int> distribution(0,Msize-1);

    // PARAMETERS
    RingLWEParams* params = new_RingLWEParams(N, k, alpha_min, alpha_max); //les deux alpha mis un peu au hasard
    // KEY
    RingLWEKey* key = new_RingLWEKey(params);
    // CIPHERTEXTS
    RingLWESample* cipher = new_RingLWESample(params);
    RingLWESample* cipherT = new_RingLWESample(params);


    //the probability that a sample with stdev alpha decrypts wrongly on
    //the a Msize message space.
    double expected_error_proba = 1.-erf(1./(sqrt(2.)*2.*Msize*alpha));


    cout << "-------------" << endl;
    cout << "WARNING:" << endl;
    cout << "All the tests below are supposed to fail with proba: " << expected_error_proba << endl; 
    cout << "It is normal and it is part of the test!" << endl; 
    cout << "-------------" << endl;

    //MESSAGE
    TorusPolynomial* mu = new_TorusPolynomial(N);
    for (int i = 0; i < N; ++i){
        int temp = distribution(generator);
        mu->coefsT[i] = modSwitchToTorus32(temp, Msize);
        //cout << mu->coefsT[i] << endl;
    }
    // PHASE, DECRYPTION
    TorusPolynomial* phi = new_TorusPolynomial(N);
    TorusPolynomial* dechif = new_TorusPolynomial(N);

    
    ringLweKeyGen(key); // KEY GENERATION
    ringLweSymEncrypt(cipher, mu, alpha, key); // ENCRYPTION
    ringLwePhase(phi, cipher, key); // PHASE COMUPTATION
    ringLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION

    cout << "Test LweSymDecrypt :" << endl;
    for (int i = 0; i < N; ++i)
    {
        if (dechif->coefsT[i] != mu->coefsT[i])
            cout << i << " - " << dechif->coefsT[i] << " =? " << mu->coefsT[i] << " error!!!" << endl;     
    }
    cout << "----------------------" << endl;



    TorusPolynomial* phiT = new_TorusPolynomial(N);

    for (int trial=1; trial<1000; trial++) {
        Torus32 muT = modSwitchToTorus32(distribution(generator), Msize);
        Torus32 dechifT = 0;

        ringLweSymEncryptT(cipherT, muT, alpha, key);
        ringLwePhase(phiT, cipherT, key);
        dechifT = ringLweSymDecryptT(cipherT, key, Msize);
        
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
    cout << "TEST Operations RingLWE :" << endl;
    cout << "-------------------------" << endl;

    // CIPHERTEXTS
    RingLWESample* cipher0 = new_RingLWESample(params);
    RingLWESample* cipher1 = new_RingLWESample(params);
            
    // MESSAGES
    TorusPolynomial* mu0 = new_TorusPolynomial(N);
    for (int i = 0; i < N; ++i){
        int temp = distribution(generator);
        mu0->coefsT[i] = modSwitchToTorus32(temp, Msize);
    }
    TorusPolynomial* mu1 = new_TorusPolynomial(N);
    for (int i = 0; i < N; ++i){
        int temp = distribution(generator);
        mu1->coefsT[i] = modSwitchToTorus32(temp, Msize);
    }


    int p = 1;
    IntPolynomial* poly = new_IntPolynomial(N);
    for (int i = 0; i < N; ++i){
        poly->coefs[i] = distribution(generator);
    }
    


    int decInt = 0;
    int muInt = 0;
    


    for (int trial=1; trial<2; trial++) {

        ringLweSymEncrypt(cipher0, mu0, alpha, key); // ENCRYPTION
        ringLweSymEncrypt(cipher1, mu1, alpha, key); // ENCRYPTION


        // cipher = cipher0 + cipher1 
        ringLweCopy(cipher, cipher0, params);
        ringLweAddTo(cipher, cipher1, params);
        torusPolynomialAdd(mu, mu0, mu1); // mu = mu0 + mu1
        ringLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION
        
        cout << "Test ringLweAddTo Trial : " << trial << endl;
        for (int i = 0; i < N; ++i)
        {
            decInt = modSwitchFromTorus32(dechif->coefsT[i], Msize);
            muInt = modSwitchFromTorus32(mu->coefsT[i], Msize);
            if (decInt != muInt)
                cout << decInt << " =? " << muInt << " error!!!" << endl;     
        }
        cout << cipher->current_variance << endl;
        cout << "----------------------" << endl;



        // cipher = cipher0 - cipher1 
        ringLweCopy(cipher, cipher0, params);
        ringLweSubTo(cipher, cipher1, params);
        torusPolynomialSub(mu, mu0, mu1); // mu = mu0 - mu1
        ringLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION
        
        cout << "Test ringLweSubTo Trial : " << trial << endl;
        for (int i = 0; i < N; ++i)
        {
            decInt = modSwitchFromTorus32(dechif->coefsT[i], Msize);
            muInt = modSwitchFromTorus32(mu->coefsT[i], Msize);
            if (decInt != muInt)
                cout << decInt << " =? " << muInt << " error!!!" << endl;     
        }
        cout << cipher->current_variance << endl;
        cout << "----------------------" << endl;
        

        
        // cipher = cipher0 + p.cipher1 
        ringLweCopy(cipher, cipher0, params);
        ringLweAddMulTo(cipher, p, cipher1, params);
        torusPolynomialAddMulZ(mu, mu0, p, mu1); // mu = mu0 + p.mu1
        ringLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION
        
        cout << "Test ingLweAddMulTo Trial :" << trial << endl;
        for (int i = 0; i < N; ++i)
        {
            decInt = modSwitchFromTorus32(dechif->coefsT[i], Msize);
            muInt = modSwitchFromTorus32(mu->coefsT[i], Msize);
            if (decInt != muInt)
                cout << decInt << " =? " << muInt << " error!!!" << endl;     
        }
        cout << cipher->current_variance << endl;
        cout << "----------------------" << endl;
        
        


        // result = result - p.sample 
        ringLweCopy(cipher, cipher0, params);
        ringLweSubMulTo(cipher, p, cipher1, params);
        torusPolynomialSubMulZ(mu, mu0, p, mu1); // mu = mu0 - p.mu1
        ringLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION
        
        cout << "Test ringLweSubMulTo Trial :" << trial << endl;
        for (int i = 0; i < N; ++i)
        {
            decInt = modSwitchFromTorus32(dechif->coefsT[i], Msize);
            muInt = modSwitchFromTorus32(mu->coefsT[i], Msize);
            if (decInt != muInt)
                cout << decInt << " =? " << muInt << " error!!!" << endl;     
        }
        cout << cipher->current_variance << endl;
        cout << "----------------------" << endl;
        

        /*
        // result = result + poly.sample 
        ringLweCopy(cipher, cipher0, params);
        ringLweAddMulRTo(cipher, poly, cipher1, params);
        // mu = mu0 + poly.mu1
        torusPolynomialCopy(mu, mu0);
        addMultKaratsuba(mu, poly, mu1);

        ringLweSymDecrypt(dechif, cipher, key, Msize); // DECRYPTION
        
        cout << "Test ringLweAddMulRTo Trial:" << trial << endl;
        for (int i = 0; i < N; ++i)
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

    

    delete_RingLWESample(cipher0);
    delete_RingLWESample(cipher1);
    delete_TorusPolynomial(mu0);
    delete_TorusPolynomial(mu1);
    delete_IntPolynomial(poly);











    // TEST ADD, SUB, LINEAR COMBINATION, POLYNOMIAL COMBINATIONS 

    cout << endl;
    cout << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "TEST Operations RingLWE with Torus32 messages :" << endl;
    cout << "-----------------------------------------------" << endl;

    // CIPHERTEXTS
    RingLWESample* cipherT0 = new_RingLWESample(params);
    RingLWESample* cipherT1 = new_RingLWESample(params);
            
    
    int pT = 1;
    
    for (int trial=1; trial<1000; trial++) {

        // MESSAGES
        Torus32 muT0 = modSwitchToTorus32(distribution(generator), Msize);
        Torus32 muT1 = modSwitchToTorus32(distribution(generator), Msize);
         
        Torus32 muT = 0;    
        Torus32 dechifT = 0;

        ringLweSymEncryptT(cipherT0, muT0, alpha, key); // ENCRYPTION
        ringLweSymEncryptT(cipherT1, muT1, alpha, key); // ENCRYPTION



        
        // cipher = cipher0 + cipher1 
        ringLweCopy(cipherT, cipherT0, params);
        ringLweAddTo(cipherT, cipherT1, params);
        muT = muT0 + muT1;
        dechifT = ringLweSymDecryptT(cipherT, key, Msize); // DECRYPTION

        decInt = modSwitchFromTorus32(dechifT, Msize);
        muInt = modSwitchFromTorus32(muT, Msize);
        if (decInt != muInt) {
            cout << "Test ringLweAddTo Trial : " << trial << endl;
            cout << decInt << " =? " << muInt << " Error!!!" << endl; 
            cout << cipherT->current_variance << endl;
            cout << "----------------------" << endl;
        } 
        

        
        // cipher = cipher0 - cipher1 
        ringLweCopy(cipherT, cipherT0, params);
        ringLweSubTo(cipherT, cipherT1, params);
        muT = muT0 - muT1;
        dechifT = ringLweSymDecryptT(cipherT, key, Msize); // DECRYPTION
        
        decInt = modSwitchFromTorus32(dechifT, Msize);
        muInt = modSwitchFromTorus32(muT, Msize);
        if (decInt != muInt) {
            cout << "Test ringLweSubTo Trial : " << trial << endl;
            cout << decInt << " =? " << muInt << " Error!!!" << endl; 
            cout << cipherT->current_variance << endl;
            cout << "----------------------" << endl;
        }  

        


        // cipher = cipher0 + p.cipher1 
        ringLweCopy(cipherT, cipherT0, params);
        ringLweAddMulTo(cipherT, pT, cipherT1, params);
        muT = muT0 + pT*muT1;
        dechifT = ringLweSymDecryptT(cipherT, key, Msize); // DECRYPTION
        
        decInt = modSwitchFromTorus32(dechifT, Msize);
        muInt = modSwitchFromTorus32(muT, Msize);
        if (decInt != muInt) {
            cout << "Test ringLweAddMulTo Trial : " << trial << endl;
            cout << decInt << " =? " << muInt << " Error!!!" << endl; 
            cout << cipherT->current_variance << endl;
            cout << "----------------------" << endl;
        } 




        // result = result - p.sample 
        ringLweCopy(cipherT, cipherT0, params);
        ringLweSubMulTo(cipherT, pT, cipherT1, params);
        muT = muT0 - pT*muT1;
        dechifT = ringLweSymDecryptT(cipherT, key, Msize); // DECRYPTION
        
        decInt = modSwitchFromTorus32(dechifT, Msize);
        muInt = modSwitchFromTorus32(muT, Msize);
        if (decInt != muInt) {
            cout << "Test ringLweSubMulTo Trial : " << trial << endl;
            cout << decInt << " =? " << muInt << " Error!!!" << endl; 
            cout << cipherT->current_variance << endl;
            cout << "----------------------" << endl;
        } 
    }

    

    delete_RingLWESample(cipherT0);
    delete_RingLWESample(cipherT1);















    delete_RingLWEKey(key);
    delete_RingLWESample(cipher);
    delete_RingLWESample(cipherT);

    delete_TorusPolynomial(mu);
    delete_TorusPolynomial(phi);
    delete_TorusPolynomial(phiT);
    delete_TorusPolynomial(dechif);

    //ATTENTION, le params est utilisé dans divers destructeurs, il faut l'effacer en dernier 
    delete_RingLWEParams(params); 
    
    return 0;
}
