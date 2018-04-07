#ifndef POLYNOMIAL_INT_H
#define POLYNOMIAL_INT_H


#include "torus.h"
#include "tfhe_core.h"
#include "polynomial_param.h"
#include "../allocator/allocator.h"
#include "threadcontext.h"


/**
 * @brief Polynomial with integer coefficients
 * 
 * @tparam INT_TYPE integer type of coefficients
 */
template<typename INT_TYPE>
class IntPolynomial
{
public:
    INT_TYPE *coefs;

    /**
     * @brief Constructs a polynomial with given parameters
     * 
     * @param params polynomial parameters
     * @param context thread execution context
     * @param alloc allocator to use
     */
    IntPolynomial(const PolynomialParams<INT_TYPE> *params,
        TfheThreadContext *context,
        Allocator *alloc);

    /**
     * @brief Destroys inner data of polynomial
     * 
     * @param params polynomial parameters used for construction
     * @param context thread execution context
     * @param alloc allocator to use
     */
    void destroy(const PolynomialParams<INT_TYPE> *params,
        TfheThreadContext *context,
        Allocator *alloc);

    PREVENT_STACK_COPY(IntPolynomial);

    /**
     * Static methods
     */
public:

    static void Clear(IntPolynomial<INT_TYPE> *result,
        const PolynomialParams<INT_TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  result = source */
    static void Copy(IntPolynomial<INT_TYPE> *result,
        const IntPolynomial<INT_TYPE> *source,
        const PolynomialParams<INT_TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  result += poly1 */
    static void AddTo(IntPolynomial<INT_TYPE> *result,
        const IntPolynomial<INT_TYPE> *poly1,
        const PolynomialParams<INT_TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  result = (X^ai-1) * source */
    static void MulByXaiMinusOne(IntPolynomial<INT_TYPE> *result,
        const int32_t ai,
        const IntPolynomial<INT_TYPE> *source,
        const PolynomialParams<INT_TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  result = (X^ai) * source */
    static void MulByXai(IntPolynomial<INT_TYPE> *result,
        const int32_t ai,
        const IntPolynomial<INT_TYPE> *source,
        const PolynomialParams<INT_TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /** Euclidean norm of an Integer Polynomial */
    static double Norm2sq(const IntPolynomial<INT_TYPE> *poly,
        const PolynomialParams<INT_TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /** Infinity norm of the distance between two integer polynomials */
    static double NormInftyDist(const IntPolynomial<INT_TYPE> *poly1,
        const IntPolynomial<INT_TYPE> *poly2,
        const PolynomialParams<INT_TYPE> *params,
        TfheThreadContext *context,
        Allocator alloc);
};

#endif // POLYNOMIAL_INT_H