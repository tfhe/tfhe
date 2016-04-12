#include <cstdlib>
#include <cmath>
#include <cassert>
#include "multiplication.h"



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


/**
 * This is the naive external multiplication of an integer polynomial
 * with a torus polynomial. (this function should yield exactly the same
 * result as the karatsuba or fft version) 
 */
EXPORT void multNaive(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2) {
    const int N = poly1->N;
    assert(poly2->N==N && result->N==N);
    Torus32 ri;
    for (int i=0; i<N; i++) {
		ri=0;
			for (int j=0; j<=i; j++) {
		    	ri += poly1->coefs[j]*poly2->coefsT[i-j];
			}
			for (int j=i+1; i<N; j++) {
		    	ri -= poly1->coefs[j]*poly2->coefsT[N+i-j];
			}
		result->coefsT[i]=ri;
    }
}

/**
 * This function multiplies 2 polynomials (an integer poly and a torus poly)
 * by using Karatsuba
 */

EXPORT void Karatsuba_aux(Torus32* R, const int* A, const Torus32* B, const int size){
	int h = size / 2;

	if (h!=1)
	{
		// split the polynomials in 2
		int* A0 = new int[h];
		int* A1 = new int[h];
		int* Atemp = new int[h];
		Torus32* B0 = new Torus32[h];
		Torus32* B1 = new Torus32[h];
		Torus32* Btemp = new Torus32[h];

		for (int i = 0; i < h; ++i)
		{
			A0[i] = A[i];
			A1[i] = A[h+i];
			Atemp[i] = A0[i] + A1[i];
			B0[i] = B[i];
			B1[i] = B[h+i];
			Btemp[i] = B0[i] + B1[i];
		}

		// Karatsuba recursivly
		Torus32* R0 = new Torus32[2*h-1];
		Torus32* R1 = new Torus32[2*h-1];
		Torus32* R2 = new Torus32[2*h-1];
		Torus32* Rtemp = new Torus32[2*h-1];
		
		
		Karatsuba_aux(R0, A0, B0, h);
		Karatsuba_aux(R2, A1, B1, h);
		Karatsuba_aux(Rtemp, Atemp, Btemp, h);
		for (int i = 0; i < 2*h-1; ++i) R1[i] = Rtemp[i] - R0[i] - R2[i];

		for (int i = 0; i < h; ++i) R[i] = R0[i];
		for (int i = h; i < 2*h-1; ++i) R[i] = R0[i] + R1[i-h];
		R[2*h-1] = R1[h-1];
		for (int i = 2*h; i < 3*h-1; ++i) R[i] = R1[i-h] + R2[i-2*h+1];
		for (int i = 3*h-1; i < 4*h-1; ++i) R[i] = R2[i-2*h+1];

		delete[] A0;
		delete[] A1;
		delete[] Atemp;
		delete[] B0;
		delete[] B1;
		delete[] Btemp;
		delete[] R0;
		delete[] R1;
		delete[] R2;
		delete[] Rtemp;
	}
	else
	{
		R[0] = A[0]*B[0];
		R[2] = A[1]*B[1];
		R[1] = (A[0]+A[1])*(B[0]+B[1]) - R[0] - R[2];
	}

}


EXPORT void multKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2){
	int N;
	N = poly1->N;
	
	int* A = new int[N];
	Torus32* B = new Torus32[N];
	for (int i = 0; i < N; ++i)
	{
		A[i] = poly1->coefs[i];
		B[i] = poly2->coefsT[i];
	}

	Torus32* R = new Torus32[2*N-1];
	
	// Karatsuba 
	Karatsuba_aux(R, A, B, N);

	// reduction mod X^N-1
	for (int i = 0; i < N-1; ++i) result->coefsT[i] = R[i] + R[N+i];
	result->coefsT[N-1] = R[N-1];

	delete[] A;
	delete[] B;
	delete[] R;
}
