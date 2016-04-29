#ifndef LWEKEYSWITCH_H
#define LWEKEYSWITCH_H

#include "lwe.h"
#include "lweparams.h"
#include "lwesamples.h"

struct LWEKeySwitchKey {
int basebit;
    int base;// peut être restreient a être une petite puissance de 2 ou même égal à 2
    int l;
    LWEParams* in_params; // params input key 
    LWEParams* out_params; // params output key 
    LWESample* ks0_raw;//tableau qui contient tout les LWE samples de taille nlbase
    LWESample** ks1_raw;// de taille nl  pointe vers un tableau ks0_raw dont les cases sont espaceés de base positions
    LWESample*** ks;// de taille n pointe vers ks1 un tableau dont les cases sont espaceés de ell positions

#ifdef __cplusplus
    LWEKeySwitchKey(int base, LWEParams* in_params, LWEParams* out_params);
    ~LWEKeySwitchKey();
    LWEKeySwitchKey(const LWEKeySwitchKey&) = delete;
    void operator=(const LWEKeySwitchKey&) = delete;
#endif
};

#endif // LWEKEYSWITCH_H
