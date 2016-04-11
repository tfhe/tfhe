#include <cstdlib>
#include <cmath>
#include "multiplication.h"

using namespace std;



IntPolynomial::IntPolynomial(const int N) {
    this->coefs = new int[N]; 
}

IntPolynomial::~IntPolynomial() {
    delete[] coefs;
}



TorusPolynomial::TorusPolynomial(const int N) {
    this->coefs = new int[N]; 
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
