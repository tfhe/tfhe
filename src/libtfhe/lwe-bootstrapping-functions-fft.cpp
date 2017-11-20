/*
 * Bootstrapping FFT functions
 */


#ifndef TFHE_TEST_ENVIRONMENT

#include <iostream>
#include <cassert>
#include "tfhe.h"

using namespace std;
#define INCLUDE_ALL
#else
#undef EXPORT
#define EXPORT
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TFHE_INIT_LWEBOOTSTRAPPINGKEY_FFT
#undef INCLUDE_TFHE_INIT_LWEBOOTSTRAPPINGKEY_FFT
//(equivalent of the C++ constructor)
EXPORT void init_LweBootstrappingKeyFFT(LweBootstrappingKeyFFT *obj, const LweBootstrappingKey *bk) {

    const LweParams *in_out_params = bk->in_out_params;
    const TGswParams *bk_params = bk->bk_params;
    const TLweParams *accum_params = bk_params->tlwe_params;
    const LweParams *extract_params = &accum_params->extracted_lweparams;
    const int32_t n = in_out_params->n;
    const int32_t t = bk->ks->t;
    const int32_t basebit = bk->ks->basebit;
    const int32_t base = bk->ks->base;
    const int32_t N = extract_params->n;

    LweKeySwitchKey *ks = new_LweKeySwitchKey(N, t, basebit, in_out_params);
    // Copy the KeySwitching key
    for (int32_t i = 0; i < N; i++) {
        for (int32_t j = 0; j < t; j++) {
            for (int32_t p = 0; p < base; p++) {
                lweCopy(&ks->ks[i][j][p], &bk->ks->ks[i][j][p], in_out_params);
            }
        }
    }

    // Bootstrapping Key FFT 
    TGswSampleFFT *bkFFT = new_TGswSampleFFT_array(n, bk_params);
    for (int32_t i = 0; i < n; ++i) {
        tGswToFFTConvert(&bkFFT[i], &bk->bk[i], bk_params);
    }

    new(obj) LweBootstrappingKeyFFT(in_out_params, bk_params, accum_params, extract_params, bkFFT, ks);
}
#endif



//destroys the LweBootstrappingKeyFFT structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweBootstrappingKeyFFT(LweBootstrappingKeyFFT *obj) {
    delete_LweKeySwitchKey((LweKeySwitchKey *) obj->ks);
    delete_TGswSampleFFT_array(obj->in_out_params->n, (TGswSampleFFT *) obj->bkFFT);

    obj->~LweBootstrappingKeyFFT();
}


void tfhe_MuxRotate_FFT(TLweSample *result, const TLweSample *accum, const TGswSampleFFT *bki, const int32_t barai,
                        const TGswParams *bk_params) {
    // ACC = BKi*[(X^barai-1)*ACC]+ACC
    // temp = (X^barai-1)*ACC
    tLweMulByXaiMinusOne(result, barai, accum, bk_params->tlwe_params);
    // temp *= BKi
    tGswFFTExternMulToTLwe(result, bki, bk_params);
    // ACC += temp
    tLweAddTo(result, accum, bk_params->tlwe_params);
}


#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BLIND_ROTATE_FFT
#undef INCLUDE_TFHE_BLIND_ROTATE_FFT
/**
 * multiply the accumulator by X^sum(bara_i.s_i)
 * @param accum the TLWE sample to multiply
 * @param bk An array of n TGSW FFT samples where bk_i encodes s_i
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
EXPORT void tfhe_blindRotate_FFT(TLweSample *accum,
                                 const TGswSampleFFT *bkFFT,
                                 const int32_t *bara,
                                 const int32_t n,
                                 const TGswParams *bk_params) {

    //TGswSampleFFT* temp = new_TGswSampleFFT(bk_params);
    TLweSample *temp = new_TLweSample(bk_params->tlwe_params);
    TLweSample *temp2 = temp;
    TLweSample *temp3 = accum;

    for (int32_t i = 0; i < n; i++) {
        const int32_t barai = bara[i];
        if (barai == 0) continue; //indeed, this is an easy case!

        tfhe_MuxRotate_FFT(temp2, temp3, bkFFT + i, barai, bk_params);
        swap(temp2, temp3);
    }
    if (temp3 != accum) {
        tLweCopy(accum, temp3, bk_params->tlwe_params);
    }

    delete_TLweSample(temp);
    //delete_TGswSampleFFT(temp);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BLIND_ROTATE_AND_EXTRACT_FFT
#undef INCLUDE_TFHE_BLIND_ROTATE_AND_EXTRACT_FFT
/**
 * result = LWE(v_p) where p=barb-sum(bara_i.s_i) mod 2N
 * @param result the output LWE sample
 * @param v a 2N-elt anticyclic function (represented by a TorusPolynomial)
 * @param bk An array of n TGSW FFT samples where bk_i encodes s_i
 * @param barb A coefficients between 0 and 2N-1
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
EXPORT void tfhe_blindRotateAndExtract_FFT(LweSample *result,
                                           const TorusPolynomial *v,
                                           const TGswSampleFFT *bk,
                                           const int32_t barb,
                                           const int32_t *bara,
                                           const int32_t n,
                                           const TGswParams *bk_params) {

    const TLweParams *accum_params = bk_params->tlwe_params;
    const LweParams *extract_params = &accum_params->extracted_lweparams;
    const int32_t N = accum_params->N;
    const int32_t _2N = 2 * N;

    // Test polynomial 
    TorusPolynomial *testvectbis = new_TorusPolynomial(N);
    // Accumulator
    TLweSample *acc = new_TLweSample(accum_params);

    // testvector = X^{2N-barb}*v
    if (barb != 0) torusPolynomialMulByXai(testvectbis, _2N - barb, v);
    else torusPolynomialCopy(testvectbis, v);
    tLweNoiselessTrivial(acc, testvectbis, accum_params);
    // Blind rotation
    tfhe_blindRotate_FFT(acc, bk, bara, n, bk_params);
    // Extraction
    tLweExtractLweSample(result, acc, extract_params, accum_params);

    delete_TLweSample(acc);
    delete_TorusPolynomial(testvectbis);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BOOTSTRAP_WO_KS_FFT
#undef INCLUDE_TFHE_BOOTSTRAP_WO_KS_FFT
/**
 * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
 * @param result The resulting LweSample
 * @param bk The bootstrapping + keyswitch key
 * @param mu The output message (if phase(x)>0)
 * @param x The input sample
 */
EXPORT void tfhe_bootstrap_woKS_FFT(LweSample *result,
                                    const LweBootstrappingKeyFFT *bk,
                                    Torus32 mu,
                                    const LweSample *x) {

    const TGswParams *bk_params = bk->bk_params;
    const TLweParams *accum_params = bk->accum_params;
    const LweParams *in_params = bk->in_out_params;
    const int32_t N = accum_params->N;
    const int32_t Nx2 = 2 * N;
    const int32_t n = in_params->n;

    TorusPolynomial *testvect = new_TorusPolynomial(N);
    int32_t *bara = new int32_t[N];


    // Modulus switching
    int32_t barb = modSwitchFromTorus32(x->b, Nx2);
    for (int32_t i = 0; i < n; i++) {
        bara[i] = modSwitchFromTorus32(x->a[i], Nx2);
    }

    // the initial testvec = [mu,mu,mu,...,mu]
    for (int32_t i = 0; i < N; i++) testvect->coefsT[i] = mu;

    // Bootstrapping rotation and extraction
    tfhe_blindRotateAndExtract_FFT(result, testvect, bk->bkFFT, barb, bara, n, bk_params);


    delete[] bara;
    delete_TorusPolynomial(testvect);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BOOTSTRAP_FFT
#undef INCLUDE_TFHE_BOOTSTRAP_FFT
/**
 * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
 * @param result The resulting LweSample
 * @param bk The bootstrapping + keyswitch key
 * @param mu The output message (if phase(x)>0)
 * @param x The input sample
 */
EXPORT void tfhe_bootstrap_FFT(LweSample *result,
                               const LweBootstrappingKeyFFT *bk,
                               Torus32 mu,
                               const LweSample *x) {

    LweSample *u = new_LweSample(&bk->accum_params->extracted_lweparams);

    tfhe_bootstrap_woKS_FFT(u, bk, mu, x);
    // Key switching
    lweKeySwitch(result, bk->ks, u);

    delete_LweSample(u);
}
#endif
















//allocate memory space for a LweBootstrappingKeyFFT

EXPORT LweBootstrappingKeyFFT *alloc_LweBootstrappingKeyFFT() {
    return (LweBootstrappingKeyFFT *) malloc(sizeof(LweBootstrappingKeyFFT));
}
EXPORT LweBootstrappingKeyFFT *alloc_LweBootstrappingKeyFFT_array(int32_t nbelts) {
    return (LweBootstrappingKeyFFT *) malloc(nbelts * sizeof(LweBootstrappingKeyFFT));
}

//free memory space for a LweKey
EXPORT void free_LweBootstrappingKeyFFT(LweBootstrappingKeyFFT *ptr) {
    free(ptr);
}
EXPORT void free_LweBootstrappingKeyFFT_array(int32_t nbelts, LweBootstrappingKeyFFT *ptr) {
    free(ptr);
}

//initialize the key structure

EXPORT void init_LweBootstrappingKeyFFT_array(int32_t nbelts, LweBootstrappingKeyFFT *obj, const LweBootstrappingKey *bk) {
    for (int32_t i = 0; i < nbelts; i++) {
        init_LweBootstrappingKeyFFT(obj + i, bk);
    }
}


EXPORT void destroy_LweBootstrappingKeyFFT_array(int32_t nbelts, LweBootstrappingKeyFFT *obj) {
    for (int32_t i = 0; i < nbelts; i++) {
        destroy_LweBootstrappingKeyFFT(obj + i);
    }
}

//allocates and initialize the LweBootstrappingKeyFFT structure
//(equivalent of the C++ new)
EXPORT LweBootstrappingKeyFFT *new_LweBootstrappingKeyFFT(const LweBootstrappingKey *bk) {
    LweBootstrappingKeyFFT *obj = alloc_LweBootstrappingKeyFFT();
    init_LweBootstrappingKeyFFT(obj, bk);
    return obj;
}
EXPORT LweBootstrappingKeyFFT *new_LweBootstrappingKeyFFT_array(int32_t nbelts, const LweBootstrappingKey *bk) {
    LweBootstrappingKeyFFT *obj = alloc_LweBootstrappingKeyFFT_array(nbelts);
    init_LweBootstrappingKeyFFT_array(nbelts, obj, bk);
    return obj;
}

//destroys and frees the LweBootstrappingKeyFFT structure
//(equivalent of the C++ delete)
EXPORT void delete_LweBootstrappingKeyFFT(LweBootstrappingKeyFFT *obj) {
    destroy_LweBootstrappingKeyFFT(obj);
    free_LweBootstrappingKeyFFT(obj);
}
EXPORT void delete_LweBootstrappingKeyFFT_array(int32_t nbelts, LweBootstrappingKeyFFT *obj) {
    destroy_LweBootstrappingKeyFFT_array(nbelts, obj);
    free_LweBootstrappingKeyFFT_array(nbelts, obj);
}






