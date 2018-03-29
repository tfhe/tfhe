#include "polynomials.h"

using namespace std;


//
// IntPolynomial
//


IntPolynomial::IntPolynomial(const PolynomialParameters *params,
                             TfheThreadContext &context,
                             Allocator &alloc) {
    coefs = alloc->newArray<int>(params->N);

}

void IntPolynomial::destroy(const PolynomialParameters *params,
                            TfheThreadContext &context,
                            Allocator &alloc) {
    alloc->deleteArray<int>(params->N, coefs);
}




//
// TorusPolynomial
//


template<typename TORUS>
TorusPolynomial::TorusPolynomial(const PolynomialParameters *params,
                                 TfheThreadContext &context,
                                 Allocator &alloc) {
    coefsT = alloc->newArray<TORUS>(params->N);

}

template<typename TORUS>
void TorusPolynomial::destroy(const PolynomialParameters *params,
                              TfheThreadContext &context,
                              Allocator &alloc) {
    alloc->deleteArray<TORUS>(params->N, coefsT);
}


