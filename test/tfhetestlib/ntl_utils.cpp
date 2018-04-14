#include "ntl_utils.h"
#include <NTL/ZZ_limbs.h>
#include <cstdint>
#include <gmp.h>
#include <core/arithmetic/big_torus.h>
#include <cassert>

using namespace NTL;
using namespace std;

// if any of this assertion fails, tfhe is not usable on your operating system
static_assert(std::is_same<NTL::ZZ_limb_t, uint64_t>::value);
static_assert(std::is_same<mp_limb_t, uint64_t>::value);


NTL::ZZ tfhe_test::to_ntl_ZZ(const BigInt *a, const ZModuleParams<BigTorus> *params) {
    ZZ reps;
    const mpz_t &aa = a->data;
    if (aa->_mp_size >= 0) {
        ZZ_limbs_set(reps, aa->_mp_d, aa->_mp_size);
    } else {
        ZZ_limbs_set(reps, aa->_mp_d, -aa->_mp_size);
        NTL::negate(reps, reps);
    }
    return reps;
}

NTL::ZZ tfhe_test::to_ntl_ZZ(const BigTorus *a, const ZModuleParams<BigTorus> *params) {
    ZZ reps;
    //find the actual number of limbs
    int length;
    for (length = params->max_nbLimbs; length > 1; length--) {
        if (a->data[length - 1] != 0) break;
    }
    ZZ_limbs_set(reps, a->data, length);
    return reps;
}

NTL::ZZ tfhe_test::posmod(const NTL::ZZ &a, const NTL::ZZ &p) {
    //return (p + (a % p)) % p;
    ZZ reps = a % p; //%p is always positive in NTL
    assert(reps >= 0);
    return reps;
}

NTL::ZZ tfhe_test::centermod(const NTL::ZZ &a, const NTL::ZZ &p) {
    ZZ halfp = p / 2; //integer division
    return posmod(a + halfp, p) - halfp;
}

NTL::RR tfhe_test::posmod_to_ntl_RR(const BigTorus *a, const ZModuleParams<BigTorus> *params) {
    RR::SetPrecision(params->p + 64);
    NTL::RR v = to_RR(to_ntl_ZZ(a, params));
    NTL::RR p = power2_RR(-params->p);
    return v * p;
}

NTL::RR tfhe_test::centermod_to_ntl_RR(const BigTorus *a, const ZModuleParams<BigTorus> *params) {
    RR::SetPrecision(params->p + 64);
    NTL::RR res = posmod_to_ntl_RR(a, params);
    if (res > 0.5) res -= 1;
    return res;
}

NTL::RR tfhe_test::distance_mod_1(const NTL::RR &a, const NTL::RR &b) {
    RR diff = b - a;
    return abs(diff - round(diff));
}

NTL::ZZ tfhe_test::extract_modulus(const ZModuleParams<BigTorus> *params) {
    return power2_ZZ(params->p);
}
