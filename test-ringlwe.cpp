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
    
    const int N = 512;
    const int k = 2;
    const int alpha_min = 0.01;
    const int alpha_max = 0.02;
    const int Msize = 7;
    const double alpha = 0.0275;
    static uniform_int_distribution<int> distribution(0,Msize-1);

    RingLWEParams* params = new_RingLWEParams(N, k, alpha_min, alpha_max); 
    //les deux alpha mis un peu au hasard
    RingLWEKey* key = new_RingLWEKey(params);
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


    TorusPolynomial* mu = new_TorusPolynomial(N);
    TorusPolynomial* phi = new_TorusPolynomial(N);
    TorusPolynomial* dechif = new_TorusPolynomial(N);

    for (int i = 0; i < N; ++i){
        int temp = distribution(generator);
        mu->coefsT[i] = modSwitchToTorus32(temp, Msize);
	//cout << mu->coefsT[i] << endl;
    }
    ringLweKeyGen(key);
    ringLweSymEncrypt(cipher, mu, alpha, key);
    ringLwePhase(phi, cipher, key);

    ringLweSymDecrypt(dechif, cipher, key, Msize);
    cout << "Test LweSymDecrypt :" << endl;
    for (int i = 0; i < N; ++i)
    {
        if (dechif->coefsT[i] != mu->coefsT[i])
            cout << dechif->coefsT[i] << " =? " << mu->coefsT[i] << " error!!!" << endl;     
    }


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
    }    
    }

    /*
    cout << "a = [";
    for (int i = 0; i < n-1; ++i) cout << t32tod(cipher->a[i]) << ", ";
    cout << t32tod(cipher->a[n-1]) << "]" << endl;
    cout << "b = " << t32tod(cipher->b) << endl;


    phi = lwePhase(cipher, key);
    cout << "phi = " << t32tod(phi) << endl;
    message = lweSymDecrypt(cipher, key, Msize);
    cout << "message = " << t32tod(message) << endl;
    */

    /*
    // RingLWE crash test
    int failures = 0;
    int trials = 1000;
    for (int i = 0; i < trials; i++) {
        Torus32 input = dtot32((i%3)/3.);
	lweKeyGen(key);
	lweSymEncrypt(cipher, input, 0.047, key); // Ila: attention au niveau de bruit!!! à voir (0.06 n'est pas le bon je crois, 0.047 marche parfaitement)
	phi = lwePhase(cipher, key);
	Torus32 decrypted = lweSymDecrypt(cipher, key, 3);
	if ( !approxEquals(input,decrypted) ) {
	    cerr << "WARNING: the msg " << t32tod(input) << " gave phase " << t32tod(phi) << " and was incorrectly decrypted to " << t32tod(decrypted) << endl;
	    failures++;
	}
    }
    cout << "There were " << failures << " failures out of " << trials << " trials" << endl;
    cout << "(it might be normal)" << endl;
    */



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
