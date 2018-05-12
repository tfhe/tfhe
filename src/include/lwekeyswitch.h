#ifndef LWEKEYSWITCH_H
#define LWEKEYSWITCH_H

///@file
///@brief This file contains the declaration of LWE keyswitch structures
// TODO: key switching function should take different input and output torus types

#include "tfhe_core.h"
#include "tfhe_alloc.h"
#include "lweparams.h"
#include "lwesamples.h"

template<typename TORUS>
struct LweKeySwitchKey: InitializerTag {
  int n; ///< length of the input key: s'
  int t; ///< decomposition length
  int basebit; ///< log_2(base)
  int base; ///< decomposition base: a power of 2
  const LweParams<TORUS>* out_params; ///< params of the output key s
  LweSample<TORUS>* ks0_raw; //tableau qui contient tout les Lwe samples de taille nlbase
  LweSample<TORUS>** ks1_raw;// de taille nl  pointe vers un tableau ks0_raw dont les cases sont espaceés de base positions
  LweSample<TORUS>*** ks; ///< the keyswitch elements: a n.l.base matrix
  // de taille n pointe vers ks1 un tableau dont les cases sont espaceés de ell positions

  LweKeySwitchKey(int n, int t, int basebit, const LweParams<TORUS>* out_params, LweSample<TORUS>* ks0_raw);
  ~LweKeySwitchKey();
  LweKeySwitchKey(const LweKeySwitchKey<TORUS>&) = delete;
  void operator=(const LweKeySwitchKey<TORUS>&) = delete;

  static void init_obj(
    LweKeySwitchKey<TORUS>* obj,
    const int n,
    const int t,
    const int basebit,
    const LweParams<TORUS>* out_params);
  static void destroy_obj(LweKeySwitchKey<TORUS>* obj);
};

template struct LweKeySwitchKey<Torus32>;
template struct LweKeySwitchKey<Torus64>;

//allocates and initialize the LweKeySwitchKey structure
//(equivalent of the C++ new)
template<typename TORUS>
inline LweKeySwitchKey<TORUS>* new_LweKeySwitchKey(int n, int t, int basebit, const LweParams<TORUS>* out_params) {
  return new_obj<LweKeySwitchKey<TORUS>>(n, t, basebit, out_params);
}
template<typename TORUS>
inline LweKeySwitchKey<TORUS>* new_LweKeySwitchKey_array(int nbelts, int n, int t, int basebit, const LweParams<TORUS>* out_params) {
  return new_obj_array<LweKeySwitchKey<TORUS>>(nbelts, n, t, basebit, out_params);
}

//destroys and frees the LweKeySwitchKey structure
//(equivalent of the C++ delete)
template<typename TORUS>
inline void delete_LweKeySwitchKey(LweKeySwitchKey<TORUS>* obj) {
  del_obj<LweKeySwitchKey<TORUS>>(obj);
}
template<typename TORUS>
inline void delete_LweKeySwitchKey_array(int nbelts, LweKeySwitchKey<TORUS>* obj) {
  del_obj_array<LweKeySwitchKey<TORUS>>(nbelts, obj);
}

#endif // LWEKEYSWITCH_H
