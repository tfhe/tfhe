#include <cstdlib>
#include "lwekey.h"
#include "lweparams.h"

using namespace std;

//allocate memory space for a LWEKey
LWEKey* alloc_LWEKey() {
   return (LWEKey*) malloc(sizeof(LWEKey));
}

//free memory space for a LWEKey
void free_LWEKey(LWEKey* ptr) {
   free(ptr);
}

//initialize the key structure and allocate the space for the key bits
//(equivalent of the C++ constructor)
void init_LWEKey(LWEKey* obj, const LWEParams* params) {
   obj->params = params; 
   obj->key = new int[params->n];
}

//initialize the key structure and free the space for the key bits
//(equivalent of the C++ destructor)
void destroy_LWEKey(LWEKey* obj) {
   delete[] obj->key;
}

