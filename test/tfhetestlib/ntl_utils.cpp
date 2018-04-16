#include "ntl_utils.h"
#include <NTL/version.h>
#include <cstdint>
#include <gmp.h>
#include <core/arithmetic/big_torus.h>
#include <cassert>

using namespace NTL;
using namespace std;

#if NTL_MAJOR_VERSION > 9

#include <NTL/ZZ_limbs.h>

// if any of this assertion fails, tfhe is not usable on your operating system
static_assert(std::is_same<NTL::ZZ_limb_t, uint64_t>::value, "TFHE is not usable on your operating system");
static_assert(std::is_same<mp_limb_t, uint64_t>::value, "TFHE is not usable on your operating system");

static NTL::ZZ mpn_to_ZZ(const mp_limb_t *data, const int n) {
    ZZ reps;
    ZZ_limbs_set(reps, data, n);
    return reps;
}

#else
//Prior to version 9.0 of NTL, there was no access to the limbs in NTL, so we provide a manual conversion
//Note also: Prior to version 9.0, NTL had GPL license. If it is an issue please upgrade NTL

/** @brief limb array to NTL ZZ conversion */
static ZZ mpn_to_ZZ(const mp_limb_t* data, int n) {
   ZZ reps(0);
   for (int i=n-1; i>=0; i--) {
       // important note: there is no uint64_t to ZZ constructor in NTL,
       // because of the sign, we have to split in 32-bit words instead of 64
       reps <<= 32;
       reps |= (data[i] >> 32u);
       reps <<= 32;
       reps |= (data[i] & 0xFFFFFFFFul);
       //cout << reps << endl;
   }
   return reps;
}

#endif

NTL::ZZ tfhe_test::to_ntl_ZZ(const BigInt *a, const ZModuleParams<BigTorus> *params) {
    const mpz_t &aa = a->data;
    if (aa->_mp_size >= 0) {
        return mpn_to_ZZ(aa->_mp_d, aa->_mp_size);
    } else {
        return -mpn_to_ZZ(aa->_mp_d, -aa->_mp_size);
    }
}

NTL::ZZ tfhe_test::to_ntl_ZZ(const BigTorus *a, const ZModuleParams<BigTorus> *params) {
    ZZ reps;
    //find the actual number of limbs
    int length;
    for (length = params->max_nbLimbs; length > 1; length--) {
        if (a->data[length - 1] != 0) break;
    }
    return mpn_to_ZZ(a->data, length);
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

NTL::ZZ tfhe_test::to_ntl_ZZ(const BigReal *a, const RealParams<BigTorus> *params) {
    return mpn_to_ZZ(a->data, params->nbLimbs);
}

NTL::RR tfhe_test::posmod_to_ntl_RR(const BigReal *a, const RealParams<BigTorus> *params) {
    ZZ t = mpn_to_ZZ(a->data, params->nbLimbs);
    return MakeRRPrec(t, -params->nbBitsFrac, params->nbBits + 64);
}

NTL::RR tfhe_test::centermod_to_ntl_RR(const BigReal *a, const RealParams<BigTorus> *params) {
    ZZ t = mpn_to_ZZ(a->data, params->nbLimbs);
    ZZ p = power2_ZZ(params->nbBits);
    return MakeRRPrec(centermod(t, p), -params->nbBitsFrac, params->nbBits + 64);
}
