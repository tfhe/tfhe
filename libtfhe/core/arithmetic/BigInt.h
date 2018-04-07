#ifndef TFHE_BIGINT_H
#define TFHE_BIGINT_H

#include <cstdint>
#include <gmp.h>
#include "tfhe_core.h"
#include "../allocator/allocator.h"
#include "zmodule_param.h"

/**
 * forward declare big torus class
 */
class BigTorus;

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
void clamp2p(BigInt *res, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = 0
 */
void zero(BigInt *res, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = src
 */
void copy(BigInt *res, const BigInt* src, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a + b (truncated mod 2^p)
 */
void add(BigInt *res, const BigInt *a, const BigInt *b, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a + b (truncated mod 2^p)
 */
void add(BigInt *res, const BigInt *a, int64_t b, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a + b (truncated mod 2^p)
 */
void add(BigInt *res, int64_t a, const BigInt *b, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a - b (truncated mod 2^p)
 */
void sub(BigInt *res, const BigInt *a, const BigInt *b, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a - b (truncated mod 2^p)
 */
void sub(BigInt *res, const BigInt *a, int64_t b, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a - b (truncated mod 2^p)
 */
void sub(BigInt *res, int64_t a, const BigInt *b, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a * b (truncated mod 2^p)
 */
void mul(BigInt *res, const BigInt *a, const BigInt *b, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a * b (truncated mod 2^p)
 */
void mul(BigInt *res, int64_t a, const BigInt *b, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a * b (truncated mod 2^p)
 */
void mul(BigInt *res, const BigInt *a, int64_t b, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = -a (truncated mod 2^p)
 */
void neg(BigInt *res, BigInt *a, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = -a (truncated mod 2^p)
 */
void neg(BigInt *res, int64_t a, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a (truncated mod 2^p)
 */
void setvalue(BigInt *res, const BigInt *a, const ZModuleParams<BigTorus> *params);

/**
 * @brief res = a (truncated mod 2^p)
 */
void setvalue(BigInt *res, const int64_t a, const ZModuleParams<BigTorus> *params);

#endif //TFHE_BIGINT_H
