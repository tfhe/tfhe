#ifndef FAKES_TLWE_FFT_H
#define FAKES_TLWE_FFT_H

#include "./tlwe.h"
#include "./lagrangehalfc.h"

namespace {

    // Fake TLWE structure 
    struct FakeTLweFFT {
        //TODO: parallelization
        static const int32_t FAKE_TYPE_UID = 816458235; // precaution: distinguish fakes from trues
        const int32_t fake_uid;
        TorusPolynomial *message;
        double current_variance;

        //this padding is here to make sure that FakeTLwe and TLweSample have the same size
        char unused_padding[sizeof(TLweSampleFFT) - sizeof(int64_t) - sizeof(TorusPolynomial *) - sizeof(double)];

        // construct
        FakeTLweFFT(int32_t N) : fake_uid(FAKE_TYPE_UID) {
            message = new_TorusPolynomial(N);
            current_variance = 0.;
        }

        // delete
        ~FakeTLweFFT() {
            if (fake_uid != FAKE_TYPE_UID) abort();
            delete_TorusPolynomial(message);
        }

        FakeTLweFFT(const FakeTLweFFT &) = delete;

        void operator=(const FakeTLweFFT &)= delete;
    };



    // At compile time, we verify that the two structures have exactly the same size
    //TODO: parallelization
    static_assert(sizeof(FakeTLweFFT) == sizeof(TLweSampleFFT), "Error: Size is not correct");

    // fake functons 
    inline FakeTLweFFT *fake(TLweSampleFFT *sample) {
        FakeTLweFFT *reps = (FakeTLweFFT *) sample;
        if (reps->fake_uid != FakeTLweFFT::FAKE_TYPE_UID) abort();
        return reps;
    }

    inline const FakeTLweFFT *fake(const TLweSampleFFT *sample) {
        const FakeTLweFFT *reps = (const FakeTLweFFT *) sample;
        if (reps->fake_uid != FakeTLweFFT::FAKE_TYPE_UID) abort();
        return reps;
    }


    //-----------------------------------------
    //  constructor/destructor
    //-----------------------------------------


    inline TLweSampleFFT *fake_new_TLweSampleFFT_array(int32_t nbelts, const TLweParams *params) {
        int32_t N = params->N;
        FakeTLweFFT *reps = (FakeTLweFFT *) malloc(sizeof(FakeTLweFFT) * nbelts);
        for (int32_t i = 0; i < nbelts; i++) new(reps + i) FakeTLweFFT(N);
        return (TLweSampleFFT *) reps;
    }


#define USE_FAKE_new_TLweSampleFFT_array \
    inline TLweSampleFFT* new_TLweSampleFFT_array(int32_t nbelts, const TLweParams* params) { \
    return fake_new_TLweSampleFFT_array(nbelts,params); \
    }

    inline void fake_delete_TLweSampleFFT_array(int32_t nbelts, TLweSampleFFT *sample) {
        FakeTLweFFT *arr = fake(sample);
        for (int32_t i = 0; i < nbelts; i++) (arr + i)->~FakeTLweFFT();
        free(arr);
    }

    // 
#define USE_FAKE_delete_TLweSampleFFT_array \
    inline void delete_TLweSampleFFT_array(int32_t nbelts, TLweSampleFFT* samples) { \
    fake_delete_TLweSampleFFT_array(nbelts,samples); \
    }


    inline TLweSampleFFT *fake_new_TLweSampleFFT(const TLweParams *params) {
        int32_t N = params->N;
        FakeTLweFFT *reps = (FakeTLweFFT *) malloc(sizeof(FakeTLweFFT));
        new(reps) FakeTLweFFT(N);
        return (TLweSampleFFT *) reps;
    }

    // 
#define USE_FAKE_new_TLweSampleFFT \
    inline TLweSampleFFT* new_TLweSampleFFT(const TLweParams* params) { \
    return fake_new_TLweSampleFFT(params); \
    }

    inline void fake_delete_TLweSampleFFT(TLweSampleFFT *sample) {
        FakeTLweFFT *ptr = fake(sample);
        (ptr)->~FakeTLweFFT();
        free(ptr);
    }

    // 
#define USE_FAKE_delete_TLweSampleFFT \
    inline void delete_TLweSampleFFT(TLweSampleFFT* sample) { \
    fake_delete_TLweSampleFFT(sample); \
    }


    //-----------------------------------------------
    //  functions
    //-----------------------------------------------


    // Computes the inverse FFT of the coefficients of the TLWE sample
    inline void fake_tLweToFFTConvert(TLweSampleFFT *result, const TLweSample *source, const TLweParams *params) {
        const FakeTLwe *fs = fake(source);
        FakeTLweFFT *fres = fake(result);

        torusPolynomialCopy(fres->message, fs->message);
        fres->current_variance = fs->current_variance;
    }

#define USE_FAKE_tLweToFFTConvert \
    inline void tLweToFFTConvert(TLweSampleFFT* result, const TLweSample* source, const TLweParams* params){ \
    fake_tLweToFFTConvert(result, source, params); \
    }


    // Computes the FFT of the coefficients of the TLWEfft sample
    inline void fake_tLweFromFFTConvert(TLweSample *result, const TLweSampleFFT *source, const TLweParams *params) {
        const FakeTLweFFT *fs = fake(source);
        FakeTLwe *fres = fake(result);

        torusPolynomialCopy(fres->message, fs->message);
        fres->current_variance = fs->current_variance;
    }


#define  USE_FAKE_tLweFromFFTConvert \
    inline void tLweFromFFTConvert(TLweSample* result, const TLweSampleFFT* source, const TLweParams* params) { \
    fake_tLweFromFFTConvert(result, source, params); \
    }

    //Arithmetic operations on TLwe samples
    /** result = (0,0) */
    inline void fake_tLweFFTClear(TLweSampleFFT *result, const TLweParams *params) {
        FakeTLweFFT *fres = fake(result);

        torusPolynomialClear(fres->message);
        fres->current_variance = 0;
    }

#define USE_FAKE_tLweFFTClear \
    inline void tLweFFTClear(TLweSampleFFT* result, const TLweParams* params) { \
    fake_tLweFFTClear(result, params); \
    }


    // result = result + p*sample
    inline void
    fake_tLweFFTAddMulRTo(TLweSampleFFT *result, const LagrangeHalfCPolynomial *p, const TLweSampleFFT *sample,
                          const TLweParams *params) {
        FakeTLweFFT *fres = fake(result);
        const FakeTLweFFT *fsample = fake(sample);
        const FakeLagrangeHalfCPolynomial *fp = fake(p);

        const IntPolynomial *pval = fp->getIntPolynomialPtr();
        torusPolynomialAddMulRKaratsuba(fres->message, pval, fsample->message);
        fres->current_variance += fsample->current_variance * intPolynomialNormSq2(pval);
    }

#define USE_FAKE_tLweFFTAddMulRTo \
    inline void tLweFFTAddMulRTo(TLweSampleFFT* result, const LagrangeHalfCPolynomial* p, const TLweSampleFFT* sample, const TLweParams* params) { \
    fake_tLweFFTAddMulRTo(result, p, sample, params); \
    }


} //end namespace

#endif // FAKES_TLWE_FFT_H
