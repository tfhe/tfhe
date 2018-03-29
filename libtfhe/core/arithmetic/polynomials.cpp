#include "polynomials.h"

using namespace std;


//
// IntPolynomial
//

// constructor
IntPolynomial::IntPolynomial(const int N) : N(N) {
    this->coefs = new int[N];
}




//
// TorusPolynomial
//

// constructor
template<typename TORUS>
TorusPolynomial<TORUS>::TorusPolynomial(const int N) : N(N) {
    this->coefsT = new TORUS[N];
}






