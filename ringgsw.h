#ifndef RINGGSW_H
#define RINGGSW_H

#include "lwe.h"
#include "ringlwe.h"
#include "multiplication.h"
#include "polynomials.h"

struct RingGSWParams {
    const int l;
    const int Bgbit;// peut être restreint à une puissance de 2 et on stocke seulement le nb de bits
    const int Bg;// base
    const int halfBg; // Bg/2
    const int maskMod; // Bg-1
    const RingLWEParams* ringlwe_params;
    const int kpl; // (k+1)*l
    Torus32* h; // powers of Bgbit
    Torus32 offset; // offset = Bg/2 * (1 + Bg + Bg^2 + ... + Bg^(l-1))

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
    RingLWESample** row_sample;// accés optionnel qui permet d'accéder aux différents lignes à l'intérieur.
    RingLWESample** bloc_sample;// accés optionnel qui permet d'accéder aux différents blocs à l'intérieur.
    double current_alpha;

#ifdef __cplusplus
    RingGSWSample(const RingGSWParams* params);
    ~RingGSWSample();
    RingGSWSample(const RingGSWSample&) = delete;
    void operator=(const RingGSWSample&) = delete;
#endif
};

struct RingGSWSampleFFT {
    const int k;
    const int l;
    RingLWESampleFFT* all_samples;
    RingLWESampleFFT** sample;
    double current_alpha;

#ifdef __cplusplus
    RingGSWSampleFFT(const RingGSWParams* params);
    ~RingGSWSampleFFT();
    RingGSWSampleFFT(const RingGSWSampleFFT&) = delete;
    void operator=(const RingGSWSampleFFT&) = delete;
#endif
};

#endif // RINGGSW_H
