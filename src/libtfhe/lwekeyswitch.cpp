#include "lwekeyswitch.h"
#include "tfhe_core.h"
#include "tfhe_alloc.h"

template<typename TORUS>
LweKeySwitchKey<TORUS>::LweKeySwitchKey(
  const int n,
  const int t,
  const int basebit,
  const LweParams<TORUS>* out_params,
  LweSample<TORUS>* ks0_raw)
:
  n(n),
  t(t),
  basebit(basebit),
  base(1<<basebit),
  out_params(out_params),
  ks0_raw(ks0_raw)
{
  ks1_raw = new LweSample<TORUS>*[n*t];
  ks = new LweSample<TORUS>**[n];

  for (int p = 0; p < n*t; ++p)
    ks1_raw[p] = ks0_raw + base*p;
  for (int p = 0; p < n; ++p)
    ks[p] = ks1_raw + t*p;
}

template<typename TORUS>
LweKeySwitchKey<TORUS>::~LweKeySwitchKey() {
  delete[] ks;
  delete[] ks1_raw;
}

/**
 * LweKeySwitchKey constructor function
 */
template<typename TORUS>
void LweKeySwitchKey<TORUS>::init_obj(LweKeySwitchKey<TORUS>* obj, int n, int t, int basebit, const LweParams<TORUS>* out_params) {
  const int base=1<<basebit;
  LweSample<TORUS>* ks0_raw = new_obj_array<LweSample<TORUS>>(n*t*base, out_params);
  new(obj) LweKeySwitchKey<TORUS>(n,t,basebit,out_params, ks0_raw);
}

/**
 * LweKeySwitchKey destructor
 */
template<typename TORUS>
void LweKeySwitchKey<TORUS>::destroy_obj(LweKeySwitchKey<TORUS>* obj) {
  const int n = obj->n;
  const int t = obj->t;
  const int base = obj->base;
  del_obj_array(n*t*base, obj->ks0_raw);
  obj->~LweKeySwitchKey();
}