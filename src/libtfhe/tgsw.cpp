#include "tfhe_core.h"
#include "tfhe_alloc.h"
#include "tlwe.h"
#include "tgsw.h"
#include "polynomials_arithmetic.h"


template<typename TORUS>
TGswParams<TORUS>::TGswParams(int l, int Bgbit, const TLweParams<TORUS>* tlwe_params)
:
  l(l),
  Bgbit(Bgbit),
  Bg(1<<Bgbit),
  halfBg(Bg>>1),
  maskMod(Bg-1),
  tlwe_params(tlwe_params),
  kpl(int((tlwe_params->k+1)*l))
{
  UTORUS temp1 = 0;
  h = new TORUS[l];
  for (int i = 0; i < l; ++i) {
    h[i] = TORUS(1) << (TorusUtils<TORUS>::bit_cnt - (i+1)*Bgbit); // 1/(Bg^(i+1)) as a TORUS
    temp1 += h[i];
  }
  // offset = Bg/2 * (2^(K-Bgbit) + 2^(K-2*Bgbit) + ... + 2^(K-l*Bgbit)), K is torus bitsize
  offset = temp1*halfBg;
}

template<typename TORUS>
TGswParams<TORUS>::~TGswParams() {
  delete[] h;
}

template<typename TORUS>
TGswSampleFFT<TORUS>::TGswSampleFFT(const TGswParams<TORUS>* params, TLweSampleFFT<TORUS>* all_samples_raw)
: k(params->tlwe_params->k),
  l(params->l)
{
  all_samples = all_samples_raw;
  sample = new TLweSampleFFT<TORUS>*[(k+1)*l];

  for (int p = 0; p < (k+1); ++p)
    sample[p] = all_samples + p*l;
}

template<typename TORUS>
TGswSampleFFT<TORUS>::~TGswSampleFFT() {
  delete[] sample;
}



//initialize the sample structure
//(equivalent of the C++ constructor)
template<typename TORUS>
void TGswSample<TORUS>::init_obj(TGswSample<TORUS>* obj, const TGswParams<TORUS>* params) {
  const int k = params->tlwe_params->k;
  const int l = params->l;
  TLweSample<TORUS>* all_sample = new_obj_array<TLweSample<TORUS>>((k+1)*l,params->tlwe_params); // tous les samples comme un vecteur ligne
  TLweSample<TORUS>** bloc_sample = new TLweSample<TORUS>*[k+1]; // blocs horizontaux (l lignes) de la matrice TGsw

  for (int p = 0; p < k+1; ++p)
    bloc_sample[p] = all_sample + p*l;

  // init_obj<TGswSample<TORUS>>(obj, all_sample, bloc_sample, k,l);
  new(obj) TGswSample<TORUS>(all_sample, bloc_sample, k,l);
}

//destroys the TGswSample structure
//(equivalent of the C++ destructor)
template<typename TORUS>
void TGswSample<TORUS>::destroy_obj(TGswSample<TORUS>* obj) {
  const int k = obj->k;
  const int l = obj->l;
  del_obj_array((k+1)*l,obj->all_sample);
  delete[] obj->bloc_sample;
  obj->~TGswSample();
}


//constructor content
template<typename TORUS>
void TGswSampleFFT<TORUS>::init_obj(TGswSampleFFT<TORUS>* obj, const TGswParams<TORUS>* params) {
  const int k = params->tlwe_params->k;
  const int l = params->l;
  TLweSampleFFT<TORUS>* all_samples = new_obj_array<TLweSampleFFT<TORUS>>((k+1)*l, params->tlwe_params);

  // init_obj<TGswSampleFFT<TORUS>>(obj, params, all_samples);
  new(obj) TGswSampleFFT<TORUS>(params, all_samples);
}

//destructor content
template<typename TORUS>
void TGswSampleFFT<TORUS>::destroy_obj(TGswSampleFFT<TORUS>* obj) {
  const int k = obj->k;
  const int l = obj->l;
  del_obj_array((k+1)*l, obj->all_samples);
  obj->~TGswSampleFFT();
}