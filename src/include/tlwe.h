#ifndef RINGLWE_H
#define RINGLWE_H

///@file
///@brief This file contains the declaration of tlwe structures

#include "tfhe_core.h"
#include "polynomials.h"
#include "lweparams.h"

struct TLweParams {
    const int N; ///< a power of 2: degree of the polynomials
    const int k; ///< number of polynomials in the mask
    const double alpha_min; ///< minimal noise s.t. the sample is secure
    const double alpha_max; ///< maximal noise s.t. we can decrypt
    const LweParams extracted_lweparams; ///< lwe params if one extracts

#ifdef __cplusplus
    TLweParams(int N, int k, double alpha_min, double alpha_max);
    ~TLweParams();
    TLweParams(const TLweParams&) = delete;
    void operator=(const TLweParams&) = delete;
#endif
};

struct TLweKey {
    const TLweParams* params; ///< the parameters of the key
    IntPolynomial* key; ///< the key (i.e k binary polynomials)
#ifdef __cplusplus
    TLweKey(const TLweParams* params);
    ~TLweKey();
    TLweKey(const TLweKey&) = delete;
    void operator=(const TLweKey&) = delete;
#endif
};


struct TLweSample {
    TorusPolynomial* a; ///< array of length k+1: mask + right term
    TorusPolynomial* b; ///< alias of a[k] to get the right term
    double current_variance; ///< avg variance of the sample
    const int k; 
#ifdef __cplusplus
    TLweSample(const TLweParams* params);
    ~TLweSample();
    TLweSample(const TLweSample&) = delete;
    void operator=(const TLweSample&) = delete;
#endif
};

struct TLweSampleFFT {
    LagrangeHalfCPolynomial* a; ///< array of length k+1: mask + right term
    LagrangeHalfCPolynomial* b; ///< alias of a[k] to get the right term
    double current_variance; ///< avg variance of the sample
    const int k; //required during the destructor call...
#ifdef __cplusplus
    TLweSampleFFT(const TLweParams* params);
    ~TLweSampleFFT();
    TLweSampleFFT(const TLweSampleFFT&) = delete;
    void operator=(const TLweSampleFFT&) = delete;
#endif
};

//allocate memory space for a TLweKey
EXPORT TLweKey* alloc_TLweKey();
EXPORT TLweKey* alloc_TLweKey_array(int nbelts);

//free memory space for a TLweKey
EXPORT void free_TLweKey(TLweKey* ptr);
EXPORT void free_TLweKey_array(int nbelts, TLweKey* ptr);

//initialize the TLweKey structure
//(equivalent of the C++ constructor)
EXPORT void init_TLweKey(TLweKey* obj, const TLweParams* params);
EXPORT void init_TLweKey_array(int nbelts, TLweKey* obj, const TLweParams* params);

//destroys the TLweKey structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TLweKey(TLweKey* obj);
EXPORT void destroy_TLweKey_array(int nbelts, TLweKey* obj);
 
//allocates and initialize the TLweKey structure
//(equivalent of the C++ new)
EXPORT TLweKey* new_TLweKey(const TLweParams* params);
EXPORT TLweKey* new_TLweKey_array(int nbelts, const TLweParams* params);

//destroys and frees the TLweKey structure
//(equivalent of the C++ delete)
EXPORT void delete_TLweKey(TLweKey* obj);
EXPORT void delete_TLweKey_array(int nbelts, TLweKey* obj);
//allocate memory space for a TLweParams
EXPORT TLweParams* alloc_TLweParams();
EXPORT TLweParams* alloc_TLweParams_array(int nbelts);

//free memory space for a TLweParams
EXPORT void free_TLweParams(TLweParams* ptr);
EXPORT void free_TLweParams_array(int nbelts, TLweParams* ptr);

//initialize the TLweParams structure
//(equivalent of the C++ constructor)
EXPORT void init_TLweParams(TLweParams* obj, int N, int k, double alpha_min, double alpha_max);
EXPORT void init_TLweParams_array(int nbelts, TLweParams* obj, int N, int k, double alpha_min, double alpha_max);

//destroys the TLweParams structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TLweParams(TLweParams* obj);
EXPORT void destroy_TLweParams_array(int nbelts, TLweParams* obj);
 
//allocates and initialize the TLweParams structure
//(equivalent of the C++ new)
EXPORT TLweParams* new_TLweParams(int N, int k, double alpha_min, double alpha_max);
EXPORT TLweParams* new_TLweParams_array(int nbelts, int N, int k, double alpha_min, double alpha_max);

//destroys and frees the TLweParams structure
//(equivalent of the C++ delete)
EXPORT void delete_TLweParams(TLweParams* obj);
EXPORT void delete_TLweParams_array(int nbelts, TLweParams* obj);
//allocate memory space for a TLweSample
EXPORT TLweSample* alloc_TLweSample();
EXPORT TLweSample* alloc_TLweSample_array(int nbelts);

//free memory space for a TLweSample
EXPORT void free_TLweSample(TLweSample* ptr);
EXPORT void free_TLweSample_array(int nbelts, TLweSample* ptr);

//initialize the TLweSample structure
//(equivalent of the C++ constructor)
EXPORT void init_TLweSample(TLweSample* obj, const TLweParams* params);
EXPORT void init_TLweSample_array(int nbelts, TLweSample* obj, const TLweParams* params);

//destroys the TLweSample structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TLweSample(TLweSample* obj);
EXPORT void destroy_TLweSample_array(int nbelts, TLweSample* obj);
 
//allocates and initialize the TLweSample structure
//(equivalent of the C++ new)
EXPORT TLweSample* new_TLweSample(const TLweParams* params);
EXPORT TLweSample* new_TLweSample_array(int nbelts, const TLweParams* params);

//destroys and frees the TLweSample structure
//(equivalent of the C++ delete)
EXPORT void delete_TLweSample(TLweSample* obj);
EXPORT void delete_TLweSample_array(int nbelts, TLweSample* obj);

//allocate memory space for a TLweSampleFFT
EXPORT TLweSampleFFT* alloc_TLweSampleFFT();
EXPORT TLweSampleFFT* alloc_TLweSampleFFT_array(int nbelts);

//free memory space for a TLweSampleFFT
EXPORT void free_TLweSampleFFT(TLweSampleFFT* ptr);
EXPORT void free_TLweSampleFFT_array(int nbelts, TLweSampleFFT* ptr);

//initialize the TLweSampleFFT structure
//(equivalent of the C++ constructor)
EXPORT void init_TLweSampleFFT(TLweSampleFFT* obj, const TLweParams* params);
EXPORT void init_TLweSampleFFT_array(int nbelts, TLweSampleFFT* obj, const TLweParams* params);

//destroys the TLweSampleFFT structure
//(equivalent of the C++ destructor)
EXPORT void destroy_TLweSampleFFT(TLweSampleFFT* obj);
EXPORT void destroy_TLweSampleFFT_array(int nbelts, TLweSampleFFT* obj);
 
//allocates and initialize the TLweSampleFFT structure
//(equivalent of the C++ new)
EXPORT TLweSampleFFT* new_TLweSampleFFT(const TLweParams* params);
EXPORT TLweSampleFFT* new_TLweSampleFFT_array(int nbelts, const TLweParams* params);

//destroys and frees the TLweSampleFFT structure
//(equivalent of the C++ delete)
EXPORT void delete_TLweSampleFFT(TLweSampleFFT* obj);
EXPORT void delete_TLweSampleFFT_array(int nbelts, TLweSampleFFT* obj);
#endif // RINGLWE_H
