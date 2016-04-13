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
			for (int j=i+1; j<N; j++) {
		    	ri -= poly1->coefs[j]*poly2->coefsT[N+i-j];
			}
		result->coefsT[i]=ri;
    }
}

/**
 * This function multiplies 2 polynomials (an integer poly and a torus poly)
 * by using Karatsuba
 */

// A and B of size = size
// R of size = 2*size-1
EXPORT void Karatsuba_aux(Torus32* R, const int* A, const Torus32* B, const int size){
	int h = size / 2;

	if (h!=1)
	{
		// split the polynomials in 2
		int* Atemp = new int[h];
		Torus32* Btemp = new Torus32[h];

		for (int i = 0; i < h; ++i)
		{
			Atemp[i] = A[i] + A[h+i];
			Btemp[i] = B[i] + B[h+i];
		}

		// Karatsuba recursivly
		Torus32* Rtemp = new Torus32[2*h-1];
		
		Karatsuba_aux((R.begin(),R.begin()+(2*h-2)), (A.begin(),A.begin()+(h-1)), (B.begin(),B.begin()+(h-1)), h);
		Karatsuba_aux((R.begin()+(2*h),R.begin()+(4*h-2)), (A.begin()+h,A.begin()+(2*h-1)), (B.begin()+h,B.begin()+(2*h-1)), h);
		Karatsuba_aux(Rtemp, Atemp, Btemp, h);
		for (int i = 0; i < 2*h-1; ++i) Rtemp[i] = Rtemp[i] - R[i] - R[2*h+i];

		for (int i = h; i < 2*h-1; ++i) R[i] += Rtemp[i-h];
		R[2*h-1] = Rtemp[h-1];
		for (int i = 2*h; i < 3*h-1; ++i) R[i] += Rtemp[i-h];

		delete[] Atemp;
		delete[] Btemp;
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
