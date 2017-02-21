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







namespace {

    // Fake TLWE structure 
    struct FakeTLwe {
    TorusPolynomial* message;
    double current_variance;

    // construct
    FakeTLwe(int N) {
        message = new_TorusPolynomial(N);
        current_variance = 0.;
    }

    // delete
    ~FakeTLwe() {
        delete_TorusPolynomial(message);
    }
    FakeTLwe(const FakeTLwe&)=delete;
    void operator=(const FakeTLwe&)=delete;
    };


    
    // Fake new TLWE sample
    inline TLweSample* fake_new_TLweSample(const TLweParams* params) {
    int N = params->N;
    return (TLweSample*) new FakeTLwe(N);
    }



    // Use fake TLWE
    #define USE_FAKE_new_TLweSample \
    inline TLweSample* new_TLweSample(const TLweParams* params) { \
        return fake_new_TLweSample(params); \
    }

    #define USE_FAKE_delete_TLweSample \
    inline void delete_TLweSample(TLweSample* sample) { \
    fake_delete_TLweSample(sample); \
    }
    


/*
    // Fake symetric encryption of a Torus message (attention car la structure originelle est avec un message poly)
    inline void fake_tLweSymEncryptT(TLweSample* result, const Torus32 message, double alpha, const TLweKey* key) {
        tLweNoiselessTrivialT(result,message,key->params);
        result->current_variance=alpha*alpha;
    }

    // Fake symetric encryption of a Torus polynomial message
    inline void fake_tLweSymEncrypt(TLweSample* result, TorusPolynomial* mu, double alpha, const TLweKey* key) {
        tLweNoiselessTrivial(result,message,key->params);
        result->current_variance=alpha*alpha;
    }
*/







    inline void fake_tGswSymEncrypt(TGswSample* result, const IntPolynomial* message, double alpha, const TGswKey* key) {
    FakeTGsw* fres = fake(result);
    intPolynomialCopy(fres->message, message);
    fres->current_variance=alpha;
    }

#define USE_FAKE_tGswSymEncrypt \
    inline void tGswSymEncrypt(TGswSample* result, const IntPolynomial* message, double alpha, const TGswKey* key) { \
    fake_tGswSymEncrypt(result, message, alpha, key); \
    }

    inline void fake_tGswSymEncryptInt(TGswSample* result, const int message, double alpha, const TGswKey* key) {
    FakeTGsw* fres = fake(result);
    intPolynomialClear(fres->message);
    fres->message->coefs[0]=message;
    fres->current_variance=alpha;
    }

#define USE_FAKE_tGswSymEncryptInt \
    inline void tGswSymEncryptInt(TGswSample* result, const int message, double alpha, const TGswKey* key) { \
    fake_tGswSymEncryptInt(result, message, alpha, key); \
    }

    inline void fake_tGswSymDecrypt(IntPolynomial* result, const TGswSample* sample, const TGswKey* key, const int Msize) {
    intPolynomialCopy(result, fake(sample)->message);
    }

#define USE_FAKE_tGswSymDecrypt \
    inline void tGswSymDecrypt(IntPolynomial* result, const TGswSample* sample, const TGswKey* key, const int Msize) { \
    fake_tGswSymDecrypt(result, sample, key, Msize); \
    }

    //EXPORT int tGswSymDecryptInt(const TGswSample* sample, const TGswKey* key); 
    //do we really decrypt Gsw samples?

    // support Functions for TGsw
    // Result = 0
    inline void fake_tGswClear(TGswSample* result, const TGswParams* params) {
    FakeTGsw* fres = fake(result);
    intPolynomialClear(fres->message);
    fres->current_variance=0.;
    }

#define USE_FAKE_tGswClear \
    inline void tGswClear(TGswSample* result, const TGswParams* params) { \
    fake_tGswClear(result, params); \
    }

    // Result += H
    inline void fake_tGswAddH(TGswSample* result, const TGswParams* params) {
    FakeTGsw* fres = fake(result);
    fres->message->coefs[0]+=1;
    }

#define USE_FAKE_tGswAddH \
    inline void tGswAddH(TGswSample* result, const TGswParams* params) { \
    return fake_tGswAddH(result,params); \
    }

    // Result += mu*H
    inline void fake_tGswAddMuH(TGswSample* result, const IntPolynomial* message, const TGswParams* params) {
    FakeTGsw* fres = fake(result);
    intPolynomialAddTo(fres->message,message);
    }


    // Result += mu*H, mu integer
    inline void fake_tGswAddMuIntH(TGswSample* result, const int message, const TGswParams* params) {
    FakeTGsw* fres = fake(result);
    fres->message->coefs[0]+=message;
    }

    // Result = tGsw(0)
    inline void fake_tGswEncryptZero(TGswSample* result, double alpha, const TGswKey* key) {
    FakeTGsw* fres = fake(result);
    intPolynomialClear(fres->message);
    fres->current_variance=alpha;
    }

    //fonction de decomposition
    //EXPORT void tGswTLweDecompH(IntPolynomial* result, const TLweSample* sample, const TGswParams* params);
    //EXPORT void tGswTorus32PolynomialDecompH(IntPolynomial* result, const TorusPolynomial* sample, const TGswParams* params);
    //EXPORT void tGswTLweDecompH(IntPolynomial* result, const TLweSample* sample,const TGswParams* params);    

    // result= (X^ai-1)*bk (ligne 5 de l'algo)
    inline void fake_tGswMulByXaiMinusOne(TGswSample* result, int ai, const TGswSample* bk, const TGswParams* params) {
    FakeTGsw* fres = fake(result);
    const FakeTGsw* fbk = fake(bk);
    intPolynomialMulByXaiMinusOne(fres->message,ai,fbk->message);
    fres->current_variance=fbk->current_variance*2;
    }

#define USE_FAKE_tGswMulByXaiMinusOne \
    inline void tGswMulByXaiMinusOne(TGswSample* result, int ai, const TGswSample* bk, const TGswParams* params) { \
    return fake_tGswMulByXaiMinusOne(result,ai,bk,params); \
    }

    inline void fake_tLweMulRTo(TLweSample* result, const IntPolynomial* u, const TLweParams* params) {
    torusPolynomialCopy(result->a,result->b);
    torusPolynomialMultNaive(result->b,u,result->a);
    torusPolynomialClear(result->a);
    result->current_variance *= intPolynomialNormSq2(u);
    }

    //ligne 5 algo,mult externe
    inline void fake_tGswExternMulToTLwe(TLweSample* accum, const TGswSample* sample,const TGswParams* params) {
    const FakeTGsw* fsample = fake(sample);
    fake_tLweMulRTo(accum,fsample->message, params->tlwe_params);
    //TODO: variance 
    }

#define USE_FAKE_tGswExternMulToTLwe \
    inline void tGswExternMulToTLwe(TLweSample* accum, const TGswSample* sample,const TGswParams* params) { \
    return fake_tGswExternMulToTLwe(accum,sample,params); \
    }


    /** result = result + sample */
    inline void fake_tGswAddTo(TGswSample* result, const TGswSample* sample, const TGswParams* params) {
    FakeTGsw* fres = fake(result);
    const FakeTGsw* fsample = fake(sample);
    intPolynomialAddTo(fres->message, fsample->message);
    fres->current_variance += fsample->current_variance;
    }

    /** result = result - sample */
    //EXPORT void tGswSubTo(TLweSample* result, const TLweSample* sample, const TLweParams* params);
    /** result = result + p.sample */
    //EXPORT void tGswAddMulTo(TLweSample* result, int p, const TLweSample* sample, const TLweParams* params);
    /** result = result - p.sample */
    //EXPORT void tGswSubMulTo(TLweSample* result, int p, const TLweSample* sample, const TLweParams* params);

    EXPORT void tfhe_createLweBootstrappingKey(LweBootstrappingKey* bk, const LweKey* key_in, const TGswKey* rgsw_key);
    EXPORT void tfhe_bootstrap(LweSample* result, const LweBootstrappingKey* bk, Torus32 mu, const LweSample* x);

}














/*

//struct TLweKey {
//    const TLweParams* params;
//    IntPolynomial* key;
//};

TLweKey::TLweKey(const TLweParams* params):
    params(params)
{
    key = new_IntPolynomial_array(params->k,params->N);   
}

TLweKey::~TLweKey() {
    delete_IntPolynomial_array(params->k,key);
}






TLweSampleFFT::TLweSampleFFT(const TLweParams* params): k(params->k) {
    //a is a table of k+1 polynomials, b is an alias for &a[k]
    a = new_LagrangeHalfCPolynomial_array(k+1, params->N);
    b = a+k;
    current_variance = 0;
}

TLweSampleFFT::~TLweSampleFFT() {
    delete_LagrangeHalfCPolynomial_array(k+1, a);
}

*/