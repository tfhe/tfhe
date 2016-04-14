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
    const int count = 1000; //number of tests to compare the 3 types of multiplication

    const int N = 1024;
    IntPolynomial* a = new_IntPolynomial(N);
    TorusPolynomial* b = new_TorusPolynomial(N);
    TorusPolynomial* resNaive = new_TorusPolynomial(N);
    TorusPolynomial* resFFT = new_TorusPolynomial(N);
    TorusPolynomial* resKaratsuba = new_TorusPolynomial(N);

    double cycles_naive[count];
    double cycles_karatsuba[count];
    double cycles_fft[count];

    for (int i = 0; i < count; ++i)
    {
        for (int i=0; i<N; i++) {
    	a->coefs[i]=(rand()%(4*N))-(2*N);
    	b->coefsT[i]=rand();
        }
    	
        //measure the execution time
        clock_t cstart, cend;

        cstart = clock();
        multNaive(resNaive,a,b);  
        cend = clock();
        cycles_naive[i] = cend - cstart;
        
        cstart = clock();
        multKaratsuba(resKaratsuba,a,b);  
        cend = clock();
        cycles_karatsuba[i] = cend - cstart;
        
        cstart = clock();
        multFFT(resFFT,a,b);  
        cend = clock();
        cycles_fft[i] = cend - cstart;        

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
    }

    // computing the average number of cycles per type of multiplication
    double cyc_naive = 0;
    double cyc_karatsuba = 0;
    double cyc_fft = 0;
    for (int i = 0; i < count; ++i)
    {
        cyc_naive += cycles_naive[i];
        cyc_karatsuba += cycles_karatsuba[i];
        cyc_fft += cycles_fft[i];
    }
    cyc_naive = cyc_naive/count;
    cyc_karatsuba = cyc_karatsuba/count;
    cyc_fft = cyc_fft/count;

    cout << "multNaive: " << cyc_naive << " clock cycles (average)" << endl;
    cout << "multNaive time: " << (cyc_naive/CLOCKS_PER_SEC) << " seconds" << endl;
    cout << endl;
    cout << "multKaratsuba: " << cyc_karatsuba << " clock cycles (average)" << endl;
    cout << "multKaratsuba time: " << (cyc_karatsuba/CLOCKS_PER_SEC) << " seconds" << endl;
    cout << endl;
    cout << "multFFT: " << cyc_fft << " clock cycles (average)" << endl;
    cout << "multFFT time: " << (cyc_fft/CLOCKS_PER_SEC) << " seconds" << endl;
    cout << endl;

    delete_IntPolynomial(a);
    delete_TorusPolynomial(b);
    delete_TorusPolynomial(resNaive);
    delete_TorusPolynomial(resKaratsuba);
    return 0;
}
