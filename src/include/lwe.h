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
// *** ()
struct LweBootstrappingKey;
struct LweBootstrappingKeyFFT;
// * ()
struct IntPolynomial;
struct TorusPolynomial;
struct LagrangeHalfCPolynomial;
// **** Ilaria (Platinum)
struct SemiTGswParams;
struct SemiTGswKey;
struct SemiTGswSample;

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
typedef struct SemiTGswParams   SemiTGswParams;
typedef struct SemiTGswKey      SemiTGswKey;
typedef struct SemiTGswSample   SemiTGswSample;


#include "toruspolynomial-functions.h"

#include "lwe-functions.h"

// Ring
EXPORT void tLweKeyGen(TLweKey* result);
EXPORT void tLweSymEncrypt(TLweSample* result, TorusPolynomial* message, double alpha, const TLweKey* key);
EXPORT void tLweSymEncryptT(TLweSample* result, Torus32 message, double alpha, const TLweKey* key);
EXPORT void tLwePhase(TorusPolynomial* phase, const TLweSample* sample, const TLweKey* key);
EXPORT void tLweApproxPhase(TorusPolynomial* message, const TorusPolynomial* phase, int Msize, int N);
EXPORT void tLweSymDecrypt(TorusPolynomial* result, const TLweSample* sample, const TLweKey* key, int Msize);
EXPORT Torus32 tLweSymDecryptT(const TLweSample* sample, const TLweKey* key, int Msize);
EXPORT void sampleExtract(LweSample* result, const TLweSample* x, const LweParams* params,  const TLweParams* rparams);

//Arithmetic operations on TLwe samples
/** result = (0,0) */
EXPORT void tLweClear(TLweSample* result, const TLweParams* params);
/** result = sample */
EXPORT void tLweCopy(TLweSample* result, const TLweSample* sample, const TLweParams* params);
/** result = (0,mu) */
EXPORT void tLweNoiselessTrivial(TLweSample* result, const TorusPolynomial* mu, const TLweParams* params);
/** result = result + sample */
EXPORT void tLweAddTo(TLweSample* result, const TLweSample* sample, const TLweParams* params);
/** result = result - sample */
EXPORT void tLweSubTo(TLweSample* result, const TLweSample* sample, const TLweParams* params);
/** result = result + p.sample */
EXPORT void tLweAddMulTo(TLweSample* result, int p, const TLweSample* sample, const TLweParams* params);
/** result = result - p.sample */
EXPORT void tLweSubMulTo(TLweSample* result, int p, const TLweSample* sample, const TLweParams* params);

// EXPORT void tLwePolyCombination(TLweSample* result, const int* combi, const TLweSample* samples, const TLweParams* params);


// Gsw
EXPORT void tGswKeyGen(TGswKey* result);
EXPORT void tGswSymEncrypt(TGswSample* result, const IntPolynomial* message, double alpha, const TGswKey* key);
EXPORT void tGswSymEncryptInt(TGswSample* result, const int message, double alpha, const TGswKey* key);
EXPORT void tGswSymDecrypt(IntPolynomial* result, const TGswSample* sample, const TGswKey* key, const int Msize);
EXPORT int tGswSymDecryptInt(const TGswSample* sample, const TGswKey* key); 
//do we really decrypt Gsw samples?

// support Functions for TGsw
// Result = 0
EXPORT void tGswClear(TGswSample* result, const TGswParams* params);
// Result += H
EXPORT void tGswAddH(TGswSample* result, const TGswParams* params);
// Result += mu*H
EXPORT void tGswAddMuH(TGswSample* result, const IntPolynomial* message, const TGswParams* params);
// Result += mu*H, mu integer
EXPORT void tGswAddMuIntH(TGswSample* result, const int message, const TGswParams* params);
// Result = tGsw(0)
EXPORT void tGswEncryptZero(TGswSample* result, double alpha, const TGswKey* key);

//fonction de decomposition
EXPORT void tLweDecompH(IntPolynomial* result, const TLweSample* sample, const TGswParams* params);

EXPORT void Torus32PolynomialDecompH(IntPolynomial* result, const TorusPolynomial* sample, const TGswParams* params);
EXPORT void tLweDecompH(IntPolynomial* result, const TLweSample* sample,const TGswParams* params);	

//TODO: Ilaria.Theoreme3.5
EXPORT void tGswExternProduct(TLweSample* result, const TGswSample* a, const TLweSample* b, const TGswParams* params);

// result=result+ (X^ai-1)*bi (ligne 5 de l'algo)
EXPORT void tGswMulByXaiMinusOne(TGswSample* result, int ai, const TGswSample* bk, const TGswParams* params);
EXPORT void tLweMulByXaiMinusOne(TLweSample* result, int ai, const TLweSample* bk, const TLweParams* params);

//ligne 5 algo,mult externe
EXPORT void tLweExternMulRGswTo(TLweSample* accum, const TGswSample* sample,const TGswParams* params);

/** result = (0,mu) */
EXPORT void tGswNoiselessTrivial(TGswSample* result, const TorusPolynomial* mu, const TGswParams* params);

/** result = result + sample */
EXPORT void tGswAddTo(TGswSample* result, const TGswSample* sample, const TGswParams* params);

/** result = result - sample */
//EXPORT void tGswSubTo(TLweSample* result, const TLweSample* sample, const TLweParams* params);
/** result = result + p.sample */
//EXPORT void tGswAddMulTo(TLweSample* result, int p, const TLweSample* sample, const TLweParams* params);
/** result = result - p.sample */
//EXPORT void tGswSubMulTo(TLweSample* result, int p, const TLweSample* sample, const TLweParams* params);


//extractions TLwe -> Lwe
EXPORT void tLweExtractKey(LweKey* result, const TLweKey*); //sans doute un param supplémentaire
EXPORT void tLweExtractSample(LweSample* result, const TLweSample* x);

//extraction TGsw -> SemiTGsw
EXPORT void tGswExtractKey(SemiTGswSample* result, const TGswKey* key);
EXPORT void tGswExtractSample(TLweSample* result, const TGswSample* x);

//Lwe to Lwe Single gate bootstrapping
//TODO: Malika
EXPORT void lweToLweBootstrap(LweSample* result, const LweBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LweSample* x);


//these functions call the bootstrapping, assuming that the message space is {0,1/4} 
EXPORT void lweNand(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b);
EXPORT void lweOr(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b);
EXPORT void lweAnd(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b);
EXPORT void lweXor(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b);
// mux(a,b,c) = a?b:c = a et b + not(a) et c 
EXPORT void lweMux(LweSample* result, const LweBootstrappingKey* bk, const LweSample* a, const LweSample* b, const LweSample* c);
EXPORT void lweNot(LweSample* result, LweSample* a);


//leveled functions

//Lwe to SemiRing Bootstrapping
EXPORT void semiRingBootstrap(LweSample* result, const LweBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LweSample* x);




EXPORT void TorusPolynomialMulByXai(TorusPolynomial* result, int a, const TorusPolynomial* bk);


#include "autogenerated.h"

#endif //LWE_H
