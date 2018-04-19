#include "big_real.h"

#include <cassert>
#include <cstring>

BigReal::BigReal(const RealParams<BigTorus> *params, Allocator *alloc) :
        data(alloc->newArray<mp_limb_t>(params->nbLimbs)) {}

void BigReal::destroy(const RealParams<BigTorus> *params, Allocator *alloc) {
    alloc->deleteArray(params->nbLimbs, data);
}

void tfhe_backend::zero(BigReal *res, const RealParams<BigTorus> *params) {
    mpn_zero(res->data, params->nbLimbs);
}

double tfhe_backend::to_double(const BigReal *src, const RealParams<BigTorus> *params) {
    constexpr int limbNbBits = sizeof(mp_limb_t) * 8;
    double res = 0;

    for (int i = 0; i < params->nbLimbsFrac; ++i)
        res += double(int64_t(src->data[i])) * pow(0.5, (params->nbLimbsFrac - i) * limbNbBits);
    for (int i = params->nbLimbsFrac; i < params->nbLimbs; ++i)
        res += double(int64_t(src->data[i])) * pow(2.0, (params->nbLimbsFrac - i) * limbNbBits);

    return res;
}

// void from_double(BigReal* dst, const double src, const RealParams<BigTorus> *params) {
//     zero(dst, params);

//     double int_part = trunc(src);
//     dst->data[params->nbLimbs] = (int64_t)int_part;
//     dst->data[params->nbLimbs-1] = (src - int_part) * pow(2.,64);
// }

void tfhe_backend::to_real(BigReal *dst, const BigTorus *src, const RealParams<BigTorus> *params) {
    mpn_copyi(dst->data, src->data, params->nbLimbsFrac);

    // copy sign bit to MSB limb
    uint64_t sign = (src->data[params->zparams->max_nbLimbs - 1] >> 63) * uint64_t(-1);
    for (int i = params->nbLimbsFrac; i < params->nbLimbs; ++i)
        dst->data[i] = sign;
}

void tfhe_backend::to_torus(BigTorus *dst, const BigReal *src, const RealParams<BigTorus> *params) {
    // truncate BigReal integer part
    mpn_copyi(dst->data, src->data, params->nbLimbsFrac);
}

void tfhe_backend::add(BigReal *res, const BigReal *a, const BigReal *b, const RealParams<BigTorus> *params) {
    // add and ignore carry
    mpn_add_n(res->data, a->data, b->data, params->nbLimbs);
}

namespace {
    /**
     * @brief Round LSB limb
     */
    void round(mp_limb_t *res, const int n) {
        // round by adding (1<<63)
        constexpr mp_limb_t round_token = (uint64_t(1) << (sizeof(mp_limb_t) * 8 - 1));
        mpn_add_1(res, res, n, round_token);
    }
};

/**
 * @brief Naive implementation of multiplication.
 *  Perform multiplication then round LSB and truncate MSB limbs.
 */
void tfhe_backend::mul_naive_round(BigReal *res, const BigReal *a, const BigReal *b, const RealParams<BigTorus> *params,
                                   Allocator alloc) {
    mp_limb_t *tmp = alloc.newArray<mp_limb_t>(2 * params->nbLimbs);

    mpn_mul_n(tmp, a->data, b->data, params->nbLimbs);

    // round and truncate result
    const int nbLimbsFrac = params->nbLimbsFrac;
    round(tmp + (nbLimbsFrac - 1), 2 * params->nbLimbs - (nbLimbsFrac - 1));
    mpn_copyi(res->data, tmp + nbLimbsFrac, params->nbLimbs);

    alloc.deleteArray(2 * params->nbLimbs, tmp);
}

/**
 * @brief Naive implementation of multiplication.
 *  Perform multiplication then simply truncate LSB and MSB limbs.
 */
void tfhe_backend::mul_naive_trunc(BigReal *res, const BigReal *a, const BigReal *b, const RealParams<BigTorus> *params,
                                   Allocator alloc) {
    mp_limb_t *tmp = alloc.newArray<mp_limb_t>(2 * params->nbLimbs);

    mpn_mul_n(tmp, a->data, b->data, params->nbLimbs);

    // truncate result
    mpn_copyi(res->data, tmp + params->nbLimbsFrac, params->nbLimbs);

    alloc.deleteArray(2 * params->nbLimbs, tmp);
}

void tfhe_backend::mul(BigReal *res, const BigReal *a, const BigReal *b, const RealParams<BigTorus> *params,
                       Allocator alloc) {
    const int nbLimbs = params->nbLimbs;
    const int nbLimbsInt = params->nbLimbsInt;
    const int nbLimbsFrac = params->nbLimbsFrac;
    const int nbLimbsTmp = nbLimbs + 1;

    assert(nbLimbsFrac > 0);
    assert(nbLimbsInt > 0);

    const mp_limb_t *const adata = a->data;
    const mp_limb_t *const bdata = b->data;

    mp_limb_t *tmp = alloc.newArray<mp_limb_t>(nbLimbsTmp);

    tmp[nbLimbsInt + 1] = mpn_mul_1(tmp, adata + (nbLimbsFrac - 1), nbLimbsInt + 1, bdata[0]);
    for (int k = 1; k < nbLimbsFrac; ++k) {
        tmp[nbLimbsInt + k + 1] = mpn_addmul_1(tmp, adata + (nbLimbsFrac - 1 - k), nbLimbsInt + k + 1, bdata[k]);
    }

    for (int k = 0; k < nbLimbsInt; ++k) {
        mpn_addmul_1(tmp + (k + 1), adata, nbLimbs - k, bdata[k + nbLimbsFrac]);
    }

    // round and truncate result
    round(tmp, nbLimbsTmp);
    mpn_copyi(res->data, tmp + 1, params->nbLimbs);

    alloc.deleteArray(nbLimbsTmp, tmp);
}
