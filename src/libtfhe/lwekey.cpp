#include <cstdlib>
#include "lwekey.h"
#include "lweparams.h"
#include "lwesamples.h"


using namespace std;

template<typename TORUS>
LweKey<TORUS>::LweKey(const LweParams<TORUS>* params) {
  this->params = params;
  this->key = new int[params->n];
}

template<typename TORUS>
LweKey<TORUS>::~LweKey() {
  delete[] key;
}





// Public key

template<typename TORUS>
LwePublicKey<TORUS>::LwePublicKey(const LwePublicParams<TORUS>* publicParams) {
  this->publicParams = publicParams;
  this->key = new_LweSample_array<TORUS>(publicParams->m,publicParams->params);
}


template<typename TORUS>
LwePublicKey<TORUS>::~LwePublicKey() {
  delete_LweSample_array(publicParams->m,key);
}

