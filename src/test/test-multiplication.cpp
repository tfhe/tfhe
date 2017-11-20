#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include "tfhe.h"
#include "polynomials.h"

using namespace std;



// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************


void dieDramatically(string message) {
    cerr << message << endl;
    abort();
}


int32_t main(int32_t argc, char **argv) {
    const int32_t count = 1000; //number of tests to compare the 3 types of multiplication

    const int32_t N = 1024;
    IntPolynomial *a = new_IntPolynomial(N);
    TorusPolynomial *b = new_TorusPolynomial(N);
    TorusPolynomial *resNaive = new_TorusPolynomial(N);
    TorusPolynomial *resFFT = new_TorusPolynomial(N);
    TorusPolynomial *resKaratsuba = new_TorusPolynomial(N);
    //LagrangeHalfCPolynomial* test_fft = new_LagrangeHalfCPolynomial(N);

    double cycles_naive[count];
    double cycles_karatsuba[count];
    double cycles_fft[count];

    for (int32_t i = 0; i < count; ++i) {
        for (int32_t i = 0; i < N; i++) {
            a->coefs[i] = (rand() % (4 * N)) - (2 * N);
            b->coefsT[i] = rand();
        }

        //measure the execution time
        clock_t cstart, cend;

        cstart = clock();
        torusPolynomialMultNaive(resNaive, a, b);
        cend = clock();
        cycles_naive[i] = cend - cstart;

        cstart = clock();
        torusPolynomialMultKaratsuba(resKaratsuba, a, b);
        cend = clock();
        cycles_karatsuba[i] = cend - cstart;

        cstart = clock();
        torusPolynomialMultFFT(resFFT, a, b);
        //TorusPolynomial_ifft(test_fft,resNaive);
        //TorusPolynomial_fft(resFFT,test_fft);
        cend = clock();
        cycles_fft[i] = cend - cstart;

        for (int32_t i = 0; i < N; i++) {
            if (abs(int32_t(resNaive->coefsT[i] - resFFT->coefsT[i])) > 1) {
                cerr << i << " " << resNaive->coefsT[i] << " vs. " << resFFT->coefsT[i] << endl;
                dieDramatically("Naive != FFT\n");
            }
        }
        for (int32_t i = 0; i < N; i++) {
            if (abs(int32_t(resNaive->coefsT[i] - resKaratsuba->coefsT[i])) > 1) {
                cerr << i << " " << resNaive->coefsT[i] << " vs. " << resKaratsuba->coefsT[i] << endl;
                dieDramatically("Naive != Karatsuba\n");
            }
        }
    }

    // computing the average number of cycles per type of multiplication
    double cyc_naive = 0;
    double cyc_karatsuba = 0;
    double cyc_fft = 0;
    for (int32_t i = 0; i < count; ++i) {
        cyc_naive += cycles_naive[i];
        cyc_karatsuba += cycles_karatsuba[i];
        cyc_fft += cycles_fft[i];
    }
    cyc_naive = cyc_naive / count;
    cyc_karatsuba = cyc_karatsuba / count;
    cyc_fft = cyc_fft / count;

    cout << "torusPolynomialMultNaive: " << cyc_naive << " clock cycles (average)" << endl;
    cout << "torusPolynomialMultNaive time: " << (cyc_naive / CLOCKS_PER_SEC) << " seconds" << endl;
    cout << endl;
    cout << "torusPolynomialMultKaratsuba: " << cyc_karatsuba << " clock cycles (average)" << endl;
    cout << "torusPolynomialMultKaratsuba time: " << (cyc_karatsuba / CLOCKS_PER_SEC) << " seconds" << endl;
    cout << endl;
    cout << "torusPolynomialMultFFT: " << cyc_fft << " clock cycles (average)" << endl;
    cout << "torusPolynomialMultFFT time: " << (cyc_fft / CLOCKS_PER_SEC) << " seconds" << endl;
    cout << endl;

    delete_IntPolynomial(a);
    delete_TorusPolynomial(b);
    delete_TorusPolynomial(resNaive);
    delete_TorusPolynomial(resKaratsuba);
    return 0;
}
