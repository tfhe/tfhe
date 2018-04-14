#ifndef TFHE_NTL_CONVERSIONS_H
#define TFHE_NTL_CONVERSIONS_H

#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include "core/arithmetic/big_int.h"

namespace tfhe_test {

    NTL::ZZ to_ntl_ZZ(const BigInt *a, const ZModuleParams<BigTorus> *params);

    NTL::ZZ to_ntl_ZZ(const BigTorus *a, const ZModuleParams<BigTorus> *params);

    NTL::RR posmod_to_ntl_RR(const BigTorus *a, const ZModuleParams<BigTorus> *params);

    NTL::RR centermod_to_ntl_RR(const BigTorus *a, const ZModuleParams<BigTorus> *params);

    NTL::RR distance_mod_1(const NTL::RR &a, const NTL::RR &b);

    NTL::ZZ posmod(const NTL::ZZ &a, const NTL::ZZ &p);

    NTL::ZZ centermod(const NTL::ZZ &a, const NTL::ZZ &p);

}


#endif //TFHE_NTL_CONVERSIONS_H
