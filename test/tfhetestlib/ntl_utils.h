#ifndef TFHE_NTL_CONVERSIONS_H
#define TFHE_NTL_CONVERSIONS_H

#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include "core/arithmetic/big_int.h"
#include <core/arithmetic/big_real.h>

namespace tfhe_test {

    /** @brief BigInt to NTL::ZZ conversion */
    NTL::ZZ to_ntl_ZZ(const BigInt *a, const ZModuleParams<BigTorus> *params);

    /** @brief BigTorus to NTL::ZZ conversion. result between 0 and 2^p */
    NTL::ZZ to_ntl_ZZ(const BigTorus *a, const ZModuleParams<BigTorus> *params);

    /** @brief 2^p */
    NTL::ZZ extract_modulus(const ZModuleParams<BigTorus> *params);

    /** @brief BigTorus to NTL::RR conversion. result between 0 and 1 */
    NTL::RR posmod_to_ntl_RR(const BigTorus *a, const ZModuleParams<BigTorus> *params);

    /** @brief BigTorus to NTL::RR conversion. result between -0.5 and 0.5 */
    NTL::RR centermod_to_ntl_RR(const BigTorus *a, const ZModuleParams<BigTorus> *params);

    /** @brief distance between two torus elements mod 1: result between 0 and 0.5 */
    NTL::RR distance_mod_1(const NTL::RR &a, const NTL::RR &b);

    /** @brief a mod p, result in [0,p-1] */
    NTL::ZZ posmod(const NTL::ZZ &a, const NTL::ZZ &p);

    /** @brief a mod p, result in [-p/2,p/2] */
    NTL::ZZ centermod(const NTL::ZZ &a, const NTL::ZZ &p);

    /** @brief BigReal to NTL::ZZ conversion. result between 0 and 2^(nbLimbsInt+nbLimbsFrac) */
    NTL::ZZ to_ntl_ZZ(const BigReal *a, const RealParams<BigTorus> *params);

    /** @brief BigReal to NTL::ZZ conversion. result between 0 and 2^(nbLimbsInt+nbLimbsFrac) */
    NTL::RR posmod_to_ntl_RR(const BigReal *a, const RealParams<BigTorus> *params);

    /** @brief BigReal to NTL::ZZ conversion. result between 0 and 2^(nbLimbsInt+nbLimbsFrac) */
    NTL::RR centermod_to_ntl_RR(const BigReal *a, const RealParams<BigTorus> *params);
}


#endif //TFHE_NTL_CONVERSIONS_H
