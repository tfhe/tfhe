#ifndef TFHE_CORE_H
#define TFHE_CORE_H

///@file
///@brief This file declares only the structures names
#include <stdint.h>

//not very important, but all the functions exported in the output library
//should use the C naming convention, for inter-compiler compatibility
//reasons, or to bind it with non C++ code.
#ifdef __cplusplus
#define EXPORT extern "C"
#include "tfhe_generic_templates.h"
#else
#define EXPORT
#endif

EXPORT void die_dramatically(const char* message);


// Torus32 and Torus64 types: approximate real numbers modulo 1

// Torus32 represents approximations of torus numbers up to precision 2^-32
//   the 32-bits (b0,b1,..,b31) little-endian encode
//   the real number 0.<b31><b30>...<b0> written in binary, and reduced modulo 1
// Under this representation,
//  -- addition, subtraction coincide bit-wise with the native 32-bit integer addition/subtraction
//  -- external multiplication by an integer with result modulo 1 coincide bit-wise with the native 32-bit product
//  -- the typedef trick below ensures that Torus32 are treated syntaxically as a primitive type and enables
//     the =,==,!=,+,-,+=,-= operators between Torus32 elements, as well as *,*= between any small integer and a Torus32.
//  -- despite the typedef line, Torus32 are not integers: any other operator, such as increment, division, cast,
//     internal product is illegal and will yield Undefined Behaviour.

// Similarly, Torus64 type represents approximations of torus numbers up to precision 2^-64
//   the 64-bits (b0,b1,..,b63) little-endian encode
//   the real number 0.<b63><b62>...<b0> written in binary, and reduced modulo 1
typedef int32_t Torus32; //avant uint32_t
//typedef int64_t Torus64; //avant uint64_t

struct LweParams;
struct LweKey;
struct LweSample;
struct LweKeySwitchKey;
struct TLweParams;
struct TLweKey;
struct TLweSample;
struct TLweSampleFFT;
struct TGswParams;
struct TGswKey;
struct TGswSample;
struct TGswSampleFFT;
struct LweBootstrappingKey;
struct LweBootstrappingKeyFFT;
struct IntPolynomial;
struct TorusPolynomial;
struct LagrangeHalfCPolynomial;
struct TFheGateBootstrappingParameterSet;
struct TFheGateBootstrappingCloudKeySet;
struct TFheGateBootstrappingSecretKeySet;

//this is for compatibility with C code, to be able to use
//"LweParams" as a type and not "struct LweParams"
typedef struct LweParams           LweParams;
typedef struct LweKey              LweKey;
typedef struct LweSample           LweSample;
typedef struct LweKeySwitchKey     LweKeySwitchKey;
typedef struct TLweParams       TLweParams;
typedef struct TLweKey          TLweKey;
typedef struct TLweSample       TLweSample;
typedef struct TLweSampleFFT       TLweSampleFFT;
typedef struct TGswParams       TGswParams;
typedef struct TGswKey          TGswKey;
typedef struct TGswSample       TGswSample;
typedef struct TGswSampleFFT       TGswSampleFFT;
typedef struct LweBootstrappingKey LweBootstrappingKey;
typedef struct LweBootstrappingKeyFFT LweBootstrappingKeyFFT;
typedef struct IntPolynomial	   IntPolynomial;
typedef struct TorusPolynomial	   TorusPolynomial;
typedef struct LagrangeHalfCPolynomial	   LagrangeHalfCPolynomial;
typedef struct TFheGateBootstrappingParameterSet TFheGateBootstrappingParameterSet;
typedef struct TFheGateBootstrappingCloudKeySet TFheGateBootstrappingCloudKeySet;
typedef struct TFheGateBootstrappingSecretKeySet TFheGateBootstrappingSecretKeySet;

#endif //TFHE_CORE_H
