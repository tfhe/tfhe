/* **************************
Fakes for LWE 
************************** */


#ifndef FAKES_LWE_H
#define FAKES_LWE_H

#include "tfhe.h"

// Fake LWE structure 
struct FakeLwe {
    static const int FAKE_LWE_UID=45287951; // precaution: do not confuse fakes with trues
    const int fake_uid;
    Torus32 message;
    double current_variance;

    // construct
    FakeLwe():fake_uid(FAKE_LWE_UID) {
        current_variance = 0.;
    }

    // delete
    ~FakeLwe() {
        if (fake_uid!=FAKE_LWE_UID) abort();
    }
    FakeLwe(const FakeLwe&)=delete;
    void operator=(const FakeLwe&)=delete;
};




inline FakeLwe* fake(LweSample* sample) {
    FakeLwe* reps = (FakeLwe*) sample;
    if (reps->fake_uid!=FakeLwe::FAKE_LWE_UID) abort();
    return reps; 
}
inline const FakeLwe* fake(const LweSample* sample) {
    const FakeLwe* reps = (const FakeLwe*) sample;
    if (reps->fake_uid!=FakeLwe::FAKE_LWE_UID) abort();
    return reps; 
}








// Fake symetric encryption of a Torus message
inline void fake_lweSymEncrypt(LweSample* result, Torus32 message, double alpha, const LweKey* key) {
    lweNoiselessTrivial(result,message,key->params);
    result->current_variance=alpha*alpha;
}

#define USE_FAKE_lweSymEncrypt \
    inline void lweSymEncrypt(LweSample* result, Torus32 message, double alpha, const LweKey* key) { \
    return fake_lweSymEncrypt(result,message,alpha,key); \
}



// The message is just the b part, as the samples are noiseless trivial 
inline Torus32 fake_lweMessage(const LweSample* sample) {
    return sample->b;
}

// Variance 
inline Torus32 fake_lweVariance(const LweSample* sample) {
    return sample->current_variance;
}







/** result = result - sample */
inline void fake_lweSubTo(LweSample* result, const LweSample* sample, const LweParams* params) {
    FakeLwe* fres = fake(result);
    const FakeLwe* fsample = fake(sample);
    fres->message -= fsample->message;
    fres->current_variance += fsample->current_variance;
}

#define USE_FAKE_lweSubTo \
    inline void lweSubTo(LweSample* result, const LweSample* sample, const LweParams* params) { \
    return fake_lweSubTo(result,sample,params); \
}


#endif //FAKES_LWE_H
