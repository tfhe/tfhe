#include <gmp.h>
#include "big_int.h"

BigInt::BigInt(int64_t value) {
    mpz_init_set_si(data, value);
}

void BigInt::destroy() {
    mpz_clear(data);
}

void tfhe_backend::clamp2p(BigInt *res, const ZModuleParams<BigTorus> *params) {
    mpz_t &r = res->data;
    if (r->_mp_size > params->max_nbLimbs) {
        for (r->_mp_size = params->max_nbLimbs; r->_mp_size > 1; r->_mp_size--) {
            if (r->_mp_d[r->_mp_size - 1] != 0) break;
        }
        return;
    } else if (res->data->_mp_size < -params->max_nbLimbs) {
        res->data->_mp_size = -params->max_nbLimbs;
        for (r->_mp_size = -params->max_nbLimbs; r->_mp_size < -1; r->_mp_size++) {
            if (r->_mp_d[-r->_mp_size - 1] != 0) break;
        }
        return;
    }
}

void tfhe_backend::zero(BigInt *res, const ZModuleParams<BigTorus> *params) {
    setvalue(res, (int64_t)0, params);
}

void tfhe_backend::copy(BigInt *res, const BigInt *src, const ZModuleParams<BigTorus> *params) {
    setvalue(res, src, params);
}

void tfhe_backend::add(BigInt *res, const BigInt *a, const BigInt *b, const ZModuleParams<BigTorus> *params) {
    mpz_add(res->data, a->data, b->data);
    clamp2p(res, params);
}

void tfhe_backend::add(BigInt *res, const BigInt *a, int64_t b, const ZModuleParams<BigTorus> *params) {
    if (b >= 0)
        mpz_add_ui(res->data, a->data, uint64_t(b));
    else
        mpz_sub_ui(res->data, a->data, uint64_t(-b));
    clamp2p(res, params);
}

void tfhe_backend::add(BigInt *res, int64_t a, const BigInt *b, const ZModuleParams<BigTorus> *params) {
    add(res, b, a, params);
}

void tfhe_backend::sub(BigInt *res, const BigInt *a, const BigInt *b, const ZModuleParams<BigTorus> *params) {
    mpz_sub(res->data, a->data, b->data);
    clamp2p(res, params);
}

void tfhe_backend::sub(BigInt *res, const BigInt *a, int64_t b, const ZModuleParams<BigTorus> *params) {
    if (b >= 0)
        mpz_sub_ui(res->data, a->data, uint64_t(b));
    else
        mpz_add_ui(res->data, a->data, uint64_t(-b));
    clamp2p(res, params);
}

void tfhe_backend::sub(BigInt *res, int64_t a, const BigInt *b, const ZModuleParams<BigTorus> *params) {
    if (a >= 0)
        mpz_ui_sub(res->data, uint64_t(a), b->data);
    else
        mpz_add_ui(res->data, b->data, uint64_t(-a));
    clamp2p(res, params);
}

void tfhe_backend::mul(BigInt *res, const BigInt *a, const BigInt *b, const ZModuleParams<BigTorus> *params) {
    mpz_mul(res->data, a->data, b->data);
    clamp2p(res, params);
}

void tfhe_backend::mul(BigInt *res, int64_t a, const BigInt *b, const ZModuleParams<BigTorus> *params) {
    mpz_mul_si(res->data, b->data, a);
    clamp2p(res, params);
}

void tfhe_backend::mul(BigInt *res, const BigInt *a, int64_t b, const ZModuleParams<BigTorus> *params) {
    mpz_mul_si(res->data, a->data, b);
    clamp2p(res, params);
}

void tfhe_backend::neg(BigInt *res, BigInt *a, const ZModuleParams<BigTorus> *params) {
    mpz_neg(res->data, a->data);
}

void tfhe_backend::neg(BigInt *res, int64_t a, const ZModuleParams<BigTorus> *params) {
    mpz_set_si(res->data, -a);
}

void tfhe_backend::setvalue(BigInt *res, const BigInt *a, const ZModuleParams<BigTorus> *params) {
    mpz_set(res->data, a->data);
}

void tfhe_backend::setvalue(BigInt *res, const int64_t a, const ZModuleParams<BigTorus> *params) {
    mpz_set_si(res->data, a);
}
