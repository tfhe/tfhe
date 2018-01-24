#ifndef LWESAMPLES_H
#define LWESAMPLES_H

///@file
///@brief This file contains the declaration of lwesamples structures

#include "tfhe_core.h"
#include "tfhe_alloc.h"

template<typename TORUS>
struct LweSample {
  TORUS* a; //-- the n coefs of the mask
  TORUS b;  //
  double current_variance; //-- average noise of the sample

  LweSample(const LweParams<TORUS>* params);
  ~LweSample();
  LweSample(const LweSample<TORUS>&)=delete;
  LweSample<TORUS>& operator=(const LweSample<TORUS>&)=delete;
};

template struct LweSample<Torus32>;
template struct LweSample<Torus64>;

/** allocates and initializes (constructor) a single LweSample
 * @param params the LWE parameters to use
 */
template<typename TORUS>
inline LweSample<TORUS>* new_LweSample(const LweParams<TORUS>* params) {
  return new_obj<LweSample<TORUS>>(params);
}
/** allocates and initializes (constructor) a single LweSample
 * @param nbelts the number of consecutive elements to create
 * @param params the LWE parameters to use
 */
template<typename TORUS>
inline LweSample<TORUS>* new_LweSample_array(int nbelts, const LweParams<TORUS>* params) {
  return new_obj_array<LweSample<TORUS>>(nbelts, params);
}

/** destroy and frees memory space for a single LweSample
 * @param ptr the pointer to release */
template<typename TORUS>
inline void delete_LweSample(LweSample<TORUS>* obj) {
  del_obj<LweSample<TORUS>>(obj);
}
/** destroys and free memory space for an array of LweSample
 * @param nbelts the number of elements
 * @param ptr the pointer to release */
template<typename TORUS>
inline void delete_LweSample_array(int nbelts, LweSample<TORUS>* obj) {
  del_obj_array<LweSample<TORUS>>(nbelts, obj);
}

#endif //LWESAMPLES_H