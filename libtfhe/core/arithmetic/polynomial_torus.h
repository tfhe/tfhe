#ifndef POLYNOMIAL_TORUS_H
#define POLYNOMIAL_TORUS_H

#include "polynomial_int.h"

/**
 * @brief Polynomial with torus coefficients
 *
 * @tparam TORUS_TYPE type of coefficients
 */
template<typename TORUS_TYPE>
class TorusPolynomial : public Polynomial<TORUS_TYPE> {
    using INT_TYPE = typename TorusUtils<TORUS_TYPE>::INT_TYPE;
    using ZModuleType = typename Polynomial<TORUS_TYPE>::ZModuleType;

public:
    /**
     * @brief Constructs a polynomial with given parameters
     *
     * @param params polynomial parameters
     * @param context thread execution context
     * @param alloc allocator to use
     */
    TorusPolynomial(const PolynomialParams<TORUS_TYPE> *params,
                    TfheThreadContext *context,
                    Allocator *alloc) : Polynomial<TORUS_TYPE>(params, context, alloc) {}

    /**
     * @brief Destroys inner data of polynomial
     *
     * @param params polynomial parameters used for construction
     * @param context thread execution context
     * @param alloc allocator to use
     */
    void destroy(const PolynomialParams<TORUS_TYPE> *params,
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
    static void MultNaive_plain_aux(TORUS_TYPE *__restrict result,
                                    const INT_TYPE *__restrict poly1,
                                    const TORUS_TYPE *__restrict poly2,
                                    const int32_t N,
                                    const ZModuleType *const zparams,
                                    TfheThreadContext *context,
                                    Allocator alloc);

    /**
     * This function multiplies two polynomials in Z[X] and T[X] of degree < N
     * and round the result modulo X^N+1
     * The result is a polynomial of T[X] of degree < N
     * poly2 and result must point to different memory areas
     */
    static void MultNaive_aux(TORUS_TYPE *__restrict result,
                              const INT_TYPE *__restrict poly1,
                              const TORUS_TYPE *__restrict poly2,
                              const int32_t N,
                              const ZModuleType *const zparams,
                              TfheThreadContext *context,
                              Allocator alloc);

    /**
     *  This function multiplies two polynomials in Z[X] and T[X] of degree < N
     *  The result is a polynomial of T[X] of degree < 2N-1
     *  @param buf a memory area of length at least 4*N*sizeof(TORUS_TYPE)
     */
    static void Karatsuba_aux(TORUS_TYPE *R,
                              const INT_TYPE *A,
                              const TORUS_TYPE *B,
                              const int32_t size,
                              const char *buf,
                              const ZModuleType *const zparams,
                              TfheThreadContext *context,
                              Allocator alloc);


    static void MultNaive_plain_aux1(TORUS_TYPE *temp,
                                     TORUS_TYPE *__restrict result,
                                     const INT_TYPE *__restrict poly1,
                                     const TORUS_TYPE *__restrict poly2,
                                     const int32_t N,
                                     const ZModuleType *const zparams);

    static void Karatsuba_aux1(TORUS_TYPE *R,
                               INT_TYPE *bufBigInt,
                               TORUS_TYPE *bufBigTorus,
                               TORUS_TYPE *temp,
                               const INT_TYPE *A,
                               const TORUS_TYPE *B,
                               const int32_t size,
                               const ZModuleType *const zparams);

public:
    /**  @brief TorusPolynomial = random */
    static void Uniform(TorusPolynomial<TORUS_TYPE> *result,
                        const PolynomialParams<TORUS_TYPE> *params,
                        TfheThreadContext *context,
                        Allocator alloc);

    /**  TorusPolynomial + p*TorusPolynomial */
    static void AddMulZ(TorusPolynomial<TORUS_TYPE> *result,
                        const TorusPolynomial<TORUS_TYPE> *poly1,
                        const INT_TYPE *p,
                        const TorusPolynomial<TORUS_TYPE> *poly2,
                        const PolynomialParams<TORUS_TYPE> *params,
                        TfheThreadContext *context,
                        Allocator alloc);

    /**  TorusPolynomial += p*TorusPolynomial */
    static void AddMulZTo(TorusPolynomial<TORUS_TYPE> *result,
                          const INT_TYPE *p,
                          const TorusPolynomial<TORUS_TYPE> *poly2,
                          const PolynomialParams<TORUS_TYPE> *params,
                          TfheThreadContext *context,
                          Allocator alloc);

    /**  TorusPolynomial - p*TorusPolynomial */
    static void SubMulZ(TorusPolynomial<TORUS_TYPE> *result,
                        const TorusPolynomial<TORUS_TYPE> *poly1,
                        const INT_TYPE *p,
                        const TorusPolynomial<TORUS_TYPE> *poly2,
                        const PolynomialParams<TORUS_TYPE> *params,
                        TfheThreadContext *context,
                        Allocator alloc);

    /**  TorusPolynomial -= p*TorusPolynomial */
    static void SubMulZTo(TorusPolynomial<TORUS_TYPE> *result,
                          const INT_TYPE *p,
                          const TorusPolynomial<TORUS_TYPE> *poly2,
                          const PolynomialParams<TORUS_TYPE> *params,
                          TfheThreadContext *context,
                          Allocator alloc);

    /**  Infinity norm of the distance between two torus polynomials */
    static double NormInftyDist(const TorusPolynomial<TORUS_TYPE> *poly1,
                                const TorusPolynomial<TORUS_TYPE> *poly2,
                                const PolynomialParams<TORUS_TYPE> *params,
                                TfheThreadContext *context,
                                Allocator alloc);

    /**
     * This is the naive external multiplication of an integer polynomial
     * with a torus polynomial. (this function should yield exactly the same
     * result as the karatsuba or fft version, but should be slower)
     */
    static void MultNaive(TorusPolynomial<TORUS_TYPE> *result,
                          const IntPolynomial<INT_TYPE> *poly1,
                          const TorusPolynomial<TORUS_TYPE> *poly2,
                          const PolynomialParams<TORUS_TYPE> *params,
                          TfheThreadContext *context,
                          Allocator alloc);

    /**
     * This function multiplies 2 polynomials (an integer poly and a torus poly)
     * by using Karatsuba
     * WARNING: N must be a power of 2 to use this function. Else, the
     * behaviour is unpredictable
     */
    static void MultKaratsuba(TorusPolynomial<TORUS_TYPE> *result,
                              const IntPolynomial<INT_TYPE> *poly1,
                              const TorusPolynomial<TORUS_TYPE> *poly2,
                              const PolynomialParams<TORUS_TYPE> *params,
                              TfheThreadContext *context,
                              Allocator alloc);

    /**
     * result += poly1 * poly2 (via Karatsuba)
     * WARNING: N must be a power of 2 to use this function. Else, the
     * behaviour is unpredictable
     */
    static void AddMulRKaratsuba(TorusPolynomial<TORUS_TYPE> *result,
                                 const IntPolynomial<INT_TYPE> *poly1,
                                 const TorusPolynomial<TORUS_TYPE> *poly2,
                                 const PolynomialParams<TORUS_TYPE> *params,
                                 TfheThreadContext *context,
                                 Allocator alloc);

    /**
     * result -= poly1 * poly2 (via karatsuba)
     * WARNING: N must be a power of 2 to use this function. Else, the
     * behaviour is unpredictable
     */
    static void SubMulRKaratsuba(TorusPolynomial<TORUS_TYPE> *result,
                                 const IntPolynomial<INT_TYPE> *poly1,
                                 const TorusPolynomial<TORUS_TYPE> *poly2,
                                 const PolynomialParams<TORUS_TYPE> *params,
                                 TfheThreadContext *context,
                                 Allocator alloc);


    static void MultKaratsuba1(TorusPolynomial<TORUS_TYPE> *result,
                               const IntPolynomial<INT_TYPE> *poly1,
                               const TorusPolynomial<TORUS_TYPE> *poly2,
                               const PolynomialParams<TORUS_TYPE> *params,
                               TfheThreadContext *context,
                               Allocator alloc);


};


#endif
