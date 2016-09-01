#ifndef LweKEY_H
#define LweKEY_H

#include "lwe.h"
//#include "lweparams.h"


struct LweKey {
   const LweParams* params;
   int* key;

#ifdef __cplusplus   
   LweKey(const LweParams* params);
   ~LweKey();
   LweKey(const LweKey&) = delete; //forbidden 
   LweKey* operator=(const LweKey&) = delete; //forbidden
#endif
};

#endif //LweKEY_H
