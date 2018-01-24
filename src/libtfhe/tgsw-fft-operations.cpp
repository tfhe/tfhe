#ifndef TFHE_TEST_ENVIRONMENT
/* ***************************************************
TGSW fft operations
*************************************************** */

#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include <ccomplex>
#include "tfhe_core.h"
#include "numeric_functions.h"
#include "lweparams.h"
#include "lwekey.h"
#include "lwesamples.h"
#include "lwe-functions.h"
#include "tlwe_functions.h"
#include "tgsw_functions.h"
#include "polynomials_arithmetic.h"
#include "lagrangehalfc_arithmetic.h"
#include "lwebootstrappingkey.h"

using namespace std;
#else
#undef EXPORT
#define EXPORT
#endif


// For all the kpl TLWE samples composing the TGSW sample
// It computes the inverse FFT of the coefficients of the TLWE sample
template<typename TORUS>
void TGswFunctions<TORUS>::ToFFTConvert(TGswSampleFFT<TORUS>* result, const TGswSample<TORUS>* source, const TGswParams<TORUS>* params) {
  const int kpl = params->kpl;

  for (int p=0; p<kpl; p++)
    tLweToFFTConvert(result->all_samples+p, source->all_sample+p, params->tlwe_params);
}

// For all the kpl TLWE samples composing the TGSW sample
// It computes the FFT of the coefficients of the TLWEfft sample
template<typename TORUS>
void TGswFunctions<TORUS>::FromFFTConvert(TGswSample<TORUS>* result, const TGswSampleFFT<TORUS>* source, const TGswParams<TORUS>* params){
  const int kpl = params->kpl;

  for (int p=0; p<kpl; p++)
    tLweFromFFTConvert(result->all_sample+p, source->all_samples+p, params->tlwe_params);
}



// result = result + H
template<typename TORUS>
void TGswFunctions<TORUS>::FFTAddH(TGswSampleFFT<TORUS>* result, const TGswParams<TORUS>* params) {
  const int k = params->tlwe_params->k;
  const int l = params->l;

  for (int j=0; j<l; j++) {
    TORUS hj = params->h[j];
    for (int i=0; i<=k; i++)
      LagrangeHalfCPolynomialAddTorusConstant(&result->sample[i][j].a[i],hj);
  }

}

// result = list of TLWE (0,0)
template<typename TORUS>
void TGswFunctions<TORUS>::FFTClear(TGswSampleFFT<TORUS>* result, const TGswParams<TORUS>* params) {
  const int kpl = params->kpl;

  for (int p=0; p<kpl; p++)
    tLweFFTClear(result->all_samples+p, params->tlwe_params);
}

// External product (*): accum = gsw (*) accum
template<typename TORUS>
void TGswFunctions<TORUS>::FFTExternMulToTLwe(TLweSample<TORUS>* accum, const TGswSampleFFT<TORUS>* gsw, const TGswParams<TORUS>* params) {
  const TLweParams<TORUS>* tlwe_params=params->tlwe_params;
  const int k = tlwe_params->k;
  const int l = params->l;
  const int kpl = params->kpl;
  const int N = tlwe_params->N;
  //TODO attention, improve these new/delete...
  IntPolynomial* deca = new_IntPolynomial_array(kpl,N); //decomposed accumulator
  LagrangeHalfCPolynomial* decaFFT=new_LagrangeHalfCPolynomial_array(kpl,N); //fft version
  TLweSampleFFT<TORUS>* tmpa = new_TLweSampleFFT<TORUS>(tlwe_params);

  for (int i=0; i<=k; i++)
    tGswTorusPolynomialDecompH(deca+i*l,accum->a+i, params);
  for (int p=0; p<kpl; p++)
    IntPolynomial_ifft(decaFFT+p,deca+p);

  tLweFFTClear(tmpa, tlwe_params);
  for (int p=0; p<kpl; p++)
    tLweFFTAddMulRTo(tmpa, decaFFT+p, gsw->all_samples+p, tlwe_params);
  tLweFromFFTConvert(accum, tmpa, tlwe_params);

  delete_TLweSampleFFT(tmpa);
  delete_LagrangeHalfCPolynomial_array(kpl,decaFFT);
  delete_IntPolynomial_array(kpl,deca);
}

// result = (X^ai -1)*bki
template<typename TORUS>
void TGswFunctions<TORUS>::FFTMulByXaiMinusOne(TGswSampleFFT<TORUS>* result, const int ai, const TGswSampleFFT<TORUS>* bki, const TGswParams<TORUS>* params) {
  const TLweParams<TORUS>* tlwe_params=params->tlwe_params;
  const int k = tlwe_params->k;
  //const int l = params->l;
  const int kpl = params->kpl;
  const int N = tlwe_params->N;
  //on calcule x^ai-1 en fft
  //TODO attention, this prevents parallelization...
  static LagrangeHalfCPolynomial* xaim1=new_LagrangeHalfCPolynomial(N);
  LagrangeHalfCPolynomialSetXaiMinusOne(xaim1,ai);
  for (int p=0; p<kpl; p++) {
    const LagrangeHalfCPolynomial* in_s = bki->all_samples[p].a;
    LagrangeHalfCPolynomial* out_s = result->all_samples[p].a;
    for (int j=0; j<=k; j++)
      LagrangeHalfCPolynomialMul(&out_s[j], xaim1, &in_s[j]);
  }
}

#if 0
// BOOTSTRAPPING (as in CGGI16b - algo 3)
//  - modswitch: torus coefs multiplied by N/2
//  - set the test vector
//  - blind rotation by the phase
//  - sample extract
//  - keyswitch
EXPORT void tfhe_bootstrapFFT(LweSample<TORUS>* result, const LweBootstrappingKeyFFT<TORUS>* bk, Torus32 mu1, Torus32 mu0, const LweSample<TORUS>* x){
  const Torus32 ab=(mu1+mu0)/2;
  const Torus32 aa = mu0-ab; // aa=(mu1-mu0)/2;
  const TGswParams<TORUS>* bk_params = bk->bk_params;
  const TLweParams<TORUS>* accum_params = bk_params->tlwe_params;
  const LweParams<TORUS>* extract_params = &accum_params->extracted_lweparams;
  const LweParams<TORUS>* in_out_params = bk->in_out_params;
  const int n=in_out_params->n;
  const int N=accum_params->N;
  const int Ns2=N/2;
  const int Nx2= 2*N;


  // Set the test vector (aa + aaX + ... + aaX^{N/2-1} -aaX^{N/2} - ... -aaX^{N-1})*X^{b}
  TorusPolynomial<TORUS>* testvect=new_TorusPolynomial<TORUS>(N);
  TorusPolynomial<TORUS>* testvectbis=new_TorusPolynomial<TORUS>(N);

  int barb=TorusUtils<TORUS>::modSwitchFromTorus(x->b,Nx2);
  //je definis le test vector (multiplié par a inclus !
  for (int i=0;i<Ns2;i++)
     testvect->coefsT[i]=aa;
  for (int i=Ns2;i<N;i++)
     testvect->coefsT[i]=-aa;
  torusPolynomialMulByXai(testvectbis, barb, testvect);



  // Accumulateur acc = fft((0, testvect))
  TLweSample<TORUS>* acc = new_TLweSample<TORUS>(accum_params);

  // acc will be used for tfhe_bootstrapFFT, acc1=acc will be used for tfhe_bootstrap
  tLweNoiselessTrivial(acc, testvectbis, accum_params);

  TGswSample<TORUS>* temp = new_TGswSample<TORUS>(bk_params);
  TGswSampleFFT<TORUS>* tempFFT = new_TGswSampleFFT<TORUS>(bk_params);


  // Blind rotation
//NICOLAS: j'ai ajouté ce bloc
#ifndef NDEBUG
  TorusPolynomial<TORUS>* phase = new_TorusPolynomial<TORUS>(N);
  int correctOffset = barb;
  cout << "starting the test..." << endl;
#endif
  // the index 1 is given when we don't use the fft
  for (int i=0; i<n; i++) {
    int bara=TorusUtils<TORUS>::modSwitchFromTorus(-x->a[i],Nx2);

    if (bara!=0) {
      tGswFFTMulByXaiMinusOne(tempFFT, bara, bk->bkFFT+i, bk_params);
      tGswFFTAddH(tempFFT, bk_params);
      tGswFFTExternMulToTLwe(acc, tempFFT, bk_params);
    }

//NICOLAS: et surtout, j'ai ajouté celui-ci!
#ifndef NDEBUG
  tLwePhase(phase,acc,debug_accum_key);  //celui-ci, c'est la phase de acc (FFT)
  if (debug_in_key->key[i]==1) correctOffset = (correctOffset+bara)%Nx2;
    torusPolynomialMulByXai(testvectbis, correctOffset, testvect); //celui-ci, c'est la phase idéale (calculée sans bruit avec la clé privée)
  for (int j=0; j<N; j++) {
       printf("Iteration %d, index %d: phase %d vs noiseless %d\n",i,j,phase->coefsT[j], testvectbis->coefsT[j]);
  }
#endif

  }


  // Sample extract
  LweSample<TORUS>* u = new_LweSample<TORUS>(extract_params);
  tLweExtractLweSample(u, acc, extract_params, accum_params);
  u->b += ab;


  // KeySwitching
  lweKeySwitch(result, bk->ks, u);



  delete_LweSample(u);
  delete_TGswSampleFFT(tempFFT);
  delete_TGswSample(temp);
  delete_TLweSample(acc);
  delete_TorusPolynomial(testvectbis);
  delete_TorusPolynomial(testvect);
}
#endif

#undef INCLUDE_ALL
