#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "polynomial_param.h"
#include "../allocator/allocator.h"
#include "../contexts/tfhe_thread_context.h"
#include "torus_utils.h"

#include <cassert>

/**
 * Polynomial coefficient types enumeration
 */
enum CoefTypeEnum {
    Torus,
    Integer,
    Real            // for complex Lagrangian representation?
};

/**
 * @brief Polynomial class
 *
 * @tparam TORUS torus type
 * @tparam CoefType = CoefTypeEnum::Torus type of coefficients
 */
template<typename TORUS, CoefTypeEnum CoefType>
class Polynomial {
public:
    static_assert(CoefType == CoefTypeEnum::Torus or CoefType == CoefTypeEnum::Integer,
                  "Polynomial<TORUS,CoefTypeEnum> only Torus and Integer underlying types are supported for the moment");

    using TYPE = typename
    std::conditional<
            CoefType == CoefTypeEnum::Torus,
            TORUS,
            typename TorusUtils<TORUS>::INT_TYPE
    >::type;

    TYPE *coefs;

    /**
     * @brief Constructs a polynomial with given parameters
     *
     * @param params polynomial parameters
     * @param context thread execution context
     * @param alloc allocator to use
     */
    Polynomial(
            const PolynomialParams<TORUS> *params,
            TfheThreadContext *context,
            Allocator *alloc);

    /**
     * @brief Destroys inner data of polynomial
     *
     * @param params polynomial parameters used for construction
     * @param context thread execution context
     * @param alloc allocator to use
     */
    void destroy(
            const PolynomialParams<TORUS> *params,
            TfheThreadContext *context,
            Allocator *alloc);

    /**
     * Static methods
     */

    /**
     * @brief Clear polynomial (zeros coefficients)
     */
    static void Clear(
            Polynomial<TORUS, CoefType> *result,
            const PolynomialParams<TORUS> *params,
            TfheThreadContext *context,
            Allocator alloc);

    /**
     * @brief Copy \c source polynomial to \c result
     */
    static void Copy(
            Polynomial<TORUS, CoefType> *result,
            const Polynomial<TORUS, CoefType> *source,
            const PolynomialParams<TORUS> *params,
            TfheThreadContext *context,
            Allocator alloc);

    /**
     * @brief Add \c poly1 to \c poly2 and store result in \c result
     *      i.e. result = poly1 + poly2
     */
    static void Add(
            Polynomial<TORUS, CoefType> *result,
            const Polynomial<TORUS, CoefType> *poly1,
            const Polynomial<TORUS, CoefType> *poly2,
            const PolynomialParams<TORUS> *params,
            TfheThreadContext *context,
            Allocator alloc);

    /**
     * @brief Add \c poly polynomial to \c result
     *      i.e. result = result + poly
     */
    static void AddTo(
            Polynomial<TORUS, CoefType> *result,
            const Polynomial<TORUS, CoefType> *poly,
            const PolynomialParams<TORUS> *params,
            TfheThreadContext *context,
            Allocator alloc);

    /**
     * @brief Substract \c poly2 from \c poly1 and store result in \c result
     *      i.e. result = poly1 - poly2
     */
    static void Sub(
            Polynomial<TORUS, CoefType> *result,
            const Polynomial<TORUS, CoefType> *poly1,
            const Polynomial<TORUS, CoefType> *poly2,
            const PolynomialParams<TORUS> *params,
            TfheThreadContext *context,
            Allocator alloc);

    /**
     * @brief Substract \c poly polynomial from \c result
     *      i.e. result = result - poly
     */
    static void SubTo(
            Polynomial<TORUS, CoefType> *result,
            const Polynomial<TORUS, CoefType> *poly,
            const PolynomialParams<TORUS> *params,
            TfheThreadContext *context,
            Allocator alloc);

    /**
     * @brief Multiply polynomial by (X^{a}-1)
     *      i.e. result = (X^{a}-1) * source
     */
    static void MulByXaiMinusOne(
            Polynomial<TORUS, CoefType> *result,
            const int32_t a,
            const Polynomial<TORUS, CoefType> *source,
            const PolynomialParams<TORUS> *params,
            TfheThreadContext *context,
            Allocator alloc);

    /**
     * @brief Multiply polynomial by X^{a}
     *      i.e. result = X^{a} * source
     */
    static void MulByXai(
            Polynomial<TORUS, CoefType> *result,
            const int32_t a,
            const Polynomial<TORUS, CoefType> *source,
            const PolynomialParams<TORUS> *params,
            TfheThreadContext *context,
            Allocator alloc);
};

#endif // POLYNOMIAL_H
