#ifndef TFHE_TEST_ENVIRONMENT

#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include "tfhe_core.h"
#include "numeric_functions.h"
#include "lweparams.h"
#include "lwekey.h"
#include "lwesamples.h"
#include "lwekeyswitch.h"
#include "lwe-functions.h"
#include "lwebootstrappingkey.h"
#include "tfhe_gate.h"
#include "tfhe_gate_functions.h"
#include "tfhe_functions.h"

using namespace std;
#else
#undef EXPORT
#define EXPORT static
#endif


//*//*****************************************
// zones on the torus -> to see
//*//*****************************************


/*
 * Homomorphic bootstrapped NAND gate
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
template<typename TORUS>
void TfheGateFunctions<TORUS>::NAND(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  static const TORUS MU = TorusUtils<TORUS>::modSwitchToTorus(1,8);
  const LweParams<TORUS>* in_out_params = bk->params->in_out_params;

  LweSample<TORUS>* temp_result = new_LweSample<TORUS>(in_out_params);

  //compute: (0,1/8) - ca - cb
  static const TORUS NandConst=TorusUtils<TORUS>::modSwitchToTorus(1,8);
  lweNoiselessTrivial(temp_result, NandConst, in_out_params);
  lweSubTo(temp_result, ca, in_out_params);
  lweSubTo(temp_result, cb, in_out_params);

  //if the phase is positive, the result is 1/8
  //if the phase is positive, else the result is -1/8
  tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

  delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped OR gate
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
template<typename TORUS>
void TfheGateFunctions<TORUS>::OR(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  static const TORUS MU = TorusUtils<TORUS>::modSwitchToTorus(1,8);
  const LweParams<TORUS>* in_out_params = bk->params->in_out_params;

  LweSample<TORUS>* temp_result = new_LweSample<TORUS>(in_out_params);

  //compute: (0,1/8) + ca + cb
  static const TORUS OrConst=TorusUtils<TORUS>::modSwitchToTorus(1,8);
  lweNoiselessTrivial(temp_result, OrConst, in_out_params);
  lweAddTo(temp_result, ca, in_out_params);
  lweAddTo(temp_result, cb, in_out_params);

  //if the phase is positive, the result is 1/8
  //if the phase is positive, else the result is -1/8
  tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

  delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped AND gate
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
template<typename TORUS>
void TfheGateFunctions<TORUS>::AND(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  static const TORUS MU = TorusUtils<TORUS>::modSwitchToTorus(1,8);
  const LweParams<TORUS>* in_out_params = bk->params->in_out_params;

  LweSample<TORUS>* temp_result = new_LweSample<TORUS>(in_out_params);

  //compute: (0,-1/8) + ca + cb
  static const TORUS AndConst=TorusUtils<TORUS>::modSwitchToTorus(-1,8);
  lweNoiselessTrivial(temp_result, AndConst, in_out_params);
  lweAddTo(temp_result, ca, in_out_params);
  lweAddTo(temp_result, cb, in_out_params);

  //if the phase is positive, the result is 1/8
  //if the phase is positive, else the result is -1/8
  tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

  delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped XOR gate
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
template<typename TORUS>
void TfheGateFunctions<TORUS>::XOR(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  static const TORUS MU = TorusUtils<TORUS>::modSwitchToTorus(1,8);
  const LweParams<TORUS>* in_out_params = bk->params->in_out_params;

  LweSample<TORUS>* temp_result = new_LweSample<TORUS>(in_out_params);

  //compute: (0,1/4) + 2*(ca + cb)
  static const TORUS XorConst=TorusUtils<TORUS>::modSwitchToTorus(1,4);
  lweNoiselessTrivial(temp_result, XorConst, in_out_params);
  lweAddMulTo(temp_result, 2, ca, in_out_params);
  lweAddMulTo(temp_result, 2, cb, in_out_params);

  //if the phase is positive, the result is 1/8
  //if the phase is positive, else the result is -1/8
  tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

  delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped XNOR gate
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
template<typename TORUS>
void TfheGateFunctions<TORUS>::XNOR(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  static const TORUS MU = TorusUtils<TORUS>::modSwitchToTorus(1,8);
  const LweParams<TORUS>* in_out_params = bk->params->in_out_params;

  LweSample<TORUS>* temp_result = new_LweSample<TORUS>(in_out_params);

  //compute: (0,-1/4) + 2*(-ca-cb)
  static const TORUS XnorConst=TorusUtils<TORUS>::modSwitchToTorus(-1,4);
  lweNoiselessTrivial(temp_result, XnorConst, in_out_params);
  lweSubMulTo(temp_result, 2, ca, in_out_params);
  lweSubMulTo(temp_result, 2, cb, in_out_params);

  //if the phase is positive, the result is 1/8
  //if the phase is positive, else the result is -1/8
  tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

  delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped NOT gate (doesn't need to be bootstrapped)
 * Takes in input 1 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE sample (with message space [-1/8,1/8], noise<1/16)
*/
template<typename TORUS>
void TfheGateFunctions<TORUS>::NOT(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  const LweParams<TORUS>* in_out_params = bk->params->in_out_params;
  lweNegate(result, ca, in_out_params);
}


/*
 * Homomorphic bootstrapped COPY gate (doesn't need to be bootstrapped)
 * Takes in input 1 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE sample (with message space [-1/8,1/8], noise<1/16)
*/
template<typename TORUS>
void TfheGateFunctions<TORUS>::COPY(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  const LweParams<TORUS>* in_out_params = bk->params->in_out_params;
  lweCopy(result, ca, in_out_params);
}

/*
 * Homomorphic Trivial Constant gate (doesn't need to be bootstrapped)
 * Takes a boolean value)
 * Outputs a LWE sample (with message space [-1/8,1/8], noise<1/16)
*/
template<typename TORUS>
void TfheGateFunctions<TORUS>::CONSTANT(LweSample<TORUS>* result, int value, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  const LweParams<TORUS>* in_out_params = bk->params->in_out_params;
  static const TORUS MU=TorusUtils<TORUS>::modSwitchToTorus(1,8);
  lweNoiselessTrivial(result, value?MU:-MU, in_out_params);
}


/*
 * Homomorphic bootstrapped NOR gate
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
template<typename TORUS>
void TfheGateFunctions<TORUS>::NOR(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  static const TORUS MU = TorusUtils<TORUS>::modSwitchToTorus(1,8);
  const LweParams<TORUS>* in_out_params = bk->params->in_out_params;

  LweSample<TORUS>* temp_result = new_LweSample<TORUS>(in_out_params);

  //compute: (0,-1/8) - ca - cb
  static const TORUS NorConst=TorusUtils<TORUS>::modSwitchToTorus(-1,8);
  lweNoiselessTrivial(temp_result, NorConst, in_out_params);
  lweSubTo(temp_result, ca, in_out_params);
  lweSubTo(temp_result, cb, in_out_params);

  //if the phase is positive, the result is 1/8
  //if the phase is positive, else the result is -1/8
  tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

  delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped AndNY Gate: not(a) and b
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
template<typename TORUS>
void TfheGateFunctions<TORUS>::ANDNY(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  static const TORUS MU = TorusUtils<TORUS>::modSwitchToTorus(1,8);
  const LweParams<TORUS>* in_out_params = bk->params->in_out_params;

  LweSample<TORUS>* temp_result = new_LweSample<TORUS>(in_out_params);

  //compute: (0,-1/8) - ca + cb
  static const TORUS AndNYConst=TorusUtils<TORUS>::modSwitchToTorus(-1,8);
  lweNoiselessTrivial(temp_result, AndNYConst, in_out_params);
  lweSubTo(temp_result, ca, in_out_params);
  lweAddTo(temp_result, cb, in_out_params);

  //if the phase is positive, the result is 1/8
  //if the phase is positive, else the result is -1/8
  tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

  delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped AndYN Gate: a and not(b)
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
template<typename TORUS>
void TfheGateFunctions<TORUS>::ANDYN(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  static const TORUS MU = TorusUtils<TORUS>::modSwitchToTorus(1,8);
  const LweParams<TORUS>* in_out_params = bk->params->in_out_params;

  LweSample<TORUS>* temp_result = new_LweSample<TORUS>(in_out_params);

  //compute: (0,-1/8) + ca - cb
  static const TORUS AndYNConst=TorusUtils<TORUS>::modSwitchToTorus(-1,8);
  lweNoiselessTrivial(temp_result, AndYNConst, in_out_params);
  lweAddTo(temp_result, ca, in_out_params);
  lweSubTo(temp_result, cb, in_out_params);

  //if the phase is positive, the result is 1/8
  //if the phase is positive, else the result is -1/8
  tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

  delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped OrNY Gate: not(a) or b
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
template<typename TORUS>
void TfheGateFunctions<TORUS>::ORNY(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  static const TORUS MU = TorusUtils<TORUS>::modSwitchToTorus(1,8);
  const LweParams<TORUS>* in_out_params = bk->params->in_out_params;

  LweSample<TORUS>* temp_result = new_LweSample<TORUS>(in_out_params);

  //compute: (0,1/8) - ca + cb
  static const TORUS OrNYConst=TorusUtils<TORUS>::modSwitchToTorus(1,8);
  lweNoiselessTrivial(temp_result, OrNYConst, in_out_params);
  lweSubTo(temp_result, ca, in_out_params);
  lweAddTo(temp_result, cb, in_out_params);

  //if the phase is positive, the result is 1/8
  //if the phase is positive, else the result is -1/8
  tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

  delete_LweSample(temp_result);
}


/*
 * Homomorphic bootstrapped OrYN Gate: a or not(b)
 * Takes in input 2 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
template<typename TORUS>
void TfheGateFunctions<TORUS>::ORYN(LweSample<TORUS>* result, const LweSample<TORUS>* ca, const LweSample<TORUS>* cb, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  static const TORUS MU = TorusUtils<TORUS>::modSwitchToTorus(1,8);
  const LweParams<TORUS>* in_out_params = bk->params->in_out_params;

  LweSample<TORUS>* temp_result = new_LweSample<TORUS>(in_out_params);

  //compute: (0,1/8) + ca - cb
  static const TORUS OrYNConst=TorusUtils<TORUS>::modSwitchToTorus(1,8);
  lweNoiselessTrivial(temp_result, OrYNConst, in_out_params);
  lweAddTo(temp_result, ca, in_out_params);
  lweSubTo(temp_result, cb, in_out_params);

  //if the phase is positive, the result is 1/8
  //if the phase is positive, else the result is -1/8
  tfhe_bootstrap_FFT(result, bk->bkFFT, MU, temp_result);

  delete_LweSample(temp_result);
}




/*
 * Homomorphic bootstrapped Mux(a,b,c) = a?b:c = a*b + not(a)*c
 * Takes in input 3 LWE samples (with message space [-1/8,1/8], noise<1/16)
 * Outputs a LWE bootstrapped sample (with message space [-1/8,1/8], noise<1/16)
*/
template<typename TORUS>
void TfheGateFunctions<TORUS>::MUX(LweSample<TORUS>* result, const LweSample<TORUS>* a, const LweSample<TORUS>* b, const LweSample<TORUS>* c, const TFheGateBootstrappingCloudKeySet<TORUS>* bk) {
  static const TORUS MU = TorusUtils<TORUS>::modSwitchToTorus(1,8);
  const LweParams<TORUS>* in_out_params = bk->params->in_out_params;
  const LweParams<TORUS>* extracted_params = &bk->params->tgsw_params->tlwe_params->extracted_lweparams;

  LweSample<TORUS>* temp_result = new_LweSample<TORUS>(in_out_params);
  LweSample<TORUS>* temp_result1 = new_LweSample<TORUS>(extracted_params);
  LweSample<TORUS>* u1 = new_LweSample<TORUS>(extracted_params);
  LweSample<TORUS>* u2 = new_LweSample<TORUS>(extracted_params);


  //compute "AND(a,b)": (0,-1/8) + a + b
  static const TORUS AndConst=TorusUtils<TORUS>::modSwitchToTorus(-1,8);
  lweNoiselessTrivial(temp_result, AndConst, in_out_params);
  lweAddTo(temp_result, a, in_out_params);
  lweAddTo(temp_result, b, in_out_params);
  // Bootstrap without KeySwitch
  tfhe_bootstrap_woKS_FFT(u1, bk->bkFFT, MU, temp_result);


  //compute "AND(not(a),c)": (0,-1/8) - a + c
  lweNoiselessTrivial(temp_result, AndConst, in_out_params);
  lweSubTo(temp_result, a, in_out_params);
  lweAddTo(temp_result, c, in_out_params);
  // Bootstrap without KeySwitch
  tfhe_bootstrap_woKS_FFT(u2, bk->bkFFT, MU, temp_result);

  // Add u1=u1+u2
  static const TORUS MuxConst=TorusUtils<TORUS>::modSwitchToTorus(1,8);
  lweNoiselessTrivial(temp_result1, MuxConst, extracted_params);
  lweAddTo(temp_result1, u1, extracted_params);
  lweAddTo(temp_result1, u2, extracted_params);
  // Key switching
    lweKeySwitch(result, bk->bkFFT->ks, temp_result1);


    delete_LweSample(u2);
    delete_LweSample(u1);
    delete_LweSample(temp_result1);
    delete_LweSample(temp_result);
}



