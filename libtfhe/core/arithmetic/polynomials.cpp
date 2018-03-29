#include "polynomials.h"

using namespace std;


//
// IntPolynomial
//


IntPolynomial::IntPolynomial(const PolynomialParameters *params, TfheThreadContext &context, Allocator &alloc) {
    coefs = alloc->newArray<int>(params->N);

}

void IntPolynomial::destroy(const PolynomialParameters *params, TfheThreadContext &context, Allocator &alloc) {
    alloc->deleteArray<int>(params->N, coefs);
}



/*
//
// TorusPolynomial
//

// constructor
template<typename TORUS>
TorusPolynomial<TORUS>::TorusPolynomial(const int N) : N(N) {
    this->coefsT = new TORUS[N];
}



*/

