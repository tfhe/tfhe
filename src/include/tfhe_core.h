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
typedef int32_t Torus32; //avant uint32_t
typedef int64_t Torus64; //avant uint64_t

template<typename TORUS>
class LweParams;

template<typename TORUS>
class LwePublicParams;

template<typename TORUS>
struct LweKey;

template<typename TORUS>
struct LwePublicKey;

template<typename TORUS>
struct LweSample;

template<typename TORUS>
struct LweSample_array;

template<typename TORUS>
struct LweKeySwitchKey;

template<typename TORUS>
struct TLweParams;

template<typename TORUS>
struct TLweKey;

template<typename TORUS>
struct TLweSample;

template<typename TORUS>
struct TLweSampleFFT;

template<typename TORUS>
struct TGswParams;

template<typename TORUS>
struct TGswKey;

template<typename TORUS>
struct TGswSample;

template<typename TORUS>
struct TGswSampleFFT;
// *** ()
template<typename TORUS>
struct LweBootstrappingKey;

template<typename TORUS>
struct LweBootstrappingKeyFFT;
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
