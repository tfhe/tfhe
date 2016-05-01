#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
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
    
    RingLWEParams* params = new_RingLWEParams(512, 2, 0.2, 0.5); //les deux alpha mis un peu au hasard
    int N = params->N;
    // int k = params->k;
    RingLWEKey* key = new_RingLWEKey(params);
    RingLWESample* cipher = new_RingLWESample(params);
    RingLWESample* cipherT = new_RingLWESample(params);

    double alpha = 0.0625;

    TorusPolynomial* mu = new_TorusPolynomial(N);
    UniformTorusPolynomial(mu);
    Torus32 muT = dtot32(0.5);

    TorusPolynomial* phi = new_TorusPolynomial(N);
    TorusPolynomial* phiT = new_TorusPolynomial(N);
    int Msize = 4;  

    TorusPolynomial* dechif = new_TorusPolynomial(N);
    Torus32 dechifT = 0;



    ringLweKeyGen(key);
    ringLweSymEncrypt(cipher, mu, alpha, key);
    ringLweSymEncryptT(cipherT, muT, alpha, key);

    ringLwePhase(phi, cipher, key);
    ringLwePhase(phiT, cipherT, key);

    ringLweSymDecrypt(dechif, cipher, key, Msize);
    cout << "Test LweSymDecrypt :" << endl;
    for (int i = 0; i < N; ++i)
    {
        if (dechif->coefsT[i] != mu->coefsT[i])
            cout << dechif->coefsT[i] << " = " << mu->coefsT[i] << " error!!!" << endl;     
    }

    ringLweSymDecryptT(dechifT, cipherT, key, Msize);
    cout << "Test LweSymDecryptT :" << endl;
    if (dechifT != muT)
            cout << "Error!!!" << endl;     




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



    delete_RingLWEParams(params); //les deux alpha mis un peu au hasard
    delete_RingLWEKey(key);
    delete_RingLWESample(cipher);
    delete_RingLWESample(cipherT);

    delete_TorusPolynomial(mu);
    delete_TorusPolynomial(phi);
    delete_TorusPolynomial(phiT);
    delete_TorusPolynomial(dechif);
    
    return 0;
}
