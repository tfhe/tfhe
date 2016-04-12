#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include "lwe.h"
#include "multiplication.h"


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
    TorusPolynomial* resKaratsuba = new_TorusPolynomial(N);

    for (int i=0; i<N; i++) {
	a->coefs[i]=rand();
	b->coefsT[i]=rand();
    }
	
    multNaive(resNaive,a,b);
    multKaratsuba(resKaratsuba,a,b);

    for (int i=0; i<N; i++) {
	if (resNaive->coefsT[i]!=resKaratsuba->coefsT[i])
	    dieDramatically("Naive != Karatsuba\n");
    }

    delete_IntPolynomial(a);
    delete_TorusPolynomial(b);
    delete_TorusPolynomial(resNaive);
    delete_TorusPolynomial(resKaratsuba);
    return 0;
}
