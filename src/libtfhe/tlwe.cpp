#include "tfhe_core.h"
#include "tfhe_alloc.h"
#include "tlwe.h"
#include "polynomials.h"


template<typename TORUS>
TLweKey<TORUS>::TLweKey(const TLweParams<TORUS>* params)
:
  params(params)
{
  key = new_obj_array<IntPolynomial>(params->k,params->N);
}

template<typename TORUS>
TLweKey<TORUS>::~TLweKey() {
  del_obj_array(params->k,key);
}


template<typename TORUS>
TLweSample<TORUS>::TLweSample(const TLweParams<TORUS>* params)
:
  k(params->k)
{
  //Small change here:
  //a is a table of k+1 polynomials, b is an alias for &a[k]
  //like that, we can access all the coefficients as before:
  //  &sample->a[0],...,&sample->a[k-1]  and &sample->b
  //or we can also do it in a single for loop
  //  &sample->a[0],...,&sample->a[k]
  a = new_obj_array<TorusPolynomial<TORUS>>(k+1, params->N);
  b = a+k;
  current_variance = 0;
}

template<typename TORUS>
TLweSample<TORUS>::~TLweSample() {
  del_obj_array(k+1, a);
}



template<typename TORUS>
void TLweSampleFFT<TORUS>::init_obj(TLweSampleFFT<TORUS>* obj, const TLweParams<TORUS>* params)
{
  //a is a table of k+1 polynomials, b is an alias for &a[k]
  const int k = params->k;
  LagrangeHalfCPolynomial* a = new_obj_array<LagrangeHalfCPolynomial>(k+1, params->N);
  double current_variance = 0;

  // init_obj<TLweSampleFFT<TORUS>>(obj, params, a, current_variance);
  new(obj) TLweSampleFFT<TORUS>(params, a, current_variance);
}

template<typename TORUS>
void TLweSampleFFT<TORUS>::destroy_obj(TLweSampleFFT<TORUS>* obj) {
  const int k = obj->k;
  del_obj_array(k+1, obj->a);
  obj->~TLweSampleFFT();
}
