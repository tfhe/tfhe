#ifndef TFHE_ZMODULE_VECTOR_H
#define TFHE_ZMODULE_VECTOR_H


#include <core/arithmetic/torus_utils.h>
#include <core/contexts/tfhe_thread_context.h>
#include "coeff_type.h"
#include "asm_mode.h"
#include "zmodule_vector_params.h"

/**
 * @brief vector class
 *
 * @tparam TORUS torus type
 * @tparam CoefType = CoefTypeEnum::Torus type of coefficients
 */
template<typename TORUS, CoefTypeEnum CoefType, AsmTypeEnum AsmType>
class ZModuleVector {
public:
    static_assert(CoefType == CoefTypeEnum::Torus or CoefType == CoefTypeEnum::Integer,
                  "vector<TORUS,CoefTypeEnum> only Torus and Integer underlying types are supported for the moment");

    using TYPE = typename
    std::conditional<
            CoefType == CoefTypeEnum::Torus,
            TORUS,
            typename TorusUtils<TORUS>::INT_TYPE
    >::type;

    TYPE *coefs;

    /**
     * @brief Constructs a vector with given parameters
     *
     * @param params vector parameters
     * @param context thread execution context
     * @param alloc allocator to use
     */
    ZModuleVector(
            const ZModuleVectorParams<TORUS, AsmType> *params,
            TfheThreadContext *context,
            Allocator *alloc);

    /**
     * @brief Destroys inner data of vector
     *
     * @param params vector parameters used for construction
     * @param context thread execution context
     * @param alloc allocator to use
     */
    void destroy(
            const ZModuleVectorParams<TORUS, AsmType> *params,
            TfheThreadContext *context,
            Allocator *alloc);

    /**
     * Static methods
     */

    /**
     * @brief Clear vector (zeros coefficients)
     */
    static void Clear(
            ZModuleVector<TORUS, CoefType, AsmType> *result,
            const ZModuleVectorParams<TORUS, AsmType> *params,
            TfheThreadContext *context,
            Allocator alloc);

    /**
     * @brief Copy \c source vector to \c result
     */
    static void Copy(
            ZModuleVector<TORUS, CoefType, AsmType> *result,
            const ZModuleVector<TORUS, CoefType, AsmType> *source,
            const ZModuleVectorParams<TORUS, AsmType> *params,
            TfheThreadContext *context,
            Allocator alloc);

    /**
     * @brief Add \c poly1 to \c poly2 and store result in \c result
     *      i.e. result = poly1 + poly2
     */
    static void Add(
            ZModuleVector<TORUS, CoefType, AsmType> *result,
            const ZModuleVector<TORUS, CoefType, AsmType> *vec1,
            const ZModuleVector<TORUS, CoefType, AsmType> *vec2,
            const ZModuleVectorParams<TORUS, AsmType> *params,
            TfheThreadContext *context,
            Allocator alloc);

    /**
     * @brief Add \c poly vector to \c result
     *      i.e. result = result + poly
     */
    static void AddTo(
            ZModuleVector<TORUS, CoefType, AsmType> *result,
            const ZModuleVector<TORUS, CoefType, AsmType> *vec2,
            const ZModuleVectorParams<TORUS, AsmType> *params,
            TfheThreadContext *context,
            Allocator alloc);

    /**
     * @brief Substract \c poly2 from \c poly1 and store result in \c result
     *      i.e. result = poly1 - poly2
     */
    static void Sub(
            ZModuleVector<TORUS, CoefType, AsmType> *result,
            const ZModuleVector<TORUS, CoefType, AsmType> *vec1,
            const ZModuleVector<TORUS, CoefType, AsmType> *vec2,
            const ZModuleVectorParams<TORUS, AsmType> *params,
            TfheThreadContext *context,
            Allocator alloc);

    /**
     * @brief Substract \c poly vector from \c result
     *      i.e. result = result - poly
     */
    static void SubTo(
            ZModuleVector<TORUS, CoefType, AsmType> *result,
            const ZModuleVector<TORUS, CoefType, AsmType> *vec2,
            const ZModuleVectorParams<TORUS, AsmType> *params,
            TfheThreadContext *context,
            Allocator alloc);

};


#endif //TFHE_ZMODULE_VECTOR_H
