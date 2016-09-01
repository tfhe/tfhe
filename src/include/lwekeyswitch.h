#ifndef LweKEYSWITCH_H
#define LweKEYSWITCH_H

#include "lwe.h"
#include "lweparams.h"
#include "lwesamples.h"

struct LweKeySwitchKey {
    int basebit; ///< log_2(base)
    int base; ///< decomposition base: a power of 2 
    int n; ///< length of the input key: s'
    int l; ///< decomposition length
    uint32_t mask; ///< mask=base-1, used for mod base ops
    const LweParams* in_params; ///< params of the input key s'
    const LweParams* out_params; ///< params of the output key s 
    LweSample* ks0_raw; //tableau qui contient tout les Lwe samples de taille nlbase
    LweSample** ks1_raw;// de taille nl  pointe vers un tableau ks0_raw dont les cases sont espaceés de base positions
    LweSample*** ks; ///< the keyswitch elements: a n.l.base matrix
    // de taille n pointe vers ks1 un tableau dont les cases sont espaceés de ell positions

#ifdef __cplusplus
    LweKeySwitchKey(int basebit, int kslength, const LweParams* in_params, const LweParams* out_params);
    ~LweKeySwitchKey();
    LweKeySwitchKey(const LweKeySwitchKey&) = delete;
    void operator=(const LweKeySwitchKey&) = delete;
#endif
};

#endif // LweKEYSWITCH_H
