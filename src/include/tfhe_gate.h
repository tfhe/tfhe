#ifndef TFHE_GATE_H
#define TFHE_GATE_H

///@file
///@brief This file declares almost everything

#include "tfhe_core.h"
#include "numeric_functions.h"
#include "polynomials_arithmetic.h"
#include "lagrangehalfc_arithmetic.h"
#include "lwe-functions.h"
#include "tlwe_functions.h"
#include "tgsw_functions.h"
#include "lwekeyswitch.h"
#include "lwebootstrappingkey.h"

//////////////////////////////////////////
// Gate bootstrapping public interface
//////////////////////////////////////////

template<typename TORUS>
struct TFheGateBootstrappingParameterSet {
  const int ks_t;
  const int ks_basebit;
  const LweParams<TORUS>* const in_out_params;
  const TGswParams<TORUS>* const tgsw_params;

  TFheGateBootstrappingParameterSet(const int ks_t, const int ks_basebit, const LweParams<TORUS>* const in_out_params, const TGswParams<TORUS>* const tgsw_params)
  :
    ks_t(ks_t),
	  ks_basebit(ks_basebit),
	  in_out_params(in_out_params),
    tgsw_params(tgsw_params) { }

  /** generate default gate bootstrapping parameters */
  static TFheGateBootstrappingParameterSet<TORUS>* new_default(int minimum_lambda);

  /** deletes gate bootstrapping parameters */
  static void del(TFheGateBootstrappingParameterSet<TORUS>* params);
};

template<typename TORUS>
struct TFheGateBootstrappingCloudKeySet {
  const TFheGateBootstrappingParameterSet<TORUS>* const params;
  const LweBootstrappingKey<TORUS>* const bk;
  const LweBootstrappingKeyFFT<TORUS>* const bkFFT;

  TFheGateBootstrappingCloudKeySet(
    const TFheGateBootstrappingParameterSet<TORUS>* const params,
    const LweBootstrappingKey<TORUS>* const bk,
    const LweBootstrappingKeyFFT<TORUS>* const bkFFT)
  :
    params(params),
    bk(bk),
    bkFFT(bkFFT) {}

  /** deletes a gate bootstrapping secret key */
  static void del(TFheGateBootstrappingCloudKeySet<TORUS>* keyset);
};

template<typename TORUS>
struct TFheGateBootstrappingSecretKeySet {
  const TFheGateBootstrappingParameterSet<TORUS>* params;
  const LweKey<TORUS>* lwe_key;
  const TGswKey<TORUS>* tgsw_key;
  const TFheGateBootstrappingCloudKeySet<TORUS> cloud;

  TFheGateBootstrappingSecretKeySet(
    const TFheGateBootstrappingParameterSet<TORUS>* const params,
    const LweBootstrappingKey<TORUS>* const bk,
    const LweBootstrappingKeyFFT<TORUS>* const bkFFT,
    const LweKey<TORUS>* lwe_key,
    const TGswKey<TORUS>* tgsw_key)
  :
	  params(params),
    lwe_key(lwe_key),
	  tgsw_key(tgsw_key),
	  cloud(params,bk,bkFFT) { }

  /** generate a random gate bootstrapping secret key */
  static TFheGateBootstrappingSecretKeySet<TORUS>* new_random(const TFheGateBootstrappingParameterSet<TORUS>* params);

  /** deletes a gate bootstrapping secret key */
  static void del(TFheGateBootstrappingSecretKeySet<TORUS>* keyset);
};

template struct TFheGateBootstrappingParameterSet<Torus32>;
template struct TFheGateBootstrappingParameterSet<Torus64>;
template struct TFheGateBootstrappingCloudKeySet<Torus32>;
template struct TFheGateBootstrappingCloudKeySet<Torus64>;
template struct TFheGateBootstrappingSecretKeySet<Torus32>;
template struct TFheGateBootstrappingSecretKeySet<Torus64>;

/** generate default gate bootstrapping parameters */
template<typename TORUS>
inline TFheGateBootstrappingParameterSet<TORUS>* new_default_gate_bootstrapping_parameters(int minimum_lambda) {
  return TFheGateBootstrappingParameterSet<TORUS>::new_default(minimum_lambda);
}

/** deletes gate bootstrapping parameters */
template<typename TORUS>
inline void delete_gate_bootstrapping_parameters(TFheGateBootstrappingParameterSet<TORUS>* params) {
  TFheGateBootstrappingParameterSet<TORUS>::del(params);
}

/** generate a random gate bootstrapping secret key */
template<typename TORUS>
inline TFheGateBootstrappingSecretKeySet<TORUS>* new_random_gate_bootstrapping_secret_keyset(const TFheGateBootstrappingParameterSet<TORUS>* params) {
  return TFheGateBootstrappingSecretKeySet<TORUS>::new_random(params);
}

/** deletes a gate bootstrapping secret key */
template<typename TORUS>
inline void delete_gate_bootstrapping_secret_keyset(TFheGateBootstrappingSecretKeySet<TORUS>* keyset) {
  TFheGateBootstrappingSecretKeySet<TORUS>::del(keyset);
}

/** deletes a gate bootstrapping secret key */
template<typename TORUS>
inline void delete_gate_bootstrapping_cloud_keyset(TFheGateBootstrappingCloudKeySet<TORUS>* keyset) {
  TFheGateBootstrappingCloudKeySet<TORUS>::del(keyset);
}

#endif //TFHE_GATE_H
