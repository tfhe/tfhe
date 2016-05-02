#ifndef LWEBOOTSTRAPPINGKEY_H
#define LWEBOOTSTRAPPINGKEY_H

#include "lwe.h"


struct LWEBootstrappingKey{
    LWEParams* params; ///< paramètre de l'input et de l'output. key: s
    RingGSWParams* bk_params; ///< params of the GSW elems in bk. key: s"
    RingLWEParams* accum_params; ///< params of the accum variable key: s"
    LWEParams* extract_params; ///< params after extraction: key: s' 
    RingGSWSample* bk; ///< the bootstrapping key (s->s")
    LWEKeySwitchKey* ks; ///< the keyswitch key (s'->s)


#ifdef __cplusplus
   LWEBootstrappingKey(LWEParams* in_out_params, RingGSWParams* bk_params);
    ~LWEBootstrappingKey();
    LWEBootstrappingKey(const LWEBootstrappingKey&) = delete;
    void operator=(const LWEBootstrappingKey&) = delete;
  
#endif


};



#endif
