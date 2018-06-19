#include "../generic/zmodule_vector_primitive.h"

// no particular specialization to add

EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_TORUS(ZModuleVector, CoefTypeEnum::Torus, AsmTypeEnum::PORTABLE);

EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_TORUS(ZModuleVector, CoefTypeEnum::Integer, AsmTypeEnum::PORTABLE);
