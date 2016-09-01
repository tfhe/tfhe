#ifndef LweKEY_H
#define LweKEY_H

///@file
///@brief This file contains the declaration of LWE key structures

#include "tfhe_core.h"
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

//allocate memory space for a LweKey
EXPORT LweKey* alloc_LweKey();
EXPORT LweKey* alloc_LweKey_array(int nbelts);

//free memory space for a LweKey
EXPORT void free_LweKey(LweKey* ptr);
EXPORT void free_LweKey_array(int nbelts, LweKey* ptr);

//initialize the LweKey structure
//(equivalent of the C++ constructor)
EXPORT void init_LweKey(LweKey* obj, const LweParams* params);
EXPORT void init_LweKey_array(int nbelts, LweKey* obj, const LweParams* params);

//destroys the LweKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_LweKey(LweKey* obj);
EXPORT void destroy_LweKey_array(int nbelts, LweKey* obj);
 
//allocates and initialize the LweKey structure
//(equivalent of the C++ new)
EXPORT LweKey* new_LweKey(const LweParams* params);
EXPORT LweKey* new_LweKey_array(int nbelts, const LweParams* params);

//destroys and frees the LweKey structure
//(equivalent of the C++ delete)
EXPORT void delete_LweKey(LweKey* obj);
EXPORT void delete_LweKey_array(int nbelts, LweKey* obj);

#endif //LweKEY_H
