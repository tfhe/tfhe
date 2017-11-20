#include "tfhe_core.h"
#include "tlwe.h"
#include "polynomials.h"

//struct TLweParams {
//    const int32_t k; //number of polynomials in the mask
//    const int32_t N; //a power of 2: degree of the polynomials
//    const double alpha_min;
//    const double alpha_max;
//};

TLweParams::TLweParams(int32_t N, int32_t k, double alpha_min, double alpha_max) :
        N(N),
        k(k),
        alpha_min(alpha_min),
        alpha_max(alpha_max),
        extracted_lweparams(N * k, alpha_min, alpha_max) {}

TLweParams::~TLweParams() {}

//struct TLweKey {
//    const TLweParams* params;
//    IntPolynomial* key;
//};

TLweKey::TLweKey(const TLweParams *params) :
        params(params) {
    key = new_IntPolynomial_array(params->k, params->N);
}

TLweKey::~TLweKey() {
    delete_IntPolynomial_array(params->k, key);
}


//struct TLweSample {
//    TorusPolynomial* a;
//    TorusPolynomial* b;
//    double current_variance;
//};

TLweSample::TLweSample(const TLweParams *params) : k(params->k) {
    //Small change here: 
    //a is a table of k+1 polynomials, b is an alias for &a[k]
    //like that, we can access all the coefficients as before:
    //  &sample->a[0],...,&sample->a[k-1]  and &sample->b
    //or we can also do it in a single for loop
    //  &sample->a[0],...,&sample->a[k]
    a = new_TorusPolynomial_array(k + 1, params->N);
    b = a + k;
    current_variance = 0;
}

TLweSample::~TLweSample() {
    delete_TorusPolynomial_array(k + 1, a);
}

TLweSampleFFT::TLweSampleFFT(const TLweParams *params, LagrangeHalfCPolynomial *arr, double current_variance) : k(
        params->k) {
    //a is a table of k+1 polynomials, b is an alias for &a[k]
    a = arr;
    b = a + k;
    current_variance = 0;
}

TLweSampleFFT::~TLweSampleFFT() {
}

