#include "BigInt.h"

BigIntParams::BigIntParams(const int max_nbLimbs) :
        max_nbLimbs(max_nbLimbs), p(max_nbLimbs * sizeof(mp_limb_t) * 8) {}

void BigIntParams::destroy() {}

BigInt::BigInt(int64_t value) {
    mpz_init_set_si(data, value);
}

void BigInt::destroy() {
    mpz_clear(data);
}

void clamp2p(BigInt *res, const BigIntParams *params) {
    if (res->data->_mp_size > params->max_nbLimbs) {
        res->data->_mp_size = params->max_nbLimbs;
        return;
    } else if (res->data->_mp_size < -params->max_nbLimbs) {
        res->data->_mp_size = -params->max_nbLimbs;
        return;
    }
}

void add(BigInt *res, const BigInt *a, const BigInt *b, const BigIntParams *params) {
    mpz_add(res->data, a->data, b->data);
    clamp2p(res, params);
}

void add(BigInt *res, const BigInt *a, int64_t b, const BigIntParams *params) {
    if (b >= 0)
        mpz_add_ui(res->data, a->data, uint64_t(b));
    else
        mpz_sub_ui(res->data, a->data, uint64_t(-b));
    clamp2p(res, params);
}

void add(BigInt *res, int64_t a, const BigInt *b, const BigIntParams *params) {
    add(res, b, a, params);
}

void sub(BigInt *res, const BigInt *a, const BigInt *b, const BigIntParams *params) {
    mpz_sub(res->data, a->data, b->data);
    clamp2p(res, params);
}

void sub(BigInt *res, const BigInt *a, int64_t b, const BigIntParams *params) {
    if (b >= 0)
        mpz_sub_ui(res->data, a->data, uint64_t(b));
    else
        mpz_add_ui(res->data, a->data, uint64_t(-b));
    clamp2p(res, params);
}

void sub(BigInt *res, int64_t a, const BigInt *b, const BigIntParams *params) {
    if (a >= 0)
        mpz_ui_sub(res->data, uint64_t(a), b->data);
    else
        mpz_add_ui(res->data, b->data, uint64_t(-a));
    clamp2p(res, params);
}

void mul(BigInt *res, const BigInt *a, const BigInt *b, const BigIntParams *params) {
    mpz_mul(res->data, a->data, b->data);
    clamp2p(res, params);
}

void mul(BigInt *res, int64_t a, const BigInt *b, const BigIntParams *params) {
    mpz_mul_si(res->data, b->data, a);
    clamp2p(res, params);
}

void mul(BigInt *res, const BigInt *a, int64_t b, const BigIntParams *params) {
    mpz_mul_si(res->data, a->data, b);
    clamp2p(res, params);
}

void neg(BigInt *res, BigInt *a, const BigIntParams *params) {
    mpz_neg(res->data, a->data);
}

void neg(BigInt *res, int64_t a, const BigIntParams *params) {
    mpz_set_si(res->data, -a);
}

void setvalue(BigInt *res, BigInt *a, const BigIntParams *params) {
    mpz_set(res->data, a->data);
}

void setvalue(BigInt *res, int64_t a, const BigIntParams *params) {
    mpz_set_si(res->data, a);
}
