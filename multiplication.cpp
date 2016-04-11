#include <cstdlib>
#include <cmath>
#include "multiplication.h"

using namespace std;



IntPolynomial::IntPolynomial(const int* coefs) {
    this->coefs = new int[1024]; //mettre N à la place de 1024
}

IntPolynomial::~IntPolynomial() {
    delete[] coefs;
}



TorusPolynomial::TorusPolynomial(const double* coefs) {
    this->coefs = new int[1024]; //mettre N à la olace de 1024
}

TorusPolynomial::~TorusPolynomial() {
    delete[] coefs;
}



/**
 * This function multiplies 2 polynomials (an integer poly and a torus poly)
 * by using Karatsuba
 */

EXPORT void multKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2){

}
