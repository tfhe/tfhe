#ifndef LWEKEY_H
#define LWEKEY_H

#include "lwe.h"
//#include "lweparams.h"


struct LWEKey {
   const LWEParams* params;
   int* key;

#ifdef __cplusplus   
   LWEKey(const LWEParams* params);
   ~LWEKey();
   LWEKey(const LWEKey&) = delete; //forbidden 
   LWEKey* operator=(const LWEKey&) = delete; //forbidden
#endif
};

#endif //LWEKEY_H
