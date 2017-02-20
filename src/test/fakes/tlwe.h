/* **************************
Fakes for TLWE 
************************** */


#ifndef FAKE_TLWE_H
#define FAKE_TLWE_H


#include "tfhe.h"


// Fake symetric encryption of a Torus message
inline void fake_tLweSymEncryptT(TLweSample* result, Torus32 message, double alpha, const TLweKey* key) {
    tLweNoiselessTrivialT(result,message,key->params);
    result->current_variance=alpha*alpha;
}

// Fake symetric encryption of a Torus polynomial message
inline void fake_tLweSymEncrypt(TLweSample* result, TorusPolynomial* mu, double alpha, const TLweKey* key) {
    tLweNoiselessTrivial(result,message,key->params);
    result->current_variance=alpha*alpha;
}


// The message is just the b part, as the samples are noiseless trivial 
inline Torus32 fake_tLweMessage(const TLweSample* sample) {
    return sample->b;
}


// Variance 
inline Torus32 fake_tLweVariance(const TLweSample* sample) {
    return sample->current_variance;
}




#endif //FAKE_TLWE_H
