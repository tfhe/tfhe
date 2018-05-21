#include "../generic/polynomial_int_big.h"

/**
 * Instantiate IntPolynomial class for big int type
 */
EXPLICIT_INSTANTIATE_CLASS(IntPolynomial, BigTorus, AsmTypeEnum::PORTABLE);
