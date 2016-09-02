#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include "polynomials_arithmetic.h"



using namespace std;



IntPolynomial::IntPolynomial(const int N): N(N)
{
    this->coefs = new int[N]; 
}

IntPolynomial::~IntPolynomial() {
    delete[] coefs;
}



TorusPolynomial::TorusPolynomial(const int N): N(N)
{
    this->coefsT = new Torus32[N]; 
}

TorusPolynomial::~TorusPolynomial() {
    delete[] coefsT;
}


void torusPolynomialMultNaive_plain_aux(Torus32* __restrict result, const int* __restrict poly1, const Torus32* __restrict poly2, const int N) {
    const int _2Nm1 = 2*N-1;
    Torus32 ri;
    for (int i=0; i<N; i++) {
	ri=0;
	for (int j=0; j<=i; j++) {
	    ri += poly1[j]*poly2[i-j];
	}
	result[i]=ri;
    }
    for (int i=N; i<_2Nm1; i++) {
	ri=0;
	for (int j=i-N+1; j<N; j++) {
	    ri += poly1[j]*poly2[i-j];
	}
	result[i]=ri;
    }
}

void torusPolynomialMultNaive_aux(Torus32* __restrict result, const int* __restrict poly1, const Torus32* __restrict poly2, const int N) {
    Torus32 ri;
    for (int i=0; i<N; i++) {
		ri=0;
			for (int j=0; j<=i; j++) {
		    	ri += poly1[j]*poly2[i-j];
			}
			for (int j=i+1; j<N; j++) {
		    	ri -= poly1[j]*poly2[N+i-j];
			}
		result[i]=ri;
    }
}

/**
 * This is the naive external multiplication of an integer polynomial
 * with a torus polynomial. (this function should yield exactly the same
 * result as the karatsuba or fft version) 
 */
EXPORT void torusPolynomialMultNaive(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2) {
    const int N = poly1->N;
    assert(result!=poly2);
    assert(poly2->N==N && result->N==N);
    torusPolynomialMultNaive_aux(result->coefsT, poly1->coefs, poly2->coefsT, N);
}



/**
 * This function multiplies 2 polynomials (an integer poly and a torus poly) by using Karatsuba
 * The karatsuba function is torusPolynomialMultKaratsuba: it takes in input two polynomials and multiplies them 
 * To do that, it uses the auxiliary function Karatsuba_aux, which is recursive ad which works with 
 * the vectors containing the coefficients of the polynomials (primitive types)
 */

// A and B of size = size
// R of size = 2*size-1
EXPORT void Karatsuba_aux(Torus32* R, const int* A, const Torus32* B, const int size, const char* buf){
    const int h = size / 2;
	const int sm1 = size-1;

	//we stop the karatsuba recursion at h=4, because on my machine,
	//it seems to be optimal
	if (h<=4)
	{
	    torusPolynomialMultNaive_plain_aux(R, A, B, size);
	    return;
	}

	//we split the polynomials in 2
	int* Atemp = (int*) buf; buf += h*sizeof(int);
	Torus32* Btemp = (Torus32*) buf; buf+= h*sizeof(Torus32);
	Torus32* Rtemp = (Torus32*) buf; buf+= size*sizeof(Torus32); 
	//Note: in the above line, I have put size instead of sm1 so that buf remains aligned on a power of 2

	for (int i = 0; i < h; ++i)
	    Atemp[i] = A[i] + A[h+i];
	for (int i = 0; i < h; ++i)
	    Btemp[i] = B[i] + B[h+i];

	// Karatsuba recursivly
	Karatsuba_aux(R, A, B, h, buf); // (R[0],R[2*h-2]), (A[0],A[h-1]), (B[0],B[h-1])
	Karatsuba_aux(R+size, A+h, B+h, h, buf); // (R[2*h],R[4*h-2]), (A[h],A[2*h-1]), (B[h],B[2*h-1])
	Karatsuba_aux(Rtemp, Atemp, Btemp, h, buf);
	R[sm1]=0; //this one needs to be set manually
	for (int i = 0; i < sm1; ++i) 
	    Rtemp[i] -= R[i] + R[size+i];
	for (int i = 0; i < sm1; ++i) 
	    R[h+i] += Rtemp[i];

}

// poly1, poly2 and result are polynomials mod X^N+1
EXPORT void torusPolynomialMultKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2){
	const int N = poly1->N;
	Torus32* R = new Torus32[2*N-1];
	char* buf = new char[16*N]; //that's large enough to store every tmp variables (2*2*N*4)
	
	// Karatsuba 
	Karatsuba_aux(R, poly1->coefs, poly2->coefsT, N, buf);

	// reduction mod X^N+1
	for (int i = 0; i < N-1; ++i) 
	    result->coefsT[i] = R[i] - R[N+i];
	result->coefsT[N-1] = R[N-1];
	
	delete[] R;
	delete[] buf;
}

// poly1, poly2 and result are polynomials mod X^N+1
EXPORT void torusPolynomialAddMulRKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2){
	const int N = poly1->N;
	Torus32* R = new Torus32[2*N-1];
	char* buf = new char[16*N]; //that's large enough to store every tmp variables (2*2*N*4)
	
	// Karatsuba 
	Karatsuba_aux(R, poly1->coefs, poly2->coefsT, N, buf);

	// reduction mod X^N+1
	for (int i = 0; i < N-1; ++i) 
	    result->coefsT[i] += R[i] - R[N+i];
	result->coefsT[N-1] += R[N-1];
	
	delete[] R;
	delete[] buf;
}

// poly1, poly2 and result are polynomials mod X^N+1
EXPORT void torusPolynomialSubMulRKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2){
	const int N = poly1->N;
	Torus32* R = new Torus32[2*N-1];
	char* buf = new char[16*N]; //that's large enough to store every tmp variables (2*2*N*4)
	
	// Karatsuba 
	Karatsuba_aux(R, poly1->coefs, poly2->coefsT, N, buf);

	// reduction mod X^N+1
	for (int i = 0; i < N-1; ++i) 
	    result->coefsT[i] -= R[i] - R[N+i];
	result->coefsT[N-1] -= R[N-1];
	
	delete[] R;
	delete[] buf;
}
