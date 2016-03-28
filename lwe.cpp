#include "lwe.h"
#include "lweparams.h"
#include "lwekey.h"




void lweKeyGen(LWEKey* result, const LWEParams* params) {
  initLWEKey(result, params);
  int n = params->n;
  for (int i=0; i<n; i++) {
    result[i]=rand()%2;
  }
}
