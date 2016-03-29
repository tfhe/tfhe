#ifndef LWEKEY_H
#define LWEKEY_H

#include "lwe.h"
//#include "lweparams.h"


struct LWEKey {
   const LWEParams* params;
   int* key;
};

//allocate memory space for a LWEKey
LWEKey* alloc_LWEKey();

//free memory space for a LWEKey
void free_LWEKey(LWEKey* ptr);

//initialize the key structure and allocate the space for the key bits
//(equivalent of the C++ constructor)
void init_LWEKey(LWEKey* obj, const LWEParams* params);

//initialize the key structure and free the space for the key bits
//(equivalent of the C++ destructor)
void destroy_LWEKey(LWEKey* obj);

#endif //LWEKEY_H
