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
#include "ringgsw.h"

using namespace std;


EXPORT void Torus32PolynomialDecompH(IntPolynomial* result, const TorusPolynomial* sample, const RingGSWParams* params);

// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************
double approxEquals(Torus32 a, Torus32 b) { return abs(a-b)<10; }

int main(int argc, char** argv) {
    for (int i=0; i<20000; i++) uniformTorus32_distrib(generator);
    
    const int N = 1024;
    const int k = 1;
    const int alpha_min_gsw = 0.;
    const int alpha_max_gsw = 0.071;
    const int Msize = 2;
    const double alpha = 1e-6;
    const int l = 3;
    const int Bgbits = 10;
    
    static uniform_int_distribution<int> unift(0,Msize-1);

    // PARAMETERS
    RingLWEParams* rlwe_params = new_RingLWEParams(N, k, alpha_min_gsw, alpha_max_gsw); //les deux alpha mis un peu au hasard
    RingGSWParams* rgsw_params = new_RingGSWParams(l,Bgbits, rlwe_params);
    // KEY
    RingGSWKey* rgsw_key = new_RingGSWKey(rgsw_params);
    RingLWEKey* rlwe_key = &rgsw_key->ringlwe_key;
    // CIPHERTEXTS
    RingGSWSample* cipherA = new_RingGSWSample(rgsw_params);
    RingLWESample* cipherB = new_RingLWESample(rlwe_params);
    RingLWESample* cipherAB = new_RingLWESample(rlwe_params);


    //the probability that a sample with stdev alpha decrypts wrongly on
    //the a Msize message space.
    double expected_error_proba = 1.-erf(1./(sqrt(2.)*2.*Msize*alpha));


    cout << "-------------" << endl;
    cout << "WARNING:" << endl;
    cout << "All the tests below are supposed to fail with proba: " << expected_error_proba << endl; 
    cout << "It is normal and it is part of the test!" << endl; 
    cout << "-------------" << endl;


    //MESSAGE RLWE
    TorusPolynomial* muB = new_TorusPolynomial(N);
    
    //test decompH
    cout << "Test decompH on TorusPolynomial" << endl;
    IntPolynomial* muBDecH = new_IntPolynomial_array(l,N);
    for (int i = 0; i < N; ++i) {
	muB->coefsT[i] = uniformTorus32_distrib(generator);
    }
    Torus32PolynomialDecompH(muBDecH, muB, rgsw_params);
    for (int i = 0; i < N; ++i) {
	Torus32 expected = muB->coefsT[i];
	Torus32 actual = 0;
	for (int j=0; j<l; j++)
	    actual += muBDecH[j].coefs[i]*rgsw_params->h[j];
	if (abs(expected-actual)>3)
	    printf("decompH error %d: %d != %d\n",i,actual,expected);
    }



    for (int i = 0; i < N; ++i){
        int temp = unift(generator);
        muB->coefsT[i] = modSwitchToTorus32(temp, Msize);
        //cout << mu->coefsT[i] << endl;
    }
    //MESSAGE RLWE
    IntPolynomial* muA = new_IntPolynomial(N);
    for (int i = 0; i < N; ++i){
        int temp = unift(generator);
        muA->coefs[i] = 1-(temp%3);
        //cout << mu->coefsT[i] << endl;
    }
    // PHASE, DECRYPTION
    IntPolynomial* dechifA = new_IntPolynomial(N);
    TorusPolynomial* dechifB = new_TorusPolynomial(N);
    TorusPolynomial* dechifAB = new_TorusPolynomial(N);
    TorusPolynomial* muAB = new_TorusPolynomial(N);

    
    ringGswKeyGen(rgsw_key); // KEY GENERATION
    ringLweSymEncrypt(cipherB, muB, alpha, rlwe_key); // ENCRYPTION
    
    //decryption test ringlwe
    cout << "Test RingLweSymDecrypt on muB:" << endl;
    cout << " variance: " << cipherB->current_variance << endl;
    ringLweSymDecrypt(dechifB, cipherB, rlwe_key, Msize); // DECRYPTION
    for (int i=0; i<N; i++) {
	int expected = modSwitchFromTorus32(muB->coefsT[i],Msize);
	int actual = modSwitchFromTorus32(dechifB->coefsT[i],Msize);
	if (expected!=actual)
	    printf("ringlwe decryption error %d: %d != %d\n",i,actual,expected);
    }
	
    //test decompH on ringLWE
    cout << "Test decompH on RingLWE(muB)" << endl;
    IntPolynomial* cipherBDecH = new_IntPolynomial_array(l*(k+1),N);
    ringLWEDecompH(cipherBDecH, cipherB, rgsw_params);
    for (int p = 0; p <=k ; ++p) {
	for (int i = 0; i < N; ++i) {
	    Torus32 expected = cipherB->a[p].coefsT[i];
	    Torus32 actual = 0;
	    for (int j=0; j<l; j++)
		actual += cipherBDecH[l*p+j].coefs[i]*rgsw_params->h[j];
	    if (abs(expected-actual)>3)
		printf("decompH error (p,i)=(%d,%d): %d != %d\n",p,i,actual,expected);
	    int expected2 = modSwitchFromTorus32(expected,Msize);
	    int actual2 = modSwitchFromTorus32(actual,Msize);
	    if (expected2!=actual2)
		printf("modswitch error %d: %d != %d\n",i,actual2,expected2);
	}
    }

    
    //test externProduct with H
    ringGSWClear(cipherA, rgsw_params);
    ringGSWAddH(cipherA, rgsw_params);
    ringGSWExternProduct(cipherAB, cipherA, cipherB, rgsw_params);
    cout << "Test cipher after product 3.5 H*muB:" << endl;
    for (int p = 0; p <=k ; ++p) {
	for (int i = 0; i < N; ++i) {
	    Torus32 expected = cipherB->a[p].coefsT[i];
	    Torus32 actual = cipherAB->a[p].coefsT[i];
	    if (abs(expected-actual)>10)
		printf("decompH error (p,i)=(%d,%d): %d != %d\n",p,i,actual,expected);
	    int expected2 = modSwitchFromTorus32(expected,Msize);
	    int actual2 = modSwitchFromTorus32(actual,Msize);
	    if (expected2!=actual2)
		printf("modswitch error %d: %d != %d\n",i,actual2,expected2);
	}
    }
    ringLweSymDecrypt(dechifAB, cipherAB, rlwe_key, Msize); // DECRYPTION
    cout << "Test LweSymDecrypt after product 3.5 H*muB:" << endl;
    cout << " variance: " << cipherAB->current_variance << endl;
    for (int i=0; i<N; i++) {
	int expected = modSwitchFromTorus32(muB->coefsT[i],Msize);
	int actual = modSwitchFromTorus32(dechifAB->coefsT[i],Msize);
	if (expected!=actual)
	    printf("ringlwe decryption error %d: %d != %d\n",i,actual,expected);
    }
    cout << "----------------------" << endl;


    //decryption test ringgsw
    cout << "decryption test ringgsw:"<< endl;
    ringGswSymEncrypt(cipherA, muA, alpha, rgsw_key); // ENCRYPTION
    ringLwePhase(dechifB, &cipherA->bloc_sample[k][0], rlwe_key);
    cout << "manual decryption test:"<< endl;
    for (int i=0; i<N; i++) {
    	int expected = muA->coefs[i];
    	int actual = modSwitchFromTorus32(-512*dechifB->coefsT[i],2);
    	if (expected!=actual)
    	    printf("ringgsw encryption error %d: %d != %d\n",i,actual,expected);
    }

    ringGswSymDecrypt(dechifA, cipherA, rgsw_key, Msize);
    cout << "automatic decryption test:"<< endl;
    for (int i=0; i<N; i++) {
    	int expected = muA->coefs[i];
    	int actual = dechifA->coefs[i];
    	if (expected!=actual)
    	    printf("ringgsw decryption error %d: %d != %d\n",i,actual,expected);
    }

    mulRTorusPolynomial(muAB, muA, muB);
    ringGSWExternProduct(cipherAB, cipherA, cipherB, rgsw_params);
    ringLweSymDecrypt(dechifAB, cipherAB, rlwe_key, Msize); // DECRYPTION

    cout << "Test LweSymDecrypt after product 3.5:" << endl;
    cout << " variance: " << cipherAB->current_variance << endl;
    for (int i=0; i<N; i++) {
	int expected = modSwitchFromTorus32(muAB->coefsT[i],Msize);
	int actual = modSwitchFromTorus32(dechifAB->coefsT[i],Msize);
	if (expected!=actual)
	    printf("ringlwe decryption error %d: %d != %d\n",i,actual,expected);
    }
    cout << "----------------------" << endl;



    delete_RingGSWKey(rgsw_key);
    delete_RingGSWSample(cipherA);
    delete_RingLWESample(cipherB);
    delete_RingLWESample(cipherAB);
    delete_TorusPolynomial(muB);
    delete_IntPolynomial(muA);
    delete_IntPolynomial(dechifA);
    delete_TorusPolynomial(dechifB);
    delete_TorusPolynomial(dechifAB);
    delete_TorusPolynomial(muAB);

    //ATTENTION, le params est utilisé dans divers destructeurs, il faut l'effacer en dernier 
    delete_RingLWEParams(rlwe_params); //les deux alpha mis un peu au hasard
    delete_RingGSWParams(rgsw_params);
    
    return 0;
}
