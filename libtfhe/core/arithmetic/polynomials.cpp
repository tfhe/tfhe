#include "polynomials.h"

using namespace std;


//
// IntPolynomial
//

IntPolynomial::IntPolynomial(const PolynomialParameters *params,
                             TfheThreadContext *context,
                             Allocator *alloc) {
    IntPolynomial *poly = alloc->newObject<IntPolynomial>(params, context, alloc);
    poly->coefs = new int[params->N];
    //poly->coefs = alloc->newObject<int>(params, context, alloc);

}

/*
void IntPolynomial::destroy(IntPolynomial *poly,
                            const PolynomialParameters *params,
                            TfheThreadContext *context,
                            Allocator *alloc) {
    alloc->deleteObject<IntPolynomial>(poly, params, context, alloc);
}
*/



//
// TorusPolynomial
//


template<typename TORUS>
TorusPolynomial<TORUS>::TorusPolynomial(const PolynomialParameters *params,
                                        TfheThreadContext *context,
                                        Allocator *alloc) {
    TorusPolynomial<TORUS> *polyT = alloc->newObject<TorusPolynomial<TORUS>>(params, context, alloc);
    polyT->coefsT = new TORUS[params->N];
    //polyT->coefsT = alloc->newObject<TORUS>(params, context, alloc);
}

template<typename TORUS>
void TorusPolynomial<TORUS>::destroy(TorusPolynomial<TORUS> *polyT,
                                     const PolynomialParameters *params,
                                     TfheThreadContext *context,
                                     Allocator *alloc) {

    alloc->deleteObject<TorusPolynomial<TORUS>>(polyT, params, context, alloc);
}


