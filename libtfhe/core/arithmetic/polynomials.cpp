#include "polynomials.h"

using namespace std;

template<typename COEF_TYPE, int tag>
Polynomial<COEF_TYPE,tag>::Polynomial(
    const PolynomialParameters *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    coefs = alloc->newArray<COEF_TYPE>(params->N);
}

template<typename COEF_TYPE, int tag>
void Polynomial<COEF_TYPE,tag>::destroy(
    const PolynomialParameters *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    alloc->deleteArray<COEF_TYPE>(params->N, coefs);
}


// INT_CLASS_IMPL_ALL(IntPolynomial);