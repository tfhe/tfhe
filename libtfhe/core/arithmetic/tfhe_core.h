#ifndef TFHE_CORE_H
#define TFHE_CORE_H

///@file
///@brief This file declares only the structures names
#include <stdint.h>

void die_dramatically(const char* message);


// Idea:
// we may want to represent an element x of the real torus by
// the integer rint(2^32.x) modulo 2^32
//  -- addition, subtraction and integer combinations are native operation
//  -- modulo 1 is mapped to mod 2^32, which is also native!
// This looks much better than using float/doubles, where modulo 1 is not
// natural at all.
typedef int32_t Torus32; //before uint32_t
typedef int64_t Torus64; //before uint64_t

template<typename TORUS>
class TLweParams;

template<typename TORUS>
class TLwePublicParams;

template<typename TORUS>
struct TLweKey;

template<typename TORUS>
struct TLwePublicKey;

template<typename TORUS>
struct TLweSample;

template<typename TORUS>
struct TLweKeySwitchKey;

template<typename TORUS>
struct TRLweParams;

template<typename TORUS>
struct TRLweKey;

template<typename TORUS>
struct TRLweSample;

template<typename TORUS>
struct TRLweSampleFFT;

template<typename TORUS>
struct TRGswParams;

template<typename TORUS>
struct TRGswKey;

template<typename TORUS>
struct TRGswSample;

template<typename TORUS>
struct TRGswSampleFFT;

// *** ()
template<typename TORUS>
struct TLweBootstrappingKey;

template<typename TORUS>
struct TLweBootstrappingKeyFFT;

// * ()
struct IntPolynomial;

template<typename TORUS>
struct TorusPolynomial;

struct LagrangeHalfCPolynomial;

// **** Platinum
//struct SemiTGswParams;
//struct SemiTGswKey;
//struct SemiTGswSample;

#endif //TFHE_CORE_H
