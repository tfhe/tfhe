#ifndef POLYNOMIALS_ARITHMETIC_H
#define POLYNOMIALS_ARITHMETIC_H

#include "tfhe_core.h"
#include "polynomials.h"

///@file
///@brief This file contains the operations on polynomials

template<typename TORUS>
struct TorusPolyFunctions
{
private:
  static void MultNaive_plain_aux(TORUS* __restrict result, const int* __restrict poly1, const TORUS* __restrict poly2, const int N);
  static void MultNaive_aux(TORUS* __restrict result, const int* __restrict poly1, const TORUS* __restrict poly2, const int N);
  static void Karatsuba_aux(TORUS* R, const int* A, const TORUS* B, const int size, const char* buf);
public:

  /**  TorusPolynomial = 0 */
  static void Clear(TorusPolynomial<TORUS>* result);

  /**  TorusPolynomial = random */
  static void Uniform(TorusPolynomial<TORUS>* result);

  /**  TorusPolynomial = TorusPolynomial */
  static void Copy(TorusPolynomial<TORUS>* result, const TorusPolynomial<TORUS>*  sample);

  /**  TorusPolynomial + TorusPolynomial */
  static void Add(TorusPolynomial<TORUS>* result, const TorusPolynomial<TORUS>* poly1, const TorusPolynomial<TORUS>* poly2);

  /**  TorusPolynomial += TorusPolynomial */
  static void AddTo(TorusPolynomial<TORUS>* result, const TorusPolynomial<TORUS>* poly2);

  /**  TorusPolynomial - TorusPolynomial */
  static void Sub(TorusPolynomial<TORUS>* result, const TorusPolynomial<TORUS>* poly1, const TorusPolynomial<TORUS>* poly2);

  /**  TorusPolynomial -= TorusPolynomial */
  static void SubTo(TorusPolynomial<TORUS>* result, const TorusPolynomial<TORUS>* poly2);

  /**  TorusPolynomial + p*TorusPolynomial */
  static void AddMulZ(TorusPolynomial<TORUS>* result, const TorusPolynomial<TORUS>* poly1, int p, const TorusPolynomial<TORUS>* poly2);

  /**  TorusPolynomial += p*TorusPolynomial */
  static void AddMulZTo(TorusPolynomial<TORUS>* result, const int p, const TorusPolynomial<TORUS>* poly2);

  /**  TorusPolynomial - p*TorusPolynomial */
  static void SubMulZ(TorusPolynomial<TORUS>* result, const TorusPolynomial<TORUS>* poly1, const int p, const TorusPolynomial<TORUS>* poly2);

  /**  TorusPolynomial -= p*TorusPolynomial */
  static void SubMulZTo(TorusPolynomial<TORUS>* result, int p, const TorusPolynomial<TORUS>* poly2);

  /**  TorusPolynomial = (X^a-1)* TorusPolynomial */
  static void MulByXaiMinusOne(TorusPolynomial<TORUS>* result, int a, const TorusPolynomial<TORUS>* source);

  /** result= X^{a}*source */
  static void MulByXai(TorusPolynomial<TORUS>* result, int a, const TorusPolynomial<TORUS>* source);

  /**  ILA: Norme infini de la distance entre deux TorusPolynomial */
  static double NormInftyDist(const TorusPolynomial<TORUS>* poly1, const TorusPolynomial<TORUS>* poly2);

  /**
   * This is the naive external multiplication of an integer polynomial
   * with a torus polynomial. (this function should yield exactly the same
   * result as the karatsuba or fft version, but should be slower)
   */
  static void MultNaive(TorusPolynomial<TORUS>* result, const IntPolynomial* poly1, const TorusPolynomial<TORUS>* poly2);

  /**
   * This function multiplies 2 polynomials (an integer poly and a torus poly)
   * by using Karatsuba
   * WARNING: N must be a power of 2 to use this function. Else, the
   * behaviour is unpredictable
   */
  static void MultKaratsuba(TorusPolynomial<TORUS>* result, const IntPolynomial* poly1, const TorusPolynomial<TORUS>* poly2);

  /**
   * result += poly1 * poly2 (via karatsuba)
   * WARNING: N must be a power of 2 to use this function. Else, the
   * behaviour is unpredictable
   */
  static void AddMulRKaratsuba(TorusPolynomial<TORUS>* result, const IntPolynomial* poly1, const TorusPolynomial<TORUS>* poly2);

  /**
   * result -= poly1 * poly2 (via karatsuba)
   * WARNING: N must be a power of 2 to use this function. Else, the
   * behaviour is unpredictable
   */
  static void SubMulRKaratsuba(TorusPolynomial<TORUS>* result, const IntPolynomial* poly1, const TorusPolynomial<TORUS>* poly2);

  static void MultFFT(TorusPolynomial<TORUS>* result, const IntPolynomial* poly1, const TorusPolynomial<TORUS>* poly2);

  static void AddMulRFFT(TorusPolynomial<TORUS>* result, const IntPolynomial* poly1, const TorusPolynomial<TORUS>* poly2);

  static void SubMulRFFT(TorusPolynomial<TORUS>* result, const IntPolynomial* poly1, const TorusPolynomial<TORUS>* poly2);
};

template struct TorusPolyFunctions<Torus32>;
template struct TorusPolyFunctions<Torus64>;

struct IntPolyFunctions
{
  /**  Norme Euclidienne d'un IntPolynomial */
  static double NormSq2(const IntPolynomial* poly);

  /**  IntPolynomial = 0 */
  static void Clear(IntPolynomial* result);

  /**  result = source */
  static void Copy(IntPolynomial* result, const IntPolynomial* source);

  /**  result += poly1 */
  static void AddTo(IntPolynomial* result, const IntPolynomial* poly1);

  /**  result = (X^ai-1) * source */
  static void MulByXaiMinusOne(IntPolynomial* result, int ai, const IntPolynomial* source);

  // Norme 2 d'un IntPolynomial
  static double Norm2sq(const IntPolynomial* poly);

  // Norme infini de la distance entre deux IntPolynomial
  static double NormInftyDist(const IntPolynomial* poly1, const IntPolynomial* poly2);
};


/**  TorusPolynomial = 0 */
template<typename TORUS>
inline void torusPolynomialClear(TorusPolynomial<TORUS>* result) {
  TorusPolyFunctions<TORUS>::Clear(result);
}

/**  TorusPolynomial = random */
template<typename TORUS>
inline void torusPolynomialUniform(TorusPolynomial<TORUS>* result) {
  TorusPolyFunctions<TORUS>::Uniform(result);
}

/**  TorusPolynomial = TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialCopy(TorusPolynomial<TORUS>* result, const TorusPolynomial<TORUS>*  sample) {
  TorusPolyFunctions<TORUS>::Copy(result, sample);
}

/**  TorusPolynomial + TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialAdd(TorusPolynomial<TORUS>* result, const TorusPolynomial<TORUS>* poly1, const TorusPolynomial<TORUS>* poly2) {
  TorusPolyFunctions<TORUS>::Add(result, poly1, poly2);
}

/**  TorusPolynomial += TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialAddTo(TorusPolynomial<TORUS>* result, const TorusPolynomial<TORUS>* poly2) {
  TorusPolyFunctions<TORUS>::AddTo(result, poly2);
}

/**  TorusPolynomial - TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialSub(TorusPolynomial<TORUS>* result, const TorusPolynomial<TORUS>* poly1, const TorusPolynomial<TORUS>* poly2) {
  TorusPolyFunctions<TORUS>::Sub(result, poly1, poly2);
}

/**  TorusPolynomial -= TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialSubTo(TorusPolynomial<TORUS>* result, const TorusPolynomial<TORUS>* poly2) {
  TorusPolyFunctions<TORUS>::SubTo(result, poly2);
}

/**  TorusPolynomial + p*TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialAddMulZ(TorusPolynomial<TORUS>* result, const TorusPolynomial<TORUS>* poly1, int p, const TorusPolynomial<TORUS>* poly2) {
  TorusPolyFunctions<TORUS>::AddMulZ(result, poly1, p, poly2);
}

/**  TorusPolynomial += p*TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialAddMulZTo(TorusPolynomial<TORUS>* result, const int p, const TorusPolynomial<TORUS>* poly2) {
  TorusPolyFunctions<TORUS>::AddMulZTo(result, p, poly2);
}

/**  TorusPolynomial - p*TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialSubMulZ(TorusPolynomial<TORUS>* result, const TorusPolynomial<TORUS>* poly1, const int p, const TorusPolynomial<TORUS>* poly2) {
  TorusPolyFunctions<TORUS>::SubMulZ(result, poly1, p, poly2);
}

/**  TorusPolynomial -= p*TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialSubMulZTo(TorusPolynomial<TORUS>* result, int p, const TorusPolynomial<TORUS>* poly2) {
  TorusPolyFunctions<TORUS>::SubMulZTo(result, p, poly2);
}

/**  TorusPolynomial = (X^a-1)* TorusPolynomial */
template<typename TORUS>
inline void torusPolynomialMulByXaiMinusOne(TorusPolynomial<TORUS>* result, int a, const TorusPolynomial<TORUS>* source) {
  TorusPolyFunctions<TORUS>::MulByXaiMinusOne(result, a, source);
}

/** result= X^{a}*source */
template<typename TORUS>
inline void torusPolynomialMulByXai(TorusPolynomial<TORUS>* result, int a, const TorusPolynomial<TORUS>* source) {
  TorusPolyFunctions<TORUS>::MulByXai(result, a, source);
}

/**  Norme Euclidienne d'un IntPolynomial */
inline double intPolynomialNormSq2(const IntPolynomial* poly) {
  return IntPolyFunctions::NormSq2(poly);
}

/**  IntPolynomial = 0 */
inline void intPolynomialClear(IntPolynomial* result) {
  IntPolyFunctions::Clear(result);
}

/**  result = source */
inline void intPolynomialCopy(IntPolynomial* result, const IntPolynomial* source) {
  IntPolyFunctions::Copy(result, source);
}

/**  result += poly1 */
inline void intPolynomialAddTo(IntPolynomial* result, const IntPolynomial* poly1) {
  IntPolyFunctions::AddTo(result, poly1);
}

/**  result = (X^ai-1) * source */
inline void intPolynomialMulByXaiMinusOne(IntPolynomial* result, int ai, const IntPolynomial* source) {
  IntPolyFunctions::MulByXaiMinusOne(result, ai, source);
}

/**  ILA: Norme infini de la distance entre deux TorusPolynomial */
template<typename TORUS>
inline double torusPolynomialNormInftyDist(const TorusPolynomial<TORUS>* poly1, const TorusPolynomial<TORUS>* poly2) {
  return TorusPolyFunctions<TORUS>::NormInftyDist(poly1, poly2);
}

// Norme 2 d'un IntPolynomial
inline double intPolynomialNorm2sq(const IntPolynomial* poly) {
  return IntPolyFunctions::Norm2sq(poly);
}

// Norme infini de la distance entre deux IntPolynomial
inline double intPolynomialNormInftyDist(const IntPolynomial* poly1, const IntPolynomial* poly2) {
  return IntPolyFunctions::NormInftyDist(poly1, poly2);
}

/**
 * This is the naive external multiplication of an integer polynomial
 * with a torus polynomial. (this function should yield exactly the same
 * result as the karatsuba or fft version, but should be slower)
 */
template<typename TORUS>
inline void torusPolynomialMultNaive(TorusPolynomial<TORUS>* result, const IntPolynomial* poly1, const TorusPolynomial<TORUS>* poly2) {
  TorusPolyFunctions<TORUS>::MultNaive(result, poly1, poly2);
}

/**
 * This function multiplies 2 polynomials (an integer poly and a torus poly)
 * by using Karatsuba
 * WARNING: N must be a power of 2 to use this function. Else, the
 * behaviour is unpredictable
 */
template<typename TORUS>
inline void torusPolynomialMultKaratsuba(TorusPolynomial<TORUS>* result, const IntPolynomial* poly1, const TorusPolynomial<TORUS>* poly2) {
  TorusPolyFunctions<TORUS>::MultKaratsuba(result, poly1, poly2);
}

/**
 * result += poly1 * poly2 (via karatsuba)
 * WARNING: N must be a power of 2 to use this function. Else, the
 * behaviour is unpredictable
 */
template<typename TORUS>
inline void torusPolynomialAddMulRKaratsuba(TorusPolynomial<TORUS>* result, const IntPolynomial* poly1, const TorusPolynomial<TORUS>* poly2) {
  TorusPolyFunctions<TORUS>::AddMulRKaratsuba(result, poly1, poly2);
}

/**
 * result -= poly1 * poly2 (via karatsuba)
 * WARNING: N must be a power of 2 to use this function. Else, the
 * behaviour is unpredictable
 */
template<typename TORUS>
inline void torusPolynomialSubMulRKaratsuba(TorusPolynomial<TORUS>* result, const IntPolynomial* poly1, const TorusPolynomial<TORUS>* poly2) {
  TorusPolyFunctions<TORUS>::SubMulRKaratsuba(result, poly1, poly2);
}


template<typename TORUS>
inline void torusPolynomialMultFFT(TorusPolynomial<TORUS>* result, const IntPolynomial* poly1, const TorusPolynomial<TORUS>* poly2) {
  TorusPolyFunctions<TORUS>::MultFFT(result, poly1, poly2);
}

template<typename TORUS>
inline void torusPolynomialAddMulRFFT(TorusPolynomial<TORUS>* result, const IntPolynomial* poly1, const TorusPolynomial<TORUS>* poly2) {
  TorusPolyFunctions<TORUS>::AddMulRFFT(result, poly1, poly2);
}

template<typename TORUS>
inline void torusPolynomialSubMulRFFT(TorusPolynomial<TORUS>* result, const IntPolynomial* poly1, const TorusPolynomial<TORUS>* poly2) {
  TorusPolyFunctions<TORUS>::SubMulRFFT(result, poly1, poly2);
}



//#define torusPolynomialMul torusPolynomialMultKaratsuba
//#define torusPolynomialAddMulR torusPolynomialAddMulRKaratsuba
//#define torusPolynomialSubMulR torusPolynomialSubMulRKaratsuba

#define torusPolynomialMul torusPolynomialMultFFT
#define torusPolynomialAddMulR torusPolynomialAddMulRFFT
#define torusPolynomialSubMulR torusPolynomialSubMulRFFT


#endif //POLYNOMIALS_ARITHMETIC_H
