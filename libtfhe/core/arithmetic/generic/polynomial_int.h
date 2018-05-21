#ifndef POLYNOMIAL_INT_H
#define POLYNOMIAL_INT_H


#include "core/arithmetic/torus.h"
#include "core/arithmetic/generic/polynomial.h"

/**
 * @brief Polynomial with integer coefficients
 *
 * @tparam TORUS torus type
 */
template<typename TORUS, AsmTypeEnum AsmType>
class IntPolynomial : public Polynomial<TORUS, CoefTypeEnum::Integer, AsmType> {
public:
    /**
     * @brief Constructs a polynomial with given parameters
     *
     * @param params polynomial parameters
     * @param context thread execution context
     * @param alloc allocator to use
     */
    IntPolynomial(
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
    void destroy(const PolynomialParams<TORUS, AsmType> *params,
                 TfheThreadContext *context,
                 Allocator *alloc);

    PREVENT_STACK_COPY(IntPolynomial);

    /**
     * Static methods
     */
public:
    /** Euclidean norm of an Integer Polynomial */
    static double Norm2sq(const IntPolynomial<TORUS, AsmType> *poly,
                          const PolynomialParams<TORUS, AsmType> *params,
                          TfheThreadContext *context,
                          Allocator alloc);

    /** Infinity norm of the distance between two integer polynomials */
    static double NormInftyDist(const IntPolynomial<TORUS, AsmType> *poly1,
                                const IntPolynomial<TORUS, AsmType> *poly2,
                                const PolynomialParams<TORUS, AsmType> *params,
                                TfheThreadContext *context,
                                Allocator alloc);
};

#endif // POLYNOMIAL_INT_H
