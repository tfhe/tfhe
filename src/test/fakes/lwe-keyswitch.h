#ifndef FAKE_LWE_KEYSWITCH_H
#define FAKE_LWE_KEYSWITCH_H

#include "tfhe.h"
#include "./lwe.h"


namespace {

    struct FakeLweKeySwitchKey {
        //TODO: parallelization
        static const int64_t FAKE_LWEKEYSWITCH_UID = 956475132024584l; // precaution: distinguish fakes from trues
        const int64_t fake_uid;
        const int32_t n;
        const int32_t t;
        const int32_t basebit;
        const int32_t base;
        double variance_overhead;

        //this padding is here to make sure that FakeLweKeySwitchKey and LweKeySwitchKey have the same size
        char unused_padding[sizeof(LweKeySwitchKey) - sizeof(int64_t) - 4 * sizeof(int32_t) - sizeof(double)];

        // construct
        FakeLweKeySwitchKey(int32_t n, int32_t t, int32_t basebit) : fake_uid(FAKE_LWEKEYSWITCH_UID),
                                                         n(n), t(t),
                                                         basebit(basebit),
                                                         base(1 << basebit) {
            variance_overhead = 0.;
        }

        // delete
        ~FakeLweKeySwitchKey() {
            if (fake_uid != FAKE_LWEKEYSWITCH_UID) abort();
        }

        FakeLweKeySwitchKey(const FakeLweKeySwitchKey &) = delete;

        void operator=(const FakeLweKeySwitchKey &)= delete;
    };

// At compile time, we verify that the two structures have exactly the same size
//TODO: parallelization
    static_assert(sizeof(FakeLweKeySwitchKey) == sizeof(LweKeySwitchKey), "Error: Size is not correct");


    inline FakeLweKeySwitchKey *fake(LweKeySwitchKey *key) {
        FakeLweKeySwitchKey *reps = (FakeLweKeySwitchKey *) key;
        if (reps->fake_uid != FakeLweKeySwitchKey::FAKE_LWEKEYSWITCH_UID) abort();
        return reps;
    }

    inline const FakeLweKeySwitchKey *fake(const LweKeySwitchKey *key) {
        const FakeLweKeySwitchKey *reps = (const FakeLweKeySwitchKey *) key;
        if (reps->fake_uid != FakeLweKeySwitchKey::FAKE_LWEKEYSWITCH_UID) abort();
        return reps;
    }


/**
 * fills the KeySwitching key array
 * @param result The (n x t x base) array of samples. 
 *        result[i][j][k] encodes k.s[i]/base^(j+1)
 * @param out_key The LWE key to encode all the output samples 
 * @param out_alpha The standard deviation of all output samples
 * @param in_key The (binary) input key
 * @param n The size of the input key
 * @param t The precision of the keyswitch (technically, 1/2.base^t)
 * @param basebit Log_2 of base
 */
    inline void fake_lweCreateKeySwitchKey_fromArray(LweSample ***result,
                                                     const LweKey *out_key, const double out_alpha,
                                                     const int32_t *in_key, const int32_t n, const int32_t t, const int32_t basebit) {
        const int32_t base = 1 << basebit;       // base=2 in [CGGI16]

        for (int32_t i = 0; i < n; i++) {
            for (int32_t j = 0; j < t; j++) {
                for (int32_t k = 0; k < base; k++) {
                    Torus32 x = (in_key[i] * k) * (1 << (32 - (j + 1) * basebit));
                    fake_lweSymEncrypt(&result[i][j][k], x, out_alpha, out_key);
                }
            }
        }
    }

#define USE_FAKE_lweCreateKeySwitchKey_fromArray \
    inline void lweCreateKeySwitchKey_fromArray(LweSample*** result, \
        const LweKey* out_key, const double out_alpha, \
        const int32_t* in_key, const int32_t n, const int32_t t, const int32_t basebit){ \
    fake_lweCreateKeySwitchKey_fromArray(result, \
        out_key, out_alpha, \
        in_key, n, t, basebit); \
    }


/**
 * translates the message of the result sample by -sum(a[i].s[i]) where s is the secret
 * embedded in ks.
 * @param result the LWE sample to translate by -sum(ai.si). 
 * @param ks The (n x t x base) key switching key 
 *        ks[i][j][k] encodes k.s[i]/base^(j+1)
 * @param params The common LWE parameters of ks and result
 * @param ai The input torus array
 * @param n The size of the input key
 * @param t The precision of the keyswitch (technically, 1/2.base^t)
 * @param basebit Log_2 of base
 */
    inline void fake_lweKeySwitchTranslate_fromArray(LweSample *result,
                                                     const LweSample ***ks, const LweParams *params,
                                                     const Torus32 *ai,
                                                     const int32_t n, const int32_t t, const int32_t basebit) {
        const int32_t base = 1 << basebit;       // base=2 in [CGGI16]
        const int32_t prec_offset = 1 << (32 - (1 + basebit * t)); //precision
        const int32_t mask = base - 1;

        for (int32_t i = 0; i < n; i++) {
            const uint32_t aibar = ai[i] + prec_offset;
            for (int32_t j = 0; j < t; j++) {
                const uint32_t aij = (aibar >> (32 - (j + 1) * basebit)) & mask;
                fake_lweSubTo(result, &ks[i][j][aij], params);
            }
        }
    }

#define USE_FAKE_lweKeySwitchTranslate_fromArray \
    inline void lweKeySwitchTranslate_fromArray(LweSample* result, \
        const LweSample*** ks, const LweParams* params, \
        const Torus32* ai, \
        const int32_t n, const int32_t t, const int32_t basebit){ \
        fake_lweKeySwitchTranslate_fromArray(result, ks, params, ai, n, t, basebit); \
    }


    inline void fake_lweCreateKeySwitchKey(LweKeySwitchKey *result, const LweKey *in_key, const LweKey *out_key) {
        const double variance = out_key->params->alpha_min * out_key->params->alpha_min;
        const int32_t n = result->n;
        const int32_t basebit = result->basebit;
        const int32_t t = result->t;
        const double epsilon2 = pow(0.5, 2 * (basebit * t + 1));

        const double variance_overhead = n * t * variance + n * epsilon2;

        FakeLweKeySwitchKey *fres = fake(result);
        fres->variance_overhead = variance_overhead;
    }

#define USE_FAKE_lweCreateKeySwitchKey \
    inline void lweCreateKeySwitchKey(LweKeySwitchKey* result, const LweKey* in_key, const LweKey* out_key) { \
    fake_lweCreateKeySwitchKey(result, in_key, out_key); \
    }


//sample=(a',b')
    inline void fake_lweKeySwitch(LweSample *result, const LweKeySwitchKey *ks, const LweSample *sample) {
        FakeLwe *fres = fake(result);
        const FakeLweKeySwitchKey *fks = fake(ks);
        const FakeLwe *fsample = fake(sample);

        fres->message = fsample->message;
        fres->current_variance = fsample->current_variance + fks->variance_overhead;
    }

//TODO: parallelization
#define USE_FAKE_lweKeySwitch \
    static inline void lweKeySwitch(LweSample *result, const LweKeySwitchKey *ks, const LweSample *sample) {\
    fake_lweKeySwitch(result, ks, sample); \
    }


    inline LweKeySwitchKey *fake_new_LweKeySwitchKey(int32_t n, int32_t t, int32_t basebit, const LweParams *params) {
        FakeLweKeySwitchKey *ks = new FakeLweKeySwitchKey(n, t, basebit);
        return (LweKeySwitchKey *) ks;
    }

#define USE_FAKE_new_LweKeySwitchKey \
inline LweKeySwitchKey* new_LweKeySwitchKey(int32_t n, int32_t t, int32_t basebit, const LweParams* params) { \
    return fake_new_LweKeySwitchKey(n,t,basebit,params); \
}


    inline void fake_delete_LweKeySwitchKey(LweKeySwitchKey *ks) {
        FakeLweKeySwitchKey *fks = fake(ks);
        delete fks;
    }

#define USE_FAKE_delete_LweKeySwitchKey \
inline void delete_LweKeySwitchKey(LweKeySwitchKey* ks) { \
    fake_delete_LweKeySwitchKey(ks); \
}


} // namespace

#endif // FAKE_LWE_KEYSWITCH_H