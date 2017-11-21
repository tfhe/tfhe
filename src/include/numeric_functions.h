#ifndef NUMERIC_FUNCTIONS_H
#define NUMERIC_FUNCTIONS_H

///@file
///@brief This file contains the operations on numerical types (especially Torus32)

#include "tfhe_core.h"

#ifdef __cplusplus
#include <random>
extern std::default_random_engine generator;
extern std::uniform_int_distribution<Torus32> uniformTorus32_distrib;
static const int64_t _two31 = INT64_C(1) << 31; // 2^31
static const int64_t _two32 = INT64_C(1) << 32; // 2^32
static const double _two32_double = _two32;
static const double _two31_double = _two31;
#endif

/** 
 * modular gaussian distribution of standard deviation sigma centered on
 * the message, on the Torus32
 */ 
EXPORT Torus32 gaussian32(Torus32 message, double sigma);

/** conversion from double to Torus32 */
EXPORT Torus32 dtot32(double d);
/** conversion from Torus32 to double */
EXPORT double t32tod(Torus32 x);


/**
 *  Used to approximate the phase to the nearest multiple of  1/Msize 
 */
EXPORT Torus32 approxPhase(Torus32 phase, int32_t Msize);

/**
 *  computes rountToNearestInteger(Msize*phase)
 */
EXPORT int32_t modSwitchFromTorus32(Torus32 phase, int32_t Msize);

/**
 *  converts mu/Msize to a Torus32 for mu in [0,Msize[
 */
EXPORT Torus32 modSwitchToTorus32(int32_t mu, int32_t Msize);

#endif //NUMERIC_FUNCTIONS_H
