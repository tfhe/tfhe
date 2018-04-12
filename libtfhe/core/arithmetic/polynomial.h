#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "polynomial_param.h"
#include "../allocator/allocator.h"
#include "threadcontext.h"

#include <cassert>

/**
 * @brief Polynomial class
 *
 * @tparam TORUS torus type
 * @tparam TYPE = TORUS polynomial coefficient type
 */
template<typename TORUS, typename TYPE = TORUS>
class Polynomial
{
public:
    TYPE *coefs;

    /**
     * @brief Constructs a polynomial with given parameters
     *
     * @param params polynomial parameters
     * @param context thread execution context
     * @param alloc allocator to use
     */
    Polynomial(const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator *alloc);

    /**
     * @brief Destroys inner data of polynomial
     *
     * @param params polynomial parameters used for construction
     * @param context thread execution context
     * @param alloc allocator to use
     */
    void destroy(const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator *alloc);

    /**
     * Static methods
     */

    /**
     * @brief Clear polynomial (zeros coefficients)
     */
    static void Clear(
        Polynomial<TORUS,TYPE> *result,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * @brief Copy \c source polynomial to \c result
     */
    static void Copy(
        Polynomial<TORUS,TYPE> *result,
        const Polynomial<TORUS,TYPE> *source,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * @brief Add \c poly1 to \c poly2 and store result in \c result
     *      i.e. result = poly1 + poly2
     */
    static void Add(
        Polynomial<TORUS,TYPE> *result,
        const Polynomial<TORUS,TYPE> *poly1,
        const Polynomial<TORUS,TYPE> *poly2,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * @brief Add \c poly polynomial to \c result
     *      i.e. result = result + poly
     */
    static void AddTo(
        Polynomial<TORUS,TYPE> *result,
        const Polynomial<TORUS,TYPE> *poly,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * @brief Substract \c poly2 from \c poly1 and store result in \c result
     *      i.e. result = poly1 - poly2
     */
    static void Sub(
        Polynomial<TORUS,TYPE> *result,
        const Polynomial<TORUS,TYPE> *poly1,
        const Polynomial<TORUS,TYPE> *poly2,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * @brief Substract \c poly polynomial from \c result
     *      i.e. result = result - poly
     */
    static void SubTo(
        Polynomial<TORUS,TYPE> *result,
        const Polynomial<TORUS,TYPE> *poly,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * @brief Multiply polynomial by (X^{a}-1)
     *      i.e. result = (X^{a}-1) * source
     */
    static void MulByXaiMinusOne(
        Polynomial<TORUS,TYPE> *result,
        const int32_t a,
        const Polynomial<TORUS,TYPE> *source,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * @brief Multiply polynomial by X^{a}
     *      i.e. result = (X^{a}-1) * source
     */
    static void MulByXai(
        Polynomial<TORUS,TYPE> *result,
        const int32_t a,
        const Polynomial<TORUS,TYPE> *source,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc);
};

#endif // POLYNOMIAL_H
