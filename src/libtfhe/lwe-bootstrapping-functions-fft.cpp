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


#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BOOTSTRAPROTATE_FFT
/**
 * multiply the accumulator by X^sum(bara_i.s_i)
 * @param accum the TLWE sample to multiply
 * @param bk An array of n TGSW FFT samples where bk_i encodes s_i
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
EXPORT void tfhe_bootstrapRotate_FFT(TLweSample* accum, 
    const TGswSampleFFT* bk, 
    const int* bara, 
    const int n, 
    const TGswParams* bk_params) {

    TGswSampleFFT* temp = new_TGswSampleFFT(bk_params);
    
    for (int i=0; i<n; i++) {
        const int barai=bara[i];
        if (barai==0) continue; //indeed, this is an easy case!
        // ILA: for now we do acc = acc * ( H + (X^{bara[i]}-1)*bk[i] )
        // but we should change it with the new formula
        tGswFFTMulByXaiMinusOne(temp, barai, bk+i, bk_params);
        tGswFFTAddH(temp, bk_params);
        tGswFFTExternMulToTLwe(accum, temp, bk_params);
    }
    delete_TGswSampleFFT(temp);
}
#endif 







#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BOOTSTRAPROTATEEXTRACT_FFT
/**
 * result = LWE(v_p) where p=barb-sum(bara_i.s_i) mod 2N
 * @param result the output LWE sample
 * @param v a 2N-elt anticyclic function (represented by a TorusPolynomial)
 * @param bk An array of n TGSW FFT samples where bk_i encodes s_i
 * @param barb A coefficients between 0 and 2N-1
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
EXPORT void tfhe_bootstrapRotateExtract_FFT(LweSample* result, 
    const TorusPolynomial* v, 
    const TGswSampleFFT* bk, 
    const int barb,
    const int* bara,
    const int n,
    const TGswParams* bk_params) {

    const TLweParams* accum_params = bk_params->tlwe_params;
    const LweParams* extract_params = &accum_params->extracted_lweparams;
    const int N = accum_params->N;
    const int _2N = 2*N;

    // Test polynomial 
    TorusPolynomial* testvectbis = new_TorusPolynomial(N);
    // Accumulator
    TLweSample* acc = new_TLweSample(accum_params);

    // testvector = X^{2N-barb}*v
    if (barb!=0) torusPolynomialMulByXai(testvectbis, _2N-barb, v);
    tLweNoiselessTrivial(acc, testvectbis, accum_params);
    // Blind rotation
    tfhe_bootstrapRotate_FFT(acc, bk, bara, n, bk_params);
    // Extraction
    tLweExtractLweSample(result, acc, extract_params, accum_params);

    delete_TLweSample(acc);
    delete_TorusPolynomial(testvectbis);
}
#endif 







   

#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BOOTSTRAP_FFT
/**
 * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
 * @param result The resulting LweSample
 * @param bk The bootstrapping + keyswitch key
 * @param mu The output message (if phase(x)>0)
 * @param x The input sample
 */
EXPORT void tfhe_bootstrap_FFT(LweSample* result, 
    const LweBootstrappingKeyFFT* bk, 
    Torus32 mu, 
    const LweSample* x){

    const TGswParams* bk_params = bk->bk_params;
    const TLweParams* accum_params = bk->accum_params;
    const LweParams* extract_params = &accum_params->extracted_lweparams;
    const LweParams* in_params = bk->in_out_params;
    const int N=accum_params->N;
    const int Nx2= 2*N;
    const int n = in_params->n;

    TorusPolynomial* testvect = new_TorusPolynomial(N);
    int* bara = new int[N];
    LweSample* u = new_LweSample(extract_params);


    // Modulus switching
    int barb = modSwitchFromTorus32(x->b,Nx2);
    for (int i=0; i<n; i++) {
        bara[i]=modSwitchFromTorus32(x->a[i],Nx2);
    }

    // the initial testvec = [mu,mu,mu,...,mu]
    for (int i=0;i<N;i++) testvect->coefsT[i]=mu;

    // Bootstrapping rotation and extraction
    tfhe_bootstrapRotateExtract_FFT(u, testvect, bk->bk, barb, bara, n, bk_params);
    // Key switching
    lweKeySwitch(result, bk->ks, u);

    
    delete_LweSample(u);
    delete[] bara;
    delete_TorusPolynomial(testvect);
}
#endif



