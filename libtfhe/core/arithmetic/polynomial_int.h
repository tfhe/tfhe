#ifndef POLYNOMIAL_INT_H
#define POLYNOMIAL_INT_H


#include "torus.h"
#include "polynomial.h"

namespace tfhe_core_intern {
    /**
     * @brief Helper class to automatically choose coefficient types for IntPolynomial class.
     *  In this case there is no need to have IntPolynomial with 2 template parameters.
     */
    template<typename TORUS, typename INT_TYPE = typename TorusUtils<TORUS>::INT_TYPE>
    struct IntPolynomial_ : public Polynomial<TORUS, INT_TYPE> {
        IntPolynomial_(
            const PolynomialParams<TORUS> *params,
            TfheThreadContext *context,
            Allocator *alloc) : Polynomial<TORUS,INT_TYPE>(params, context, alloc) { }

        void destroy(
            const PolynomialParams<TORUS> *params,
            TfheThreadContext *context,
            Allocator *alloc)
        {
            destroy(params, context, alloc);
        }
    };
};

/**
 * @brief Polynomial with integer coefficients
 *
 * @tparam TORUS torus type
 */
template<typename TORUS>
class IntPolynomial : public tfhe_core_intern::IntPolynomial_<TORUS>
{
public:
    /**
     * @brief Constructs a polynomial with given parameters
     *
     * @param params polynomial parameters
     * @param context thread execution context
     * @param alloc allocator to use
     */
    IntPolynomial(
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator *alloc) : tfhe_core_intern::IntPolynomial_<TORUS>(params, context, alloc) { }

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
    static double Norm2sq(const IntPolynomial<TORUS> *poly,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc);

    /** Infinity norm of the distance between two integer polynomials */
    static double NormInftyDist(const IntPolynomial<TORUS> *poly1,
        const IntPolynomial<TORUS> *poly2,
        const PolynomialParams<TORUS> *params,
        TfheThreadContext *context,
        Allocator alloc);
};

#endif // POLYNOMIAL_INT_H
