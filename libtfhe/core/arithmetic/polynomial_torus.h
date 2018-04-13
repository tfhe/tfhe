#ifndef POLYNOMIAL_TORUS_H
#define POLYNOMIAL_TORUS_H

#include "polynomial_int.h"

/**
 * @brief Polynomial with torus coefficients
 *
 * @tparam TORUS type of coefficients
 */
template<typename TORUS>
class TorusPolynomial : public Polynomial<TORUS, CoefTypeEnum::Torus> {
    using INT_TYPE = typename TorusUtils<TORUS>::INT_TYPE;

public:
    /**
     * @brief Constructs a polynomial with given parameters
     *
     * @param params polynomial parameters
     * @param context thread execution context
     * @param alloc allocator to use
     */
    TorusPolynomial(const PolynomialParams<TORUS> *params,
                    TfheThreadContext *context,
                    Allocator *alloc) : Polynomial<TORUS,
            CoefTypeEnum::Torus>(params, context, alloc) {}

    /**
     * @brief Destroys inner data of polynomial
     *
     * @param params polynomial parameters used for construction
     * @param context thread execution context
     * @param alloc allocator to use
     */
    void destroy(const PolynomialParams<TORUS> *params,
                 TfheThreadContext *context,
                 Allocator *alloc) {
        destroy(params, context, alloc);
    }

    PREVENT_STACK_COPY(TorusPolynomial);

    /**
     * Static methods
     */

private:
    /**
     * This function multiplies two polynomials in Z[X] and T[X] of degree < N
     * The result is a polynomial of T[X] of degree < 2N-1
     * poly2 and result must point to different memory areas
     */
    static void MultNaive_plain_aux_old(TORUS *__restrict result,
                                        const INT_TYPE *__restrict poly1,
                                        const TORUS *__restrict poly2,
                                        const int32_t N,
                                        const ZModuleParams<TORUS> *const zparams,
                                        TfheThreadContext *context,
                                        Allocator alloc);

    /**
     * This function multiplies two polynomials in Z[X] and T[X] of degree < N
     * and round the result modulo X^N+1
     * The result is a polynomial of T[X] of degree < N
     * poly2 and result must point to different memory areas
     */
    static void MultNaive_aux(TORUS *__restrict result,
                              const INT_TYPE *__restrict poly1,
                              const TORUS *__restrict poly2,
                              const int32_t N,
                              const ZModuleParams<TORUS> *const zparams,
                              TfheThreadContext *context,
                              Allocator alloc);

    /**
     *  This function multiplies two polynomials in Z[X] and T[X] of degree < N
     *  The result is a polynomial of T[X] of degree < 2N-1
     *  @param buf a memory area of length at least 4*N*sizeof(TORUS)
     */
    static void Karatsuba_aux_old(TORUS *R,
                                  const INT_TYPE *A,
                                  const TORUS *B,
                                  const int32_t size,
                                  const char *buf,
                                  const ZModuleParams<TORUS> *const zparams,
                                  TfheThreadContext *context,
                                  Allocator alloc);


public:
    /**  @brief TorusPolynomial = random */
    static void Uniform(TorusPolynomial<TORUS> *result,
                        const PolynomialParams<TORUS> *params,
                        TfheThreadContext *context,
                        Allocator alloc);

    /**  TorusPolynomial + p*TorusPolynomial */
    static void AddMulZ(TorusPolynomial<TORUS> *result,
                        const TorusPolynomial<TORUS> *poly1,
                        const INT_TYPE *p,
                        const TorusPolynomial<TORUS> *poly2,
                        const PolynomialParams<TORUS> *params,
                        TfheThreadContext *context,
                        Allocator alloc);

    /**  TorusPolynomial += p*TorusPolynomial */
    static void AddMulZTo(TorusPolynomial<TORUS> *result,
                          const INT_TYPE *p,
                          const TorusPolynomial<TORUS> *poly2,
                          const PolynomialParams<TORUS> *params,
                          TfheThreadContext *context,
                          Allocator alloc);

    /**  TorusPolynomial - p*TorusPolynomial */
    static void SubMulZ(TorusPolynomial<TORUS> *result,
                        const TorusPolynomial<TORUS> *poly1,
                        const INT_TYPE *p,
                        const TorusPolynomial<TORUS> *poly2,
                        const PolynomialParams<TORUS> *params,
                        TfheThreadContext *context,
                        Allocator alloc);

    /**  TorusPolynomial -= p*TorusPolynomial */
    static void SubMulZTo(TorusPolynomial<TORUS> *result,
                          const INT_TYPE *p,
                          const TorusPolynomial<TORUS> *poly2,
                          const PolynomialParams<TORUS> *params,
                          TfheThreadContext *context,
                          Allocator alloc);

    /**  Infinity norm of the distance between two torus polynomials */
    static double NormInftyDist(const TorusPolynomial<TORUS> *poly1,
                                const TorusPolynomial<TORUS> *poly2,
                                const PolynomialParams<TORUS> *params,
                                TfheThreadContext *context,
                                Allocator alloc);

    /**
     * This is the naive external multiplication of an integer polynomial
     * with a torus polynomial. (this function should yield exactly the same
     * result as the karatsuba or fft version, but should be slower)
     */
    static void MultNaive(TorusPolynomial<TORUS> *result,
                          const IntPolynomial<TORUS> *poly1,
                          const TorusPolynomial<TORUS> *poly2,
                          const PolynomialParams<TORUS> *params,
                          TfheThreadContext *context,
                          Allocator alloc);

    /**
     * This function multiplies 2 polynomials (an integer poly and a torus poly)
     * by using Karatsuba
     * WARNING: N must be a power of 2 to use this function. Else, the
     * behaviour is unpredictable
     */
    static void MultKaratsuba_old(TorusPolynomial<TORUS> *result,
                                  const IntPolynomial<TORUS> *poly1,
                                  const TorusPolynomial<TORUS> *poly2,
                                  const PolynomialParams<TORUS> *params,
                                  TfheThreadContext *context,
                                  Allocator alloc);

    /**
     * result += poly1 * poly2 (via Karatsuba)
     * WARNING: N must be a power of 2 to use this function. Else, the
     * behaviour is unpredictable
     */
    static void AddMulRKaratsuba_old(TorusPolynomial<TORUS> *result,
                                     const IntPolynomial<TORUS> *poly1,
                                     const TorusPolynomial<TORUS> *poly2,
                                     const PolynomialParams<TORUS> *params,
                                     TfheThreadContext *context,
                                     Allocator alloc);

    /**
     * result -= poly1 * poly2 (via karatsuba)
     * WARNING: N must be a power of 2 to use this function. Else, the
     * behaviour is unpredictable
     */
    static void SubMulRKaratsuba_old(TorusPolynomial<TORUS> *result,
                                     const IntPolynomial<TORUS> *poly1,
                                     const TorusPolynomial<TORUS> *poly2,
                                     const PolynomialParams<TORUS> *params,
                                     TfheThreadContext *context,
                                     Allocator alloc);


    static void MultKaratsuba(TorusPolynomial<TORUS> *result,
                              const IntPolynomial<TORUS> *poly1,
                              const TorusPolynomial<TORUS> *poly2,
                              const PolynomialParams<TORUS> *params,
                              TfheThreadContext *context,
                              Allocator alloc);

    static void AddMulRKaratsuba(TorusPolynomial<TORUS> *result,
                                 const IntPolynomial<TORUS> *poly1,
                                 const TorusPolynomial<TORUS> *poly2,
                                 const PolynomialParams<TORUS> *params,
                                 TfheThreadContext *context,
                                 Allocator alloc);


    static void SubMulRKaratsuba(TorusPolynomial<TORUS> *result,
                                 const IntPolynomial<TORUS> *poly1,
                                 const TorusPolynomial<TORUS> *poly2,
                                 const PolynomialParams<TORUS> *params,
                                 TfheThreadContext *context,
                                 Allocator alloc);

};


#endif
