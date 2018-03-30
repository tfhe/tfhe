#ifndef POLYNOMIALS_ARITHMETIC_H
#define POLYNOMIALS_ARITHMETIC_H

#include "tfhe_core.h"
#include "polynomials.h"

///@file
///@brief This file contains the operations on polynomials

template<typename TORUS>
struct TorusPolyFunctions {
private:
    static void
    MultNaive_plain_aux(TORUS *__restrict result,
                        const int *__restrict poly1,
                        const TORUS *__restrict poly2,
                        const int N,
                        TfheThreadContext *context,
                        Allocator alloc);

    static void
    MultNaive_aux(TORUS *__restrict result,
                  const int *__restrict poly1,
                  const TORUS *__restrict poly2,
                  const int N,
                  TfheThreadContext *context,
                  Allocator alloc);

    static void
    Karatsuba_aux(TORUS *R,
                  const int *A,
                  const TORUS *B,
                  const int size,
                  const char *buf,
                  TfheThreadContext *context,
                  Allocator alloc);

public:

    /**  TorusPolynomial = 0 */
    static void Clear(TorusPolynomial<TORUS> *result,
                      const PolynomialParameters *params,
                      TfheThreadContext *context,
                      Allocator alloc);

    /**  TorusPolynomial = random */
    static void Uniform(TorusPolynomial<TORUS> *result,
                        const PolynomialParameters *params,
                        TfheThreadContext *context,
                        Allocator alloc);

    /**  TorusPolynomial = TorusPolynomial */
    static void Copy(TorusPolynomial<TORUS> *result,
                     const TorusPolynomial<TORUS> *sample,
                     const PolynomialParameters *params,
                     TfheThreadContext *context,
                     Allocator alloc);

    /**  TorusPolynomial + TorusPolynomial */
    static void
    Add(TorusPolynomial<TORUS> *result,
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
    static void
    Sub(TorusPolynomial<TORUS> *result,
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
                        int p,
                        const TorusPolynomial<TORUS> *poly2,
                        const PolynomialParameters *params,
                        TfheThreadContext *context,
                        Allocator alloc);

    /**  TorusPolynomial += p*TorusPolynomial */
    static void AddMulZTo(TorusPolynomial<TORUS> *result,
                          const int p,
                          const TorusPolynomial<TORUS> *poly2,
                          const PolynomialParameters *params,
                          TfheThreadContext *context,
                          Allocator alloc);

    /**  TorusPolynomial - p*TorusPolynomial */
    static void SubMulZ(TorusPolynomial<TORUS> *result,
                        const TorusPolynomial<TORUS> *poly1,
                        const int p,
                        const TorusPolynomial<TORUS> *poly2,
                        const PolynomialParameters *params,
                        TfheThreadContext *context,
                        Allocator alloc);

    /**  TorusPolynomial -= p*TorusPolynomial */
    static void SubMulZTo(TorusPolynomial<TORUS> *result,
                          int p,
                          const TorusPolynomial<TORUS> *poly2,
                          const PolynomialParameters *params,
                          TfheThreadContext *context,
                          Allocator alloc);

    /**  TorusPolynomial = (X^a-1)* TorusPolynomial */
    static void MulByXaiMinusOne(TorusPolynomial<TORUS> *result,
                                 int a,
                                 const TorusPolynomial<TORUS> *source,
                                 const PolynomialParameters *params,
                                 TfheThreadContext *context,
                                 Allocator alloc);

    /** result= X^{a}*source */
    static void MulByXai(TorusPolynomial<TORUS> *result,
                         int a,
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
    static void
    MultNaive(TorusPolynomial<TORUS> *result,
              const IntPolynomial *poly1,
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
    static void
    MultKaratsuba(TorusPolynomial<TORUS> *result,
                  const IntPolynomial *poly1,
                  const TorusPolynomial<TORUS> *poly2,
                  const PolynomialParameters *params,
                  TfheThreadContext *context,
                  Allocator alloc);

    /**
     * result += poly1 * poly2 (via Karatsuba)
     * WARNING: N must be a power of 2 to use this function. Else, the
     * behaviour is unpredictable
     */
    static void
    AddMulRKaratsuba(TorusPolynomial<TORUS> *result,
                     const IntPolynomial *poly1,
                     const TorusPolynomial<TORUS> *poly2,
                     const PolynomialParameters *params,
                     TfheThreadContext *context,
                     Allocator alloc);

    /**
     * result -= poly1 * poly2 (via karatsuba)
     * WARNING: N must be a power of 2 to use this function. Else, the
     * behaviour is unpredictable
     */
    static void
    SubMulRKaratsuba(TorusPolynomial<TORUS> *result,
                     const IntPolynomial *poly1,
                     const TorusPolynomial<TORUS> *poly2,
                     const PolynomialParameters *params,
                     TfheThreadContext *context,
                     Allocator alloc);

};

template
struct TorusPolyFunctions<Torus32>;
template
struct TorusPolyFunctions<Torus64>;






struct IntPolyFunctions {
    /**  IntPolynomial = 0 */
    static void Clear(IntPolynomial *result,
                      const PolynomialParameters *params,
                      TfheThreadContext *context,
                      Allocator alloc);

    /**  result = source */
    static void Copy(IntPolynomial *result,
                     const IntPolynomial *source,
                     const PolynomialParameters *params,
                     TfheThreadContext *context,
                     Allocator alloc);

    /**  result += poly1 */
    static void AddTo(IntPolynomial *result,
                      const IntPolynomial *poly1,
                      const PolynomialParameters *params,
                      TfheThreadContext *context,
                      Allocator alloc);

    /**  result = (X^ai-1) * source */
    static void MulByXaiMinusOne(IntPolynomial *result,
                                 int ai,
                                 const IntPolynomial *source,
                                 const PolynomialParameters *params,
                                 TfheThreadContext *context,
                                 Allocator alloc);

    /** Euclidean norm of an Integer Polynomial */
    static double Norm2sq(const IntPolynomial *poly,
                          const PolynomialParameters *params,
                          TfheThreadContext *context,
                          Allocator alloc);

    /** Infinity norm of the distance between two integer polynomials */
    static double NormInftyDist(const IntPolynomial *poly1,
                                const IntPolynomial *poly2,
                                const PolynomialParameters *params,
                                TfheThreadContext *context,
                                Allocator alloc);
};











/**  TorusPolynomial = 0 */
template<typename TORUS>
inline void torusPolynomialClear(TorusPolynomial<TORUS> *result,
                                 const PolynomialParameters *params,
                                 TfheThreadContext *context,
                                 Allocator alloc) {
    TorusPolyFunctions<TORUS>::Clear(result, params, context, alloc);
}

/**  TorusPolynomial = random */
template<typename TORUS>
inline void torusPolynomialUniform(TorusPolynomial<TORUS> *result,
                                   const PolynomialParameters *params,
                                   TfheThreadContext *context,
                                   Allocator alloc) {
    TorusPolyFunctions<TORUS>::Uniform(result, params, context, alloc);
}

/**  TorusPolynomial = TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialCopy(TorusPolynomial<TORUS> *result,
                                const TorusPolynomial<TORUS> *sample,
                                const PolynomialParameters *params,
                                TfheThreadContext *context,
                                Allocator alloc) {
    TorusPolyFunctions<TORUS>::Copy(result, sample, params, context, alloc);
}

/**  TorusPolynomial + TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialAdd(TorusPolynomial<TORUS> *result,
                               const TorusPolynomial<TORUS> *poly1,
                               const TorusPolynomial<TORUS> *poly2,
                               const PolynomialParameters *params,
                               TfheThreadContext *context,
                               Allocator alloc) {
    TorusPolyFunctions<TORUS>::Add(result, poly1, poly2, params, context, alloc);
}

/**  TorusPolynomial += TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialAddTo(TorusPolynomial<TORUS> *result,
                                 const TorusPolynomial<TORUS> *poly2,
                                 const PolynomialParameters *params,
                                 TfheThreadContext *context,
                                 Allocator alloc) {
    TorusPolyFunctions<TORUS>::AddTo(result, poly2, params, context, alloc);
}

/**  TorusPolynomial - TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialSub(TorusPolynomial<TORUS> *result,
                               const TorusPolynomial<TORUS> *poly1,
                               const TorusPolynomial<TORUS> *poly2,
                               const PolynomialParameters *params,
                               TfheThreadContext *context,
                               Allocator alloc) {
    TorusPolyFunctions<TORUS>::Sub(result, poly1, poly2, params, context, alloc);
}

/**  TorusPolynomial -= TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialSubTo(TorusPolynomial<TORUS> *result,
                                 const TorusPolynomial<TORUS> *poly2,
                                 const PolynomialParameters *params,
                                 TfheThreadContext *context,
                                 Allocator alloc) {
    TorusPolyFunctions<TORUS>::SubTo(result, poly2, params, context, alloc);
}

/**  TorusPolynomial + p*TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialAddMulZ(TorusPolynomial<TORUS> *result,
                                   const TorusPolynomial<TORUS> *poly1,
                                   int p,
                                   const TorusPolynomial<TORUS> *poly2,
                                   const PolynomialParameters *params,
                                   TfheThreadContext *context,
                                   Allocator alloc) {
    TorusPolyFunctions<TORUS>::AddMulZ(result, poly1, p, poly2, params, context, alloc);
}

/**  TorusPolynomial += p*TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialAddMulZTo(TorusPolynomial<TORUS> *result,
                                     const int p,
                                     const TorusPolynomial<TORUS> *poly2,
                                     const PolynomialParameters *params,
                                     TfheThreadContext *context,
                                     Allocator alloc) {
    TorusPolyFunctions<TORUS>::AddMulZTo(result, p, poly2, params, context, alloc);
}

/**  TorusPolynomial - p*TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialSubMulZ(TorusPolynomial<TORUS> *result,
                                   const TorusPolynomial<TORUS> *poly1,
                                   const int p,
                                   const TorusPolynomial<TORUS> *poly2,
                                   const PolynomialParameters *params,
                                   TfheThreadContext *context,
                                   Allocator alloc) {
    TorusPolyFunctions<TORUS>::SubMulZ(result, poly1, p, poly2, params, context, alloc);
}

/**  TorusPolynomial -= p*TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialSubMulZTo(TorusPolynomial<TORUS> *result,
                                     int p,
                                     const TorusPolynomial<TORUS> *poly2,
                                     const PolynomialParameters *params,
                                     TfheThreadContext *context,
                                     Allocator alloc) {
    TorusPolyFunctions<TORUS>::SubMulZTo(result, p, poly2, params, context, alloc);
}

/**  TorusPolynomial = (X^a-1)* TorusPolynomial */
template<typename TORUS>
inline void
torusPolynomialMulByXaiMinusOne(TorusPolynomial<TORUS> *result,
                                int a,
                                const TorusPolynomial<TORUS> *source,
                                const PolynomialParameters *params,
                                TfheThreadContext *context,
                                Allocator alloc) {
    TorusPolyFunctions<TORUS>::MulByXaiMinusOne(result, a, source, params, context, alloc);
}

/** result= X^{a}*source */
template<typename TORUS>
inline void torusPolynomialMulByXai(TorusPolynomial<TORUS> *result,
                                    int a,
                                    const TorusPolynomial<TORUS> *source,
                                    const PolynomialParameters *params,
                                    TfheThreadContext *context,
                                    Allocator alloc) {
    TorusPolyFunctions<TORUS>::MulByXai(result, a, source, params, context, alloc);
}

/**  Euclidean norm of an Integer Polynomial */
inline double intPolynomialNormSq2(const IntPolynomial *poly,
                                   const PolynomialParameters *params,
                                   TfheThreadContext *context,
                                   Allocator alloc) {
    return IntPolyFunctions::Norm2sq(poly, params, context, alloc);
}

/**  IntPolynomial = 0 */
inline void intPolynomialClear(IntPolynomial *result,
                               const PolynomialParameters *params,
                               TfheThreadContext *context,
                               Allocator alloc) {
    IntPolyFunctions::Clear(result, params, context, alloc);
}

/**  result = source */
inline void intPolynomialCopy(IntPolynomial *result,
                              const IntPolynomial *source,
                              const PolynomialParameters *params,
                              TfheThreadContext *context,
                              Allocator alloc) {
    IntPolyFunctions::Copy(result, source, params, context, alloc);
}

/**  result += poly1 */
inline void intPolynomialAddTo(IntPolynomial *result,
                               const IntPolynomial *poly1,
                               const PolynomialParameters *params,
                               TfheThreadContext *context,
                               Allocator alloc) {
    IntPolyFunctions::AddTo(result, poly1, params, context, alloc);
}

/**  result = (X^ai-1) * source */
inline void intPolynomialMulByXaiMinusOne(IntPolynomial *result,
                                          int ai,
                                          const IntPolynomial *source,
                                          const PolynomialParameters *params,
                                          TfheThreadContext *context,
                                          Allocator alloc) {
    IntPolyFunctions::MulByXaiMinusOne(result, ai, source, params, context, alloc);
}

/**  Infinity norm of the distance between two torus polynomials */
template<typename TORUS>
inline double torusPolynomialNormInftyDist(const TorusPolynomial<TORUS> *poly1,
                                           const TorusPolynomial<TORUS> *poly2,
                                           const PolynomialParameters *params,
                                           TfheThreadContext *context,
                                           Allocator alloc) {
    return TorusPolyFunctions<TORUS>::NormInftyDist(poly1, poly2, params, context, alloc);
}

/** Euclidean norm of an integer polynomial */
inline double intPolynomialNorm2sq(const IntPolynomial *poly,
                                   const PolynomialParameters *params,
                                   TfheThreadContext *context,
                                   Allocator alloc) {
    return IntPolyFunctions::Norm2sq(poly, params, context, alloc);
}

/** Infinity norm of the distance between two integer polynomials */
inline double intPolynomialNormInftyDist(const IntPolynomial *poly1,
                                         const IntPolynomial *poly2,
                                         const PolynomialParameters *params,
                                         TfheThreadContext *context,
                                         Allocator alloc) {
    return IntPolyFunctions::NormInftyDist(poly1, poly2, params, context, alloc);
}

/**
 * This is the naive external multiplication of an integer polynomial
 * with a torus polynomial. (this function should yield exactly the same
 * result as the karatsuba or fft version, but should be slower)
 */
template<typename TORUS>
inline void torusPolynomialMultNaive(TorusPolynomial<TORUS> *result,
                                     const IntPolynomial *poly1,
                                     const TorusPolynomial<TORUS> *poly2,
                                     const PolynomialParameters *params,
                                     TfheThreadContext *context,
                                     Allocator alloc) {
    TorusPolyFunctions<TORUS>::MultNaive(result, poly1, poly2, params, context, alloc);
}

/**
 * This function multiplies 2 polynomials (an integer poly and a torus poly)
 * by using Karatsuba
 * WARNING: N must be a power of 2 to use this function. Else, the
 * behaviour is unpredictable
 */
template<typename TORUS>
inline void torusPolynomialMultKaratsuba(TorusPolynomial<TORUS> *result,
                                         const IntPolynomial *poly1,
                                         const TorusPolynomial<TORUS> *poly2,
                                         const PolynomialParameters *params,
                                         TfheThreadContext *context,
                                         Allocator alloc) {
    TorusPolyFunctions<TORUS>::MultKaratsuba(result, poly1, poly2, params, context, alloc);
}

/**
 * result += poly1 * poly2 (via karatsuba)
 * WARNING: N must be a power of 2 to use this function. Else, the
 * behaviour is unpredictable
 */
template<typename TORUS>
inline void torusPolynomialAddMulRKaratsuba(TorusPolynomial<TORUS> *result,
                                            const IntPolynomial *poly1,
                                            const TorusPolynomial<TORUS> *poly2,
                                            const PolynomialParameters *params,
                                            TfheThreadContext *context,
                                            Allocator alloc) {
    TorusPolyFunctions<TORUS>::AddMulRKaratsuba(result, poly1, poly2, params, context, alloc);
}

/**
 * result -= poly1 * poly2 (via karatsuba)
 * WARNING: N must be a power of 2 to use this function. Else, the
 * behaviour is unpredictable
 */
template<typename TORUS>
inline void torusPolynomialSubMulRKaratsuba(TorusPolynomial<TORUS> *result,
                                            const IntPolynomial *poly1,
                                            const TorusPolynomial<TORUS> *poly2,
                                            const PolynomialParameters *params,
                                            TfheThreadContext *context,
                                            Allocator alloc) {
    TorusPolyFunctions<TORUS>::SubMulRKaratsuba(result, poly1, poly2, params, context, alloc);
}



#endif //POLYNOMIALS_ARITHMETIC_H
