#include "zmodule_param.h"

#include <gmp.h>

ZModuleParams<BigTorus>::ZModuleParams(const int max_nbLimbs) :
        max_nbLimbs(max_nbLimbs), p(max_nbLimbs * sizeof(mp_limb_t) * 8) {}

void ZModuleParams<BigTorus>::destroy() {}

