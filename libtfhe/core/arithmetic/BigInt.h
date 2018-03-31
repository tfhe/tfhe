#ifndef TFHE_BIGINT_H
#define TFHE_BIGINT_H

#define PREVENT_STACK_COPY(TYPENAME) \
    TYPENAME(const TYPENAME&) = delete; \
    void operator=(const TYPENAME &)= delete; \
    ~TYPENAME() = delete

#include <cstdint>
#include <gmp.h>
#include "../allocator/allocator.h"

/**
 * @brief params of the BigInt
 * Namely, the maximum number of limbs, and the number of bits p of the modulus.
 * (every operation is modulo 2^p)
 */
class BigIntParams {
public:
    const int max_nbLimbs;
    const int p;

    BigIntParams(const int max_nbLimbs);

    void destroy();

    PREVENT_STACK_COPY(BigIntParams);
};

/**
 * @brief This class represents big integers modulo 2^p where p is a multiple of 64 bits.
 * A big integer is represented by a mpz_t structure (GMP):
 * one unsigned array of mplimb_t and its size (always non zero),
 * the sign of the size field is the sign of the represented integer.
 */
class BigInt {
public:
    mpz_t data;

    BigInt(int64_t value = 0);

    void destroy();

    PREVENT_STACK_COPY(BigInt);
};

/**
 * @brief res := res truncated modulo 2^p
 * Very fast implementation using gmp internals
 * @param res the bigint to truncate
 * @param params bigint parameters
 */
void clamp2p(BigInt *res, const BigIntParams *params);

/**
 * @brief res = a + b (truncated mod 2^p)
 */
void add(BigInt *res, const BigInt *a, const BigInt *b, const BigIntParams *params);

/**
 * @brief res = a + b (truncated mod 2^p)
 */
void add(BigInt *res, const BigInt *a, int64_t b, const BigIntParams *params);

/**
 * @brief res = a + b (truncated mod 2^p)
 */
void add(BigInt *res, int64_t a, const BigInt *b, const BigIntParams *params);

/**
 * @brief res = a - b (truncated mod 2^p)
 */
void sub(BigInt *res, const BigInt *a, const BigInt *b, const BigIntParams *params);

/**
 * @brief res = a - b (truncated mod 2^p)
 */
void sub(BigInt *res, const BigInt *a, int64_t b, const BigIntParams *params);

/**
 * @brief res = a - b (truncated mod 2^p)
 */
void sub(BigInt *res, int64_t a, const BigInt *b, const BigIntParams *params);

/**
 * @brief res = a * b (truncated mod 2^p)
 */
void mul(BigInt *res, const BigInt *a, const BigInt *b, const BigIntParams *params);

/**
 * @brief res = a * b (truncated mod 2^p)
 */
void mul(BigInt *res, int64_t a, const BigInt *b, const BigIntParams *params);

/**
 * @brief res = a * b (truncated mod 2^p)
 */
void mul(BigInt *res, const BigInt *a, int64_t b, const BigIntParams *params);

/**
 * @brief res = -a (truncated mod 2^p)
 */
void neg(BigInt *res, BigInt *a, const BigIntParams *params);

/**
 * @brief res = -a (truncated mod 2^p)
 */
void neg(BigInt *res, int64_t a, const BigIntParams *params);

/**
 * @brief res = a (truncated mod 2^p)
 */
void setvalue(BigInt *res, BigInt *a, const BigIntParams *params);

/**
 * @brief res = a (truncated mod 2^p)
 */
void setvalue(BigInt *res, int64_t a, const BigIntParams *params);

#endif //TFHE_BIGINT_H
