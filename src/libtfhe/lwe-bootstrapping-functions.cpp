#ifndef TFHE_TEST_ENVIRONMENT
#include <iostream>
#include <cassert>
#include "tfhe_gate.h"
#include "tfhe_functions.h"
using namespace std;
#define INCLUDE_ALL
#else
#undef EXPORT
#define EXPORT
#endif


template<typename TORUS>
void TfheFunctions<TORUS>::MuxRotate(TLweSample<TORUS>* result, const TLweSample<TORUS>* accum, const TGswSample<TORUS>* bki, const int barai, const TGswParams<TORUS>* bk_params) {
  // ACC = BKi*[(X^barai-1)*ACC]+ACC
  // temp = (X^barai-1)*ACC
  tLweMulByXaiMinusOne(result, barai, accum, bk_params->tlwe_params);
  // temp *= BKi
  tGswExternMulToTLwe(result, bki, bk_params);
  // ACC += temp
  tLweAddTo(result, accum, bk_params->tlwe_params);
}





#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BLIND_ROTATE
#undef INCLUDE_TFHE_BLIND_ROTATE
/**
 * multiply the accumulator by X^sum(bara_i.s_i)
 * @param accum the TLWE sample to multiply
 * @param bk An array of n TGSW samples where bk_i encodes s_i
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
template<typename TORUS>
void TfheFunctions<TORUS>::blindRotate(TLweSample<TORUS>* accum, const TGswSample<TORUS>* bk, const int* bara, const int n, const TGswParams<TORUS>* bk_params) {

  //TGswSample<TORUS>* temp = new_TGswSample<TORUS>(bk_params);
  TLweSample<TORUS>* temp = new_TLweSample<TORUS>(bk_params->tlwe_params);
  TLweSample<TORUS>* temp2 = temp;
  TLweSample<TORUS>* temp3 = accum;

  for (int i=0; i<n; i++) {
  const int barai=bara[i];
  if (barai==0) continue; //indeed, this is an easy case!

  tfhe_MuxRotate(temp2, temp3, bk+i, barai, bk_params);
  swap(temp2, temp3);

  }
  if (temp3 != accum) {
    tLweCopy(accum, temp3, bk_params->tlwe_params);
  }

  delete_TLweSample(temp);
  //delete_TGswSample(temp);
}
#endif




#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BLIND_ROTATE_AND_EXTRACT
#undef INCLUDE_TFHE_BLIND_ROTATE_AND_EXTRACT
/**
 * result = LWE(v_p) where p=barb-sum(bara_i.s_i) mod 2N
 * @param result the output LWE sample
 * @param v a 2N-elt anticyclic function (represented by a TorusPolynomial)
 * @param bk An array of n TGSW samples where bk_i encodes s_i
 * @param barb A coefficients between 0 and 2N-1
 * @param bara An array of n coefficients between 0 and 2N-1
 * @param bk_params The parameters of bk
 */
template<typename TORUS>
void TfheFunctions<TORUS>::blindRotateAndExtract(LweSample<TORUS>* result,
  const TorusPolynomial<TORUS>* v,
  const TGswSample<TORUS>* bk,
  const int barb,
  const int* bara,
  const int n,
  const TGswParams<TORUS>* bk_params) {

  const TLweParams<TORUS>* accum_params = bk_params->tlwe_params;
  const LweParams<TORUS>* extract_params = &accum_params->extracted_lweparams;
  const int N = accum_params->N;
  const int _2N = 2*N;

  TorusPolynomial<TORUS>* testvectbis=new_TorusPolynomial<TORUS>(N);
  TLweSample<TORUS>* acc = new_TLweSample<TORUS>(accum_params);

  if (barb!=0) torusPolynomialMulByXai(testvectbis, _2N-barb, v);
  else torusPolynomialCopy(testvectbis, v);
  tLweNoiselessTrivial(acc, testvectbis, accum_params);
  tfhe_blindRotate(acc, bk, bara, n, bk_params);
  tLweExtractLweSample(result, acc, extract_params, accum_params);

  delete_TLweSample(acc);
  delete_TorusPolynomial(testvectbis);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BOOTSTRAP_WO_KS
#undef INCLUDE_TFHE_BOOTSTRAP_WO_KS
/**
 * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
 * @param result The resulting LweSample
 * @param bk The bootstrapping + keyswitch key
 * @param mu The output message (if phase(x)>0)
 * @param x The input sample
 */
template<typename TORUS>
void TfheFunctions<TORUS>::bootstrap_woKS(LweSample<TORUS>* result,
  const LweBootstrappingKey<TORUS>* bk,
  TORUS mu, const LweSample<TORUS>* x){

  const TGswParams<TORUS>* bk_params = bk->bk_params;
  const TLweParams<TORUS>* accum_params = bk->accum_params;
  const LweParams<TORUS>* in_params = bk->in_out_params;
  const int N=accum_params->N;
  const int Nx2= 2*N;
  const int n = in_params->n;

  TorusPolynomial<TORUS>* testvect=new_TorusPolynomial<TORUS>(N);
  int* bara = new int[N];

  int barb=TorusUtils<TORUS>::modSwitchFromTorus(x->b,Nx2);
  for (int i=0; i<n; i++) {
  bara[i]=TorusUtils<TORUS>::modSwitchFromTorus(x->a[i],Nx2);
  }

  //the initial testvec = [mu,mu,mu,...,mu]
  for (int i=0;i<N;i++) testvect->coefsT[i]=mu;

  tfhe_blindRotateAndExtract(result, testvect, bk->bk, barb, bara, n, bk_params);

  delete[] bara;
  delete_TorusPolynomial(testvect);
}
#endif


#if defined INCLUDE_ALL || defined INCLUDE_TFHE_BOOTSTRAP
#undef INCLUDE_TFHE_BOOTSTRAP
/**
 * result = LWE(mu) iff phase(x)>0, LWE(-mu) iff phase(x)<0
 * @param result The resulting LweSample
 * @param bk The bootstrapping + keyswitch key
 * @param mu The output message (if phase(x)>0)
 * @param x The input sample
 */
template<typename TORUS>
void TfheFunctions<TORUS>::bootstrap(LweSample<TORUS>* result,
  const LweBootstrappingKey<TORUS>* bk,
  TORUS mu, const LweSample<TORUS>* x){

  LweSample<TORUS>* u = new_LweSample<TORUS>(&bk->accum_params->extracted_lweparams);

  tfhe_bootstrap_woKS(u, bk, mu, x);
  // Key Switching
  lweKeySwitch(result, bk->ks, u);

  delete_LweSample(u);
}
#endif

#if defined INCLUDE_ALL || defined INCLUDE_TFHE_CREATEBOOTSTRAPPINGKEY
#undef INCLUDE_TFHE_CREATEBOOTSTRAPPINGKEY
template<typename TORUS>
void TfheFunctions<TORUS>::createLweBootstrappingKey(
  LweBootstrappingKey<TORUS>* bk,
  const LweKey<TORUS>* key_in,
  const TGswKey<TORUS>* rgsw_key) {
  assert(bk->bk_params==rgsw_key->params);
  assert(bk->in_out_params==key_in->params);

  const LweParams<TORUS>* in_out_params = bk->in_out_params;
  const TGswParams<TORUS>* bk_params = bk->bk_params;
  const TLweParams<TORUS>* accum_params = bk_params->tlwe_params;
  const LweParams<TORUS>* extract_params = &accum_params->extracted_lweparams;

  //LweKeySwitchKey<TORUS>* ks; ///< the keyswitch key (s'->s)
  const TLweKey<TORUS>* accum_key = &rgsw_key->tlwe_key;
  LweKey<TORUS>* extracted_key = new_LweKey<TORUS>(extract_params);
  tLweExtractKey(extracted_key, accum_key);
  lweCreateKeySwitchKey(bk->ks, extracted_key, key_in);
  delete_LweKey(extracted_key);

  //TGswSample<TORUS>* bk; ///< the bootstrapping key (s->s")
  int* kin = key_in->key;
  const double alpha = accum_params->alpha_min;
  const int n = in_out_params->n;
  //const int kpl = bk_params->kpl;
  //const int k = accum_params->k;
  //const int N = accum_params->N;
  //cout << "create the bootstrapping key bk ("  << "  " << n*kpl*(k+1)*N*4 << " bytes)" << endl;
  //cout << "  with noise_stdev: " << alpha << endl;
  for (int i=0; i<n; i++) {
  tGswSymEncryptInt(&bk->bk[i], kin[i], alpha, rgsw_key);
  }
}
#endif

#undef INCLUDE_ALL
