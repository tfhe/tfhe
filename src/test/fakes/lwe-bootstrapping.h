#ifndef FAKE_LWE_BOOTSTRAPPING_H
#define FAKE_LWE_BOOTSTRAPPING_H

#include <tfhe.h>
#include "./lwe-keyswitch.h"
#include "./tgsw.h"


namespace {

    struct FakeLweBootstrappingKey {
        const LweParams *in_out_params; ///< paramètre de l'input et de l'output. key: s
        const TGswParams *bk_params; ///< params of the Gsw elems in bk. key: s"
        const TLweParams *accum_params; ///< params of the accum variable key: s"
        const LweParams *extract_params; ///< params after extraction: key: s'
        TGswSample *bk; ///< the bootstrapping key (s->s")
        LweKeySwitchKey *ks; ///< the keyswitch key (s'->s)

        FakeLweBootstrappingKey(int32_t ks_t, int32_t ks_basebit, const LweParams *in_out_params, const TGswParams *bk_params) {
            this->in_out_params = in_out_params;
            this->bk_params = bk_params;
            this->accum_params = bk_params->tlwe_params;
            this->extract_params = &accum_params->extracted_lweparams;

            const int32_t n = in_out_params->n;

            this->bk = fake_new_TGswSample_array(n, this->bk_params);
            this->ks = fake_new_LweKeySwitchKey(extract_params->n, ks_t, ks_basebit, in_out_params);
        }

        ~FakeLweBootstrappingKey() {
            fake_delete_LweKeySwitchKey(ks);
            fake_delete_TGswSample_array(in_out_params->n, bk);
        }

        FakeLweBootstrappingKey(const LweBootstrappingKey &) = delete;

        void operator=(const FakeLweBootstrappingKey &) = delete;
    };


    FakeLweBootstrappingKey *fake(LweBootstrappingKey *key) {
        FakeLweBootstrappingKey *fkey = (FakeLweBootstrappingKey *) key;
        fake(fkey->bk); //test if bk is a fake
        return fkey;
    }

    const FakeLweBootstrappingKey *fake(const LweBootstrappingKey *key) {
        const FakeLweBootstrappingKey *fkey = (const FakeLweBootstrappingKey *) key;
        fake(fkey->bk); //test if bk is a fake
        return fkey;
    }


    inline LweBootstrappingKey *fake_new_LweBootstrappingKey(int32_t ks_t, int32_t ks_basebit, const LweParams *in_out_params,
                                                             const TGswParams *bk_params) {
        return (LweBootstrappingKey *) new FakeLweBootstrappingKey(ks_t, ks_basebit, in_out_params, bk_params);
    }

    inline void fake_delete_LweBootstrappingKey(LweBootstrappingKey *bk) {
        FakeLweBootstrappingKey *fbk = fake(bk);
        delete fbk;
    }


    inline void fake_tfhe_createLweBootstrappingKey(
            LweBootstrappingKey *bk,
            const LweKey *key_in,
            const TGswKey *rgsw_key) {
        FakeLweBootstrappingKey *fbk = fake(bk);
        assert(fbk->bk_params == rgsw_key->params);
        assert(fbk->in_out_params == key_in->params);

        const LweParams *in_out_params = fbk->in_out_params;
        const TGswParams *bk_params = fbk->bk_params;
        const TLweParams *accum_params = bk_params->tlwe_params;
        const LweParams *extract_params = &accum_params->extracted_lweparams;

        //LweKeySwitchKey* ks; ///< the keyswitch key (s'->s)
        const TLweKey *accum_key = &rgsw_key->tlwe_key;
        LweKey *extracted_key = new_LweKey(extract_params);
        tLweExtractKey(extracted_key, accum_key);
        fake_lweCreateKeySwitchKey(bk->ks, extracted_key, key_in); //TODO: fake
        delete_LweKey(extracted_key);

        //TGswSample* bk; ///< the bootstrapping key (s->s")
        int32_t *kin = key_in->key;
        const double alpha = accum_params->alpha_min;
        const int32_t n = in_out_params->n;
        //const int32_t kpl = bk_params->kpl;
        //const int32_t k = accum_params->k;
        //const int32_t N = accum_params->N;
        for (int32_t i = 0; i < n; i++) {
            fake_tGswSymEncryptInt(&bk->bk[i], kin[i], alpha, rgsw_key);
        }
    }


#define USE_FAKE_tfhe_createLweBootstrappingKey \
    inline void tfhe_createLweBootstrappingKey(LweBootstrappingKey* bk, const LweKey* key_in, const TGswKey* rgsw_key) { \
        fake_tfhe_createLweBootstrappingKey(bk,key_in,rgsw_key); \
    }


/**
 * multiply the accumulator by X^sum(bara_i.s_i)
 * @param accum the (fake) TLWE sample to multiply
 * @param bk An array of n (fakes) TGSW samples where bk_i encodes s_i
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
    inline void fake_tfhe_blindRotate(TLweSample *accum, const TGswSample *bk, const int32_t *bara, const int32_t n,
                                      const TGswParams *bk_params) {

        FakeTLwe *facc = fake(accum);
        TorusPolynomial *temp = new_TorusPolynomial(bk_params->tlwe_params->N);

        int32_t offset = 0;
        for (int32_t i = 0; i < n; i++) {
            const int32_t si = fake(bk + i)->message->coefs[0];
            const int32_t barai = bara[i];
            if (barai == 0 || si == 0) continue; //indeed, this is an easy case!
            offset = (offset + barai * si) % (2 * bk_params->tlwe_params->N); // sum_{i=...n-1} barai*si mod 2N
        }

        torusPolynomialCopy(temp, facc->message);
        torusPolynomialMulByXai(facc->message, offset, temp);
        //TODO update the variance

        delete_TorusPolynomial(temp);
    }

#define USE_FAKE_tfhe_blindRotate \
    inline void tfhe_blindRotate(TLweSample* accum, \
        const TGswSample* bk, \
        const int32_t* bara, \
        const int32_t n, \
        const TGswParams* bk_params) { \
    fake_tfhe_blindRotate(accum,bk,bara,n,bk_params); \
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
    inline void fake_tfhe_blindRotateAndExtract(LweSample *result,
                                                const TorusPolynomial *v,
                                                const TGswSample *bk,
                                                const int32_t barb,
                                                const int32_t *bara,
                                                const int32_t n,
                                                const TGswParams *bk_params) {

        FakeLwe *fres = fake(result);

        const int32_t N = bk_params->tlwe_params->N;
        const int32_t _2N = 2 * N;

        // or maybe use the fake_tfhe_blindRotate
        int32_t offset = barb;
        for (int32_t i = 0; i < n; i++) {
            int32_t si = fake(bk + i)->message->coefs[0];
            offset = (offset + _2N - si * bara[i]) % (_2N);
        }

        fres->message = (offset < N) ? (v->coefsT[offset]) : (-v->coefsT[offset - N]);
        fres->current_variance = 0; //TODO variance
    }

#define USE_FAKE_tfhe_blindRotateAndExtract \
    inline void tfhe_blindRotateAndExtract(LweSample* result, \
        const TorusPolynomial* v, \
        const TGswSample* bk, \
        const int32_t barb, \
        const int32_t* bara, \
        const int32_t n, \
        const TGswParams* bk_params) { \
    fake_tfhe_blindRotateAndExtract(result,v,bk,barb,bara,n,bk_params); \
    }


/**
 * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
 * @param result The resulting LweSample
 * @param bk The bootstrapping + keyswitch key
 * @param mu The output message (if phase(x)>0)
 * @param x The input sample
 */
    inline void fake_tfhe_bootstrap_woKS(LweSample *result,
                                         const LweBootstrappingKey *bk,
                                         Torus32 mu, const LweSample *x) {

        FakeLwe *fres = fake(result);
        const FakeLwe *fx = fake(x);
        if (fx->message >= 0)
            fres->message = mu;
        else
            fres->message = -mu;
    }

//TODO: parallelization
#define USE_FAKE_tfhe_bootstrap_woKS \
    static inline void tfhe_bootstrap_woKS(LweSample *result, const LweBootstrappingKey *bk, Torus32 mu, const LweSample *x) {\
    fake_tfhe_bootstrap_woKS(result, bk, mu, x); \
    }


/**
 * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
 * @param result The resulting LweSample
 * @param bk The bootstrapping + keyswitch key
 * @param mu The output message (if phase(x)>0)
 * @param x The input sample
 */
    inline void fake_tfhe_bootstrap(LweSample *result,
                                    const LweBootstrappingKey *bk,
                                    Torus32 mu, const LweSample *x) {

        FakeLwe *fres = fake(result);
        const FakeLwe *fx = fake(x);
        if (fx->message >= 0)
            fres->message = mu;
        else
            fres->message = -mu;
    }

//TODO: parallelization
#define USE_FAKE_tfhe_bootstrap \
static inline void tfhe_bootstrap(LweSample *result, const LweBootstrappingKey *bk, Torus32 mu, const LweSample *x) { \
    fake_tfhe_bootstrap(result, bk, mu, x); \
}


/**
 * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
 * @param result The resulting LweSample
 * @param bk The bootstrapping + keyswitch key
 * @param mu The output message (if phase(x)>0)
 * @param x The input sample
 */
    inline void old_fake_tfhe_bootstrap(LweSample *result,
                                        const LweBootstrappingKey *bk,
                                        Torus32 mu, const LweSample *x) {

        //FakeLwe* fres = fake(result); // the fake will be created in the fake_tfhe_blindRotateAndExtract
        const FakeLweBootstrappingKey *fbk = fake(bk);

        const TGswParams *bk_params = fbk->bk_params;
        const TLweParams *accum_params = fbk->accum_params;
        const LweParams *in_params = fbk->in_out_params;
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

        //fake_tfhe_blindRotateAndExtract(u, testvect, bk->bk, barb, bara, n, bk_params);
        fake_tfhe_blindRotateAndExtract(result, testvect, bk->bk, barb, bara, n, bk_params);
        // lweKeySwitch(result, bk->ks, u);
        fake_lweKeySwitch(result, bk->ks, result);

        //delete_LweSample(u);
        delete[] bara;
        delete_TorusPolynomial(testvect);
    }

#define USE_OLD_FAKE_tfhe_bootstrap \
inline void old_tfhe_bootstrap(LweSample* result, const LweBootstrappingKey* bk, Torus32 mu, const LweSample* x) { \
    old_fake_tfhe_bootstrap(result,bk,mu,x); \
}


} //namespace

#endif // FAKE_LWE_BOOTSTRAPPING_H
