#include <gmp.h>
#include <cassert>
#include "BigTorus.h"

BigTorus::BigTorus(const BigIntParams *params, Allocator *alloc) :
        data(alloc->newArray<mp_limb_t>(params->max_nbLimbs)) {}

void BigTorus::destroy(const BigIntParams *params, Allocator *alloc) {
    alloc->deleteArray(params->max_nbLimbs, data);
}

void add(BigTorus *res, const BigTorus *a, const BigTorus *b, const BigIntParams *params) {
    mpn_add_n(res->data, a->data, b->data, params->max_nbLimbs);
}

void sub(BigTorus *res, const BigTorus *a, const BigTorus *b, const BigIntParams *params) {
    mpn_sub_n(res->data, a->data, b->data, params->max_nbLimbs);
}

void mul(BigTorus *res, int64_t a, const BigTorus *b, const BigIntParams *params) {
    if (a >= 0) {
        mpn_mul_1(res->data, b->data, params->max_nbLimbs, uint64_t(a));
    } else {
        mpn_mul_1(res->data, b->data, params->max_nbLimbs, uint64_t(-a));
        mpn_neg(res->data, res->data, params->max_nbLimbs);
    }
}

void mul(BigTorus *res, const BigInt *a, const BigTorus *b, const BigIntParams *params) {
    assert(res!=b);
    if (a->data->_mp_size > 0) { //positive
        const int m = a->data->_mp_size;
        mpn_mul_1(res->data, b->data, params->max_nbLimbs, a->data->_mp_d[0]);
        for (int i = 1; i < m; i++) {
            mpn_addmul_1(res->data+i, b->data, params->max_nbLimbs, a->data->_mp_d[i]);
        }
    } else { //negative case
        const int m = -a->data->_mp_size;
        mpn_mul_1(res->data, b->data, params->max_nbLimbs, a->data->_mp_d[0]);
        for (int i = 1; i < m; i++) {
            mpn_addmul_1(res->data+i, b->data, params->max_nbLimbs-i, a->data->_mp_d[i]);
        }
        mpn_neg(res->data, res->data, params->max_nbLimbs);
    }
}

void neg(BigTorus *res, BigTorus *a, const BigIntParams *params) {
    mpn_neg(res->data, res->data, params->max_nbLimbs);
}

void from_double(BigTorus *reps, const double d, const BigIntParams *params) {
    //dissect the input double: extract sign, exponent, mantissa
    static constexpr uint64_t mantissa_msb = (uint64_t(1) << 52);
    static constexpr uint64_t mantissa_mask = mantissa_msb - 1;
    static constexpr uint64_t expo_mask = (uint64_t(1) << 11) - 1;
    uint64_t di = *((uint64_t *) &d); //get the bits of d
    uint64_t mantissa = (di & mantissa_mask) | mantissa_msb;
    uint64_t expo = (di >> 52) & expo_mask;
    uint64_t sign = (di >> 63);
    // printf("mantissa: %lu\n", mantissa);
    // printf("expo: %lu\n", expo);
    // printf("sign: %lu\n", sign);

    //position of the lsb of the mantissa in the bigtorus
    int64_t lsbmantissapos = (int64_t(expo) - 1023l + params->p) - 52l;
    // printf("newlsbpos: %ld\n", lsbmantissapos);

    //set answer to 0
    for (int i=0; i<params->max_nbLimbs; i++)
        reps->data[i]=0;

    //copy the mantissa at the right place
    if (lsbmantissapos >= 0) { //left shift by lsbmantissapos positions
        int64_t blockshift = (lsbmantissapos >> 6); // /64
        int64_t inshift = (lsbmantissapos & 63);    // %64
        if (blockshift < params->max_nbLimbs) {
            reps->data[blockshift] = mantissa << inshift;
            // printf("put %lu at block %ld\n", (mantissa << inshift), blockshift);
            if (blockshift+1 < params->max_nbLimbs) {
                reps->data[blockshift + 1] = mantissa >> (64 - inshift);
                // printf("put %lu at block %ld\n", mantissa >> ((64 - inshift)), blockshift+1);
            }
        }
    } else { //right shift by -lsbmantissapos positions
        int64_t inshift = (-lsbmantissapos);
        if (inshift<64) {
            reps->data[0] = (mantissa >> inshift);
            // printf("put %lu at block %ld\n", (mantissa >> inshift), 0l);
        }
    }

    //deal with the sign
    if (sign) {
        printf("negate all\n");
        mpn_neg(reps->data, reps->data, params->max_nbLimbs);
    }
}
