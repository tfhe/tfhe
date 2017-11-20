#ifndef FAKES_TGSW_H
#define FAKES_TGSW_H

#include "./tlwe.h"

namespace {

    // Fake TGSW structure 
    struct FakeTGsw {
        //TODO: parallelization
        static const int64_t FAKE_TGSW_UID = 123444802642375465l; // precaution: do not confuse fakes with trues
        const int64_t fake_uid;
        IntPolynomial *message;
        double current_variance;

        //this padding is here to make sure that FakeTLwe and TLweSample have the same size
        char unused_padding[sizeof(TGswSample) - sizeof(int64_t) - sizeof(IntPolynomial *) - sizeof(double)];

        void setMessageVariance(bool mess, double variance) {
            intPolynomialClear(message);
            message->coefs[0] = mess;
            current_variance = variance;
        }

        // construct
        FakeTGsw(int32_t N) : fake_uid(FAKE_TGSW_UID) {
            message = new_IntPolynomial(N);
            current_variance = 0.;
        }

        // delete
        ~FakeTGsw() {
            if (fake_uid != FAKE_TGSW_UID) abort();
            delete_IntPolynomial(message);
        }

        FakeTGsw(const FakeTGsw &) = delete;

        void operator=(const FakeTGsw &)= delete;
    };

    // At compile time, we verify that the two structures have exactly the same size
    //TODO: parallelization
    static_assert(sizeof(FakeTGsw) == sizeof(TGswSample), "Error: Size is not correct");


    inline FakeTGsw *fake(TGswSample *sample) {
        FakeTGsw *reps = (FakeTGsw *) sample;
        if (reps->fake_uid != FakeTGsw::FAKE_TGSW_UID) abort();
        return reps;
    }

    inline const FakeTGsw *fake(const TGswSample *sample) {
        const FakeTGsw *reps = (const FakeTGsw *) sample;
        if (reps->fake_uid != FakeTGsw::FAKE_TGSW_UID) abort();
        return reps;
    }


    inline void fake_init_TGswSample(TGswSample *ptr, const TGswParams *params) {
        int32_t N = params->tlwe_params->N;
        FakeTGsw *arr = (FakeTGsw *) ptr;
        new(arr) FakeTGsw(N);
    }

#define USE_FAKE_init_TGswSample \
    inline void init_TGswSample(TGswSample* ptr, const TGswParams* params) { \
    return fake_init_TGswSample(ptr,params); \
    }

    inline void fake_destroy_TGswSample(TGswSample *ptr) {
        FakeTGsw *arr = (FakeTGsw *) ptr;
        (arr)->~FakeTGsw();
    }

#define USE_FAKE_destroy_TGswSample \
    inline void destroy_TGswSample(TGswSample* ptr) { \
    return fake_destroy_TGswSample(ptr); \
    }

    inline TGswSample *fake_new_TGswSample_array(int32_t nbelts, const TGswParams *params) {
        int32_t N = params->tlwe_params->N;
        FakeTGsw *arr = (FakeTGsw *) malloc(nbelts * sizeof(FakeTGsw));
        for (int32_t i = 0; i < nbelts; i++) new(arr + i) FakeTGsw(N);
        return (TGswSample *) arr;
    }

#define USE_FAKE_new_TGswSample_array \
    inline TGswSample* new_TGswSample_array(int32_t nbelts, const TGswParams* params) { \
    return fake_new_TGswSample_array(nbelts,params); \
    }

    inline void fake_delete_TGswSample_array(int32_t nbelts, TGswSample *sample) {
        FakeTGsw *arr = fake(sample);
        for (int32_t i = 0; i < nbelts; i++) (arr + i)->~FakeTGsw();
        free(arr);
    }

#define USE_FAKE_delete_TGswSample_array \
    inline void delete_TGswSample_array(int32_t nbelts, TGswSample* sample) { \
    fake_delete_TGswSample_array(nbelts, sample); \
    }

    inline TGswSample *fake_new_TGswSample(const TGswParams *params) {
        int32_t N = params->tlwe_params->N;
        return (TGswSample *) new FakeTGsw(N);
    }

#define USE_FAKE_new_TGswSample \
    inline TGswSample* new_TGswSample(const TGswParams* params) { \
    return fake_new_TGswSample(params); \
    }

    inline void fake_delete_TGswSample(TGswSample *sample) {
        delete fake(sample);
    }

#define USE_FAKE_delete_TGswSample \
    inline void delete_TGswSample(TGswSample* sample) { \
    fake_delete_TGswSample(sample); \
    }

    inline void
    fake_tGswSymEncrypt(TGswSample *result, const IntPolynomial *message, double alpha, const TGswKey *key) {
        FakeTGsw *fres = fake(result);
        intPolynomialCopy(fres->message, message);
        fres->current_variance = alpha;
    }

#define USE_FAKE_tGswSymEncrypt \
    inline void tGswSymEncrypt(TGswSample* result, const IntPolynomial* message, double alpha, const TGswKey* key) { \
    fake_tGswSymEncrypt(result, message, alpha, key); \
    }

    inline void fake_tGswSymEncryptInt(TGswSample *result, const int32_t message, double alpha, const TGswKey *key) {
        FakeTGsw *fres = fake(result);
        intPolynomialClear(fres->message);
        fres->message->coefs[0] = message;
        fres->current_variance = alpha;
    }

#define USE_FAKE_tGswSymEncryptInt \
    inline void tGswSymEncryptInt(TGswSample* result, const int32_t message, double alpha, const TGswKey* key) { \
    fake_tGswSymEncryptInt(result, message, alpha, key); \
    }

    inline void
    fake_tGswSymDecrypt(IntPolynomial *result, const TGswSample *sample, const TGswKey *key, const int32_t Msize) {
        intPolynomialCopy(result, fake(sample)->message);
    }

#define USE_FAKE_tGswSymDecrypt \
    inline void tGswSymDecrypt(IntPolynomial* result, const TGswSample* sample, const TGswKey* key, const int32_t Msize) { \
    fake_tGswSymDecrypt(result, sample, key, Msize); \
    }

    //EXPORT int32_t tGswSymDecryptInt(const TGswSample* sample, const TGswKey* key); 
    //do we really decrypt Gsw samples?

    // support Functions for TGsw
    // Result = 0
    inline void fake_tGswClear(TGswSample *result, const TGswParams *params) {
        FakeTGsw *fres = fake(result);
        intPolynomialClear(fres->message);
        fres->current_variance = 0.;
    }

#define USE_FAKE_tGswClear \
    inline void tGswClear(TGswSample* result, const TGswParams* params) { \
    fake_tGswClear(result, params); \
    }

    // Result += H
    inline void fake_tGswAddH(TGswSample *result, const TGswParams *params) {
        FakeTGsw *fres = fake(result);
        fres->message->coefs[0] += 1;
    }

#define USE_FAKE_tGswAddH \
    inline void tGswAddH(TGswSample* result, const TGswParams* params) { \
    return fake_tGswAddH(result,params); \
    }

    // Result += mu*H
    inline void fake_tGswAddMuH(TGswSample *result, const IntPolynomial *message, const TGswParams *params) {
        FakeTGsw *fres = fake(result);
        intPolynomialAddTo(fres->message, message);
    }

#define USE_FAKE_tGswAddMuH \
    inline void tGswAddMuH(TGswSample* result, const IntPolynomial* message, const TGswParams* params) { \
    return fake_tGswAddMuH(result,message, params); \
    }


    // Result += mu*H, mu integer
    inline void fake_tGswAddMuIntH(TGswSample *result, const int32_t message, const TGswParams *params) {
        FakeTGsw *fres = fake(result);
        fres->message->coefs[0] += message;
    }

#define USE_FAKE_tGswAddMuIntH \
    inline void tGswAddMuIntH(TGswSample* result, const int32_t message, const TGswParams* params) { \
    return fake_tGswAddMuIntH(result,message, params); \
    }

    // Result = tGsw(0)
    inline void fake_tGswEncryptZero(TGswSample *result, double alpha, const TGswKey *key) {
        FakeTGsw *fres = fake(result);
        intPolynomialClear(fres->message);
        fres->current_variance = alpha * alpha;
    }

#define USE_FAKE_tGswEncryptZero \
    inline void tGswEncryptZero(TGswSample* result, double alpha, const TGswKey* key) { \
    fake_tGswEncryptZero(result, alpha, key); \
    }

    //fonction de decomposition
    //EXPORT void tGswTLweDecompH(IntPolynomial* result, const TLweSample* sample, const TGswParams* params);
    //EXPORT void tGswTorus32PolynomialDecompH(IntPolynomial* result, const TorusPolynomial* sample, const TGswParams* params);
    //EXPORT void tGswTLweDecompH(IntPolynomial* result, const TLweSample* sample,const TGswParams* params);	

    // result= (X^ai-1)*bk (ligne 5 de l'algo)
    inline void fake_tGswMulByXaiMinusOne(TGswSample *result, int32_t ai, const TGswSample *bk, const TGswParams *params) {
        FakeTGsw *fres = fake(result);
        const FakeTGsw *fbk = fake(bk);
        intPolynomialMulByXaiMinusOne(fres->message, ai, fbk->message);
        fres->current_variance = fbk->current_variance * 2;
    }

#define USE_FAKE_tGswMulByXaiMinusOne \
    inline void tGswMulByXaiMinusOne(TGswSample* result, int32_t ai, const TGswSample* bk, const TGswParams* params) { \
    return fake_tGswMulByXaiMinusOne(result,ai,bk,params); \
    }

    //ligne 5 algo,mult externe
    inline void fake_tGswExternMulToTLwe(TLweSample *accum, const TGswSample *sample, const TGswParams *params) {
        const int32_t N = params->tlwe_params->N;
        const FakeTGsw *fgsw = fake(sample);
        const FakeTLwe *faccum = fake(accum);
        TorusPolynomial *tmp = new_TorusPolynomial(N);
        torusPolynomialMultKaratsuba(tmp, fgsw->message, faccum->message);
        torusPolynomialCopy(faccum->message, tmp);
        delete_TorusPolynomial(tmp);
    }

#define USE_FAKE_tGswExternMulToTLwe \
    inline void tGswExternMulToTLwe(TLweSample* accum, const TGswSample* sample,const TGswParams* params) { \
    return fake_tGswExternMulToTLwe(accum,sample,params); \
    }


    /** result = result + sample */
    inline void fake_tGswAddTo(TGswSample *result, const TGswSample *sample, const TGswParams *params) {
        FakeTGsw *fres = fake(result);
        const FakeTGsw *fsample = fake(sample);
        intPolynomialAddTo(fres->message, fsample->message);
        fres->current_variance += fsample->current_variance;
    }

    /** result = result - sample */
    //EXPORT void tGswSubTo(TLweSample* result, const TLweSample* sample, const TLweParams* params);
    /** result = result + p.sample */
    //EXPORT void tGswAddMulTo(TLweSample* result, int32_t p, const TLweSample* sample, const TLweParams* params);
    /** result = result - p.sample */
    //EXPORT void tGswSubMulTo(TLweSample* result, int32_t p, const TLweSample* sample, const TLweParams* params);

    EXPORT void tfhe_createLweBootstrappingKey(LweBootstrappingKey *bk, const LweKey *key_in, const TGswKey *rgsw_key);
    EXPORT void tfhe_bootstrap(LweSample *result, const LweBootstrappingKey *bk, Torus32 mu, const LweSample *x);

}

#endif// FAKES_TGSW_H
