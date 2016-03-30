#ifndef LWEKEY_H
#define LWEKEY_H

#include "lwe.h"
//#include "lweparams.h"


struct LWEKey {
   const LWEParams* params;
   int* key;

   LWEKey(const LWEParams* params);
   ~LWEKey();
   LWEKey(const LWEKey&) = delete; //forbidden 
   LWEKey* operator=(const LWEKey&) = delete; //forbidden
};

#endif //LWEKEY_H
