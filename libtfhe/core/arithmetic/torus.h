#ifndef TORUS_H
#define TORUS_H

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
 * @brief Macro for instantiating a template class with one parameter
 */
#define TORUS_CLASS_IMPL_1(TC,T)\
    template class TC<T>;

/**
 * @brief Macro for instantiating a template class with two parameters
 */
#define TORUS_CLASS_IMPL_2(TC,T1,T2)\
    template class TC<T1,T2>;

/**
 * @brief Macro for instantiating a template class with all available torus types
 */
#define TORUS_CLASS_IMPL_ALL(T)\
    TORUS_CLASS_IMPL_1(T, Torus32);\
    TORUS_CLASS_IMPL_1(T, Torus64);

#endif