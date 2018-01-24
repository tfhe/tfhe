#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include "tfhe_gate.h"
#include "tfhe_functions.h"
#include "random_gen.h"

using namespace std;

#define TORUS Torus32

// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************


void dieDramatically(string message) {
  cerr << message << endl;
  abort();
}


//EXPORT void tLweExtractKey(LweKey* result, const TLweKey* key); //TODO: change the name and put in a .h
//EXPORT void tfhe_createLweBootstrappingKeyFFT(LweBootstrappingKeyFFT* bk, const LweKey* key_in, const TGswKey* rgsw_key);
//EXPORT void tfhe_bootstrapFFT(LweSample<TORUS>* result, const LweBootstrappingKeyFFT* bk, TORUS mu1, TORUS mu0, const LweSample<TORUS>* x);


// #ifndef NDEBUG
// extern const TLweKey* debug_accum_key;
// extern const LweKey* debug_extract_key;
// extern const LweKey* debug_in_key;
// #endif

int main(int argc, char** argv) {
#ifndef NDEBUG
  cout << "DEBUG MODE!" << endl;
#endif
  const int nb_samples = 50;
  const TORUS mu_boot = TorusUtils<TORUS>::modSwitchToTorus(1,8);

  // generate params 
  int minimum_lambda = 100;
  TFheGateBootstrappingParameterSet<TORUS>* params = new_default_gate_bootstrapping_parameters<TORUS>(minimum_lambda);
  const LweParams<TORUS>* in_out_params = params->in_out_params;
  // generate the secret keyset
  TFheGateBootstrappingSecretKeySet<TORUS>* keyset = new_random_gate_bootstrapping_secret_keyset(params);

  // generate input samples
  LweSample<TORUS>* test_in = new_LweSample_array(nb_samples, in_out_params);
  for (int i = 0; i < nb_samples; ++i) {
    lweSymEncrypt(test_in+i, TorusUtils<TORUS>::modSwitchToTorus(i,nb_samples), 0.01, keyset->lwe_key);
  }
  // output samples
  LweSample<TORUS>* test_out = new_LweSample_array(nb_samples, in_out_params);


  // bootstrap input samples
  cout << "starting bootstrapping..." << endl;
  clock_t begin = clock();
  for (int i = 0; i < nb_samples; ++i) {
    tfhe_bootstrap_FFT(test_out+i, keyset->cloud.bkFFT, mu_boot, test_in+i);
  }
  clock_t end = clock();
  cout << "finished " << nb_samples << " bootstrappings" << endl;
  cout << "time per bootstrapping (microsecs)... " << (end-begin)/double(nb_samples) << endl;

  for (int i = 0; i < nb_samples; ++i) {
    TORUS phase = lwePhase(test_out+i, keyset->lwe_key);
    cout << "phase " << i << " = " << TorusUtils<TORUS>::to_double(phase) << endl;
  }

  delete_LweSample_array(nb_samples,test_out);
  delete_LweSample_array(nb_samples,test_in);
  delete_gate_bootstrapping_secret_keyset(keyset);
  delete_gate_bootstrapping_parameters(params);

  return 0;
}
