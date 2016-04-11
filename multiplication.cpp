#include <cstdlib>
#include <cmath>
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
 * This function multiplies 2 polynomials (an integer poly and a torus poly)
 * by using Karatsuba
 */

EXPORT void multKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2){
	int size, h;
	size = sizeof(poly1) / sizeof(poly1[0]);
	h = size / 2;
	if (h!=1)
	{
		// split the polynomials in 2
		IntPolynomial* A0[h], A1[h], Atemp[h];
		TorusPolynomial* B0[h], B1[h], Btemp[h];
		for (int i = 0; i < h; ++i)
		{
			A0[i] = poly1[i];
			A1[i] = poly1[h+i];
			Atemp[i] = A0[i] + A1[i];
			B0[i] = poly2[i];
			B1[i] = poly2[h+i];
			Btemp[i] = B0[i] + B1[i];

		}

		// Karatsuba recursivly
		TorusPolynomial* R0[2*h-1], R1[2*h-1], R2[2*h-1], Rtemp[2*h-1];
		
		multKaratsuba(R0, A0, B0);
		multKaratsuba(R2, A1, B1);
		multKaratsuba(Rtemp, Atemp, Btemp);
		for (int i = 0; i < 2*h-1; ++i) R1[i] = Rtemp[i] - R0[i] - R2[i];

		for (int i = 0; i < h; ++i) result[i] = R0[i];
		for (int i = h; i < 2*h-1; ++i) result[i] = R0[i] + R1[i-h];
		result[2*h-1] = R1[h-1];
		for (int i = 2*h; i < 3*h-1; ++i) result[i] = R1[i-h] + R2[i-2*h+1];
		for (int i = 3*h-1; i < 4*h-1; ++i) result[i] = R2[i-2*h+1];
	}
	else
	{
		result[0] = A[0]*B[0];
		result[2] = A[1]*B[1];
		result[1] = (A[0]+A[1])*(B[0]*B[1]) - result[0] - result[2];
	}

}
