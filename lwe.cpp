#include <cstdlib>
#include "lwe.h"
#include "lweparams.h"
#include "lwekey.h"

using namespace std;

/**
 * This function generates a random LWE key for the given parameters.
 * The LWE key for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
EXPORT void lweKeyGen(LWEKey* result) {
  int n = result->params->n;
  for (int i=0; i<n; i++) {
    result->key[i]=rand()%2;
  }
}
