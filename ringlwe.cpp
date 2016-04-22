#include "lwe.h"
#include "ringlwe.h"

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
    a = new_TorusPolynomial_array(k, params->N);
    b = new_TorusPolynomial(params->N);
    current_alpha = 0;
}

RingLWESample::~RingLWESample() {
    delete_TorusPolynomial_array(k, a);
    delete_TorusPolynomial(b);
}

