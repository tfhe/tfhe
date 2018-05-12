#ifndef LWEBOOTSTRAPPINGKEY_H
#define LWEBOOTSTRAPPINGKEY_H

///@file
///@brief This file contains the declaration of bootstrapping key structures

#include "tfhe_core.h"
#include "tfhe_alloc.h"

template<typename TORUS>
struct LweBootstrappingKey: InitializerTag {
  const LweParams<TORUS>* in_out_params; ///< paramètre de l'input et de l'output. key: s
  const TGswParams<TORUS>* bk_params; ///< params of the Gsw elems in bk. key: s"
  const TLweParams<TORUS>* accum_params; ///< params of the accum variable key: s"
  const LweParams<TORUS>* extract_params; ///< params after extraction: key: s'
  TGswSample<TORUS>* bk; ///< the bootstrapping key (s->s")
  LweKeySwitchKey<TORUS>* ks; ///< the keyswitch key (s'->s)

  inline LweBootstrappingKey(
    const LweParams<TORUS>* in_out_params,
    const TGswParams<TORUS>* bk_params,
    const TLweParams<TORUS>* accum_params,
    const LweParams<TORUS>* extract_params,
    TGswSample<TORUS>* bk,
    LweKeySwitchKey<TORUS>* ks)
  :
    in_out_params(in_out_params),
    bk_params(bk_params),
    accum_params(accum_params),
    extract_params(extract_params),
    bk(bk), ks(ks) { }

  inline ~LweBootstrappingKey() { }

  LweBootstrappingKey(const LweBootstrappingKey&) = delete;
  void operator=(const LweBootstrappingKey&) = delete;

  static void init_obj(
    LweBootstrappingKey<TORUS>* obj,
    const int ks_t,
    const int ks_basebit,
    const LweParams<TORUS>* in_out_params,
    const TGswParams<TORUS>* bk_params);
  static void destroy_obj(LweBootstrappingKey<TORUS>* obj);
};

template<typename TORUS>
struct LweBootstrappingKeyFFT: InitializerTag {
  const LweParams<TORUS>* in_out_params; ///< paramètre de l'input et de l'output. key: s
  const TGswParams<TORUS>* bk_params; ///< params of the Gsw elems in bk. key: s"
  const TLweParams<TORUS>* accum_params; ///< params of the accum variable key: s"
  const LweParams<TORUS>* extract_params; ///< params after extraction: key: s'
  const TGswSampleFFT<TORUS>* bkFFT; ///< the bootstrapping key (s->s")
  const LweKeySwitchKey<TORUS>* ks; ///< the keyswitch key (s'->s)

  inline LweBootstrappingKeyFFT(
    const LweParams<TORUS>* in_out_params,
    const TGswParams<TORUS>* bk_params,
    const TLweParams<TORUS>* accum_params,
    const LweParams<TORUS>* extract_params,
    const TGswSampleFFT<TORUS>* bkFFT,
    const LweKeySwitchKey<TORUS>* ks)
  :
    in_out_params(in_out_params),
    bk_params(bk_params),
    accum_params(accum_params),
    extract_params(extract_params),
    bkFFT(bkFFT), ks(ks) { }

  inline ~LweBootstrappingKeyFFT() { }

  LweBootstrappingKeyFFT(const LweBootstrappingKeyFFT&) = delete;
  void operator=(const LweBootstrappingKeyFFT&) = delete;

  static void init_obj(LweBootstrappingKeyFFT<TORUS>* obj, const LweBootstrappingKey<TORUS>* bk);
  static void destroy_obj(LweBootstrappingKeyFFT<TORUS>* obj);
};

template struct LweBootstrappingKey<Torus32>;
template struct LweBootstrappingKey<Torus64>;
template struct LweBootstrappingKeyFFT<Torus32>;
template struct LweBootstrappingKeyFFT<Torus64>;


//allocates and initialize the LweBootstrappingKey structure
//(equivalent of the C++ new)
template<typename TORUS>
inline LweBootstrappingKey<TORUS>* new_LweBootstrappingKey(const int ks_t, const int ks_basebit, const LweParams<TORUS>* in_out_params, const TGswParams<TORUS>* bk_params) {
  return new_obj<LweBootstrappingKey<TORUS>>(ks_t, ks_basebit, in_out_params, bk_params);
}
template<typename TORUS>
inline LweBootstrappingKey<TORUS>* new_LweBootstrappingKey_array(int nbelts, const int ks_t, const int ks_basebit, const LweParams<TORUS>* in_out_params, const TGswParams<TORUS>* bk_params) {
  return new_obj_array<LweBootstrappingKey<TORUS>>(nbelts, ks_t, ks_basebit, in_out_params, bk_params);
}

//destroys and frees the LweBootstrappingKey structure
//(equivalent of the C++ delete)
template<typename TORUS>
inline void delete_LweBootstrappingKey(LweBootstrappingKey<TORUS>* obj) {
  del_obj<LweBootstrappingKey<TORUS>>(obj);
}
template<typename TORUS>
inline void delete_LweBootstrappingKey_array(int nbelts, LweBootstrappingKey<TORUS>* obj) {
  del_obj_array<LweBootstrappingKey<TORUS>>(nbelts, obj);
}

//allocates and initialize the LweBootstrappingKeyFFT structure
//(equivalent of the C++ new)
template<typename TORUS>
inline LweBootstrappingKeyFFT<TORUS>* new_LweBootstrappingKeyFFT(const LweBootstrappingKey<TORUS>* bk) {
  return new_obj<LweBootstrappingKeyFFT<TORUS>>(bk);
}
template<typename TORUS>
inline LweBootstrappingKeyFFT<TORUS>* new_LweBootstrappingKeyFFT_array(int nbelts, const LweBootstrappingKey<TORUS>* bk) {
  return new_obj_array<LweBootstrappingKeyFFT<TORUS>>(nbelts, bk);
}

//destroys and frees the LweBootstrappingKeyFFT structure
//(equivalent of the C++ delete)
template<typename TORUS>
inline void delete_LweBootstrappingKeyFFT(LweBootstrappingKeyFFT<TORUS>* obj) {
  del_obj<LweBootstrappingKeyFFT<TORUS>>(obj);
}
template<typename TORUS>
inline void delete_LweBootstrappingKeyFFT_array(int nbelts, LweBootstrappingKeyFFT<TORUS>* obj) {
  del_obj_array<LweBootstrappingKeyFFT<TORUS>>(nbelts, obj);
}


#endif
