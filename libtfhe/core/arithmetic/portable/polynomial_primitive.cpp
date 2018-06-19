#include "../generic/polynomial_primitive.h"

/**
 * Instantiate Polynomial class for native torus and int types
 */
EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_TORUS(Polynomial, CoefTypeEnum::Torus, AsmTypeEnum::PORTABLE);

EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_TORUS(Polynomial, CoefTypeEnum::Integer, AsmTypeEnum::PORTABLE);
