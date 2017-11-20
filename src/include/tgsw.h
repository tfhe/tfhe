#ifndef TGSW_H
#define TGSW_H

///@file
///@brief This file contains the declaration of TGSW structures

#include "tfhe_core.h"
#include "tlwe.h"

struct TGswParams {
    const int32_t l; ///< decomp length
    const int32_t Bgbit;///< log_2(Bg)
    const int32_t Bg;///< decomposition base (must be a power of 2)
    const int32_t halfBg; ///< Bg/2
    const uint32_t maskMod; ///< Bg-1
    const TLweParams *tlwe_params; ///< Params of each row
    const int32_t kpl; ///< number of rows = (k+1)*l
    Torus32 *h; ///< powers of Bgbit
    uint32_t offset; ///< offset = Bg/2 * (2^(32-Bgbit) + 2^(32-2*Bgbit) + ... + 2^(32-l*Bgbit))

#ifdef __cplusplus

    TGswParams(int32_t l, int32_t Bgbit, const TLweParams *tlwe_params);

    ~TGswParams();

    TGswParams(const TGswParams &) = delete;

    void operator=(const TGswParams &) = delete;

#endif
};


struct TGswKey {
    const TGswParams *params; ///< the parameters
    const TLweParams *tlwe_params; ///< the tlwe params of each rows
    IntPolynomial *key; ///< the key (array of k polynomials)
    TLweKey tlwe_key;

#ifdef __cplusplus

    TGswKey(const TGswParams *params);

    ~TGswKey();

    TGswKey(const TGswKey &) = delete;

    void operator=(const TGswKey &) = delete;

#endif
};


struct TGswSample {
    TLweSample *all_sample; ///< TLweSample* all_sample; (k+1)l TLwe Sample
    TLweSample **bloc_sample;///< accès optionnel aux différents blocs de taille l.
    // double current_variance;
    const int32_t k;
    const int32_t l;

#ifdef __cplusplus

    inline TGswSample(TLweSample *all_sample, TLweSample **bloc_sample, const int32_t k, const int32_t l) :
            all_sample(all_sample),
            bloc_sample(bloc_sample),
            k(k), l(l) {}

    inline ~TGswSample() {}

    TGswSample(const TGswSample &) = delete;

    void operator=(const TGswSample &) = delete;

#endif
};

struct TGswSampleFFT {
    TLweSampleFFT *all_samples; ///< TLweSample* all_sample; (k+1)l TLwe Sample
    TLweSampleFFT **sample; ///< accès optionnel aux différents blocs de taille l.
    //double current_variance;
    const int32_t k;
    const int32_t l;

#ifdef __cplusplus

    TGswSampleFFT(const TGswParams *params, TLweSampleFFT *all_samples);

    ~TGswSampleFFT();

    TGswSampleFFT(const TGswSampleFFT &) = delete;

    void operator=(const TGswSampleFFT &) = delete;

#endif
};

//allocate memory space for a TGswKey
EXPORT TGswKey *alloc_TGswKey();
EXPORT TGswKey *alloc_TGswKey_array(int32_t nbelts);

//free memory space for a TGswKey
EXPORT void free_TGswKey(TGswKey *ptr);
EXPORT void free_TGswKey_array(int32_t nbelts, TGswKey *ptr);

//initialize the TGswKey structure
//(equivalent of the C++ constructor)
EXPORT void init_TGswKey(TGswKey *obj, const TGswParams *params);
EXPORT void init_TGswKey_array(int32_t nbelts, TGswKey *obj, const TGswParams *params);

//destroys the TGswKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TGswKey(TGswKey *obj);
EXPORT void destroy_TGswKey_array(int32_t nbelts, TGswKey *obj);

//allocates and initialize the TGswKey structure
//(equivalent of the C++ new)
EXPORT TGswKey *new_TGswKey(const TGswParams *params);
EXPORT TGswKey *new_TGswKey_array(int32_t nbelts, const TGswParams *params);

//destroys and frees the TGswKey structure
//(equivalent of the C++ delete)
EXPORT void delete_TGswKey(TGswKey *obj);
EXPORT void delete_TGswKey_array(int32_t nbelts, TGswKey *obj);
//allocate memory space for a TGswParams
EXPORT TGswParams *alloc_TGswParams();
EXPORT TGswParams *alloc_TGswParams_array(int32_t nbelts);

//free memory space for a TGswParams
EXPORT void free_TGswParams(TGswParams *ptr);
EXPORT void free_TGswParams_array(int32_t nbelts, TGswParams *ptr);

//initialize the TGswParams structure
//(equivalent of the C++ constructor)
EXPORT void init_TGswParams(TGswParams *obj, int32_t l, int32_t Bgbit, const TLweParams *tlwe_params);
EXPORT void init_TGswParams_array(int32_t nbelts, TGswParams *obj, int32_t l, int32_t Bgbit, const TLweParams *tlwe_params);

//destroys the TGswParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TGswParams(TGswParams *obj);
EXPORT void destroy_TGswParams_array(int32_t nbelts, TGswParams *obj);

//allocates and initialize the TGswParams structure
//(equivalent of the C++ new)
EXPORT TGswParams *new_TGswParams(int32_t l, int32_t Bgbit, const TLweParams *tlwe_params);
EXPORT TGswParams *new_TGswParams_array(int32_t nbelts, int32_t l, int32_t Bgbit, const TLweParams *tlwe_params);

//destroys and frees the TGswParams structure
//(equivalent of the C++ delete)
EXPORT void delete_TGswParams(TGswParams *obj);
EXPORT void delete_TGswParams_array(int32_t nbelts, TGswParams *obj);
//allocate memory space for a TGswSample
EXPORT TGswSample *alloc_TGswSample();
EXPORT TGswSample *alloc_TGswSample_array(int32_t nbelts);

//free memory space for a TGswSample
EXPORT void free_TGswSample(TGswSample *ptr);
EXPORT void free_TGswSample_array(int32_t nbelts, TGswSample *ptr);

//initialize the TGswSample structure
//(equivalent of the C++ constructor)
EXPORT void init_TGswSample(TGswSample *obj, const TGswParams *params);
EXPORT void init_TGswSample_array(int32_t nbelts, TGswSample *obj, const TGswParams *params);

//destroys the TGswSample structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TGswSample(TGswSample *obj);
EXPORT void destroy_TGswSample_array(int32_t nbelts, TGswSample *obj);

//allocates and initialize the TGswSample structure
//(equivalent of the C++ new)
EXPORT TGswSample *new_TGswSample(const TGswParams *params);
EXPORT TGswSample *new_TGswSample_array(int32_t nbelts, const TGswParams *params);

//destroys and frees the TGswSample structure
//(equivalent of the C++ delete)
EXPORT void delete_TGswSample(TGswSample *obj);
EXPORT void delete_TGswSample_array(int32_t nbelts, TGswSample *obj);

//allocate memory space for a TGswSampleFFT
EXPORT TGswSampleFFT *alloc_TGswSampleFFT();
EXPORT TGswSampleFFT *alloc_TGswSampleFFT_array(int32_t nbelts);

//free memory space for a TGswSampleFFT
EXPORT void free_TGswSampleFFT(TGswSampleFFT *ptr);
EXPORT void free_TGswSampleFFT_array(int32_t nbelts, TGswSampleFFT *ptr);

//initialize the TGswSampleFFT structure
//(equivalent of the C++ constructor)
EXPORT void init_TGswSampleFFT(TGswSampleFFT *obj, const TGswParams *params);
EXPORT void init_TGswSampleFFT_array(int32_t nbelts, TGswSampleFFT *obj, const TGswParams *params);

//destroys the TGswSampleFFT structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TGswSampleFFT(TGswSampleFFT *obj);
EXPORT void destroy_TGswSampleFFT_array(int32_t nbelts, TGswSampleFFT *obj);

//allocates and initialize the TGswSampleFFT structure
//(equivalent of the C++ new)
EXPORT TGswSampleFFT *new_TGswSampleFFT(const TGswParams *params);
EXPORT TGswSampleFFT *new_TGswSampleFFT_array(int32_t nbelts, const TGswParams *params);

//destroys and frees the TGswSampleFFT structure
//(equivalent of the C++ delete)
EXPORT void delete_TGswSampleFFT(TGswSampleFFT *obj);
EXPORT void delete_TGswSampleFFT_array(int32_t nbelts, TGswSampleFFT *obj);

#endif // TGSW_H
