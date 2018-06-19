#include "../generic/polynomial_big.h"

EXPLICIT_INSTANTIATE_CLASS(Polynomial, BigTorus, CoefTypeEnum::Integer, AsmTypeEnum::PORTABLE);

EXPLICIT_INSTANTIATE_CLASS(Polynomial, BigTorus, CoefTypeEnum::Torus, AsmTypeEnum::PORTABLE);
