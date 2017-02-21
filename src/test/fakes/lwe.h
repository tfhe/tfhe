/* **************************
Fakes for LWE 
************************** */


#ifndef FAKE_LWE_H
#define FAKE_LWE_H

#include "tfhe.h"

// Fake symetric encryption of a Torus message
inline void fake_lweSymEncrypt(LweSample* result, Torus32 message, double alpha, const LweKey* key) {
    lweNoiselessTrivial(result,message,key->params);
    result->current_variance=alpha*alpha;
}

// The message is just the b part, as the samples are noiseless trivial 
inline Torus32 fake_lweMessage(const LweSample* sample) {
    return sample->b;
}

// Variance 
inline Torus32 fake_lweVariance(const LweSample* sample) {
    return sample->current_variance;
}

#endif //FAKE_LWE_H
