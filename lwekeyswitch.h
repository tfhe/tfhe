#ifndef LWEKEYSWITCH_H
#define LWEKEYSWITCH_H

#include "lwe.h"
#include "lweparams.h"
#include "lwesamples.h"

struct LWEKeySwitchKey {
    int basebit; ///< log_2(base)
    int base; ///< decomposition base: a power of 2 
    int n; ///< length of the input key: s'
    int l; ///< decomposition length
    uint32_t mask; ///< mask=base-1, used for mod base ops
    const LWEParams* in_params; ///< params of the input key s'
    const LWEParams* out_params; ///< params of the output key s 
    LWESample* ks0_raw; //tableau qui contient tout les LWE samples de taille nlbase
    LWESample** ks1_raw;// de taille nl  pointe vers un tableau ks0_raw dont les cases sont espaceés de base positions
    LWESample*** ks; ///< the keyswitch elements: a n.l.base matrix
    // de taille n pointe vers ks1 un tableau dont les cases sont espaceés de ell positions

#ifdef __cplusplus
    LWEKeySwitchKey(int basebit, int kslength, const LWEParams* in_params, const LWEParams* out_params);
    ~LWEKeySwitchKey();
    LWEKeySwitchKey(const LWEKeySwitchKey&) = delete;
    void operator=(const LWEKeySwitchKey&) = delete;
#endif
};

#endif // LWEKEYSWITCH_H
