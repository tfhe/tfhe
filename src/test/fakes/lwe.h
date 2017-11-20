/* **************************
Fakes for LWE 
************************** */


#ifndef FAKES_LWE_H
#define FAKES_LWE_H

#include "tfhe.h"

// Fake LWE structure 
struct FakeLwe {
    //TODO: parallelization
    static const int32_t FAKE_LWE_UID = 45287951; // precaution: do not confuse fakes with trues
    const int32_t fake_uid;
    Torus32 message;
    double current_variance;

    //this padding is here to make sure that FakeLwe and LweSample have the same size
    char unused_padding[sizeof(LweSample) - sizeof(int32_t) - sizeof(Torus32) - sizeof(double)];

    // construct
    FakeLwe() : fake_uid(FAKE_LWE_UID) {
        current_variance = 0.;
    }

    // delete
    ~FakeLwe() {
        if (fake_uid != FAKE_LWE_UID) abort();
    }

    FakeLwe(const FakeLwe &) = delete;

    void operator=(const FakeLwe &)= delete;
};



// At compile time, we verify that the two structures have exactly the same size
//TODO: parallelization
static_assert(sizeof(FakeLwe) == sizeof(LweSample), "Error: Size is not correct");


inline FakeLwe *fake(LweSample *sample) {
    FakeLwe *reps = (FakeLwe *) sample;
    if (reps->fake_uid != FakeLwe::FAKE_LWE_UID) abort();
    return reps;
}

inline const FakeLwe *fake(const LweSample *sample) {
    const FakeLwe *reps = (const FakeLwe *) sample;
    if (reps->fake_uid != FakeLwe::FAKE_LWE_UID) abort();
    return reps;
}


inline LweSample *fake_new_LweSample(const LweParams *params) {
    FakeLwe *reps = (FakeLwe *) malloc(sizeof(FakeLwe));
    new(reps) FakeLwe();
    return (LweSample *) reps;
}

//TODO: parallelization
#define USE_FAKE_new_LweSample \
static inline LweSample *new_LweSample(const LweParams *params) { \
    return fake_new_LweSample(params); \
}

inline void fake_delete_LweSample(LweSample *sample) {
    FakeLwe *ptr = fake(sample);
    (ptr)->~FakeLwe();
    free(ptr);
}

//TODO: parallelization
#define USE_FAKE_delete_LweSample \
static inline void delete_LweSample(LweSample *sample) { \
    fake_delete_LweSample(sample); \
}


inline LweSample *fake_new_LweSample_array(int32_t nbelts, const LweParams *params) {
    FakeLwe *arr = (FakeLwe *) malloc(nbelts * sizeof(FakeLwe));
    for (int32_t i = 0; i < nbelts; i++) new(arr + i) FakeLwe();
    return (LweSample *) arr;
}

#define USE_FAKE_new_LweSample_array \
inline LweSample* new_LweSample_array(int32_t nbelts, const LweParams* params) { \
    return fake_new_LweSample_array(nbelts, params); \
}

inline void fake_delete_LweSample_array(int32_t nbelts, LweSample *sample) {
    FakeLwe *arr = fake(sample);
    for (int32_t i = 0; i < nbelts; i++) (arr + i)->~FakeLwe();
    free(arr);
}

#define USE_FAKE_delete_LweSample_array \
inline void delete_LweSample_array(int32_t nbelts, LweSample* samples) { \
    fake_delete_LweSample_array(nbelts,samples); \
}


// The message is just the b part, as the samples are noiseless trivial
inline Torus32 fake_lweMessage(const LweSample *sample) {
    return sample->b;
}

// Variance 
inline Torus32 fake_lweVariance(const LweSample *sample) {
    return sample->current_variance;
}


// Fake Copy
inline void fake_lweCopy(LweSample *result, const LweSample *sample, const LweParams *params) {
    FakeLwe *fres = fake(result);
    const FakeLwe *fsample = fake(sample);
    fres->message = fsample->message;
    fres->current_variance = fsample->current_variance;
}

//TODO: parallelization
#define USE_FAKE_lweCopy \
static inline void lweCopy(LweSample *result, const LweSample *sample, const LweParams *params) {\
    return fake_lweCopy(result, sample, params); \
}


// Fake Negate
inline void fake_lweNegate(LweSample *result, const LweSample *sample, const LweParams *params) {
    FakeLwe *fres = fake(result);
    const FakeLwe *fsample = fake(sample);
    fres->message = -fsample->message;
    fres->current_variance = fsample->current_variance;
}

//TODO: parallelization
#define USE_FAKE_lweNegate \
static inline void lweNegate(LweSample *result, const LweSample *sample, const LweParams *params) {\
    return fake_lweNegate(result, sample, params); \
}


// Fake symetric encryption of a Torus message
inline void fake_lweSymEncrypt(LweSample *result, Torus32 message, double alpha, const LweKey *key) {
    FakeLwe *fres = fake(result);
    fres->message = message;
    fres->current_variance = alpha * alpha;
}

#define USE_FAKE_lweSymEncrypt \
    inline void lweSymEncrypt(LweSample* result, Torus32 message, double alpha, const LweKey* key) { \
    return fake_lweSymEncrypt(result,message,alpha,key); \
}


// Fake symetric encryption with external noise of a Torus message
inline void
fake_lweSymEncryptWithExternalNoise(LweSample *result, Torus32 message, double noise, double alpha, const LweKey *key) {
    FakeLwe *fres = fake(result);
    fres->message = message;
    fres->current_variance = alpha * alpha;
}

#define USE_FAKE_lweSymEncryptWithExternalNoise \
    inline void lweSymEncryptWithExternalNoise(LweSample* result, Torus32 message, double noise, double alpha, const LweKey* key) { \
    return fake_lweSymEncryptWithExternalNoise(result,message,noise,alpha,key); \
}


// Noiseless trivial of a Torus message
inline void fake_lweNoiselessTrivial(LweSample *result, Torus32 message, const LweParams *params) {
    FakeLwe *fres = fake(result);
    fres->message = message;
    fres->current_variance = 0;
}

//TODO: parallelization
#define USE_FAKE_lweNoiselessTrivial \
static inline void lweNoiselessTrivial(LweSample *result, Torus32 message, const LweParams *params) {\
    return fake_lweNoiselessTrivial(result, message, params); \
}


// phase
inline Torus32 fake_lwePhase(const LweSample *sample, const LweKey *key) {
    return sample->b;
}

//TODO: parallelization
#define USE_FAKE_lwePhase \
static inline Torus32 lwePhase(const LweSample *sample, const LweKey *key) {\
    return fake_lwePhase(sample, key); \
}


/** result = result - sample */
inline void fake_lweSubTo(LweSample *result, const LweSample *sample, const LweParams *params) {
    FakeLwe *fres = fake(result);
    const FakeLwe *fsample = fake(sample);
    fres->message -= fsample->message;
    fres->current_variance += fsample->current_variance;
}

//TODO: parallelization
#define USE_FAKE_lweSubTo \
static inline void lweSubTo(LweSample *result, const LweSample *sample, const LweParams *params) {\
    return fake_lweSubTo(result, sample, params); \
}

/** result = result + sample */
inline void fake_lweAddTo(LweSample *result, const LweSample *sample, const LweParams *params) {
    FakeLwe *fres = fake(result);
    const FakeLwe *fsample = fake(sample);
    fres->message += fsample->message;
    fres->current_variance += fsample->current_variance;
}

//TODO: parallelization
#define USE_FAKE_lweAddTo \
static inline void lweAddTo(LweSample *result, const LweSample *sample, const LweParams *params) {\
    return fake_lweAddTo(result, sample, params); \
}


/** result = result + p*sample */
inline void fake_lweAddMulTo(LweSample *result, const int32_t p, const LweSample *sample, const LweParams *params) {
    FakeLwe *fres = fake(result);
    const FakeLwe *fsample = fake(sample);
    fres->message += p * fsample->message;
    fres->current_variance += p * fsample->current_variance;
}

//TODO: parallelization
#define USE_FAKE_lweAddMulTo \
static inline void lweAddMulTo(LweSample *result, const int32_t p, const LweSample *sample, const LweParams *params) {\
    return fake_lweAddMulTo(result, p, sample, params); \
}


/** result = result - p*sample */
inline void fake_lweSubMulTo(LweSample *result, const int32_t p, const LweSample *sample, const LweParams *params) {
    FakeLwe *fres = fake(result);
    const FakeLwe *fsample = fake(sample);
    fres->message -= p * fsample->message;
    fres->current_variance += p * fsample->current_variance;
}

//TODO: parallelization
#define USE_FAKE_lweSubMulTo \
static inline void lweSubMulTo(LweSample *result, const int32_t p, const LweSample *sample, const LweParams *params) {\
    return fake_lweSubMulTo(result, p, sample, params); \
}


#endif //FAKES_LWE_H
