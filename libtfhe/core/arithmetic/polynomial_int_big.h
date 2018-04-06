#ifndef POLYNOMIAL_INT_BIG_H
#define POLYNOMIAL_INT_BIG_H

#include "polynomial_int.h"
#include "BigInt.h"
#include "../allocator/allocator.h"
#include "threadcontext.h"

template class IntPolynomial<BigInt>;

#endif // POLYNOMIAL_INT_BIG_H