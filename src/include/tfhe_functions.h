#ifndef TFHE_FUNCTIONS_H
#define TFHE_FUNCTIONS_H

template<typename TORUS>
struct TfheFunctions
{
  /** sets the seed of the random number generator to the given values */
  static void createLweBootstrappingKey(LweBootstrappingKey<TORUS>* bk, const LweKey<TORUS>* key_in, const TGswKey<TORUS>* rgsw_key);

  static void MuxRotate(TLweSample<TORUS>* result, const TLweSample<TORUS>* accum, const TGswSample<TORUS>* bki, const int barai, const TGswParams<TORUS>* bk_params);
  static void blindRotate(TLweSample<TORUS>* accum, const TGswSample<TORUS>* bk, const int* bara, const int n, const TGswParams<TORUS>* bk_params);
  static void blindRotateAndExtract(LweSample<TORUS>* result, const TorusPolynomial<TORUS>* v, const TGswSample<TORUS>* bk, const int barb, const int* bara, const int n, const TGswParams<TORUS>* bk_params);
  static void bootstrap_woKS(LweSample<TORUS>* result, const LweBootstrappingKey<TORUS>* bk, TORUS mu, const LweSample<TORUS>* x);
  static void bootstrap(LweSample<TORUS>* result, const LweBootstrappingKey<TORUS>* bk, TORUS mu, const LweSample<TORUS>* x);

  static void MuxRotate_FFT(TLweSample<TORUS>* result, const TLweSample<TORUS>* accum, const TGswSampleFFT<TORUS>* bki, const int barai, const TGswParams<TORUS>* bk_params);
  static void blindRotate_FFT(TLweSample<TORUS>* accum, const TGswSampleFFT<TORUS>* bk, const int* bara, const int n, const TGswParams<TORUS>* bk_params);
  static void blindRotateAndExtract_FFT(LweSample<TORUS>* result, const TorusPolynomial<TORUS>* v, const TGswSampleFFT<TORUS>* bk, const int barb, const int* bara, const int n, const TGswParams<TORUS>* bk_params);
  static void bootstrap_woKS_FFT(LweSample<TORUS>* result, const LweBootstrappingKeyFFT<TORUS>* bk, TORUS mu, const LweSample<TORUS>* x);
  static void bootstrap_FFT(LweSample<TORUS>* result, const LweBootstrappingKeyFFT<TORUS>* bk, TORUS mu, const LweSample<TORUS>* x);
};

template struct TfheFunctions<Torus32>;
template struct TfheFunctions<Torus64>;

///////////////////////////////////////////////////
//  TFHE bootstrapping internal functions
//////////////////////////////////////////////////

template<typename TORUS>
inline void tfhe_MuxRotate(TLweSample<TORUS>* result, const TLweSample<TORUS>* accum, const TGswSample<TORUS>* bki, const int barai, const TGswParams<TORUS>* bk_params) {
  TfheFunctions<TORUS>::MuxRotate(result, accum, bki, barai, bk_params);
}


template<typename TORUS>
inline void tfhe_blindRotate(TLweSample<TORUS>* accum, const TGswSample<TORUS>* bk, const int* bara, const int n, const TGswParams<TORUS>* bk_params) {
  TfheFunctions<TORUS>::blindRotate(accum, bk, bara, n, bk_params);
}

template<typename TORUS>
inline void tfhe_blindRotateAndExtract(LweSample<TORUS>* result, const TorusPolynomial<TORUS>* v, const TGswSample<TORUS>* bk, const int barb, const int* bara, const int n, const TGswParams<TORUS>* bk_params) {
  TfheFunctions<TORUS>::blindRotateAndExtract(result, v, bk, barb, bara, n, bk_params);
}

template<typename TORUS>
inline void tfhe_bootstrap_woKS(LweSample<TORUS>* result, const LweBootstrappingKey<TORUS>* bk, TORUS mu, const LweSample<TORUS>* x) {
  TfheFunctions<TORUS>::bootstrap_woKS(result, bk, mu, x);
}

template<typename TORUS>
inline void tfhe_bootstrap(LweSample<TORUS>* result, const LweBootstrappingKey<TORUS>* bk, TORUS mu, const LweSample<TORUS>* x) {
  TfheFunctions<TORUS>::bootstrap(result, bk, mu, x);
}

template<typename TORUS>
inline void tfhe_createLweBootstrappingKey(LweBootstrappingKey<TORUS>* bk, const LweKey<TORUS>* key_in, const TGswKey<TORUS>* rgsw_key) {
  TfheFunctions<TORUS>::createLweBootstrappingKey(bk, key_in, rgsw_key);
}


template<typename TORUS>
inline void tfhe_MuxRotate_FFT(TLweSample<TORUS>* result, const TLweSample<TORUS>* accum, const TGswSampleFFT<TORUS>* bki, const int barai, const TGswParams<TORUS>* bk_params) {
  TfheFunctions<TORUS>::MuxRotate_FFT(result, accum, bki, barai, bk_params);
}

template<typename TORUS>
inline void tfhe_blindRotate_FFT(TLweSample<TORUS>* accum, const TGswSampleFFT<TORUS>* bk, const int* bara, const int n, const TGswParams<TORUS>* bk_params) {
  TfheFunctions<TORUS>::blindRotate_FFT(accum, bk, bara, n, bk_params);
}

template<typename TORUS>
inline void tfhe_blindRotateAndExtract_FFT(LweSample<TORUS>* result, const TorusPolynomial<TORUS>* v, const TGswSampleFFT<TORUS>* bk, const int barb, const int* bara, const int n, const TGswParams<TORUS>* bk_params) {
  TfheFunctions<TORUS>::blindRotateAndExtract_FFT(result, v, bk, barb, bara, n, bk_params);
}

template<typename TORUS>
inline void tfhe_bootstrap_woKS_FFT(LweSample<TORUS>* result, const LweBootstrappingKeyFFT<TORUS>* bk, TORUS mu, const LweSample<TORUS>* x) {
  TfheFunctions<TORUS>::bootstrap_woKS_FFT(result, bk, mu, x);
}

template<typename TORUS>
inline void tfhe_bootstrap_FFT(LweSample<TORUS>* result, const LweBootstrappingKeyFFT<TORUS>* bk, TORUS mu, const LweSample<TORUS>* x) {
  TfheFunctions<TORUS>::bootstrap_FFT(result, bk, mu, x);
}


#endif //TFHE_FUNCTIONS_H