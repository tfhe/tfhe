#ifndef LWEKEY_H
#define LWEKEY_H

#include "lwe.h"
//#include "lweparams.h"


struct LWEKey {
   const LWEParams* params;
   int* key;
};

//initialize the key structure and allocate the space for the key bits
void initLWEKey(LWEKey* obj, const LWEParams* params) {
   obj->params = params; 
   obj->key = new int[params.n];
}

void destroyLWEKey(LWEKey* obj) {
   delete[] obj->key;
}

#endif;
