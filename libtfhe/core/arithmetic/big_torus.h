#ifndef TFHE_BIGTORUS_H
#define TFHE_BIGTORUS_H

#include <cmath>
#include "gmp.h"
#include "big_int.h"

/**
 * @brief big torus with fixed precision > 64 bits
 * This class represents a BigTorus with p = 64.k bits of precision.
 * The underlying representation is just a (fixed-size) array of k gmp limbs.
 * The most significant bit of these limbs corresponds to 1/2.
 *
 * Arithmetic is expressed with the GMP low-level functions.
 */
class BigTorus {
public:
    mp_limb_t *const data;

    BigTorus(const ZModuleParams<BigTorus> *params, Allocator *alloc);

    void destroy(const ZModuleParams<BigTorus> *params, Allocator *alloc);

    PREVENT_STACK_COPY(BigTorus);
};

/**
 * @brief res = 0
 */
void zero(BigTorus *res, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = src
 */
void copy(BigTorus *res, const BigTorus *src, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = 2^-k
 */
void setPowHalf(BigTorus *res, const int k, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a + b
 */
void add(BigTorus *res, const BigTorus *a, const BigTorus *b, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a - b
 */
void sub(BigTorus *res, const BigTorus *a, const BigTorus *b, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a * b
 */
void mul(BigTorus *res, int64_t a, const BigTorus *b, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a * b
 * WARNING: for this function, res and b must not overlap.
 */
void mul_no_overlap(BigTorus *res, const BigInt *a, const BigTorus *b, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a * b
 */
void mul(BigTorus *res, const BigInt *a, const BigTorus *b, const ZModuleParams<BigTorus> *params, Allocator alloc);

/**
 * @brief res = -a
 */
void neg(BigTorus *res, const BigTorus *a, const ZModuleParams<BigTorus> *params);


/**
 * @brief Converts real number to torus. This function preserves the 53bits of mantissa (whenever applicable)
 * @param d real to convert
 * @return torus value
 */
void from_double(BigTorus *reps, const double d, const ZModuleParams<BigTorus> *params);

/**
 * @brief Converts torus to real number (mainly for printing).
 * @param x torus element to convert
 * @return real number
*/
double to_double(const BigTorus *a, const ZModuleParams<BigTorus> *b);

/**
 * @brief Rounds the torus value to the nearest multiple of 1/Msize
 * @param phase torus value
 * @param Msize discrete space size
 * @return approximated torus value
 */
void
approxPhase(BigTorus *res, const BigTorus *phase, uint64_t Msize, ZModuleParams<BigTorus> *params, Allocator alloc);

/**
 * @brief Mod-Rescale from the torus to Z/Msize.Z
 *  i.e. computes roundToNearestInteger(Msize*phase)
 * @param phase torus value to convert
 * @param Msize discrete space size
 * @return discrete space value in [0, MSize[
 */
uint64_t modSwitchFromTorus(const BigTorus *phase, uint64_t Msize, ZModuleParams<BigTorus> *params, Allocator alloc);

/**
 * @brief Converts discrete message space to torus
 *  i.e. value mu/Msize to a torus for mu
 * @param mu discrete space value (from [0,Msize[) to convert
 * @param Msize discrete space size
 * @return torus value
 */
void modSwitchToTorus(BigTorus *res, const uint64_t mu, const uint64_t Msize, ZModuleParams<BigTorus> *params,
                      Allocator alloc);


#endif //TFHE_BIGTORUS_H
