#ifndef POLYNOMIALS_ARITHMETIC_H
#define POLYNOMIALS_ARITHMETIC_H

#include "tfhe_core.h"
#include "polynomials.h"
#include "numeric_utils.h"

///@file
///@brief This file contains the operations on polynomials

template<typename TORUS>
struct TorusPolyFunctions {
private:
    using INT_TYPE = typename TorusUtils<TORUS>::INT_TYPE;

    /**
     * This function multiplies two polynomials in Z[X] and T[X] of degree < N
     * The result is a polynomial of T[X] of degree < 2N-1
     * poly2 and result must point to different memory areas
     */
    static void MultNaive_plain_aux(TORUS *__restrict result,
        const INT_TYPE *__restrict poly1,
        const TORUS *__restrict poly2,
        const int32_t N,
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
        TfheThreadContext *context,
        Allocator alloc);

    /**
     *  This function multiplies two polynomials in Z[X] and T[X] of degree < N
     *  The result is a polynomial of T[X] of degree < 2N-1
     *  @param buf a memory area of length at least 4*N*sizeof(TORUS)
     */
    static void Karatsuba_aux(TORUS *R,
        const INT_TYPE *A,
        const TORUS *B,
        const int32_t size,
        const char *buf,
        TfheThreadContext *context,
        Allocator alloc);

public:

    /**  @brief TorusPolynomial := 0 */
    static void Clear(TorusPolynomial<TORUS> *result,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  @brief TorusPolynomial = random */
    static void Uniform(TorusPolynomial<TORUS> *result,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  @brief TorusPolynomial = TorusPolynomial */
    static void Copy(TorusPolynomial<TORUS> *result,
        const TorusPolynomial<TORUS> *sample,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  TorusPolynomial + TorusPolynomial */
    static void Add(TorusPolynomial<TORUS> *result,
        const TorusPolynomial<TORUS> *poly1,
        const TorusPolynomial<TORUS> *poly2,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  TorusPolynomial += TorusPolynomial */
    static void AddTo(TorusPolynomial<TORUS> *result,
        const TorusPolynomial<TORUS> *poly2,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  TorusPolynomial - TorusPolynomial */
    static void Sub(TorusPolynomial<TORUS> *result,
        const TorusPolynomial<TORUS> *poly1,
        const TorusPolynomial<TORUS> *poly2,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  TorusPolynomial -= TorusPolynomial */
    static void SubTo(TorusPolynomial<TORUS> *result,
        const TorusPolynomial<TORUS> *poly2,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  TorusPolynomial + p*TorusPolynomial */
    static void AddMulZ(TorusPolynomial<TORUS> *result,
        const TorusPolynomial<TORUS> *poly1,
        const INT_TYPE p,
        const TorusPolynomial<TORUS> *poly2,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  TorusPolynomial += p*TorusPolynomial */
    static void AddMulZTo(TorusPolynomial<TORUS> *result,
        const INT_TYPE p,
        const TorusPolynomial<TORUS> *poly2,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  TorusPolynomial - p*TorusPolynomial */
    static void SubMulZ(TorusPolynomial<TORUS> *result,
        const TorusPolynomial<TORUS> *poly1,
        const INT_TYPE p,
        const TorusPolynomial<TORUS> *poly2,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  TorusPolynomial -= p*TorusPolynomial */
    static void SubMulZTo(TorusPolynomial<TORUS> *result,
        const INT_TYPE p,
        const TorusPolynomial<TORUS> *poly2,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  TorusPolynomial = (X^a-1)* TorusPolynomial */
    static void MulByXaiMinusOne(TorusPolynomial<TORUS> *result,
         const INT_TYPE a,
         const TorusPolynomial<TORUS> *source,
         const PolynomialParameters *params,
         TfheThreadContext *context,
         Allocator alloc);

    /** result= X^{a}*source */
    static void MulByXai(TorusPolynomial<TORUS> *result,
        const INT_TYPE a,
        const TorusPolynomial<TORUS> *source,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  Infinity norm of the distance between two torus polynomials */
    static double NormInftyDist(const TorusPolynomial<TORUS> *poly1,
        const TorusPolynomial<TORUS> *poly2,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * This is the naive external multiplication of an integer polynomial
     * with a torus polynomial. (this function should yield exactly the same
     * result as the karatsuba or fft version, but should be slower)
     */
    static void MultNaive(TorusPolynomial<TORUS> *result,
        const IntPolynomial<INT_TYPE> *poly1,
        const TorusPolynomial<TORUS> *poly2,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * This function multiplies 2 polynomials (an integer poly and a torus poly)
     * by using Karatsuba
     * WARNING: N must be a power of 2 to use this function. Else, the
     * behaviour is unpredictable
     */
    static void MultKaratsuba(TorusPolynomial<TORUS> *result,
        const IntPolynomial<INT_TYPE> *poly1,
        const TorusPolynomial<TORUS> *poly2,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * result += poly1 * poly2 (via Karatsuba)
     * WARNING: N must be a power of 2 to use this function. Else, the
     * behaviour is unpredictable
     */
    static void AddMulRKaratsuba(TorusPolynomial<TORUS> *result,
        const IntPolynomial<INT_TYPE> *poly1,
        const TorusPolynomial<TORUS> *poly2,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**
     * result -= poly1 * poly2 (via karatsuba)
     * WARNING: N must be a power of 2 to use this function. Else, the
     * behaviour is unpredictable
     */
    static void SubMulRKaratsuba(TorusPolynomial<TORUS> *result,
        const IntPolynomial<INT_TYPE> *poly1,
        const TorusPolynomial<TORUS> *poly2,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

};

TORUS_CLASS_IMPL_ALL(TorusPolyFunctions);

template<typename INT_TYPE>
struct IntPolyFunctions {
    /**  IntPolynomial = 0 */
    static void Clear(IntPolynomial<INT_TYPE> *result,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  result = source */
    static void Copy(IntPolynomial<INT_TYPE> *result,
        const IntPolynomial<INT_TYPE> *source,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  result += poly1 */
    static void AddTo(IntPolynomial<INT_TYPE> *result,
        const IntPolynomial<INT_TYPE> *poly1,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /**  result = (X^ai-1) * source */
    static void MulByXaiMinusOne(IntPolynomial<INT_TYPE> *result,
        const INT_TYPE ai,
        const IntPolynomial<INT_TYPE> *source,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /** Euclidean norm of an Integer Polynomial */
    static double Norm2sq(const IntPolynomial<INT_TYPE> *poly,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);

    /** Infinity norm of the distance between two integer polynomials */
    static double NormInftyDist(const IntPolynomial<INT_TYPE> *poly1,
        const IntPolynomial<INT_TYPE> *poly2,
        const PolynomialParameters *params,
        TfheThreadContext *context,
        Allocator alloc);
};

#endif //POLYNOMIALS_ARITHMETIC_H
