#ifndef FAKES_TLWE_H
#define FAKES_TLWE_H

#include "./lwe.h"


/* **************************
Fakes for TLWE 
************************** */

namespace {

    // Fake TLWE structure 
    struct FakeTLwe {
        //TODO: parallelization
        static const int32_t FAKE_TLWE_UID = 542354312; // precaution: distinguish fakes from trues
        const int32_t fake_uid;
        TorusPolynomial *message;
        double current_variance;

        //this padding is here to make sure that FakeTLwe and TLweSample have the same size
        char unused_padding[sizeof(TLweSample) - sizeof(int64_t) - sizeof(TorusPolynomial *) - sizeof(double)];

        // construct
        FakeTLwe(int32_t N) : fake_uid(FAKE_TLWE_UID) {
            message = new_TorusPolynomial(N);
            current_variance = 0.;
        }

        // delete
        ~FakeTLwe() {
            assert(sizeof(FakeTLwe) == sizeof(TLweSample));
            if (fake_uid != FAKE_TLWE_UID) abort();
            delete_TorusPolynomial(message);
        }

        FakeTLwe(const FakeTLwe &) = delete;

        void operator=(const FakeTLwe &)= delete;
    };



    // At compile time, we verify that the two structures have exactly the same size
    //TODO: parallelization
    static_assert(sizeof(FakeTLwe) == sizeof(TLweSample), "Error: Size is not correct");

    // fake functons 
    inline FakeTLwe *fake(TLweSample *sample) {
        FakeTLwe *reps = (FakeTLwe *) sample;
        if (reps->fake_uid != FakeTLwe::FAKE_TLWE_UID) abort();
        return reps;
    }

    inline const FakeTLwe *fake(const TLweSample *sample) {
        const FakeTLwe *reps = (const FakeTLwe *) sample;
        if (reps->fake_uid != FakeTLwe::FAKE_TLWE_UID) abort();
        return reps;
    }


    // A new array containing TLWE samples (not a TGSW!!!) --> why?
    inline TLweSample *fake_new_TLweSample_array(int32_t nbelts, const TLweParams *params) {
        int32_t N = params->N;
        //TLweSample* arr = (TLweSample*) malloc(nbelts*sizeof(TLweSample));
        //for (int32_t i=0; i<nbelts; i++) new(arr+i) FakeTLwe(N); 
        FakeTLwe *arr = (FakeTLwe *) malloc(nbelts * sizeof(FakeTLwe));
        for (int32_t i = 0; i < nbelts; i++) new(arr + i) FakeTLwe(N);
        return (TLweSample *) arr;
    }

    // 
#define USE_FAKE_new_TLweSample_array \
    inline TLweSample* new_TLweSample_array(int32_t nbelts, const TLweParams* params) { \
        return fake_new_TLweSample_array(nbelts,params); \
    }

    inline void fake_delete_TLweSample_array(int32_t nbelts, TLweSample *sample) {
        FakeTLwe *arr = fake(sample);
        for (int32_t i = 0; i < nbelts; i++) (arr + i)->~FakeTLwe();
        free(arr);
    }

    // 
#define USE_FAKE_delete_TLweSample_array \
    inline void delete_TLweSample_array(int32_t nbelts, TLweSample* samples) { \
        fake_delete_TLweSample_array(nbelts,samples); \
    }


    inline TLweSample *fake_new_TLweSample(const TLweParams *params) {
        int32_t N = params->N;
        FakeTLwe *reps = (FakeTLwe *) malloc(sizeof(FakeTLwe));
        ((TLweSample *) reps)->a = ((TorusPolynomial *) int64_t(
                rand())); //it is necessary to fake-initialize this chunk, because one of tgswFFT tests needs it
        new(reps) FakeTLwe(N);
        return (TLweSample *) reps;
    }

    // 
#define USE_FAKE_new_TLweSample \
    inline TLweSample* new_TLweSample(const TLweParams* params) { \
        return fake_new_TLweSample(params); \
    }

    inline void fake_delete_TLweSample(TLweSample *sample) {
        FakeTLwe *ptr = fake(sample);
        (ptr)->~FakeTLwe();
        free(ptr);
    }

    // 
#define USE_FAKE_delete_TLweSample \
    inline void delete_TLweSample(TLweSample* sample) { \
        fake_delete_TLweSample(sample); \
    }


    // Fake symmetric encryption of a Torus polynomial message (encryption is a noiseless trivial sample)
    inline void fake_tLweSymEncrypt(TLweSample *result, TorusPolynomial *message, double alpha, const TLweKey *key) {
        FakeTLwe *fres = fake(result);

        torusPolynomialCopy(fres->message, message);
        fres->current_variance = alpha * alpha;
    }

#define USE_FAKE_tLweSymEncrypt \
    inline void tLweSymEncrypt(TLweSample* result, TorusPolynomial* message, double alpha, const TLweKey* key) { \
        fake_tLweSymEncrypt(result, message, alpha, key); \
    }


    // Fake symmetric encryption of a Torus message (encryption is a noiseless trivial sample)
    inline void fake_tLweSymEncryptT(TLweSample *result, Torus32 message, double alpha, const TLweKey *key) {
        int32_t N = key->params->N;
        FakeTLwe *fres = fake(result);

        fres->message->coefsT[0] = message;
        for (int32_t j = 1; j < N; ++j) fres->message->coefsT[j] = 0;
        fres->current_variance = alpha * alpha;
    }

#define USE_FAKE_tLweSymEncryptT \
    inline void tLweSymEncryptT(TLweSample* result, Torus32 message, double alpha, const TLweKey* key) { \
        fake_tLweSymEncryptT(result, message, alpha, key); \
    }


    // Fake Torus polynomial phase (samples are noiseless trivial)
    inline void fake_tLwePhase(TorusPolynomial *phase, const TLweSample *sample, const TLweKey *key) {
        const FakeTLwe *fsamp = fake(sample);

        torusPolynomialCopy(phase, fsamp->message);
    }

#define USE_FAKE_tLwePhase \
    inline void tLwePhase(TorusPolynomial* phase, const TLweSample* sample, const TLweKey* key) { \
        fake_tLwePhase(phase,sample,key); \
    }


    // Fake Torus polynomial approximate phase (samples are noiseless trivial)
    inline void fake_tLweApproxPhase(TorusPolynomial *message, const TorusPolynomial *phase, int32_t Msize, int32_t N) {
        torusPolynomialCopy(message, phase);
    }

#define USE_FAKE_tLweApproxPhase \
    inline void tLweApproxPhase(TorusPolynomial* message, const TorusPolynomial* phase, int32_t Msize, int32_t N) { \
        fake_tLweApproxPhase(message, phase, Msize, N); \
    }


    // Fake decryption of a Torus polynomial message
    inline void fake_tLweSymDecrypt(TorusPolynomial *result, const TLweSample *sample, const TLweKey *key, int32_t Msize) {
        int32_t N = key->params->N;
        const FakeTLwe *fsamp = fake(sample);

        fake_tLweApproxPhase(result, fsamp->message, Msize, N);
    }

#define USE_FAKE_tLweSymDecrypt \
    inline void tLweSymDecrypt(TorusPolynomial* result, const TLweSample* sample, const TLweKey* key, int32_t Msize) { \
        fake_tLweSymDecrypt(result, sample, key, Msize); \
    }


    // Fake decryption of a Torus message
    inline Torus32 fake_tLweSymDecryptT(const TLweSample *sample, const TLweKey *key, int32_t Msize) {
        Torus32 message;
        const FakeTLwe *fsamp = fake(sample);

        message = fsamp->message->coefsT[0];

        return message;
    }

#define USE_FAKE_tLweSymDecryptT \
    inline Torus32 tLweSymDecryptT(const TLweSample* sample, const TLweKey* key, int32_t Msize) { \
        return fake_tLweSymDecryptT(sample, key, Msize); \
    }








    // Arithmetic operations on TLwe samples

    /** result = (0,0) */
    inline void fake_tLweClear(TLweSample *result, const TLweParams *params) {
        FakeTLwe *fres = fake(result);

        torusPolynomialClear(fres->message);
        fres->current_variance = 0.;
    }

#define USE_FAKE_tLweClear \
    inline void tLweClear(TLweSample* result, const TLweParams* params) { \
        fake_tLweClear(result, params); \
    }


    /** result = sample */
    inline void fake_tLweCopy(TLweSample *result, const TLweSample *sample, const TLweParams *params) {
        const FakeTLwe *fsamp = fake(sample);
        FakeTLwe *fres = fake(result);

        torusPolynomialCopy(fres->message, fsamp->message);
        fres->current_variance = fsamp->current_variance;
    }

#define USE_FAKE_tLweCopy \
    inline void tLweCopy(TLweSample* result, const TLweSample* sample, const TLweParams* params) { \
        fake_tLweCopy(result,sample,params); \
    }


    /** result = (0,mu) */
    inline void fake_tLweNoiselessTrivial(TLweSample *result, const TorusPolynomial *mu, const TLweParams *params) {
        FakeTLwe *fres = fake(result);

        torusPolynomialCopy(fres->message, mu);
        fres->current_variance = 0.;
    }

#define USE_FAKE_tLweNoiselessTrivial \
    inline void tLweNoiselessTrivial(TLweSample* result, const TorusPolynomial* mu, const TLweParams* params) { \
        fake_tLweNoiselessTrivial(result,mu,params); \
    }


    /** result = result + sample */
    inline void fake_tLweAddTo(TLweSample *result, const TLweSample *sample, const TLweParams *params) {
        const FakeTLwe *fsamp = fake(sample);
        FakeTLwe *fres = fake(result);

        torusPolynomialAddTo(fres->message, fsamp->message);
        fres->current_variance += fsamp->current_variance;
    }

#define USE_FAKE_tLweAddTo \
    inline void tLweAddTo(TLweSample* result, const TLweSample* sample, const TLweParams* params) { \
        fake_tLweAddTo(result,sample,params); \
    }


    /** result = result - sample */
    inline void fake_tLweSubTo(TLweSample *result, const TLweSample *sample, const TLweParams *params) {
        const FakeTLwe *fsamp = fake(sample);
        FakeTLwe *fres = fake(result);

        torusPolynomialSubTo(fres->message, fsamp->message);
        fres->current_variance += fsamp->current_variance;
    }

#define USE_FAKE_tLweSubTo \
    inline void tLweSubTo(TLweSample* result, const TLweSample* sample, const TLweParams* params) { \
        fake_tLweSubTo(result,sample,params); \
    }


    /** result = result + p.sample */
    inline void fake_tLweAddMulTo(TLweSample *result, int32_t p, const TLweSample *sample, const TLweParams *params) {
        const FakeTLwe *fsamp = fake(sample);
        FakeTLwe *fres = fake(result);

        torusPolynomialAddMulZTo(fres->message, p, fsamp->message);
        fres->current_variance += p * fsamp->current_variance;
    }

#define USE_FAKE_tLweAddMulTo \
    inline void tLweAddMulTo(TLweSample* result, int32_t p, const TLweSample* sample, const TLweParams* params) { \
        fake_tLweAddMulTo(result,p,sample,params); \
    }


    /** result = result - p.sample */
    inline void fake_tLweSubMulTo(TLweSample *result, int32_t p, const TLweSample *sample, const TLweParams *params) {
        const FakeTLwe *fsamp = fake(sample);
        FakeTLwe *fres = fake(result);

        torusPolynomialSubMulZTo(fres->message, p, fsamp->message);
        fres->current_variance += p * fsamp->current_variance;
    }

#define USE_FAKE_tLweSubMulTo \
    inline void tLweSubMulTo(TLweSample* result, int32_t p, const TLweSample* sample, const TLweParams* params) { \
        fake_tLweSubMulTo(result,p,sample,params); \
    }


    /** create an homogeneous tlwe sample*/
    inline void fake_tLweSymEncryptZero(TLweSample *result, double alpha, const TLweKey *key) {
        FakeTLwe *fres = fake(result);

        torusPolynomialClear(fres->message);
        fres->current_variance = alpha * alpha;
    }

#define USE_FAKE_tLweSymEncryptZero \
    inline void tLweSymEncryptZero(TLweSample* result, double alpha, const TLweKey* key) { \
        fake_tLweSymEncryptZero(result,alpha,key); \
    }


    /** result = result + p.sample */
    inline void
    fake_tLweAddMulRTo(TLweSample *result, const IntPolynomial *p, const TLweSample *sample, const TLweParams *params) {
        const FakeTLwe *fsamp = fake(sample);
        FakeTLwe *fres = fake(result);
        TorusPolynomial *tmp = new_TorusPolynomial(params->N);

        torusPolynomialMultKaratsuba(tmp, p, fsamp->message);
        torusPolynomialAddTo(fres->message, tmp);

        fres->current_variance += intPolynomialNorm2sq(p) * fsamp->current_variance;
        delete_TorusPolynomial(tmp);
    }

#define USE_FAKE_tLweAddMulRTo \
    inline void tLweAddMulRTo(TLweSample* result, const IntPolynomial* p, const TLweSample* sample, const TLweParams* params) { \
        fake_tLweAddMulRTo(result,p,sample,params); \
    }


    /** result = (X^{a}-1)*source */
    inline void fake_tLweMulByXaiMinusOne(TLweSample *result, int32_t ai, const TLweSample *bk, const TLweParams *params) {
        const FakeTLwe *fbk = fake(bk);
        FakeTLwe *fres = fake(result);

        torusPolynomialMulByXaiMinusOne(fres->message, ai, fbk->message);
        fres->current_variance = (ai == 0 ? 1 : 2) * fbk->current_variance;
    }

#define USE_FAKE_tLweMulByXaiMinusOne \
    inline void tLweMulByXaiMinusOne(TLweSample* result, int32_t ai, const TLweSample* bk, const TLweParams* params) { \
        fake_tLweMulByXaiMinusOne(result,ai,bk,params); \
    }


    /** result = ExtractLweSample(x) */
    inline void fake_tLweExtractLweSample(LweSample *result, const TLweSample *x, const LweParams *params,
                                          const TLweParams *rparams) {
        const FakeTLwe *fx = fake(x);
        FakeLwe *fres = fake(result);

        fres->message = fx->message->coefsT[0];
        fres->current_variance = fx->current_variance;
    }

#define USE_FAKE_tLweExtractLweSample \
    inline void tLweExtractLweSample(LweSample* result, const TLweSample* x, const LweParams* params,  const TLweParams* rparams) { \
        fake_tLweExtractLweSample(result,x,params,rparams); \
    }

}




// Ring
// Est-ce qu'on doit aussi faker les clés?
// EXPORT void tLweKeyGen(TLweKey* result);


////////////////////////////////////////////
// these functions are not implemented, and will probably never be
// implemented
// EXPORT void tLwePolyCombination(TLweSample* result, const int32_t* combi, const TLweSample* samples, const TLweParams* params);
//EXPORT void tLweExtractLweSample(LweSample* result, const TLweSample* x, const LweParams* params,  const TLweParams* rparams);
//extractions TLwe -> Lwe
//EXPORT void tLweExtractKey(LweKey* result, const TLweKey*); //sans doute un param supplémentaire
//EXPORT void tLweExtractSample(LweSample* result, const TLweSample* x);
///////////////////////////////////////////

#endif //FAKES_TLWE_H
