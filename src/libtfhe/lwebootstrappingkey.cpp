#include <cstdlib>
#include <iostream>
#include "tfhe_core.h"
#include "tfhe_alloc.h"
#include "lwebootstrappingkey.h"
#include "lwekeyswitch.h"
#include "lwe-functions.h"
#include "tgsw.h"
#include "tgsw_functions.h"


using namespace std;


template<typename TORUS>
void LweBootstrappingKey<TORUS>::init_obj(
  LweBootstrappingKey<TORUS>* obj,
  const int ks_t,
  const int ks_basebit,
  const LweParams<TORUS>* in_out_params,
  const TGswParams<TORUS>* bk_params)
{
  const TLweParams<TORUS>* accum_params = bk_params->tlwe_params;
  const LweParams<TORUS>* extract_params = &accum_params->extracted_lweparams;
  const int n = in_out_params->n;
  const int N = extract_params->n;

  TGswSample<TORUS>* bk = new_obj_array<TGswSample<TORUS>>(n,bk_params);
  LweKeySwitchKey<TORUS>* ks = new_obj<LweKeySwitchKey<TORUS>>(N, ks_t, ks_basebit, in_out_params);

  new(obj) LweBootstrappingKey<TORUS>(in_out_params, bk_params, accum_params, extract_params, bk, ks);
}

template<typename TORUS>
void LweBootstrappingKey<TORUS>::destroy_obj(LweBootstrappingKey<TORUS>* obj) {
  del_obj(obj->ks);
  del_obj_array(obj->in_out_params->n,obj->bk);
  obj->~LweBootstrappingKey();
}


//(equivalent of the C++ constructor)
template<typename TORUS>
void LweBootstrappingKeyFFT<TORUS>::init_obj(LweBootstrappingKeyFFT<TORUS>* obj, const LweBootstrappingKey<TORUS>* bk) {
  const LweParams<TORUS>* in_out_params = bk->in_out_params;
  const TGswParams<TORUS>* bk_params = bk->bk_params;
  const TLweParams<TORUS>* accum_params = bk_params->tlwe_params;
  const LweParams<TORUS>* extract_params = &accum_params->extracted_lweparams;
  const int n = in_out_params->n;
  const int t = bk->ks->t;
  const int basebit = bk->ks->basebit;
  const int base = bk->ks->base;
  const int N = extract_params->n;

  LweKeySwitchKey<TORUS>* ks = new_obj<LweKeySwitchKey<TORUS>>(N, t, basebit, in_out_params);
  // Copy the KeySwitching key
  for(int i=0; i<N; i++) {
    for(int j=0; j<t; j++){
      for(int p=0; p<base; p++) {
        lweCopy(&ks->ks[i][j][p], &bk->ks->ks[i][j][p], in_out_params);
      }
    }
  }

  // Bootstrapping Key FFT
  TGswSampleFFT<TORUS>* bkFFT = new_obj_array<TGswSampleFFT<TORUS>>(n,bk_params);
  for (int i=0; i<n; ++i) {
    tGswToFFTConvert(&bkFFT[i], &bk->bk[i], bk_params);
  }

  new(obj) LweBootstrappingKeyFFT<TORUS>(in_out_params, bk_params, accum_params, extract_params, bkFFT, ks);
}

//destroys the LweBootstrappingKeyFFT structure
//(equivalent of the C++ destructor)
template<typename TORUS>
void LweBootstrappingKeyFFT<TORUS>::destroy_obj(LweBootstrappingKeyFFT<TORUS>* obj) {
  del_obj((LweKeySwitchKey<TORUS>*)obj->ks);
  del_obj_array(obj->in_out_params->n, (TGswSampleFFT<TORUS>*)obj->bkFFT);
  obj->~LweBootstrappingKeyFFT();
}