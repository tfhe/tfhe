#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
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
	a->coefs[i]=(rand()%(4*N))-(2*N);
	b->coefsT[i]=rand();
    }
	
    //measure the time
    clock_t cstart, cend;
    double cycles, temps;

    cstart = clock();
    multNaive(resNaive,a,b);  
    cend = clock();
    cycles = cend - cstart;
    temps = cycles/CLOCKS_PER_SEC;
    cout << "multNaive: " << cycles << " clock cycles" << endl;
    cout << "multNaive time: " << temps << " seconds" << endl;
    cout << endl;

    cstart = clock();
    multKaratsuba(resKaratsuba,a,b);  
    cend = clock();
    cycles = cend - cstart;
    temps = cycles/CLOCKS_PER_SEC;
    cout << "multKaratsuba: " << cycles << " clock cycles" << endl;
    cout << "multKaratsuba time: " << temps << " seconds" << endl;
    cout << endl;
    
    cstart = clock();
    multFFT(resFFT,a,b);  
    cend = clock();
    cycles = cend - cstart;
    temps = cycles/CLOCKS_PER_SEC;
    cout << "multFFT: " << cycles << " clock cycles" << endl;
    cout << "multFFT time: " << temps << " seconds" << endl;
    cout << endl;
    

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
