#ifndef LWEPARAMS_H
#define LWEPARAMS_H

///@file
///@brief This file contains the declaration of lwe parameters structures

#include "tfhe_core.h"
#include "tfhe_alloc.h"

//this structure contains Lwe parameters
//this structure is constant (cannot be modified once initialized):
//the pointer to the param can be passed directly
//to all the Lwe keys that use these params.
template<typename TORUS>
struct LweParams {
  const int n;
  const double alpha_min;//le plus petit bruit tq sur
  const double alpha_max;//le plus gd bruit qui permet le déchiffrement

  //since all members are declared constant, a constructor is
  //required in the structure.
  inline LweParams(int n, double alpha_min, double alpha_max)
  : n(n),
    alpha_min(alpha_min),
    alpha_max(alpha_max) { }

  inline ~LweParams() { }

  LweParams(const LweParams<TORUS>&) = delete; //forbidden
  LweParams<TORUS>& operator=(const LweParams<TORUS>& ) = delete; //forbidden
};

template struct LweParams<Torus32>;
template struct LweParams<Torus64>;


//allocates and initialize the LweParams structure
//(equivalent of the C++ new)
template<typename TORUS>
inline LweParams<TORUS>* new_LweParams(int n, double alpha_min, double alpha_max) {
  return new_obj<LweParams<TORUS>>(n, alpha_min, alpha_max);
}
template<typename TORUS>
inline LweParams<TORUS>* new_LweParams_array(int nbelts, int n, double alpha_min, double alpha_max) {
  return new_obj_array<LweParams<TORUS>>(nbelts, n, alpha_min, alpha_max);
}

//destroys and frees the LweParams structure
//(equivalent of the C++ delete)
template<typename TORUS>
inline void delete_LweParams(LweParams<TORUS>* obj) {
  del_obj<LweParams<TORUS>>(obj);
}
template<typename TORUS>
inline void delete_LweParams_array(int nbelts, LweParams<TORUS>* obj) {
  del_obj_array<LweParams<TORUS>>(nbelts, obj);
}

#endif //LWEPARAMS_H
