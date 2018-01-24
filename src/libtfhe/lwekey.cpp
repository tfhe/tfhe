#include <cstdlib>
#include "lwekey.h"
#include "lweparams.h"

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

