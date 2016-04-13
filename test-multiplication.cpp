#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include "lwe.h"
#include "multiplication.h"
#include "polynomials.h"

using namespace std;



// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************


void dieDramatically(string message) {
    cerr << message << endl;
    abort();
}


int main(int argc, char** argv) {
 
    const int N = 1024;
    IntPolynomial* a = new_IntPolynomial(N);
    TorusPolynomial* b = new_TorusPolynomial(N);
    TorusPolynomial* resNaive = new_TorusPolynomial(N);
    TorusPolynomial* resFFT = new_TorusPolynomial(N);
    TorusPolynomial* resKaratsuba = new_TorusPolynomial(N);

    for (int i=0; i<N; i++) {
	a->coefs[i]=(rand()%4096)-2048;
	b->coefsT[i]=rand();
    }
	
    multNaive(resNaive,a,b);
    multKaratsuba(resKaratsuba,a,b);
    multFFT(resFFT,a,b);

    for (int i=0; i<N; i++) {
	if (abs(int(resNaive->coefsT[i]-resFFT->coefsT[i])) > 1) {
	    cerr << i << " " << resNaive->coefsT[i] << " vs. " << resFFT->coefsT[i] << endl;
	    dieDramatically("Naive != FFT\n");
	}
    }
    for (int i=0; i<N; i++) {
	if (abs(int(resNaive->coefsT[i]-resKaratsuba->coefsT[i])) > 1) {
	    cerr << i << " " << resNaive->coefsT[i] << " vs. " << resKaratsuba->coefsT[i] << endl;
	    dieDramatically("Naive != Karatsuba\n");
	}
    }

    delete_IntPolynomial(a);
    delete_TorusPolynomial(b);
    delete_TorusPolynomial(resNaive);
    delete_TorusPolynomial(resKaratsuba);
    return 0;
}
