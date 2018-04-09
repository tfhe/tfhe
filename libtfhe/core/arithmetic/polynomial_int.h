#ifndef POLYNOMIAL_INT_H
#define POLYNOMIAL_INT_H


#include "torus.h"
#include "polynomial.h"

/**
 * @brief Polynomial with integer coefficients
 *
 * @tparam INT_TYPE integer type of coefficients
 */
template<typename INT_TYPE>
class IntPolynomial : public Polynomial<INT_TYPE>
{
    static_assert(std::is_same<INT_TYPE, BigInt>::value or
        (std::is_integral<INT_TYPE>::value and std::is_signed<INT_TYPE>::value),
        "IntPolynomial<T> defined only for native signed integer types and BigInt");

public:
    /**
     * @brief Constructs a polynomial with given parameters
     *
     * @param params polynomial parameters
     * @param context thread execution context
     * @param alloc allocator to use
     */
    IntPolynomial(
        const PolynomialParams<INT_TYPE> *params,
        TfheThreadContext *context,
        Allocator *alloc) : Polynomial<INT_TYPE>(params, context, alloc) { }

    /**
     * @brief Destroys inner data of polynomial
     *
     * @param params polynomial parameters used for construction
     * @param context thread execution context
     * @param alloc allocator to use
     */
    void destroy(
        const PolynomialParams<INT_TYPE> *params,
        TfheThreadContext *context,
        Allocator *alloc)
    {
        destroy(params, context, alloc);
    }

    PREVENT_STACK_COPY(IntPolynomial);

    /**
     * Static methods
     */
public:
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
