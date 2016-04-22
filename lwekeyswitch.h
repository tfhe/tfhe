#ifndef LWEKEYSWITCH_H
#define LWEKEYSWITCH_H

#include "lwe.h"
#include "lweparams.h"
#include "lwesamples.h"

struct LWEKeySwitchKey {
    int base;
    int l;
    LWEParams* in_params; // params input key 
    LWEParams* out_params; // params output key 
    LWESample* ks0_raw;
    LWESample** ks1_raw;
    LWESample*** ks2_raw;
    LWESample**** ks;

#ifdef __cplusplus
    LWEKeySwitchKey(int base, LWEParams* in_params, LWEParams* out_params);
    ~LWEKeySwitchKey();
    LWEKeySwitchKey(const LWEKeySwitchKey&) = delete;
    void operator=(const LWEKeySwitchKey&) = delete;
#endif
};

#endif // LWEKEYSWITCH_H
