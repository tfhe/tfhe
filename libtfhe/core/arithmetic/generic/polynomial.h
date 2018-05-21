#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "polynomial_param.h"
#include "core/allocator/allocator.h"
#include "core/contexts/tfhe_thread_context.h"
#include "core/arithmetic/torus_utils.h"
#include "coeff_type.h"
#include "zmodule_vector.h"

#include <cassert>

/**
 * @brief Polynomial class
 *
 * @tparam TORUS torus type
 * @tparam CoefType = CoefTypeEnum::Torus type of coefficients
 */
template<typename TORUS, CoefTypeEnum CoefType, AsmTypeEnum AsmType>
class Polynomial : public ZModuleVector<TORUS, CoefType, AsmType> {
public:
    static_assert(CoefType == CoefTypeEnum::Torus or CoefType == CoefTypeEnum::Integer,
                  "Polynomial<TORUS,CoefTypeEnum> only Torus and Integer underlying types are supported for the moment");

    using typename ZModuleVector<TORUS, CoefType, AsmType>::TYPE;

    /**
     * @brief Constructs a polynomial with given parameters
     *
     * @param params polynomial parameters
     * @param context thread execution context
     * @param alloc allocator to use
     */
    Polynomial(
            const PolynomialParams<TORUS, AsmType> *params,
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
            const PolynomialParams<TORUS, AsmType> *params,
            TfheThreadContext *context,
            Allocator *alloc);

    /**
     * Static methods
     */

    /**
     * @brief Multiply polynomial by (X^{a}-1)
     *      i.e. result = (X^{a}-1) * source
     */
    static void MulByXaiMinusOne(
            Polynomial<TORUS, CoefType, AsmType> *result,
            const int32_t a,
            const Polynomial<TORUS, CoefType, AsmType> *source,
            const PolynomialParams<TORUS, AsmType> *params,
            TfheThreadContext *context,
            Allocator alloc);

    /**
     * @brief Multiply polynomial by X^{a}
     *      i.e. result = (X^{a}-1) * source
     */
    static void MulByXai(
            Polynomial<TORUS, CoefType, AsmType> *result,
            const int32_t a,
            const Polynomial<TORUS, CoefType, AsmType> *source,
            const PolynomialParams<TORUS, AsmType> *params,
            TfheThreadContext *context,
            Allocator alloc);
};

#endif // POLYNOMIAL_H
