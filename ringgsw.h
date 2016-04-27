#ifndef RINGGSW_H
#define RINGGSW_H

#include "lwe.h"
#include "ringlwe.h"
#include "multiplication.h"

struct RingGSWParams {
    const int l;
    const RingLWEParams* ringlwe_params;

#ifdef __cplusplus
    RingGSWParams(int l, RingLWEParams* ringlwe_params);
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
    RingLWESample* sample_raw;
    RingLWESample** sample;
    double current_alpha;

#ifdef __cplusplus
    RingGSWSample(const RingGSWParams* params);
    ~RingGSWSample();
    RingGSWSample(const RingGSWSample&) = delete;
    void operator=(const RingGSWSample&) = delete;
#endif
};

#endif // RINGGSW_H
