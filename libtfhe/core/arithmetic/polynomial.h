#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "polynomial_param.h"
#include "../allocator/allocator.h"
#include "threadcontext.h"

#include <cassert>

template<typename TYPE>
class Polynomial
{
protected:
    using ZModuleType = typename PolynomialParams<TYPE>::ZModuleType;

public:
    TYPE *coefs;

    /**
     * @brief Constructs a polynomial with given parameters
     *
     * @param params polynomial parameters
     * @param context thread execution context
     * @param alloc allocator to use
     */
    Polynomial(const PolynomialParams<TYPE> *params,
        TfheThreadContext *context,
        Allocator *alloc);

    /**
     * @brief Destroys inner data of polynomial
     *
     * @param params polynomial parameters used for construction
     * @param context thread execution context
     * @param alloc allocator to use
     */
    void destroy(const PolynomialParams<TYPE> *params,
        TfheThreadContext *context,
        Allocator *alloc);

    /**
     * Static methods
     */

    /**
     * @brief Clear polynomial (zeros coefficients)
     */
    static void Clear(
        Polynomial<TYPE> *result,
        const PolynomialParams<TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * @brief Copy \c source polynomial to \c result
     */
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

    /**
     * @brief Multiply polynomial by (X^{a}-1)
     *      i.e. result = (X^{a}-1) * source
     */
    static void MulByXaiMinusOne(
        Polynomial<TYPE> *result,
        const int32_t a,
        const Polynomial<TYPE> *source,
        const PolynomialParams<TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * @brief Multiply polynomial by X^{a}
     *      i.e. result = (X^{a}-1) * source
     */
    static void MulByXai(
        Polynomial<TYPE> *result,
        const int32_t a,
        const Polynomial<TYPE> *source,
        const PolynomialParams<TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);
};

#endif // POLYNOMIAL_H
