#include <cstdlib>
#include "lwe.h"
#include "lweparams.h"
#include "lwekey.h"

using namespace std;

/**
 * This function generates a random LWE key for the given parameters.
 * The LWE key for the result must be allocated, but not initialized
 */
EXPORT void lweKeyGen(LWEKey* result, const LWEParams* params) {
  init_LWEKey(result, params);
  int n = params->n;
  for (int i=0; i<n; i++) {
    result->key[i]=rand()%2;
  }
}
