#ifndef LWEKEY_H
#define LWEKEY_H

///@file
///@brief This file contains the declaration of LWE key structures

#include "tfhe_core.h"
#include "tfhe_alloc.h"

template<typename TORUS>
struct LweKey {
  const LweParams<TORUS>* params;
  int* key;

  LweKey(const LweParams<TORUS>* params);
  ~LweKey();
  LweKey(const LweKey<TORUS>&) = delete; //forbidden
  LweKey<TORUS>* operator=(const LweKey<TORUS>&) = delete; //forbidden
};

template struct LweKey<Torus32>;
template struct LweKey<Torus64>;

/** allocates and initializes (constructor) a single LweKey
 * @param params the LWE parameters to use
 */
template<typename TORUS>
inline LweKey<TORUS>* new_LweKey(const LweParams<TORUS>* params) {
  return new_obj<LweKey<TORUS>>(params);
}
/** allocates and initializes (constructor) a single LweKey
 * @param nbelts the number of consecutive elements to create
 * @param params the LWE parameters to use
 */
template<typename TORUS>
inline LweKey<TORUS>* new_LweKey_array(int nbelts, const LweParams<TORUS>* params) {
  return new_obj_array<LweKey<TORUS>>(nbelts, params);
}

/** destroy and frees memory space for a single LweKey
 * @param ptr the pointer to release */
template<typename TORUS>
inline void delete_LweKey(LweKey<TORUS>* obj) {
  del_obj<LweKey<TORUS>>(obj);
}
/** destroys and free memory space for an array of LweKey
 * @param nbelts the number of elements
 * @param ptr the pointer to release */
template<typename TORUS>
inline void delete_LweKey_array(int nbelts, LweKey<TORUS>* obj) {
  del_obj_array<LweKey<TORUS>>(nbelts, obj);
}





/**
 * Lwe public key
 */

template<typename TORUS>
struct LwePublicKey {
    const LwePublicParams<TORUS>* publicParams;
    LweSample<TORUS>* key;

    LwePublicKey(const LwePublicParams<TORUS>* publicParams);
    ~LwePublicKey();
    LwePublicKey(const LwePublicKey<TORUS>&) = delete; //forbidden
    LwePublicKey<TORUS>* operator=(const LwePublicKey<TORUS>&) = delete; //forbidden
};

template struct LwePublicKey<Torus32>;
template struct LwePublicKey<Torus64>;

/** allocates and initializes (constructor) a single LwePublicKey
 * @param params the LWE public parameters to use
 */
template<typename TORUS>
inline LwePublicKey<TORUS>* new_LwePublicKey(const LwePublicParams<TORUS>* publicParams) {
  return new_obj<LwePublicKey<TORUS>>(publicParams);
}
/** allocates and initializes (constructor) a single LweKey
 * @param nbelts the number of consecutive elements to create
 * @param params the LWE public parameters to use
 */
template<typename TORUS>
inline LwePublicKey<TORUS>* new_LwePublicKey_array(int nbelts, const LwePublicParams<TORUS>* publicParams) {
  return new_obj_array<LwePublicKey<TORUS>>(nbelts, publicParams);
}

/** destroy and frees memory space for a single LwePublicKey
 * @param ptr the pointer to release */
template<typename TORUS>
inline void delete_LwePublicKey(LwePublicKey<TORUS>* obj) {
  del_obj<LwePublicKey<TORUS>>(obj);
}
/** destroys and free memory space for an array of LwePublicKey
 * @param nbelts the number of elements
 * @param ptr the pointer to release */
template<typename TORUS>
inline void delete_LwePublicKey_array(int nbelts, LwePublicKey<TORUS>* obj) {
  del_obj_array<LwePublicKey<TORUS>>(nbelts, obj);
}

#endif //LWEKEY_H
