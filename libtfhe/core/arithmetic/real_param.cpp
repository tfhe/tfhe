#include "real_param.h"

#include <gmp.h>
#include <cassert>

RealParams<BigTorus>::RealParams(const ZModuleParams<BigTorus> *const zparams, const int nbLimbsInt) :
        zparams(zparams),
        nbLimbsInt(nbLimbsInt),
        nbLimbsFrac(zparams->max_nbLimbs),
        nbLimbs(nbLimbsInt+nbLimbsFrac),
        nbBitsInt(nbLimbsInt * sizeof(mp_limb_t) * 8),
        nbBitsFrac(nbLimbsFrac * sizeof(mp_limb_t) * 8),
        nbBits(nbLimbs * sizeof(mp_limb_t) * 8)
{
    assert(nbLimbsInt > 0);
}

void RealParams<BigTorus>::destroy() {}

