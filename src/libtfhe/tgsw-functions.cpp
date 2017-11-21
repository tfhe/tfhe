#ifndef TFHE_TEST_ENVIRONMENT

#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include "tfhe_core.h"
#include "numeric_functions.h"
#include "tlwe_functions.h"
#include "tgsw_functions.h"
#include "polynomials_arithmetic.h"
#include "lagrangehalfc_arithmetic.h"

#define INCLUDE_ALL
#else
#undef EXPORT
#define EXPORT
#endif

#if defined INCLUDE_ALL || defined INCLUDE_TGSW_INIT
#undef INCLUDE_TGSW_INIT
//initialize the sample structure
//(equivalent of the C++ constructor)
EXPORT void init_TGswSample(TGswSample *obj, const TGswParams *params) {
    const int32_t k = params->tlwe_params->k;
    const int32_t l = params->l;
    TLweSample *all_sample = new_TLweSample_array((k + 1) * l,
                                                  params->tlwe_params); // tous les samples comme un vecteur ligne
    TLweSample **bloc_sample = new TLweSample *[k + 1]; // blocs horizontaux (l lignes) de la matrice TGsw

    for (int32_t p = 0; p < k + 1; ++p)
        bloc_sample[p] = all_sample + p * l;

    new(obj) TGswSample(all_sample, bloc_sample, k, l);
}
#endif

#if defined INCLUDE_ALL || defined INCLUDE_TGSW_DESTROY
#undef INCLUDE_TGSW_DESTROY
//destroys the TGswSample structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TGswSample(TGswSample *obj) {
    const int32_t k = obj->k;
    const int32_t l = obj->l;
    delete_TLweSample_array((k + 1) * l, obj->all_sample);
    delete[] obj->bloc_sample;
    obj->~TGswSample();
}
#endif

#if defined INCLUDE_ALL || defined INCLUDE_TGSW_KEYGEN
#undef INCLUDE_TGSW_KEYGEN
// TGsw
/** generate a tgsw key (in fact, a tlwe key) */
EXPORT void tGswKeyGen(TGswKey *result) {
    tLweKeyGen(&result->tlwe_key);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TGSW_CLEAR
#undef INCLUDE_TGSW_CLEAR
// support Functions for TGsw
// Result = 0
EXPORT void tGswClear(TGswSample *result, const TGswParams *params) {
    const int32_t kpl = params->kpl;

    for (int32_t p = 0; p < kpl; ++p)
        tLweClear(&result->all_sample[p], params->tlwe_params);
}
#endif

#if defined INCLUDE_ALL || defined INCLUDE_TGSW_ADD_H
#undef INCLUDE_TGSW_ADD_H
// Result += H
EXPORT void tGswAddH(TGswSample *result, const TGswParams *params) {
    const int32_t k = params->tlwe_params->k;
    const int32_t l = params->l;
    const Torus32 *h = params->h;

    // compute result += H
    for (int32_t bloc = 0; bloc <= k; ++bloc)
        for (int32_t i = 0; i < l; i++)
            result->bloc_sample[bloc][i].a[bloc].coefsT[0] += h[i];
}
#endif

#if defined INCLUDE_ALL || defined INCLUDE_TGSW_ADD_MU_H
#undef INCLUDE_TGSW_ADD_MU_H
// Result += mu*H
EXPORT void tGswAddMuH(TGswSample *result, const IntPolynomial *message, const TGswParams *params) {
    const int32_t k = params->tlwe_params->k;
    const int32_t N = params->tlwe_params->N;
    const int32_t l = params->l;
    const Torus32 *h = params->h;
    const int32_t *mu = message->coefs;

    // compute result += H
    for (int32_t bloc = 0; bloc <= k; ++bloc)
        for (int32_t i = 0; i < l; i++) {
            Torus32 *target =
                    result->bloc_sample[bloc][i].a[bloc].coefsT;
            const Torus32 hi = h[i];
            for (int32_t j = 0; j < N; j++) {
                target[j] += mu[j] * hi;
            }
        }
}
#endif

#if defined INCLUDE_ALL || defined INCLUDE_TGSW_ADD_MU_INT_H
#undef INCLUDE_TGSW_ADD_MU_INT_H
// Result += mu*H, mu integer
EXPORT void tGswAddMuIntH(TGswSample *result, const int32_t message, const TGswParams *params) {
    const int32_t k = params->tlwe_params->k;
    const int32_t l = params->l;
    const Torus32 *h = params->h;

    // compute result += H
    for (int32_t bloc = 0; bloc <= k; ++bloc)
        for (int32_t i = 0; i < l; i++)
            result->bloc_sample[bloc][i].a[bloc].coefsT[0] += message * h[i];
}
#endif

#if defined INCLUDE_ALL || defined INCLUDE_TGSW_ENCRYPT_ZERO
#undef INCLUDE_TGSW_ENCRYPT_ZERO
// Result = tGsw(0)
EXPORT void tGswEncryptZero(TGswSample *result, double alpha, const TGswKey *key) {
    const TLweKey *rlkey = &key->tlwe_key;
    const int32_t kpl = key->params->kpl;

    for (int32_t p = 0; p < kpl; ++p) {
        tLweSymEncryptZero(&result->all_sample[p], alpha, rlkey);
    }
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TGSW_MUL_BY_XAI_MINUS_ONE
#undef INCLUDE_TGSW_MUL_BY_XAI_MINUS_ONE
//mult externe de X^{a_i} par bki
EXPORT void tGswMulByXaiMinusOne(TGswSample *result, int32_t ai, const TGswSample *bk, const TGswParams *params) {
    const TLweParams *par = params->tlwe_params;
    const int32_t kpl = params->kpl;
    for (int32_t i = 0; i < kpl; i++)
        tLweMulByXaiMinusOne(&result->all_sample[i], ai, &bk->all_sample[i], par);
}
#endif

#if defined INCLUDE_ALL || defined INCLUDE_TGSW_EXTERN_MUL_TO_TLWE
#undef INCLUDE_TGSW_EXTERN_MUL_TO_TLWE
//Update l'accumulateur ligne 5 de l'algo toujours
//void tGswTLweDecompH(IntPolynomial* result, const TLweSample* sample,const TGswParams* params);
//accum *= sample
EXPORT void tGswExternMulToTLwe(TLweSample *accum, const TGswSample *sample, const TGswParams *params) {
    const TLweParams *par = params->tlwe_params;
    const int32_t N = par->N;
    const int32_t kpl = params->kpl;
    //TODO: improve this new/delete
    IntPolynomial *dec = new_IntPolynomial_array(kpl, N);

    tGswTLweDecompH(dec, accum, params);
    tLweClear(accum, par);
    for (int32_t i = 0; i < kpl; i++) {
        tLweAddMulRTo(accum, &dec[i], &sample->all_sample[i], par);
    }

    delete_IntPolynomial_array(kpl, dec);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TGSW_SYM_ENCRYPT
#undef INCLUDE_TGSW_SYM_ENCRYPT
/**
 * encrypts a poly message
 */
EXPORT void tGswSymEncrypt(TGswSample *result, const IntPolynomial *message, double alpha, const TGswKey *key) {
    tGswEncryptZero(result, alpha, key);
    tGswAddMuH(result, message, key->params);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TGSW_SYM_ENCRYPT_INT
#undef INCLUDE_TGSW_SYM_ENCRYPT_INT
/**
 * encrypts a constant message
 */
EXPORT void tGswSymEncryptInt(TGswSample *result, const int32_t message, double alpha, const TGswKey *key) {
    tGswEncryptZero(result, alpha, key);
    tGswAddMuIntH(result, message, key->params);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TGSW_ENCRYPT_B
#undef INCLUDE_TGSW_ENCRYPT_B
/**
 * encrypts a message = 0 ou 1
 */
EXPORT void tGswEncryptB(TGswSample *result, const int32_t message, double alpha, const TGswKey *key) {
    tGswEncryptZero(result, alpha, key);
    if (message == 1)
        tGswAddH(result, key->params);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TGSW_DECRYPT
#undef INCLUDE_TGSW_DECRYPT
// à revoir
EXPORT void tGswSymDecrypt(IntPolynomial *result, const TGswSample *sample, const TGswKey *key, const int32_t Msize) {
    const TGswParams *params = key->params;
    const TLweParams *rlwe_params = params->tlwe_params;
    const int32_t N = rlwe_params->N;
    const int32_t l = params->l;
    const int32_t k = rlwe_params->k;
    TorusPolynomial *testvec = new_TorusPolynomial(N);
    TorusPolynomial *tmp = new_TorusPolynomial(N);
    IntPolynomial *decomp = new_IntPolynomial_array(l, N);

    const Torus32 indic = modSwitchToTorus32(1, Msize);
    torusPolynomialClear(testvec);
    testvec->coefsT[0] = indic;
    tGswTorus32PolynomialDecompH(decomp, testvec, params);

    torusPolynomialClear(testvec);
    for (int32_t i = 0; i < l; i++) {
        for (int32_t j = 1; j < N; j++) assert(decomp[i].coefs[j] == 0);
        tLwePhase(tmp, &sample->bloc_sample[k][i], &key->tlwe_key);
        torusPolynomialAddMulR(testvec, decomp + i, tmp);
    }
    for (int32_t i = 0; i < N; i++)
        result->coefs[i] = modSwitchFromTorus32(testvec->coefsT[i], Msize);

    delete_TorusPolynomial(testvec);
    delete_TorusPolynomial(tmp);
    delete_IntPolynomial_array(l, decomp);
}
#endif

/*
// à revoir
EXPORT int32_t tGswSymDecryptInt(const TGswSample* sample, const TGswKey* key){
    TorusPolynomial* phase = new_TorusPolynomial(key->params->tlwe_params->N);

    tGswPhase(phase, sample, key);
    int32_t result = modSwitchFromTorus32(phase->coefsT[0], Msize);

    delete_TorusPolynomial(phase);
    return result;
}
*/
//do we really decrypt Gsw samples?
// EXPORT void tGswMulByXaiMinusOne(Gsw* result, int32_t ai, const Gsw* bk);
// EXPORT void tLweExternMulRLweTo(RLwe* accum, Gsw* a); //  accum = a \odot accum


#if defined INCLUDE_ALL || defined INCLUDE_TGSW_TLWE_DECOMP_H
#undef INCLUDE_TGSW_TLWE_DECOMP_H
//fonction de decomposition
EXPORT void tGswTLweDecompH(IntPolynomial *result, const TLweSample *sample, const TGswParams *params) {
    const int32_t k = params->tlwe_params->k;
    const int32_t l = params->l;

    for (int32_t i = 0; i <= k; ++i) // b=a[k]
        tGswTorus32PolynomialDecompH(result + (i * l), &sample->a[i], params);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TGSW_TORUS32POLYNOMIAL_DECOMP_H_OLD
#undef INCLUDE_TGSW_TORUS32POLYNOMIAL_DECOMP_H_OLD
EXPORT void
Torus32PolynomialDecompH_old(IntPolynomial *result, const TorusPolynomial *sample, const TGswParams *params) {
    const int32_t N = params->tlwe_params->N;
    const int32_t l = params->l;
    const int32_t Bgbit = params->Bgbit;
    const uint32_t maskMod = params->maskMod;
    const int32_t halfBg = params->halfBg;
    const uint32_t offset = params->offset;

    for (int32_t j = 0; j < N; ++j) {
        uint32_t temp0 = sample->coefsT[j] + offset;
        for (int32_t p = 0; p < l; ++p) {
            uint32_t temp1 = (temp0 >> (32 - (p + 1) * Bgbit)) & maskMod; // doute
            result[p].coefs[j] = temp1 - halfBg;
        }
    }
}
#endif

#if defined INCLUDE_ALL || defined INCLUDE_TGSW_TORUS32POLYNOMIAL_DECOMP_H
#undef INCLUDE_TGSW_TORUS32POLYNOMIAL_DECOMP_H
EXPORT void
tGswTorus32PolynomialDecompH(IntPolynomial *result, const TorusPolynomial *sample, const TGswParams *params) {
    const int32_t N = params->tlwe_params->N;
    const int32_t l = params->l;
    const int32_t Bgbit = params->Bgbit;
    uint32_t *buf = (uint32_t *) sample->coefsT;
//#define __AVX2__ //(to test)
#ifndef __AVX2__
    const uint32_t maskMod = params->maskMod;
    const int32_t halfBg = params->halfBg;
    const uint32_t offset = params->offset;
#else
    const uint32_t* maskMod_addr = &params->maskMod;
    const int32_t* halfBg_addr = &params->halfBg;
    const uint32_t* offset_addr = &params->offset;
    //const uint32_t offset = params->offset;
    //const uint32_t maskMod = params->maskMod;
    //const int32_t halfBg = params->halfBg;
#endif

    //First, add offset to everyone
#ifndef __AVX2__
    for (int32_t j = 0; j < N; ++j) buf[j] += offset;
#else
    {
    const uint32_t* sit = buf;
    const uint32_t* send = buf+N;
    __asm__ __volatile__ (
        "vpbroadcastd (%2),%%ymm0\n"
        "0:\n"
        "vmovdqu (%0),%%ymm3\n"
        "vpaddd %%ymm0,%%ymm3,%%ymm3\n" // add offset
        "vmovdqu %%ymm3,(%0)\n"
        "addq $32,%0\n"
        "cmpq %1,%0\n"
        "jb 0b\n"
        : "=r"(sit),"=r"(send),"=r"(offset_addr)
        :  "0"(sit), "1"(send), "2"(offset_addr)
        : "%ymm0","%ymm3","memory"
        );
    }
#endif

    //then, do the decomposition (in parallel)
    for (int32_t p = 0; p < l; ++p) {
        const int32_t decal = (32 - (p + 1) * Bgbit);
#ifndef __AVX2__
        int32_t *res_p = result[p].coefs;
        for (int32_t j = 0; j < N; ++j) {
            uint32_t temp1 = (buf[j] >> decal) & maskMod;
            res_p[j] = temp1 - halfBg;
        }
#else
        int32_t* dst = result[p].coefs;
        const uint32_t* sit = buf;
        const uint32_t* send = buf+N;
        const int32_t* decal_addr = &decal;
        __asm__ __volatile__ (
            "vpbroadcastd (%4),%%ymm0\n"
            "vpbroadcastd (%5),%%ymm1\n"
            "vmovd (%3),%%xmm2\n"
            "1:\n"
            "vmovdqu (%1),%%ymm3\n"
            "VPSRLD %%xmm2,%%ymm3,%%ymm3\n" // shift by decal
            "VPAND %%ymm1,%%ymm3,%%ymm3\n"  // and maskMod
            "VPSUBD %%ymm0,%%ymm3,%%ymm3\n" // sub halfBg
            "vmovdqu %%ymm3,(%0)\n"
            "addq $32,%0\n"
            "addq $32,%1\n"
            "cmpq %2,%1\n"
            "jb 1b\n"
            : "=r"(dst),"=r"(sit),"=r"(send),"=r"(decal_addr),"=r"(halfBg_addr),"=r"(maskMod_addr)
            :  "0"(dst), "1"(sit), "2"(send), "3"(decal_addr), "4"(halfBg_addr) ,"5"(maskMod_addr)
            : "%ymm0","%ymm1","%ymm2","%ymm3","memory"
            );
        /* // verify that the assembly block was ok
        int32_t* res_p = result[p].coefs;
        for (int32_t j = 0; j < N; ++j)
        {
            uint32_t temp1 = (buf[j] >> decal) & maskMod;
            if (res_p[j] != int32_t(temp1 - halfBg)) {
            fprintf(stderr, "j=%d,buf[j]=%u,decal=%u,mask=%u,halfbg=%d,res_p[j]=%d\n",j,buf[j],decal,maskMod,halfBg,res_p[j]);
            abort();
            }
        }*/

#endif
    }

    //finally, remove offset to everyone
#ifndef __AVX2__
    for (int32_t j = 0; j < N; ++j) buf[j] -= offset;
#else
    {
    const uint32_t* sit = buf;
    const uint32_t* send = buf+N;
    __asm__ __volatile__ (
        "vpbroadcastd (%2),%%ymm0\n"
        "2:\n"
        "vmovdqu (%0),%%ymm3\n"
        "vpsubd %%ymm0,%%ymm3,%%ymm3\n" // add offset
        "vmovdqu %%ymm3,(%0)\n"
        "addq $32,%0\n"
        "cmpq %1,%0\n"
        "jb 2b\n"
        "vzeroall\n"
        : "=r"(sit),"=r"(send),"=r"(offset_addr)
        :  "0"(sit), "1"(send), "2"(offset_addr)
        : "%ymm0","%ymm3","memory"
        );
    }
#endif
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TGSW_EXTERN_PRODUCT
#undef INCLUDE_TGSW_EXTERN_PRODUCT
//result = a*b
EXPORT void tGswExternProduct(TLweSample *result, const TGswSample *a, const TLweSample *b, const TGswParams *params) {
    const TLweParams *parlwe = params->tlwe_params;
    const int32_t N = parlwe->N;
    const int32_t kpl = params->kpl;
    IntPolynomial *dec = new_IntPolynomial_array(kpl, N);

    tGswTLweDecompH(dec, b, params);

    tLweClear(result, parlwe);
    for (int32_t i = 0; i < kpl; i++)
        tLweAddMulRTo(result, &dec[i], &a->all_sample[i], parlwe);

    result->current_variance += b->current_variance; //todo + the error term?

    delete_IntPolynomial_array(kpl, dec);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TGSW_NOISELESS_TRIVIAL
#undef INCLUDE_TGSW_NOISELESS_TRIVIAL
/**
 * result = (0,mu)
 */
EXPORT void tGswNoiselessTrivial(TGswSample *result, const IntPolynomial *mu, const TGswParams *params) {
    tGswClear(result, params);
    tGswAddMuH(result, mu, params);
}
#endif






//Autogenerated templates for allocation/construction/initialization...
//allocate memory space for a TGswSample
USE_DEFAULT_CONSTRUCTOR_DESTRUCTOR_IMPLEMENTATIONS1(TGswSample, TGswParams);


#undef INCLUDE_ALL
