/*
 * Bootstrapping FFT functions
 */


#ifndef TFHE_TEST_ENVIRONMENT
#include <iostream>
#include <cassert>
#include "tfhe_gate.h"
#include "tfhe_alloc.h"
#include "tfhe_functions.h"
using namespace std;
#define INCLUDE_ALL
#else
#undef EXPORT
#define EXPORT
#endif



template<typename TORUS>
void TfheFunctions<TORUS>::MuxRotate_FFT(TLweSample<TORUS>* result, const TLweSample<TORUS>* accum, const TGswSampleFFT<TORUS>* bki, const int barai, const TGswParams<TORUS>* bk_params) {
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
template<typename TORUS>
void TfheFunctions<TORUS>::blindRotate_FFT(TLweSample<TORUS>* accum,
  const TGswSampleFFT<TORUS>* bkFFT,
  const int* bara,
  const int n,
  const TGswParams<TORUS>* bk_params) {

  //TGswSampleFFT<TORUS>* temp = new_TGswSampleFFT<TORUS>(bk_params);
  TLweSample<TORUS>* temp = new_TLweSample<TORUS>(bk_params->tlwe_params);
  TLweSample<TORUS>* temp2 = temp;
  TLweSample<TORUS>* temp3 = accum;

  for (int i=0; i<n; i++) {
    const int barai=bara[i];
    if (barai==0) continue; //indeed, this is an easy case!

    tfhe_MuxRotate_FFT(temp2, temp3, bkFFT+i, barai, bk_params);
    swap(temp2,temp3);
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
template<typename TORUS>
void TfheFunctions<TORUS>::blindRotateAndExtract_FFT(LweSample<TORUS>* result,
  const TorusPolynomial<TORUS>* v,
  const TGswSampleFFT<TORUS>* bk,
  const int barb,
  const int* bara,
  const int n,
  const TGswParams<TORUS>* bk_params) {

  const TLweParams<TORUS>* accum_params = bk_params->tlwe_params;
  const LweParams<TORUS>* extract_params = &accum_params->extracted_lweparams;
  const int N = accum_params->N;
  const int _2N = 2*N;

  // Test polynomial
  TorusPolynomial<TORUS>* testvectbis = new_TorusPolynomial<TORUS>(N);
  // Accumulator
  TLweSample<TORUS>* acc = new_TLweSample<TORUS>(accum_params);

  // testvector = X^{2N-barb}*v
  if (barb!=0) torusPolynomialMulByXai(testvectbis, _2N-barb, v);
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
template<typename TORUS>
void TfheFunctions<TORUS>::bootstrap_woKS_FFT(LweSample<TORUS>* result,
  const LweBootstrappingKeyFFT<TORUS>* bk,
  TORUS mu,
  const LweSample<TORUS>* x){

  const TGswParams<TORUS>* bk_params = bk->bk_params;
  const TLweParams<TORUS>* accum_params = bk->accum_params;
  const LweParams<TORUS>* in_params = bk->in_out_params;
  const int N=accum_params->N;
  const int Nx2= 2*N;
  const int n = in_params->n;

  TorusPolynomial<TORUS>* testvect = new_TorusPolynomial<TORUS>(N);
  int* bara = new int[N];


  // Modulus switching
  int barb = TorusUtils<TORUS>::modSwitchFromTorus(x->b,Nx2);
  for (int i=0; i<n; i++) {
    bara[i]=TorusUtils<TORUS>::modSwitchFromTorus(x->a[i],Nx2);
  }

  // the initial testvec = [mu,mu,mu,...,mu]
  for (int i=0;i<N;i++) testvect->coefsT[i]=mu;

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
template<typename TORUS>
void TfheFunctions<TORUS>::bootstrap_FFT(LweSample<TORUS>* result,
  const LweBootstrappingKeyFFT<TORUS>* bk,
  TORUS mu,
  const LweSample<TORUS>* x){

  LweSample<TORUS>* u = new_LweSample<TORUS>(&bk->accum_params->extracted_lweparams);

  tfhe_bootstrap_woKS_FFT(u, bk, mu, x);
  // Key switching
  lweKeySwitch(result, bk->ks, u);

  delete_LweSample(u);
}
#endif