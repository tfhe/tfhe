#ifndef POLYNOMIAL_TORUS_BIG_H
#define POLYNOMIAL_TORUS_BIG_H

#include "polynomial_torus.h"
#include "BigTorus.h"
#include "../allocator/allocator.h"
#include "threadcontext.h"
#include "torus_utils_big.h"

template class TorusPolynomial<BigTorus>;

#endif // POLYNOMIAL_TORUS_BIG_H