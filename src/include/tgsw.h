#ifndef TGSW_H
#define TGSW_H

///@file
///@brief This file contains the declaration of TGSW structures

#include "tfhe_core.h"
#include "tfhe_alloc.h"
#include "tlwe.h"
#include "polynomials.h"
#include "numeric_functions.h"

template<typename TORUS>
struct TGswParams {
  typedef typename TorusUtils<TORUS>::UTORUS UTORUS;

  const int l; ///< decomp length
  const int Bgbit;///< log_2(Bg)
  const int Bg;///< decomposition base (must be a power of 2)
  const int32_t halfBg; ///< Bg/2
  const uint32_t maskMod; ///< Bg-1
  const TLweParams<TORUS>* tlwe_params; ///< Params of each row
  const int kpl; ///< number of rows = (k+1)*l
  TORUS* h; ///< powers of Bgbit
  UTORUS offset; ///< offset = Bg/2 * (2^(K-Bgbit) + 2^(K-2*Bgbit) + ... + 2^(K-l*Bgbit)), K is torus bitsize

  TGswParams(int l, int Bgbit, const TLweParams<TORUS>* tlwe_params);
  ~TGswParams();
  TGswParams(const TGswParams<TORUS>&) = delete;
  void operator=(const TGswParams<TORUS>&) = delete;
};


template<typename TORUS>
struct TGswKey {
  const TGswParams<TORUS>* params; ///< the parameters
  const TLweParams<TORUS>* tlwe_params; ///< the tlwe params of each rows
  IntPolynomial* key; ///< the key (array of k polynomials)
  TLweKey<TORUS> tlwe_key;

  inline TGswKey(const TGswParams<TORUS>* params)
  : params(params),
    tlwe_params(params->tlwe_params),
    tlwe_key(tlwe_params)
  {
    key = tlwe_key.key;
  }

  inline ~TGswKey() { }

  TGswKey(const TGswKey<TORUS>&) = delete;
  void operator=(const TGswKey<TORUS>&) = delete;
};


template<typename TORUS>
struct TGswSample: InitializerTag {
  TLweSample<TORUS>* all_sample; ///< TLweSample<TORUS>* all_sample; (k+1)l TLwe Sample
  TLweSample<TORUS>** bloc_sample;///< accès optionnel aux différents blocs de taille l.
  // double current_variance;
  const int k;
  const int l;

  inline TGswSample(TLweSample<TORUS>* all_sample, TLweSample<TORUS>** bloc_sample, const int k, const int l)
  : all_sample(all_sample),
    bloc_sample(bloc_sample),
    k(k),
    l(l) { }

  inline ~TGswSample() { }

  TGswSample(const TGswSample<TORUS>&) = delete;
  void operator=(const TGswSample<TORUS>&) = delete;

  static void init_obj(TGswSample<TORUS>* obj, const TGswParams<TORUS>* params);
  static void destroy_obj(TGswSample<TORUS>* obj);
};

template<typename TORUS>
struct TGswSampleFFT: InitializerTag {
  TLweSampleFFT<TORUS>* all_samples; ///< TLweSample<TORUS>* all_sample; (k+1)l TLwe Sample
  TLweSampleFFT<TORUS>** sample; ///< accès optionnel aux différents blocs de taille l.
  //double current_variance;
  const int k;
  const int l;

  TGswSampleFFT(const TGswParams<TORUS>* params, TLweSampleFFT<TORUS>* all_samples);
  ~TGswSampleFFT();
  TGswSampleFFT(const TGswSampleFFT<TORUS>&) = delete;
  void operator=(const TGswSampleFFT<TORUS>&) = delete;

  static void init_obj(TGswSampleFFT<TORUS>* obj, const TGswParams<TORUS>* params);
  static void destroy_obj(TGswSampleFFT<TORUS>* obj);
};

template struct TGswParams<Torus32>;
template struct TGswParams<Torus64>;

template struct TGswKey<Torus32>;
template struct TGswKey<Torus64>;

template struct TGswSample<Torus32>;
template struct TGswSample<Torus64>;

template struct TGswSampleFFT<Torus32>;
template struct TGswSampleFFT<Torus64>;

//allocates and initialize the TGswKey structure
//(equivalent of the C++ new)
template<typename TORUS>
inline TGswKey<TORUS>* new_TGswKey(const TGswParams<TORUS>* params) {
  return new_obj<TGswKey<TORUS>>(params);
}
template<typename TORUS>
inline TGswKey<TORUS>* new_TGswKey_array(int nbelts, const TGswParams<TORUS>* params) {
  return new_obj_array<TGswKey<TORUS>>(nbelts, params);
}

//destroys and frees the TGswKey structure
//(equivalent of the C++ delete)
template<typename TORUS>
inline void delete_TGswKey(TGswKey<TORUS>* obj) {
  del_obj<TGswKey<TORUS>>(obj);
}
template<typename TORUS>
inline void delete_TGswKey_array(int nbelts, TGswKey<TORUS>* obj) {
  del_obj<TGswKey<TORUS>>(nbelts, obj);
}


//allocates and initialize the TGswParams structure
//(equivalent of the C++ new)
template<typename TORUS>
inline TGswParams<TORUS>* new_TGswParams(int l, int Bgbit, const TLweParams<TORUS>* tlwe_params) {
  return new_obj<TGswParams<TORUS>>(l, Bgbit, tlwe_params);
}
template<typename TORUS>
inline TGswParams<TORUS>* new_TGswParams_array(int nbelts, int l, int Bgbit, const TLweParams<TORUS>* tlwe_params) {
  return new_obj_array<TGswParams<TORUS>>(nbelts, l, Bgbit, tlwe_params);
}

//destroys and frees the TGswParams structure
//(equivalent of the C++ delete)
template<typename TORUS>
inline void delete_TGswParams(TGswParams<TORUS>* obj) {
  del_obj<TGswParams<TORUS>>(obj);
}
template<typename TORUS>
inline void delete_TGswParams_array(int nbelts, TGswParams<TORUS>* obj) {
  del_obj_array<TGswParams<TORUS>>(nbelts, obj);
}


//allocates and initialize the TGswSample structure
//(equivalent of the C++ new)
template<typename TORUS>
inline TGswSample<TORUS>* new_TGswSample(const TGswParams<TORUS>* params) {
  return new_obj<TGswSample<TORUS>>(params);
}
template<typename TORUS>
inline TGswSample<TORUS>* new_TGswSample_array(int nbelts, const TGswParams<TORUS>* params) {
  return new_obj_array<TGswSample<TORUS>>(nbelts, params);
}

//destroys and frees the TGswSample structure
//(equivalent of the C++ delete)
template<typename TORUS>
inline void delete_TGswSample(TGswSample<TORUS>* obj) {
  del_obj<TGswSample<TORUS>>(obj);
}
template<typename TORUS>
inline void delete_TGswSample_array(int nbelts, TGswSample<TORUS>* obj) {
  del_obj_array<TGswSample<TORUS>>(nbelts, obj);
}


//allocates and initialize the TGswSampleFFT structure
//(equivalent of the C++ new)
template<typename TORUS>
inline TGswSampleFFT<TORUS>* new_TGswSampleFFT(const TGswParams<TORUS>* params) {
  return new_obj<TGswSampleFFT<TORUS>>(params);
}
template<typename TORUS>
inline TGswSampleFFT<TORUS>* new_TGswSampleFFT_array(int nbelts, const TGswParams<TORUS>* params) {
  return new_obj_array<TGswSampleFFT<TORUS>>(nbelts, params);
}

//destroys and frees the TGswSampleFFT structure
//(equivalent of the C++ delete)
template<typename TORUS>
inline void delete_TGswSampleFFT(TGswSampleFFT<TORUS>* obj) {
  del_obj<TGswSampleFFT<TORUS>>(obj);
}
template<typename TORUS>
inline void delete_TGswSampleFFT_array(int nbelts, TGswSampleFFT<TORUS>* obj) {
  del_obj<TGswSampleFFT<TORUS>>(nbelts, obj);
}

#endif // TGSW_H
