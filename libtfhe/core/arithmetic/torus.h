#ifndef TORUS_H
#define TORUS_H

#include "torus_utils.h"

#include <cstdint>

// Idea:
// we may want to represent an element x of the real torus by
// the integer rint(2^32.x) modulo 2^32
//  -- addition, subtraction and integer combinations are native operation
//  -- modulo 1 is mapped to mod 2^32, which is also native!
// This looks much better than using float/doubles, where modulo 1 is not
// natural at all.
typedef int32_t Torus32;
typedef int64_t Torus64;

/**
 * @brief Macro for instantiating a template class with at least one parameter
 */
#define EXPLICIT_INSTANTIATE_CLASS(C,T,...)\
    template class C<T,##__VA_ARGS__>;

/**
 * @brief Macro for instantiating a template class for all available torus types
 */
#define EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_TORUS(C,...)\
    EXPLICIT_INSTANTIATE_CLASS(C, Torus32, ##__VA_ARGS__);\
    EXPLICIT_INSTANTIATE_CLASS(C, Torus64, ##__VA_ARGS__);

/**
 * @brief Macro for instantiating a template class for all available torus types integer equivalents
 */
#define EXPLICIT_INSTANTIATE_ALL_PRIMITIVE_INT(C,...)\
    EXPLICIT_INSTANTIATE_CLASS(C, TorusUtils<Torus32>::INT_TYPE, ##__VA_ARGS__);\
    EXPLICIT_INSTANTIATE_CLASS(C, TorusUtils<Torus64>::INT_TYPE, ##__VA_ARGS__);

#endif
