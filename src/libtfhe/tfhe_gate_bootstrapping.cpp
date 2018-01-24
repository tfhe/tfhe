#include <cstdio>
#include <iostream>
#include "tfhe_gate.h"
#include "tfhe_functions.h"
#include "tfhe_gate_functions.h"
#include "tfhe_garbage_collector.h"

using namespace std;

struct Exception42 {};

void die_dramatically(const char* message) {
  cerr << message << endl;
  abort(); throw Exception42();
}

/**
 * multiplies by sqrt(2/pi)  to convert a "litterature" gaussian
 * parameter into a standard deviation
 */
constexpr inline double mulBySqrtTwoOverPi(double x) { return x*sqrt(2./M_PI); }

/** generate default gate bootstrapping parameters */
template<typename TORUS>
TFheGateBootstrappingParameterSet<TORUS>* TFheGateBootstrappingParameterSet<TORUS>::new_default(int minimum_lambda) {
  if (minimum_lambda>128)
    die_dramatically("Sorry, for now, the parameters are only implemented for about 128bit of security!");

  static const int N = 1024;
  static const int k = 1;
  static const int n = 500;
  static const int bk_l = 3;
  static const int bk_Bgbit = 10;
  static const int ks_basebit = 1;
  static const int ks_length = 15;
  static const double ks_stdev = mulBySqrtTwoOverPi(pow(2.,-15));   //standard deviation
  static const double bk_stdev = mulBySqrtTwoOverPi(9.e-9);      //standard deviation
  static const double max_stdev = mulBySqrtTwoOverPi(pow(2.,-4)/4.); //max standard deviation for a 1/4 msg space

  LweParams<TORUS>* params_in = new_LweParams<TORUS>(n, ks_stdev, max_stdev);
  TLweParams<TORUS>* params_accum = new_TLweParams<TORUS>(N, k, bk_stdev, max_stdev);
  TGswParams<TORUS>* params_bk = new_TGswParams<TORUS>(bk_l, bk_Bgbit, params_accum);

  TfheGarbageCollector<TORUS>::register_param(params_in);
  TfheGarbageCollector<TORUS>::register_param(params_accum);
  TfheGarbageCollector<TORUS>::register_param(params_bk);

  return new TFheGateBootstrappingParameterSet(ks_length, ks_basebit, params_in, params_bk);
}

/** deletes gate bootstrapping parameters */
template<typename TORUS>
void TFheGateBootstrappingParameterSet<TORUS>::del(TFheGateBootstrappingParameterSet<TORUS>* params) {
  delete params;
}

/** generate a gate bootstrapping secret key */
template<typename TORUS>
TFheGateBootstrappingSecretKeySet<TORUS>* TFheGateBootstrappingSecretKeySet<TORUS>::new_random(const TFheGateBootstrappingParameterSet<TORUS>* params) {
  LweKey<TORUS>* lwe_key = new_LweKey<TORUS>(params->in_out_params);
  lweKeyGen(lwe_key);
  TGswKey<TORUS>* tgsw_key = new_TGswKey<TORUS>(params->tgsw_params);
  tGswKeyGen(tgsw_key);
  LweBootstrappingKey<TORUS>* bk = new_LweBootstrappingKey<TORUS>(params->ks_t, params->ks_basebit, params->in_out_params, params->tgsw_params);
  tfhe_createLweBootstrappingKey(bk, lwe_key, tgsw_key);
  LweBootstrappingKeyFFT<TORUS>* bkFFT = new_LweBootstrappingKeyFFT<TORUS>(bk);
  return new TFheGateBootstrappingSecretKeySet(params, bk, bkFFT, lwe_key, tgsw_key);
}

/** deletes a gate bootstrapping secret key */
template<typename TORUS>
void TFheGateBootstrappingSecretKeySet<TORUS>::del(TFheGateBootstrappingSecretKeySet<TORUS>* keyset) {
  LweKey<TORUS>* lwe_key = (LweKey<TORUS>*) keyset->lwe_key;
  TGswKey<TORUS>* tgsw_key = (TGswKey<TORUS>*) keyset->tgsw_key;
  LweBootstrappingKey<TORUS>* bk = (LweBootstrappingKey<TORUS>*) keyset->cloud.bk;
  LweBootstrappingKeyFFT<TORUS>* bkFFT = (LweBootstrappingKeyFFT<TORUS>*) keyset->cloud.bkFFT;
  if (bkFFT) delete_LweBootstrappingKeyFFT(bkFFT);
  if (bk) delete_LweBootstrappingKey(bk);
  delete_TGswKey(tgsw_key);
  delete_LweKey(lwe_key);
  delete keyset;
}

/** deletes a gate bootstrapping cloud key */
template<typename TORUS>
void TFheGateBootstrappingCloudKeySet<TORUS>::del(TFheGateBootstrappingCloudKeySet<TORUS>* keyset) {
  LweBootstrappingKey<TORUS>* bk = (LweBootstrappingKey<TORUS>*) keyset->bk;
  LweBootstrappingKeyFFT<TORUS>* bkFFT = (LweBootstrappingKeyFFT<TORUS>*) keyset->bkFFT;
  if (bkFFT) delete_LweBootstrappingKeyFFT(bkFFT);
  if (bk) delete_LweBootstrappingKey(bk);
  delete keyset;
}

/** generate a new unititialized ciphertext */
template<typename TORUS>
LweSample<TORUS>* TfheGateFunctions<TORUS>::new_ctxt(const TFheGateBootstrappingParameterSet<TORUS>* params) {
  return new_LweSample<TORUS>(params->in_out_params);
}

/** generate a new unititialized ciphertext array of length nbelems */
template<typename TORUS>
LweSample<TORUS>* TfheGateFunctions<TORUS>::new_ctxt_array(int nbelems, const TFheGateBootstrappingParameterSet<TORUS>* params) {
  return new_LweSample_array<TORUS>(nbelems, params->in_out_params);
}

/** deletes a ciphertext */
template<typename TORUS>
void TfheGateFunctions<TORUS>::del_ctxt(LweSample<TORUS>* sample) {
  delete_LweSample(sample);
}

/** deletes a ciphertext array of length nbelems */
template<typename TORUS>
void TfheGateFunctions<TORUS>::del_ctxt_array(int nbelems, LweSample<TORUS>* samples) {
  delete_LweSample_array(nbelems, samples);
}

/** encrypts a boolean */
template<typename TORUS>
void TfheGateFunctions<TORUS>::SymEncrypt(LweSample<TORUS>* result, int message, const TFheGateBootstrappingSecretKeySet<TORUS>* key) {
  TORUS _1s8 = TorusUtils<TORUS>::modSwitchToTorus(1,8);
  TORUS mu = message?_1s8:-_1s8;
  double alpha = key->params->in_out_params->alpha_min; //TODO: specify noise
  lweSymEncrypt(result, mu, alpha, key->lwe_key);
}

/** decrypts a boolean */
template<typename TORUS>
int TfheGateFunctions<TORUS>::SymDecrypt(const LweSample<TORUS>* sample, const TFheGateBootstrappingSecretKeySet<TORUS>* key) {
  TORUS mu = lwePhase(sample, key->lwe_key);
  return (mu>0?1:0); //we have to do that because of the C binding
}
