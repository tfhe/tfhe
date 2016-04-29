#ifndef RINGGSW_H
#define RINGGSW_H

#include "lwe.h"
#include "ringlwe.h"
#include "multiplication.h"

struct RingGSWParams {
    const int l;
    const int Bgbit;// peut être restreint à une puissance de 2 et on stocke seulement le nb de bits
    const RingLWEParams* ringlwe_params;
    const int kpl; // (k+1)*l

#ifdef __cplusplus
    RingGSWParams(int l, int Bgbit, RingLWEParams* ringlwe_params);
    ~RingGSWParams();
    RingGSWParams(const RingGSWParams&) = delete;
    void operator=(const RingGSWParams&) = delete;
#endif
};


struct RingGSWKey {
    const RingGSWParams* params;
    IntPolynomial* key;

#ifdef __cplusplus
    RingGSWKey(const RingGSWParams* params);
    ~RingGSWKey();
    RingGSWKey(const RingGSWKey&) = delete;
    void operator=(const RingGSWKey&) = delete;
#endif
};


struct RingGSWSample {
    const int k;
    const int l;
    RingLWESample* all_sample;//RingLWESample* all_sample; (k+1)\ell RingLWE Sample 
    RingLWESample** sample;// accés optionnel qui permet d'accéder aux différents blocs à l'intérieur.
    double current_alpha;

#ifdef __cplusplus
    RingGSWSample(const RingGSWParams* params);
    ~RingGSWSample();
    RingGSWSample(const RingGSWSample&) = delete;
    void operator=(const RingGSWSample&) = delete;
#endif
};

#endif // RINGGSW_H
