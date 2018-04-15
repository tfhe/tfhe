#ifndef TFHE_BIG_REAL_H
#define TFHE_BIG_REAL_H

#include <gmp.h>
#include "big_torus.h"
#include "real_param.h"

/**
 * @brief fixed-point number with large precision
 *  This class represents a fixed-point number with p = 64.(A+B) bits of precision.
 *  The underlying representation is just a (fixed-size) array of A+B gmp limbs.
 *  The fractional part is 64.A limbs and the integral 64.B.
 *  A+1..A+B most significant limbs are the integral part.
 */
class BigReal {
public:
    mp_limb_t *const data;

    BigReal(const RealParams<BigTorus> *params, Allocator *alloc);

    void destroy(const RealParams<BigTorus> *params, Allocator *alloc);

    PREVENT_STACK_COPY(BigReal);
};

/**
 * @brief Zeros a real
 *
 * @param res destination real
 * @param params fixed point real parameters
 */
void zero(BigReal *dst, const RealParams<BigTorus> *params);

/**
 * @brief Transform a real to a double precision floating point
 * @details Obtained result is an low-resolution approximation of input real.
 *
 * @param src source real
 * @param params fixed point real parameters
 *
 * @return obtained double precision value
 */
double to_double(const BigReal *src, const RealParams<BigTorus> *params);

// void from_double(BigReal* dst, const double src, const RealParams<BigTorus> *params);

/**
 * @brief Transforms a torus to a fixed point real
 *
 * @param dst destination real
 * @param src source torus
 * @param params fixed point real parameters
 */
void to_real(BigReal *dst, const BigTorus *src, const RealParams<BigTorus> *params);

/**
 * @brief Transforms a fixed point real to torus
 *
 * @param dst destination torus
 * @param src source real
 * @param params fixed point real parameters
 */
void from_real(BigTorus *dst, const BigReal *src, const RealParams<BigTorus> *params);

/**
 * @brief Add 2 big reals
 * @details Resulting big real has the same sizes of integral and fractional parts.
 *  Addition is performed modulo 2^(64.params->nbLimbsInt), carry limb is ignored.
 *
 * @param res resulting big real
 * @param a first big real operand
 * @param b second big real operand
 * @param params big real parameters
 */
void add(BigReal *res, BigReal *a, BigReal *b, const RealParams<BigTorus> *params);

/**
 * @brief Multiply 2 big reals
 * @details Resulting big real has the same size as inputs.
 *  This corresponds to a multiplication modulo 2^(64.params->nbLimbsInt) and where
 *      the fractional part rounded to params->nbLimbsFrac limbs.
 *
 * @param res resulting big real
 * @param a first big real operand
 * @param b second big real operand
 * @param params big real parameters
 */
void mul(BigReal *res, BigReal *a, BigReal *b, const RealParams<BigTorus> *params, Allocator alloc);

/**
 * @brief Naive multiplication with rounding
 */
void mul_naive_round(BigReal *res, BigReal *a, BigReal *b, const RealParams<BigTorus> *params, Allocator alloc);

/**
 * @brief Naive multiplication with rounding towards zero (i.e. truncate fractional part)
 */
void mul_naive_trunc(BigReal *res, BigReal *a, BigReal *b, const RealParams<BigTorus> *params, Allocator alloc);


#endif // TFHE_BIG_REAL_H
