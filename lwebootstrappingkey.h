#ifndef LWEBOOTSTRAPPINGKEY_H
#define LWEBOOTSTRAPPINGKEY_H

#include "lwe.h"


struct LWEBootstrappingKey{
    const LWEParams* in_out_params; ///< paramètre de l'input et de l'output. key: s
    const RingGSWParams* bk_params; ///< params of the GSW elems in bk. key: s"
    const RingLWEParams* accum_params; ///< params of the accum variable key: s"
    const LWEParams* extract_params; ///< params after extraction: key: s' 
    RingGSWSample* bk; ///< the bootstrapping key (s->s")
    LWEKeySwitchKey* ks; ///< the keyswitch key (s'->s)


#ifdef __cplusplus
   LWEBootstrappingKey(const LWEParams* in_out_params, const RingGSWParams* bk_params);
    ~LWEBootstrappingKey();
    LWEBootstrappingKey(const LWEBootstrappingKey&) = delete;
    void operator=(const LWEBootstrappingKey&) = delete;
  
#endif


};


struct LWEBootstrappingKeyFFT {
    const LWEParams* in_out_params; ///< paramètre de l'input et de l'output. key: s
    const RingGSWParams* bk_params; ///< params of the GSW elems in bk. key: s"
    const RingLWEParams* accum_params; ///< params of the accum variable key: s"
    const LWEParams* extract_params; ///< params after extraction: key: s' 
    RingGSWSampleFFT* bk; ///< the bootstrapping key (s->s")
    LWEKeySwitchKey* ks; ///< the keyswitch key (s'->s)


#ifdef __cplusplus
   LWEBootstrappingKeyFFT(const LWEParams* in_out_params, const RingGSWParams* bk_params);
    ~LWEBootstrappingKeyFFT();
    LWEBootstrappingKeyFFT(const LWEBootstrappingKeyFFT&) = delete;
    void operator=(const LWEBootstrappingKeyFFT&) = delete;
  
#endif


};



#endif
