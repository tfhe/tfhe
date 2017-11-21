#ifndef FAKE_LWE_BOOTSTRAPPING_FFT_H
#define FAKE_LWE_BOOTSTRAPPING_FFT_H

#include <tfhe.h>
#include "./lwe-keyswitch.h"
#include "./lwe-bootstrapping.h"
#include "./tgsw.h"
#include "./tgsw-fft.h"


namespace {

    struct FakeLweBootstrappingKeyFFT {
        const LweParams *in_out_params; ///< paramètre de l'input et de l'output. key: s
        const TGswParams *bk_params; ///< params of the Gsw elems in bk. key: s"
        const TLweParams *accum_params; ///< params of the accum variable key: s"
        const LweParams *extract_params; ///< params after extraction: key: s'
        TGswSampleFFT *bkFFT; ///< the bootstrapping key FFT (s->s")
        LweKeySwitchKey *ks; ///< the keyswitch key (s'->s)

        FakeLweBootstrappingKeyFFT(const FakeLweBootstrappingKey *fbk) {
            this->in_out_params = fbk->in_out_params;
            this->bk_params = fbk->bk_params;
            this->accum_params = bk_params->tlwe_params;
            this->extract_params = &accum_params->extracted_lweparams;

            const int32_t n = in_out_params->n;
            const int32_t kslength = 15;
            const int32_t basebit = 2;

            this->bkFFT = fake_new_TGswSampleFFT_array(n, this->bk_params);
            for (int32_t i = 0; i < n; ++i) {
                fake_tGswToFFTConvert(bkFFT + i, fbk->bk + i, bk_params);
            }

            this->ks = fake_new_LweKeySwitchKey(n, kslength, basebit, in_out_params);
            FakeLweKeySwitchKey *fks = fake(ks);
            fks->variance_overhead = fake(fbk->ks)->variance_overhead;

        }

        ~FakeLweBootstrappingKeyFFT() {
            fake_delete_LweKeySwitchKey(ks);
            fake_delete_TGswSampleFFT_array(in_out_params->n, bkFFT);
        }

        FakeLweBootstrappingKeyFFT(const LweBootstrappingKeyFFT &) = delete;

        void operator=(const FakeLweBootstrappingKeyFFT &) = delete;
    };


    FakeLweBootstrappingKeyFFT *fake(LweBootstrappingKeyFFT *key) {
        FakeLweBootstrappingKeyFFT *fkey = (FakeLweBootstrappingKeyFFT *) key;
        fake(fkey->bkFFT); //test if bkFFT is a fake
        return fkey;
    }

    const FakeLweBootstrappingKeyFFT *fake(const LweBootstrappingKeyFFT *key) {
        const FakeLweBootstrappingKeyFFT *fkey = (const FakeLweBootstrappingKeyFFT *) key;
        fake(fkey->bkFFT); //test if bk is a fake
        return fkey;
    }


    inline LweBootstrappingKeyFFT *fake_new_LweBootstrappingKeyFFT(const LweBootstrappingKey *bk) {
        const FakeLweBootstrappingKey *fbk = fake(bk);
        return (LweBootstrappingKeyFFT *) new FakeLweBootstrappingKeyFFT(fbk);
    }

    inline void fake_delete_LweBootstrappingKeyFFT(LweBootstrappingKeyFFT *bkFFT) {
        FakeLweBootstrappingKeyFFT *fbkFFT = fake(bkFFT);
        delete fbkFFT;
    }


/*
inline void fake_tfhe_createLweBootstrappingKeyFFT(LweBootstrappingKeyFFT* bkFFT, const LweBootstrappingKey* bk) {
    FakeLweBootstrappingKeyFFT* fbkFFT = fake(bkFFT);
    const FakeLweBootstrappingKey* fbk = fake(bk);
    FakeLweKeySwitchKey* fksFFT = fake(bkFFT->ks);
    const FakeLweKeySwitchKey* fks = fake(bk->ks);
    
    assert(fbkFFT->bk_params == fbk->bk_params);
    assert(fbkFFT->in_out_params == fbk->in_out_params);

    const LweParams* in_out_params = fbkFFT->in_out_params; 
    const int32_t n = in_out_params->n;
    const TGswParams* bk_params = fbkFFT->bk_params;

    // KeySwitching key
    fksFFT->variance_overhead = fks->variance_overhead;

    // Bootstrapping Key FFT
    for (int32_t i=0; i<n; ++i) {
        fake_tGswToFFTConvert(&fbkFFT->bkFFT[i], &fbk->bk[i], bk_params);
    }
}
*/

/**
 * multiply the accumulator by X^sum(bara_i.s_i)
 * @param accum the (fake) TLWE sample to multiply
 * @param bk An array of n (fakes) TGSW samples where bk_i encodes s_i
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
    inline void fake_tfhe_blindRotate_FFT(TLweSample *accum, const TGswSampleFFT *bkFFT, const int32_t *bara, const int32_t n,
                                          const TGswParams *bk_params) {

        FakeTLwe *facc = fake(accum);
        TorusPolynomial *temp = new_TorusPolynomial(bk_params->tlwe_params->N);

        int32_t offset = 0;
        for (int32_t i = 0; i < n; i++) {
            const int32_t si = fake(bkFFT + i)->message->coefs[0];
            const int32_t barai = bara[i];
            if (barai == 0 || si == 0) continue; //indeed, this is an easy case!
            offset = (offset + barai * si) % (2 * bk_params->tlwe_params->N); // sum_{i=...n-1} barai*si mod 2N
        }

        torusPolynomialCopy(temp, facc->message);
        torusPolynomialMulByXai(facc->message, offset, temp);
        //TODO update the variance

        delete_TorusPolynomial(temp);
    }

#define USE_FAKE_tfhe_blindRotate_FFT \
    inline void tfhe_blindRotate_FFT(TLweSample* accum, \
        const TGswSampleFFT* bkFFT, \
        const int32_t* bara, \
        const int32_t n, \
        const TGswParams* bk_params) { \
    fake_tfhe_blindRotate_FFT(accum,bkFFT,bara,n,bk_params); \
    }

/**
 * result = LWE(v_p) where p=barb-sum(bara_i.s_i) mod 2N
 * @param result the output LWE sample
 * @param v a 2N-elt anticyclic function (represented by a TorusPolynomial)
 * @param bk An array of n TGSW samples where bk_i encodes s_i
 * @param barb A coefficients between 0 and 2N-1
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
    inline void fake_tfhe_blindRotateAndExtract_FFT(LweSample *result,
                                                    const TorusPolynomial *v,
                                                    const TGswSampleFFT *bkFFT,
                                                    const int32_t barb,
                                                    const int32_t *bara,
                                                    const int32_t n,
                                                    const TGswParams *bk_params) {

        FakeLwe *fres = fake(result);

        const int32_t N = bk_params->tlwe_params->N;
        const int32_t _2N = 2 * N;

        // or maybe use the fake_tfhe_blindRotate_FFT
        int32_t offset = barb;
        for (int32_t i = 0; i < n; i++) {
            int32_t si = fake(bkFFT + i)->message->coefs[0];
            offset = (offset + _2N - si * bara[i]) % (_2N);
        }

        fres->message = (offset < N) ? (v->coefsT[offset]) : (-v->coefsT[offset - N]);
        fres->current_variance = 0; //TODO variance
    }

#define USE_FAKE_tfhe_blindRotateAndExtract_FFT \
    inline void tfhe_blindRotateAndExtract_FFT(LweSample* result, \
        const TorusPolynomial* v, \
        const TGswSampleFFT* bkFFT, \
        const int32_t barb, \
        const int32_t* bara, \
        const int32_t n, \
        const TGswParams* bk_params) { \
    fake_tfhe_blindRotateAndExtract_FFT(result,v,bkFFT,barb,bara,n,bk_params); \
    }


/**
 * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
 * @param result The resulting LweSample
 * @param bk The bootstrapping + keyswitch key
 * @param mu The output message (if phase(x)>0)
 * @param x The input sample
 */
    inline void fake_tfhe_bootstrap_woKS_FFT(LweSample *result, const LweBootstrappingKeyFFT *bkFFT,
                                             Torus32 mu, const LweSample *x) {

        FakeLwe *fres = fake(result);
        const FakeLwe *fx = fake(x);
        if (fx->message >= 0)
            fres->message = mu;
        else
            fres->message = -mu;
    }

//TODO: parallelization
#define USE_FAKE_tfhe_bootstrap_woKS_FFT \
    static inline void tfhe_bootstrap_woKS_FFT(LweSample *result, const LweBootstrappingKeyFFT *bkFFT, Torus32 mu, const LweSample *x) {\
    fake_tfhe_bootstrap_woKS_FFT(result, bkFFT, mu, x); \
    }


/**
 * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
 * @param result The resulting LweSample
 * @param bk The bootstrapping + keyswitch key
 * @param mu The output message (if phase(x)>0)
 * @param x The input sample
 */
    inline void fake_tfhe_bootstrap_FFT(LweSample *result,
                                        const LweBootstrappingKeyFFT *bkFFT,
                                        Torus32 mu, const LweSample *x) {

        FakeLwe *fres = fake(result);
        const FakeLwe *fx = fake(x);
        if (fx->message >= 0)
            fres->message = mu;
        else
            fres->message = -mu;
    }

//TODO: parallelization
#define USE_FAKE_tfhe_bootstrap_FFT \
    static inline void tfhe_bootstrap_FFT(LweSample *result, const LweBootstrappingKeyFFT *bkFFT, Torus32 mu, const LweSample *x) {\
        fake_tfhe_bootstrap_FFT(result, bkFFT, mu, x); \
    }


/**
 * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
 * @param result The resulting LweSample
 * @param bk The bootstrapping + keyswitch key
 * @param mu The output message (if phase(x)>0)
 * @param x The input sample
 */
    inline void old_fake_tfhe_bootstrap_FFT(LweSample *result,
                                            const LweBootstrappingKeyFFT *bkFFT,
                                            Torus32 mu, const LweSample *x) {

        // FakeLwe* fres = fake(result); // The fake will be created in fake_tfhe_blindRotateAndExtract_FFT
        const FakeLweBootstrappingKeyFFT *fbkFFT = fake(bkFFT);

        const TGswParams *bk_params = fbkFFT->bk_params;
        const TLweParams *accum_params = fbkFFT->accum_params;
        const LweParams *in_params = fbkFFT->in_out_params;
        const int32_t N = accum_params->N;
        const int32_t Nx2 = 2 * N;
        const int32_t n = in_params->n;

        TorusPolynomial *testvect = new_TorusPolynomial(N);
        int32_t *bara = new int32_t[N];
        //LweSample* u = new_LweSample(extract_params);

        int32_t barb = modSwitchFromTorus32(x->b, Nx2);
        for (int32_t i = 0; i < n; i++) {
            bara[i] = modSwitchFromTorus32(x->a[i], Nx2);
        }

        //the initial testvec = [mu,mu,mu,...,mu]
        for (int32_t i = 0; i < N; i++) testvect->coefsT[i] = mu;

        //fake_tfhe_blindRotateAndExtract_FFT(u, testvect, bkFFT->bkFFT, barb, bara, n, bk_params);
        fake_tfhe_blindRotateAndExtract_FFT(result, testvect, bkFFT->bkFFT, barb, bara, n, bk_params);
        // fake_lweKeySwitch(result, bkFFT->ks, u);
        fake_lweKeySwitch(result, bkFFT->ks, result);

        //delete_LweSample(u);
        delete[] bara;
        delete_TorusPolynomial(testvect);
    }

#define USE_OLD_FAKE_tfhe_bootstrap_FFT \
inline void old_tfhe_bootstrap_FFT(LweSample* result, const LweBootstrappingKeyFFT* bkFFT, Torus32 mu, const LweSample* x) { \
    old_fake_tfhe_bootstrap_FFT(result,bkFFT,mu,x); \
}


} // namespace

#endif // FAKE_LWE_BOOTSTRAPPING_FFT_H
