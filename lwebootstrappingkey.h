#ifndef LWEBOOTSTRAPPINGKEY_H
#define LWEBOOTSTRAPPINGKEY_H

#include "lwe.h"


struct LWEBootstrappingKey{
    LWEParams* params;//paramètre de l'input et de l'output
    RingGSWParams* bk_params; // params input key  
    RingGSWSample* bk;
    RingLWEParams* accum_params; 
    LWEKeySwitchKey* ks;//inclus l'Extract


#ifdef __cplusplus
   LWEBootstrappingKey(RingGSWParams* bk_params,RingGSWSample* bk);
    ~LWEBootstrappingKey();
    LWEBootstrappingKey(const LWEBootstrappingKey&) = delete;
    void operator=(const LWEBootstrappingKey&) = delete;
  
#endif


};



#endif
