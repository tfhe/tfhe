#include "big_real.h"

#include <cassert>
#include <cstring>

BigReal::BigReal(const RealParams<BigTorus> *params, Allocator *alloc) :
        data(alloc->newArray<mp_limb_t>(params->nbLimbs)) {}

void BigReal::destroy(const RealParams<BigTorus> *params, Allocator *alloc) {
    alloc->deleteArray(params->nbLimbs, data);
}

void zero(BigReal *res, const RealParams<BigTorus> *params) {
    mpn_zero(res->data, params->nbLimbs);
}

double to_double(const BigReal *src, const RealParams<BigTorus> *params) {
    constexpr int limbNbBits = sizeof(mp_limb_t)*8;
    double res = 0;

    for (int i = 0; i < params->nbLimbsInt; ++i)
        res += double(int64_t(src->data[i])) * pow(2.0, params->nbBitsInt - (i+1) * limbNbBits);
    for (int i = params->nbLimbsInt; i < params->nbLimbs; ++i)
        res += double(int64_t(src->data[i])) * pow(0.5, (i+1-params->nbLimbsInt) * limbNbBits);

    return res;
}

// void from_double(BigReal* dst, const double src, const RealParams<BigTorus> *params) {
//     zero(dst, params);

//     double int_part = trunc(src);
//     dst->data[params->nbLimbs] = (int64_t)int_part;
//     dst->data[params->nbLimbs-1] = (src - int_part) * pow(2.,64);
// }

void to_real(BigReal *dst, const BigTorus *src, const RealParams<BigTorus> *params) {
    mpn_copyi(dst->data, src->data, params->nbLimbsFrac);

    // copy sign bit to MSB limb
    uint64_t sign = (src->data[params->nbLimbs-1] >> 63) * uint64_t(-1);
    for (int i = 0; i < params->nbLimbsInt; ++i)
        dst->data[i] = sign;
}

void from_real(BigTorus *dst, const BigReal *src, const RealParams<BigTorus> *params) {
    // truncate BigReal integer part
    mpn_copyi(dst->data, src->data, params->nbLimbsFrac);
}

void add(BigReal *res, BigReal *a, BigReal *b, const RealParams<BigTorus> *params) {
    // add and ignore carry
    mpn_add_n(res->data, a->data, b->data, params->nbLimbs);
}

namespace {
    /**
     * @brief Round LSB limb
     */
    void round(mp_limb_t *res, const int n) {
        // round by adding (1<<62)
        constexpr mp_limb_t round_token = (uint64_t(1) << (sizeof(mp_limb_t)*8-2));
        mpn_add_1(res, res, n, round_token);
   }
};

/**
 * @brief Naive implementation of multiplication.
 *  Perform multiplication then round LSB and truncate MSB limbs.
 */
void mul_naive_round(BigReal *res, BigReal *a, BigReal *b, const RealParams<BigTorus> *params, Allocator alloc) {
    assert(res != a);
    assert(res != b);

    mp_limb_t *tmp = alloc.newArray<mp_limb_t>(2*params->nbLimbs);

    mpn_mul_n(tmp, a->data, b->data, params->nbLimbs);

    // round and truncate result
    const int nbLimbsFrac = params->nbLimbsFrac;
    round(tmp + (nbLimbsFrac-1), 2*params->nbLimbs - (nbLimbsFrac-1));
    mpn_copyi(res->data, tmp + nbLimbsFrac, params->nbLimbs);

    alloc.deleteArray(2*params->nbLimbs, tmp);
}

/**
 * @brief Naive implementation of multiplication.
 *  Perform multiplication then simply truncate LSB and MSB limbs.
 */
void mul_naive_trunc(BigReal *res, BigReal *a, BigReal *b, const RealParams<BigTorus> *params, Allocator alloc) {
    assert(res != a);
    assert(res != b);

    mp_limb_t *tmp = alloc.newArray<mp_limb_t>(2*params->nbLimbs);

    mpn_mul_n(tmp, a->data, b->data, params->nbLimbs);

    // truncate result
    mpn_copyi(res->data, tmp + params->nbLimbsFrac, params->nbLimbs);

    alloc.deleteArray(2*params->nbLimbs, tmp);
}

void mul(BigReal *res, BigReal *a, BigReal *b, const RealParams<BigTorus> *params, Allocator alloc) {
    assert(res != a);
    assert(res != b);

    const int nbLimbs = params->nbLimbs;
    const int nbLimbsInt = params->nbLimbsInt;
    const int nbLimbsFrac = params->nbLimbsFrac;
    const int nbLimbsTmp = nbLimbs + 1;

    assert(nbLimbsFrac > 0);
    assert(nbLimbsInt > 0);

    const mp_limb_t *const adata = a->data;
    const mp_limb_t *const bdata = b->data;

    mp_limb_t *tmp = alloc.newArray<mp_limb_t>(nbLimbsTmp);

    tmp[nbLimbsInt+1] = mpn_mul_1(tmp, adata + (nbLimbsFrac-1), nbLimbsInt+1, bdata[0]);
    for (int k = 1; k < nbLimbsFrac; ++k) {
        tmp[nbLimbsInt+k+1] = mpn_addmul_1(tmp, adata + (nbLimbsFrac-1-k), nbLimbsInt+k+1, bdata[k]);
    }

    for (int k = 0; k < nbLimbsInt; ++k) {
        mpn_addmul_1(tmp + (k+1), adata, nbLimbs-k, bdata[k+nbLimbsFrac]);
    }

    // round and truncate result
    round(tmp, nbLimbsTmp);
    mpn_copyi(res->data, tmp + 1, params->nbLimbs);

    alloc.deleteArray(nbLimbsTmp, tmp);
}
