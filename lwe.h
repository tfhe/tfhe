#ifndef LWE_H
#define LWE_H

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


/**
 * this function converts a double to a Torus32
 */
EXPORT Torus32 dtot32(double d);
/**
 * this function converts a torus32 to a double in [-0.5;0.5]
 */
EXPORT double t32tod(Torus32 x);




// TorusPolynomial = 0
EXPORT void ClearTorusPolynomial(TorusPolynomial* result);
// TorusPolynomial = random
EXPORT void UniformTorusPolynomial(TorusPolynomial* result);
// TorusPolynomial = TorusPolynomial
EXPORT void CopyTorusPolynomial(TorusPolynomial* __restrict result, const TorusPolynomial* __restrict sample);
// TorusPolynomial + TorusPolynomial
EXPORT void AddTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2);
// TorusPolynomial += TorusPolynomial
EXPORT void AddToTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly2);
// TorusPolynomial - TorusPolynomial
EXPORT void SubTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, const TorusPolynomial* poly2);
// TorusPolynomial -= TorusPolynomial
EXPORT void SubToTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly2);
// TorusPolynomial + p*TorusPolynomial
EXPORT void AddMulZTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, int p, const TorusPolynomial* poly2);
// TorusPolynomial += p*TorusPolynomial
EXPORT void AddMulZToTorusPolynomial(TorusPolynomial* result, const int p, const TorusPolynomial* poly2);
// TorusPolynomial - p*TorusPolynomial
EXPORT void SubMulZTorusPolynomial(TorusPolynomial* result, const TorusPolynomial* poly1, const int p, const TorusPolynomial* poly2);
// TorusPolynomial -= p*TorusPolynomial
EXPORT void SubMulZToTorusPolynomial(TorusPolynomial* result, int p, const TorusPolynomial* poly2);



// Norme Euclidienne d'un IntPolynomial
EXPORT int NormEuclidIntPolynomial(const IntPolynomial* poly);
// Gaussian sample centered in message, with standard deviation sigma
Torus32 gaussian32(Torus32 message, double sigma);

// Used to approximate the phase to the nearest message possible in the message space
// The constant Msize will indicate on which message space we are working (how many messages possible)
Torus32 approxPhase(Torus32 phase, int Msize);








/**
 * This function generates a random LWE key for the given parameters.
 * The LWE key for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
EXPORT void lweKeyGen(LWEKey* result);
/**
 * This function lwe encrypts a message with the given key
 */
EXPORT void lweSymEncrypt(LWESample* result, Torus32 message, double alpha, const LWEKey* key);
/**
 * this function computes the phase of a lwe sample
 */
EXPORT Torus32 lwePhase(const LWESample* sample, const LWEKey* key);
/**
 * this function decrypts a lwe sample by rounding the phase on the message space (1/Msize.Z) / Z
 */
EXPORT Torus32 lweSymDecrypt(const LWESample* sample, const LWEKey* key, const int Msize);

//Arithmetic operations on LWE samples
/** result = (0,0) */
EXPORT void lweClear(LWESample* result, const LWEParams* params);
/** result = (0,mu) */
EXPORT void lweNoiselessTrivial(LWESample* result, Torus32 mu, const LWEParams* params);
/** result = result + sample */
EXPORT void lweAddTo(LWESample* result, const LWESample* sample, const LWEParams* params);
/** result = result - sample */
EXPORT void lweSubTo(LWESample* result, const LWESample* sample, const LWEParams* params);
/** result = result + p.sample */
EXPORT void lweAddMulTo(LWESample* result, int p, const LWESample* sample, const LWEParams* params);
/** result = result - p.sample */
EXPORT void lweSubMulTo(LWESample* result, int p, const LWESample* sample, const LWEParams* params);

EXPORT void createKeySwitchKey(LWEKeySwitchKey* result, const LWEKey* in_key, const LWEKey* out_key);
EXPORT void lweKeySwitch(LWESample* result, const LWEKeySwitchKey* ks, const LWESample* sample);


// Ring
EXPORT void ringLweKeyGen(RingLWEKey* result);
EXPORT void ringLweSymEncrypt(RingLWESample* result, TorusPolynomial* message, double alpha, const RingLWEKey* key);
EXPORT void ringLweSymEncryptT(RingLWESample* result, Torus32 message, double alpha, const RingLWEKey* key);
EXPORT void ringLwePhase(TorusPolynomial* phase, const RingLWESample* sample, const RingLWEKey* key);
EXPORT void ringLweApproxPhase(TorusPolynomial* message, const TorusPolynomial* phase, int Msize, int N);
EXPORT void ringLweSymDecrypt(TorusPolynomial* result, const RingLWESample* sample, const RingLWEKey* key, int Msize);
EXPORT void ringLweSymDecryptT(Torus32 result, const RingLWESample* sample, const RingLWEKey* key, int Msize);

//Arithmetic operations on RingLWE samples
/** result = (0,0) */
EXPORT void RingLweClear(RingLWESample* result, const RingLWEParams* params);
/** result = (0,mu) */
EXPORT void RingLweNoiselessTrivial(RingLWESample* result, const TorusPolynomial* mu, const RingLWEParams* params);
/** result = result + sample */
EXPORT void RingLweAddTo(RingLWESample* result, const RingLWESample* sample, const RingLWEParams* params);
/** result = result - sample */
EXPORT void RingLweSubTo(RingLWESample* result, const RingLWESample* sample, const RingLWEParams* params);
/** result = result + p.sample */
EXPORT void RingLweAddMulTo(RingLWESample* result, int p, const RingLWESample* sample, const RingLWEParams* params);
/** result = result - p.sample */
EXPORT void RingLweSubMulTo(RingLWESample* result, int p, const RingLWESample* sample, const RingLWEParams* params);

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
// Result = RingGsw(0)
EXPORT void ringGSWEncryptZero(RingGSWSample* result, double alpha, const RingGSWKey* key);

//fonction de decomposition
EXPORT void ringLWEDecompH(IntPolynomial* result, const RingLWESample* sample, const RingGSWParams* params);

EXPORT void Torus32PolynomialDecompH(IntPolynomial* result, const TorusPolynomial* sample, const RingGSWParams* params);
EXPORT void ringLWEDecompH(IntPolynomial* result, const RingLWESample* sample,const RingGSWParams* params);	

//TODO: Ilaria.Theoreme3.5
EXPORT void ringGSWExternProduct(RingLWESample* result, const RingGSWSample* a, const RingLWESample* b, const RingGSWParams* params);

// result=result+ (X^ai-1)*bi (ligne 5 de l'algo)
EXPORT void ringGSWMulByXaiMinusOne(RingGSWSample* result, int ai, const RingGSWSample* bk, const RingGSWParams* params);
EXPORT void ringLWEMulByXaiMinusOne(RingLWESample* result, int ai, const RingLWESample* bk, const RingLWEParams* params);

//ligne 5 algo,mult externe
EXPORT void ringLWEExternMulRGSWTo(RingLWESample* accum, const RingGSWSample* sample,const RingGSWParams* params);

/** result = (0,mu) */
EXPORT void RingGSwNoiselessTrivial(RingGSWSample* result, const TorusPolynomial* mu, const RingGSWParams* params);

/** result = result + sample */
EXPORT void RingGswAddTo(RingGSWSample* result, const RingGSWSample* sample, const RingGSWParams* params);

/** result = result - sample */
//EXPORT void RingGswSubTo(RingLWESample* result, const RingLWESample* sample, const RingLWEParams* params);
/** result = result + p.sample */
//EXPORT void RingGswAddMulTo(RingLWESample* result, int p, const RingLWESample* sample, const RingLWEParams* params);
/** result = result - p.sample */
//EXPORT void RingGswSubMulTo(RingLWESample* result, int p, const RingLWESample* sample, const RingLWEParams* params);


//extractions RingLWE -> LWE
EXPORT void ringLweExtractKey(LWEKey* result, const RingLWEKey*); //sans doute un param supplémentaire
EXPORT void ringLweExtractSample(LWESample* result, const RingLWESample* x);

//extraction RingGSW -> SemiRingGSW
EXPORT void ringGswExtractKey(SemiRingGSWSample* result, const RingGSWKey* key);
EXPORT void ringGswExtractSample(RingLWESample* result, const RingGSWSample* x);

//LWE to LWE Single gate bootstrapping
//TODO: Malika
EXPORT void bootstrap(LWESample* result, const LWEBootstrappingKey* bk, Torus32 mu1, Torus32 mu0, const LWESample* x);


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



//multiplication Karatsuba
EXPORT void multKaratsuba(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);

//multiplication naive 
EXPORT void multNaive(TorusPolynomial* result, const IntPolynomial* poly1, const TorusPolynomial* poly2);


#include "autogenerated.h"

#endif //LWE_H
