#include "polynomials.h"

using namespace std;


//
// IntPolynomial
//

IntPolynomial::IntPolynomial(const PolynomialParameters *params,
                             TfheThreadContext *context,
                             Allocator *alloc) {

    this->coefs = alloc->newArray<int32_t>(params->N);
}


void IntPolynomial::destroy(const PolynomialParameters *params,
                            TfheThreadContext *context,
                            Allocator *alloc) {

    alloc->deleteArray<int32_t>(params->N, coefs);
}




//
// TorusPolynomial
//


template<typename TORUS>
TorusPolynomial<TORUS>::TorusPolynomial(const PolynomialParameters *params,
                                        TfheThreadContext *context,
                                        Allocator *alloc) {

    this->coefsT = alloc->newArray<TORUS>(params->N);
}

template<typename TORUS>
void TorusPolynomial<TORUS>::destroy(const PolynomialParameters *params,
                                     TfheThreadContext *context,
                                     Allocator *alloc) {

    alloc->deleteArray<TORUS>(params->N, coefsT);
}


