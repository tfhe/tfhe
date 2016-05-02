#ifndef RINGGSW_H
#define RINGGSW_H

#include "lwe.h"
#include "ringlwe.h"
#include "multiplication.h"
#include "polynomials.h"

struct RingGSWParams {
    const int l; ///< decomp length
    const int Bgbit;///< log_2(Bg)
    const int Bg;///< decomposition base (must be a power of 2)
    const int32_t halfBg; ///< Bg/2
    const uint32_t maskMod; ///< Bg-1
    const RingLWEParams* ringlwe_params; ///< Params of each row
    const int kpl; ///< number of rows = (k+1)*l
    Torus32* h; ///< powers of Bgbit
    uint32_t offset; ///< offset = Bg/2 * (2^(32-Bgbit) + 2^(32-2*Bgbit) + ... + 2^(32-l*Bgbit))

#ifdef __cplusplus
    RingGSWParams(int l, int Bgbit, RingLWEParams* ringlwe_params);
    ~RingGSWParams();
    RingGSWParams(const RingGSWParams&) = delete;
    void operator=(const RingGSWParams&) = delete;
#endif
};


struct RingGSWKey {
    const RingGSWParams* params; ///< the parameters
    IntPolynomial* key; ///< the key (array of k polynomials)

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
    RingLWESample* all_sample; ///< RingLWESample* all_sample; (k+1)\ell RingLWE Sample 
    RingLWESample** bloc_sample;///< accés optionnel qui permet d'accéder aux différents blocs à l'intérieur.
    // double current_variance;

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
    double current_variance;

#ifdef __cplusplus
    RingGSWSampleFFT(const RingGSWParams* params);
    ~RingGSWSampleFFT();
    RingGSWSampleFFT(const RingGSWSampleFFT&) = delete;
    void operator=(const RingGSWSampleFFT&) = delete;
#endif
};

#endif // RINGGSW_H
