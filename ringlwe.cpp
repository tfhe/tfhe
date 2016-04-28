#include "lwe.h"
#include "ringlwe.h"
#include "multiplication.h"

//struct RingLWEParams {
//    const int k; //number of polynomials in the mask
//    const int N; //a power of 2: degree of the polynomials
//    const double alpha_min;
//    const double alpha_max;
//};

RingLWEParams::RingLWEParams(int N, int k, double alpha_min, double alpha_max):
    N(N),k(k),alpha_min(alpha_min),alpha_max(alpha_max) {}

RingLWEParams::~RingLWEParams() {}

//struct RingLWEKey {
//    const RingLWEParams* params;
//    IntPolynomial* key;
//};

RingLWEKey::RingLWEKey(const RingLWEParams* params):
    params(params)
{
    key = new_IntPolynomial_array(params->k,params->N);   
}

RingLWEKey::~RingLWEKey() {
    delete_IntPolynomial_array(params->k,key);
}


//struct RingLWESample {
//    TorusPolynomial* a;
//    TorusPolynomial* b;
//    double current_alpha;
//};

RingLWESample::RingLWESample(const RingLWEParams* params): k(params->k) {
    //Small change here: 
    //a is a table of k+1 polynomials, b is an alias for &a[k]
    //like that, we can access all the coefficients as before:
    //  &sample->a[0],...,&sample->a[k-1]  and &sample->b
    //or we can also do it in a single for loop
    //  &sample->a[0],...,&sample->a[k]
    a = new_TorusPolynomial_array(k+1, params->N);
    b = a+k;
    current_alpha = 0;
}

RingLWESample::~RingLWESample() {
    delete_TorusPolynomial_array(k+1, a);
}

RingLWESampleFFT::RingLWESampleFFT(const RingLWEParams* params): k(params->k) {
    //a is a table of k+1 polynomials, b is an alias for &a[k]
    a = new_LagrangeHalfCPolynomial_array(k+1, params->N);
    b = a+k;
    current_alpha = 0;
}

RingLWESampleFFT::~RingLWESampleFFT() {
    delete_LagrangeHalfCPolynomial_array(k+1, a);
}

