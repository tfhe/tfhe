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

#endif //LWEKEY_H
