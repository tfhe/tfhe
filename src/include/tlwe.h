#ifndef RINGLWE_H
#define RINGLWE_H

///@file
///@brief This file contains the declaration of tlwe structures

#include "tfhe_core.h"
#include "tfhe_alloc.h"
#include "polynomials.h"
#include "lweparams.h"

template<typename TORUS>
struct TLweParams {
  const int N; ///< a power of 2: degree of the polynomials
  const int k; ///< number of polynomials in the mask
  const double alpha_min; ///< minimal noise s.t. the sample is secure
  const double alpha_max; ///< maximal noise s.t. we can decrypt
  const LweParams<TORUS> extracted_lweparams; ///< lwe params if one extracts

  inline TLweParams(int N, int k, double alpha_min, double alpha_max)
  : N(N),
    k(k),
    alpha_min(alpha_min),
    alpha_max(alpha_max),
    extracted_lweparams(N*k,alpha_min, alpha_max) { }

  inline ~TLweParams() { }

  TLweParams(const TLweParams<TORUS>&) = delete;
  void operator=(const TLweParams<TORUS>&) = delete;
};

template<typename TORUS>
struct TLweKey {
  const TLweParams<TORUS>* params; ///< the parameters of the key
  IntPolynomial* key; ///< the key (i.e k binary polynomials)

  TLweKey(const TLweParams<TORUS>* params);
  ~TLweKey();
  TLweKey(const TLweKey<TORUS>&) = delete;
  void operator=(const TLweKey<TORUS>&) = delete;
};

template<typename TORUS>
struct TLweSample {
  TorusPolynomial<TORUS>* a; ///< array of length k+1: mask + right term
  TorusPolynomial<TORUS>* b; ///< alias of a[k] to get the right term
  double current_variance; ///< avg variance of the sample
  const int k;

  TLweSample(const TLweParams<TORUS>* params);
  ~TLweSample();
  TLweSample(const TLweSample<TORUS>&) = delete;
  void operator=(const TLweSample<TORUS>&) = delete;
};

template<typename TORUS>
struct TLweSampleFFT: InitializerTag {
  const int k; //required during the destructor call...
  LagrangeHalfCPolynomial* a; ///< array of length k+1: mask + right term
  LagrangeHalfCPolynomial* b; ///< alias of a[k] to get the right term
  double current_variance; ///< avg variance of the sample

  inline TLweSampleFFT(const TLweParams<TORUS>* params, LagrangeHalfCPolynomial* arr, double current_variance)
  : k(params->k),
    a(arr),
    b(a+k),
    current_variance(0) { }

  inline ~TLweSampleFFT() { }

  TLweSampleFFT(const TLweSampleFFT<TORUS>&) = delete;
  void operator=(const TLweSampleFFT<TORUS>&) = delete;

  static void init_obj(TLweSampleFFT<TORUS>* obj, const TLweParams<TORUS>* params);
  static void destroy_obj(TLweSampleFFT<TORUS>* obj);
};

template struct TLweParams<Torus32>;
template struct TLweParams<Torus64>;

template struct TLweKey<Torus32>;
template struct TLweKey<Torus64>;

template struct TLweSample<Torus32>;
template struct TLweSample<Torus64>;

template struct TLweSampleFFT<Torus32>;
template struct TLweSampleFFT<Torus64>;


//allocates and initialize the TLweParams structure
//(equivalent of the C++ new)
template<typename TORUS>
inline TLweParams<TORUS>* new_TLweParams(int N, int k, double alpha_min, double alpha_max) {
  return new_obj<TLweParams<TORUS>>(N, k, alpha_min, alpha_max);
}

template<typename TORUS>
inline TLweParams<TORUS>* new_TLweParams_array(int nbelts, int N, int k, double alpha_min, double alpha_max) {
  return new_obj_array<TLweParams<TORUS>>(nbelts, N, k, alpha_min, alpha_max);
}

//destroys and frees the TLweParams structure
//(equivalent of the C++ delete)
template<typename TORUS>
inline void delete_TLweParams(TLweParams<TORUS>* obj) {
  del_obj<TLweParams<TORUS>>(obj);
}

template<typename TORUS>
inline void delete_TLweParams_array(int nbelts, TLweParams<TORUS>* obj) {
  del_obj_array<TLweParams<TORUS>>(nbelts, obj);
}


/** allocates and initializes (constructor) a single TLweKey
 * @param params the LWE parameters to use
 */
template<typename TORUS>
inline TLweKey<TORUS>* new_TLweKey(const TLweParams<TORUS>* params) {
  return new_obj<TLweKey<TORUS>>(params);
}

/** allocates and initializes (constructor) a single TLweKey
 * @param nbelts the number of consecutive elements to create
 * @param params the LWE parameters to use
 */
template<typename TORUS>
inline TLweKey<TORUS>* new_TLweKey_array(int nbelts, const TLweParams<TORUS>* params) {
  return new_obj_array<TLweKey<TORUS>>(nbelts, params);
}

/** destroy and frees memory space for a single TLweKey
 * @param ptr the pointer to release */
template<typename TORUS>
inline void delete_TLweKey(TLweKey<TORUS>* obj) {
  del_obj<TLweKey<TORUS>>(obj);
}

/** destroys and free memory space for an array of TLweKey
 * @param nbelts the number of elements
 * @param ptr the pointer to release */
template<typename TORUS>
inline void delete_TLweKey_array(int nbelts, TLweKey<TORUS>* obj) {
  del_obj_array<TLweKey<TORUS>>(nbelts, obj);
}


/** allocates and initializes (constructor) a single TLweSample
 * @param params the LWE parameters to use
 */
template<typename TORUS>
inline TLweSample<TORUS>* new_TLweSample(const TLweParams<TORUS>* params) {
  return new_obj<TLweSample<TORUS>>(params);
}

/** allocates and initializes (constructor) a single TLweSample
 * @param nbelts the number of consecutive elements to create
 * @param params the LWE parameters to use
 */
template<typename TORUS>
inline TLweSample<TORUS>* new_TLweSample_array(int nbelts, const TLweParams<TORUS>* params) {
  return new_obj_array<TLweSample<TORUS>>(nbelts, params);
}

/** destroy and frees memory space for a single TLweSample
 * @param ptr the pointer to release */
template<typename TORUS>
inline void delete_TLweSample(TLweSample<TORUS>* obj) {
  del_obj<TLweSample<TORUS>>(obj);
}

/** destroys and free memory space for an array of TLweSample
 * @param nbelts the number of elements
 * @param ptr the pointer to release */
template<typename TORUS>
inline void delete_TLweSample_array(int nbelts, TLweSample<TORUS>* obj) {
  del_obj<TLweSample<TORUS>>(nbelts, obj);
}


/** allocates and initializes (constructor) a single TLweSampleFFT
 * @param params the LWE parameters to use
 */
template<typename TORUS>
inline TLweSampleFFT<TORUS>* new_TLweSampleFFT(const TLweParams<TORUS>* params) {
  return new_obj<TLweSampleFFT<TORUS>>(params);
}

/** allocates and initializes (constructor) a single TLweSampleFFT
 * @param nbelts the number of consecutive elements to create
 * @param params the LWE parameters to use
 */
template<typename TORUS>
inline TLweSampleFFT<TORUS>* new_TLweSampleFFT_array(int nbelts, const TLweParams<TORUS>* params) {
  return new_obj_array<TLweSampleFFT<TORUS>>(nbelts, params);
}

/** destroy and frees memory space for a single TLweSampleFFT
 * @param ptr the pointer to release */
template<typename TORUS>
inline void delete_TLweSampleFFT(TLweSampleFFT<TORUS>* obj) {
  del_obj<TLweSampleFFT<TORUS>>(obj);
}

/** destroys and free memory space for an array of TLweSampleFFT
 * @param nbelts the number of elements
 * @param ptr the pointer to release */
template<typename TORUS>
inline void delete_TLweSampleFFT_array(int nbelts, TLweSampleFFT<TORUS>* obj) {
  del_obj<TLweSampleFFT<TORUS>>(nbelts, obj);
}

#endif // RINGLWE_H
