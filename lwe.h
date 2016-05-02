#ifndef LWE_H
#define LWE_H

///@file
///@brief This file declares almost everything

#include <stdint.h>

//not very important, but all the functions exported in the output library
//should use the C naming convention, for inter-compiler compatibility
//reasons, or to bind it with non C++ code.
#ifdef __cplusplus
#define EXPORT extern "C"
#else
#define EXPORT 
#endif

// Idea:
// we may want to represent an element x of the real torus by 
// the integer rint(2^32.x) modulo 2^32
//  -- addition, subtraction and integer combinations are native operation
//  -- modulo 1 is mapped to mod 2^32, which is also native!
// This looks much better than using float/doubles, where modulo 1 is not
// natural at all.
typedef int32_t Torus32; //avant uint32_t
//typedef int64_t Torus64; //avant uint64_t

#include "numeric-functions.h"

struct LWEParams;
struct LWEKey;
struct LWESample;
struct LWEKeySwitchKey;
struct RingLWEParams;
struct RingLWEKey;
struct RingLWESample;
struct RingLWESampleFFT;
struct RingGSWParams;
struct RingGSWKey;
struct RingGSWSample;
struct RingGSWSampleFFT;
// *** ()
struct LWEBootstrappingKey;
// * ()
struct IntPolynomial;
struct TorusPolynomial;
struct LagrangeHalfCPolynomial;
// **** Ilaria (Platinum)
struct SemiRingGSWParams;
struct SemiRingGSWKey;
struct SemiRingGSWSample;

//this is for compatibility with C code, to be able to use
//"LWEParams" as a type and not "struct LWEParams"
typedef struct LWEParams           LWEParams;
typedef struct LWEKey              LWEKey;
typedef struct LWESample           LWESample;
typedef struct LWEKeySwitchKey     LWEKeySwitchKey;
typedef struct RingLWEParams       RingLWEParams;
typedef struct RingLWEKey          RingLWEKey;
typedef struct RingLWESample       RingLWESample;
typedef struct RingLWESampleFFT       RingLWESampleFFT;
typedef struct RingGSWParams       RingGSWParams;
typedef struct RingGSWKey          RingGSWKey;
typedef struct RingGSWSample       RingGSWSample;
typedef struct RingGSWSampleFFT       RingGSWSampleFFT;
typedef struct LWEBootstrappingKey LWEBootstrappingKey;
typedef struct IntPolynomial	   IntPolynomial;
typedef struct TorusPolynomial	   TorusPolynomial;
typedef struct LagrangeHalfCPolynomial	   LagrangeHalfCPolynomial;
typedef struct SemiRingGSWParams   SemiRingGSWParams;
typedef struct SemiRingGSWKey      SemiRingGSWKey;
typedef struct SemiRingGSWSample   SemiRingGSWSample;


#include "toruspolynomial-functions.h"

#include "lwe-functions.h"

// Ring
EXPORT void ringLweKeyGen(RingLWEKey* result);
EXPORT void ringLweSymEncrypt(RingLWESample* result, TorusPolynomial* message, double alpha, const RingLWEKey* key);
EXPORT void ringLweSymEncryptT(RingLWESample* result, Torus32 message, double alpha, const RingLWEKey* key);
EXPORT void ringLwePhase(TorusPolynomial* phase, const RingLWESample* sample, const RingLWEKey* key);
EXPORT void ringLweApproxPhase(TorusPolynomial* message, const TorusPolynomial* phase, int Msize, int N);
EXPORT void ringLweSymDecrypt(TorusPolynomial* result, const RingLWESample* sample, const RingLWEKey* key, int Msize);
EXPORT Torus32 ringLweSymDecryptT(const RingLWESample* sample, const RingLWEKey* key, int Msize);

//Arithmetic operations on RingLWE samples
/** result = (0,0) */
EXPORT void ringLweClear(RingLWESample* result, const RingLWEParams* params);
/** result = sample */
EXPORT void ringLweCopy(RingLWESample* result, const RingLWESample* sample, const RingLWEParams* params);
/** result = (0,mu) */
EXPORT void ringLweNoiselessTrivial(RingLWESample* result, const TorusPolynomial* mu, const RingLWEParams* params);
/** result = result + sample */
EXPORT void ringLweAddTo(RingLWESample* result, const RingLWESample* sample, const RingLWEParams* params);
/** result = result - sample */
EXPORT void ringLweSubTo(RingLWESample* result, const RingLWESample* sample, const RingLWEParams* params);
/** result = result + p.sample */
EXPORT void ringLweAddMulTo(RingLWESample* result, int p, const RingLWESample* sample, const RingLWEParams* params);
/** result = result - p.sample */
EXPORT void ringLweSubMulTo(RingLWESample* result, int p, const RingLWESample* sample, const RingLWEParams* params);

// EXPORT void ringLwePolyCombination(RingLWESample* result, const int* combi, const RingLWESample* samples, const RingLWEParams* params);


// GSW
EXPORT void ringGswKeyGen(RingGSWKey* result);
EXPORT void ringGswSymEncrypt(RingGSWSample* result, const IntPolynomial* message, double alpha, const RingGSWKey* key);
EXPORT void ringGswSymEncryptInt(RingGSWSample* result, const int message, double alpha, const RingGSWKey* key);
EXPORT void ringGswSymDecrypt(IntPolynomial* result, const RingGSWSample* sample, const RingGSWKey* key); 
EXPORT int ringGswSymDecryptInt(const RingGSWSample* sample, const RingGSWKey* key); 
//do we really decrypt GSW samples?

// support Functions for RingGSW
// Result = 0
EXPORT void ringGSWClear(RingGSWSample* result, const RingGSWParams* params);
// Result += H
EXPORT void ringGSWAddH(RingGSWSample* result, const RingGSWParams* params);
// Result += mu*H
EXPORT void ringGSWAddMuH(RingGSWSample* result, const IntPolynomial* message, const RingGSWParams* params);
// Result += mu*H, mu integer
EXPORT void ringGSWAddMuIntH(RingGSWSample* result, const int message, const RingGSWParams* params);
// Result = ringGsw(0)
EXPORT void ringGSWEncryptZero(RingGSWSample* result, double alpha, const RingGSWKey* key);

//fonction de decomposition
EXPORT void ringLWEDecompH(IntPolynomial* result, const RingLWESample* sample, const RingGSWParams* params);

EXPORT void Torus32PolynomialDecompH(IntPolynomial* result, const TorusPolynomial* sample, const RingGSWParams* params);
EXPORT void ringLWEDecompH(IntPolynomial* result, const RingLWESample* sample,const RingGSWParams* params);	

//TODO: Ilaria.Theoreme3.5
EXPORT void ringGSWExternProduct(RingLWESample* result, const RingGSWSample* a, const RingLWESample* b, const RingGSWParams* params);

// result=result+ (X^ai-1)*bi (ligne 5 de l'algo)
EXPORT void ringGSWMulByXaiMinusOne(RingGSWSample* result, int ai, const RingGSWSample* bk, const RingGSWParams* params);
EXPORT void ringLWEMulByXaiMinusOne(RingLWESample* result, int ai, const RingGSWSample* bk, const RingLWEParams* params);

//ligne 5 algo,mult externe
EXPORT void ringLWEExternMulRGSWTo(RingLWESample* accum, const RingGSWSample* sample,const RingGSWParams* params);

/** result = (0,mu) */
EXPORT void ringGswNoiselessTrivial(RingGSWSample* result, const TorusPolynomial* mu, const RingGSWParams* params);

/** result = result + sample */
EXPORT void ringGswAddTo(RingGSWSample* result, const RingGSWSample* sample, const RingGSWParams* params);

/** result = result - sample */
//EXPORT void ringGswSubTo(RingLWESample* result, const RingLWESample* sample, const RingLWEParams* params);
/** result = result + p.sample */
//EXPORT void ringGswAddMulTo(RingLWESample* result, int p, const RingLWESample* sample, const RingLWEParams* params);
/** result = result - p.sample */
//EXPORT void ringGswSubMulTo(RingLWESample* result, int p, const RingLWESample* sample, const RingLWEParams* params);


//extractions RingLWE -> LWE
EXPORT void ringLweExtractKey(LWEKey* result, const RingLWEKey*); //sans doute un param supplémentaire
EXPORT void ringLweExtractSample(LWESample* result, const RingLWESample* x);

//extraction RingGSW -> SemiRingGSW
EXPORT void ringGswExtractKey(SemiRingGSWSample* result, const RingGSWKey* key);
EXPORT void ringGswExtractSample(RingLWESample* result, const RingGSWSample* x);

//LWE to LWE Single gate bootstrapping
//TODO: Malika
EXPORT void lweToLweBootstrap(LWESample* result, const LWEBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LWESample* x);


//these functions call the bootstrapping, assuming that the message space is {0,1/4} 
EXPORT void lweNand(LWESample* result, const LWEBootstrappingKey* bk, const LWESample* a, const LWESample* b);
EXPORT void lweOr(LWESample* result, const LWEBootstrappingKey* bk, const LWESample* a, const LWESample* b);
EXPORT void lweAnd(LWESample* result, const LWEBootstrappingKey* bk, const LWESample* a, const LWESample* b);
EXPORT void lweXor(LWESample* result, const LWEBootstrappingKey* bk, const LWESample* a, const LWESample* b);
// mux(a,b,c) = a?b:c = a et b + not(a) et c 
EXPORT void lweMux(LWESample* result, const LWEBootstrappingKey* bk, const LWESample* a, const LWESample* b, const LWESample* c);
EXPORT void lweNot(LWESample* result, LWESample* a);


//leveled functions

//LWE to SemiRing Bootstrapping
EXPORT void semiRingBootstrap(LWESample* result, const LWEBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LWESample* x);




#include "autogenerated.h"

#endif //LWE_H
