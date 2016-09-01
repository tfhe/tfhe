#ifndef TGSW_H
#define TGSW_H

#include "lwe.h"
#include "tlwe.h"
#include "multiplication.h"
#include "polynomials.h"

struct TGswParams {
    const int l; ///< decomp length
    const int Bgbit;///< log_2(Bg)
    const int Bg;///< decomposition base (must be a power of 2)
    const int32_t halfBg; ///< Bg/2
    const uint32_t maskMod; ///< Bg-1
    const TLweParams* tlwe_params; ///< Params of each row
    const int kpl; ///< number of rows = (k+1)*l
    Torus32* h; ///< powers of Bgbit
    uint32_t offset; ///< offset = Bg/2 * (2^(32-Bgbit) + 2^(32-2*Bgbit) + ... + 2^(32-l*Bgbit))

#ifdef __cplusplus
    TGswParams(int l, int Bgbit, TLweParams* tlwe_params);
    ~TGswParams();
    TGswParams(const TGswParams&) = delete;
    void operator=(const TGswParams&) = delete;
#endif
};


struct TGswKey {
    const TGswParams* params; ///< the parameters
    const TLweParams* tlwe_params; ///< the tlwe params of each rows
    IntPolynomial* key; ///< the key (array of k polynomials)
    TLweKey tlwe_key;

#ifdef __cplusplus
    TGswKey(const TGswParams* params);
    ~TGswKey();
    TGswKey(const TGswKey&) = delete;
    void operator=(const TGswKey&) = delete;
#endif
};


struct TGswSample {
    TLweSample* all_sample; ///< TLweSample* all_sample; (k+1)l TLwe Sample 
    TLweSample** bloc_sample;///< accès optionnel aux différents blocs de taille l.
    // double current_variance;
    const int k;
    const int l;

#ifdef __cplusplus
    TGswSample(const TGswParams* params);
    ~TGswSample();
    TGswSample(const TGswSample&) = delete;
    void operator=(const TGswSample&) = delete;
#endif
};

struct TGswSampleFFT {
    TLweSampleFFT* all_samples; ///< TLweSample* all_sample; (k+1)l TLwe Sample 
    TLweSampleFFT** sample; ///< accès optionnel aux différents blocs de taille l.
    //double current_variance;
    const int k;
    const int l;

#ifdef __cplusplus
    TGswSampleFFT(const TGswParams* params);
    ~TGswSampleFFT();
    TGswSampleFFT(const TGswSampleFFT&) = delete;
    void operator=(const TGswSampleFFT&) = delete;
#endif
};

#endif // TGSW_H
