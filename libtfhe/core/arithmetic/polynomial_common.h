#ifndef POLYNOMIAL_COMMON_H
#define POLYNOMIAL_COMMON_H

#include "polynomial_param.h"
#include "../allocator/allocator.h"
#include "threadcontext.h"

#include <cstdint>
#include <cassert>
#include <type_traits>

template<typename TYPE, template<typename> class Polynomial>
struct PolynomialCommon
{
    static void Clear(
        Polynomial<TYPE> *result,        
        const PolynomialParams<TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);

    static void Copy(
        Polynomial<TYPE> *result,
        const Polynomial<TYPE> *source,     
        const PolynomialParams<TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * @brief Add \c poly1 to \c poly2 and store result in \c result
     *      i.e. result = poly1 + poly2
     */
    static void Add(
        Polynomial<TYPE> *result,
        const Polynomial<TYPE> *poly1,
        const Polynomial<TYPE> *poly2,
        const PolynomialParams<TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * @brief Add \c poly polynomial to \c result
     *      i.e. result = result + poly
     */
    static void AddTo(
        Polynomial<TYPE> *result,
        const Polynomial<TYPE> *poly,
        const PolynomialParams<TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * @brief Substract \c poly2 from \c poly1 and store result in \c result
     *      i.e. result = poly1 - poly2
     */
    static void Sub(
        Polynomial<TYPE> *result,
        const Polynomial<TYPE> *poly1,
        const Polynomial<TYPE> *poly2,
        const PolynomialParams<TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * @brief Substract \c poly polynomial from \c result
     *      i.e. result = result - poly
     */
    static void SubTo(
        Polynomial<TYPE> *result,
        const Polynomial<TYPE> *poly,
        const PolynomialParams<TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);

  //result = (X^{a}-1)*source
  static void MulByXaiMinusOne(
        Polynomial<TYPE> *result,
        const int32_t a,
        const Polynomial<TYPE> *source,
        const PolynomialParams<TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);

    static void MulByXai(
        Polynomial<TYPE> *result,
        const int32_t a,
        const Polynomial<TYPE> *source,
        const PolynomialParams<TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);
};

#endif // POLYNOMIAL_COMMON_H