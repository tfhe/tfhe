#include "polynomial_torus_big.h"

template<>
TorusPolynomial<BigTorus>::TorusPolynomial(
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    // coefs = alloc->newArray<BigTorus>(params->N, params->zmodule_params, alloc);
}

template<>
void TorusPolynomial<BigTorus>::destroy(
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator *alloc)
{
    // alloc->deleteArray<BigTorus>(params->N, coefs, params->zmodule_params, alloc);
}


// TorusPolynomial = 0
template<>
void TorusPolynomial<BigTorus>::Clear(
    TorusPolynomial<BigTorus> *result,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;

    // for (int32_t i = 0; i < N; ++i)
    //     result->coefs[i] = 0;
}

// TorusPolynomial = random
template<>
void TorusPolynomial<BigTorus>::Uniform(
    TorusPolynomial<BigTorus> *result,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // BigTorus *x = result->coefs;

    // for (int32_t i = 0; i < N; ++i)
    //     x[i] = RandomGenTorus<BigTorus>::uniform();
}

// TorusPolynomial = TorusPolynomial
template<>
void TorusPolynomial<BigTorus>::Copy(
    TorusPolynomial<BigTorus> *result,
    const TorusPolynomial<BigTorus> *sample,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // assert(result != sample);
    // const int32_t N = params->N;
    // const BigTorus *s = sample->coefs;
    // BigTorus *r = result->coefs;

    // for (int32_t i = 0; i < N; ++i)
    //     r[i] = s[i];
}

// TorusPolynomial + TorusPolynomial
template<>
void TorusPolynomial<BigTorus>::Add(
    TorusPolynomial<BigTorus> *result,
    const TorusPolynomial<BigTorus> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // assert(result != poly1); //if it fails here, please use addTo
    // assert(result != poly2); //if it fails here, please use addTo
    // BigTorus *r = result->coefs;
    // const BigTorus *a = poly1->coefs;
    // const BigTorus *b = poly2->coefs;

    // for (int32_t i = 0; i < N; ++i)
    //     r[i] = a[i] + b[i];
}

// TorusPolynomial += TorusPolynomial
template<>
void TorusPolynomial<BigTorus>::AddTo(
    TorusPolynomial<BigTorus> *result,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // BigTorus *r = result->coefs;
    // const BigTorus *b = poly2->coefs;

    // for (int32_t i = 0; i < N; ++i)
    //     r[i] += b[i];
}


// TorusPolynomial - TorusPolynomial
template<>
void TorusPolynomial<BigTorus>::Sub(
    TorusPolynomial<BigTorus> *result,
    const TorusPolynomial<BigTorus> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // assert(result != poly1); //if it fails here, please use subTo
    // assert(result != poly2); //if it fails here, please use subTo
    // BigTorus *r = result->coefs;
    // const BigTorus *a = poly1->coefs;
    // const BigTorus *b = poly2->coefs;

    // for (int32_t i = 0; i < N; ++i)
    //     r[i] = a[i] - b[i];
}

// TorusPolynomial -= TorusPolynomial
template<>
void TorusPolynomial<BigTorus>::SubTo(
    TorusPolynomial<BigTorus> *result,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // BigTorus *r = result->coefs;
    // const BigTorus *b = poly2->coefs;

    // for (int32_t i = 0; i < N; ++i)
    //     r[i] -= b[i];
}

// TorusPolynomial + p*TorusPolynomial
template<>
void TorusPolynomial<BigTorus>::AddMulZ(
    TorusPolynomial<BigTorus> *result,
    const TorusPolynomial<BigTorus> *poly1,
    const INT_TYPE p,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // assert(result != poly1); //if it fails here, please use AddMulZTo
    // BigTorus *r = result->coefs;
    // const BigTorus *a = poly1->coefs;
    // const BigTorus *b = poly2->coefs;

    // for (int32_t i = 0; i < N; ++i)
    //     r[i] = a[i] + p * b[i];
}

// TorusPolynomial += p*TorusPolynomial
template<>
void TorusPolynomial<BigTorus>::AddMulZTo(
    TorusPolynomial<BigTorus> *result,
    const INT_TYPE p,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // BigTorus *r = result->coefs;
    // const BigTorus *b = poly2->coefs;

    // for (int32_t i = 0; i < N; ++i)
    //     r[i] += p * b[i];
}

// TorusPolynomial - p*TorusPolynomial
template<>
void TorusPolynomial<BigTorus>::SubMulZ(
    TorusPolynomial<BigTorus> *result,
    const TorusPolynomial<BigTorus> *poly1,
    const INT_TYPE p,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // assert(result != poly1); //if it fails here, please use SubMulZTo
    // BigTorus *r = result->coefs;
    // const BigTorus *a = poly1->coefs;
    // const BigTorus *b = poly2->coefs;

    // for (int32_t i = 0; i < N; ++i)
    //     r[i] = a[i] - p * b[i];
}

// TorusPolynomial -= p*TorusPolynomial
template<>
void TorusPolynomial<BigTorus>::SubMulZTo(
    TorusPolynomial<BigTorus> *result,
    const INT_TYPE p,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // BigTorus *r = result->coefs;
    // const BigTorus *b = poly2->coefs;

    // for (int32_t i = 0; i < N; ++i)
    //     r[i] -= p * b[i];
}

//result = (X^{a}-1)*source
template<>
void TorusPolynomial<BigTorus>::MulByXaiMinusOne(
    TorusPolynomial<BigTorus> *result,
    const INT_TYPE a,
    const TorusPolynomial<BigTorus> *source,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // BigTorus *out = result->coefs;
    // BigTorus *in = source->coefs;

    // assert(a >= 0 && a < 2 * N);

    // if (a < N) {
    //     for (int32_t i = 0; i < a; i++) // i-a < 0
    //         out[i] = -in[i - a + N] - in[i];
    //     for (int32_t i = a; i < N; i++) // N > i-a >= 0
    //         out[i] = in[i - a] - in[i];
    // } else {
    //     const int32_t aa = a - N;
    //     for (int32_t i = 0; i < aa; i++)//sur que i-a<0
    //         out[i] = in[i - aa + N] - in[i];
    //     for (int32_t i = aa; i < N; i++)//sur que N>i-a>=0
    //         out[i] = -in[i - aa] - in[i];
    // }
}


//result= X^{a}*source
template<>
void TorusPolynomial<BigTorus>::MulByXai(
    TorusPolynomial<BigTorus> *result,
    const INT_TYPE a,
    const TorusPolynomial<BigTorus> *source,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // BigTorus *out = result->coefs;
    // BigTorus *in = source->coefs;

    // assert(a >= 0 && a < 2 * N);
    // assert(result != source);

    // if (a < N) {
    //     for (int32_t i = 0; i < a; i++)//sur que i-a<0
    //         out[i] = -in[i - a + N];
    //     for (int32_t i = a; i < N; i++)//sur que N>i-a>=0
    //         out[i] = in[i - a];
    // } else {
    //     const int32_t aa = a - N;
    //     for (int32_t i = 0; i < aa; i++)//sur que i-a<0
    //         out[i] = in[i - aa + N];
    //     for (int32_t i = aa; i < N; i++)//sur que N>i-a>=0
    //         out[i] = -in[i - aa];
    // }
}


// Infinity norm of the distance between two TorusPolynomial
template<>
double TorusPolynomial<BigTorus>::NormInftyDist(
    const TorusPolynomial<BigTorus> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // double norm = 0;

    // // Max between the coefficients of abs(poly1-poly2)
    // for (int32_t i = 0; i < N; ++i) {
    //     double r = abs(TorusUtils<BigTorus>::to_double(poly1->coefs[i] - poly2->coefs[i], params->zmodule_params));
    //     if (r > norm) { norm = r; }
    // }
    // return norm;
    return 0;
}


template<>
void TorusPolynomial<BigTorus>::MultNaive_plain_aux(
    BigTorus *__restrict result,
    const INT_TYPE *__restrict poly1,
    const BigTorus *__restrict poly2,
    const int32_t N,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t _2Nm1 = 2 * N - 1;
    // BigTorus ri;

    // for (int32_t i = 0; i < N; i++) {
    //     ri = 0;
    //     for (int32_t j = 0; j <= i; j++) {
    //         ri += poly1[j] * poly2[i - j];
    //     }
    //     result[i] = ri;
    // }
    
    // for (int32_t i = N; i < _2Nm1; i++) {
    //     ri = 0;
    //     for (int32_t j = i - N + 1; j < N; j++) {
    //         ri += poly1[j] * poly2[i - j];
    //     }
    //     result[i] = ri;
    // }
}

template<>
void TorusPolynomial<BigTorus>::MultNaive_aux(
    BigTorus *__restrict result,
    const INT_TYPE *__restrict poly1,
    const BigTorus *__restrict poly2,
    const int32_t N,
    TfheThreadContext *context,
    Allocator alloc)
{
    // BigTorus ri;

    // for (int32_t i = 0; i < N; i++) {
    //     ri = 0;
    //     for (int32_t j = 0; j <= i; j++) {
    //         ri += poly1[j] * poly2[i - j];
    //     }
    //     for (int32_t j = i + 1; j < N; j++) {
    //         ri -= poly1[j] * poly2[N + i - j];
    //     }
    //     result[i] = ri;
    // }
}

/**
 * This is the naive external multiplication of an integer polynomial
 * with a torus polynomial. (this function should yield exactly the same
 * result as the karatsuba or fft version)
 */
template<>
void TorusPolynomial<BigTorus>::MultNaive(
    TorusPolynomial<BigTorus> *result,
    const IntPolynomial<INT_TYPE> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // assert(result != poly2);
    
    // const int32_t N = params->N;

    // TorusPolynomial<BigTorus>::MultNaive_aux(result->coefs, poly1->coefs, 
    //     poly2->coefs, N, context, alloc);
}
// 
/**
 * This function multiplies 2 polynomials (an integer poly and a torus poly) by using Karatsuba
 * The karatsuba function is torusPolynomialMultKaratsuba: it takes in input two polynomials and multiplies them
 * To do that, it uses the auxiliary function Karatsuba_aux, which is recursive ad which works with
 * the vectors containing the coefficients of the polynomials (primitive types)
 */

// A and B of size = size
// R of size = 2*size-1
template<>
void TorusPolynomial<BigTorus>::Karatsuba_aux(
    BigTorus *R,
    const INT_TYPE *A,
    const BigTorus *B,
    const int32_t size,
    const char *buf,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t h = size / 2;
    // const int32_t sm1 = size - 1;

    // //we stop the karatsuba recursion at h=4, because on my machine,
    // //it seems to be optimal
    // if (h <= 4)
    // {
    //     TorusPolynomial<BigTorus>::MultNaive_plain_aux(R, A, B, size, context, alloc);
    //     return;
    // }

    // //we split the polynomials in 2
    // INT_TYPE *Atemp = (INT_TYPE *) buf;
    // buf += h * sizeof(INT_TYPE);
    // BigTorus *Btemp = (BigTorus *) buf;
    // buf += h * sizeof(BigTorus);
    // BigTorus *Rtemp = (BigTorus *) buf;
    // buf += size * sizeof(BigTorus);
    // //Note: in the above line, I have put size instead of sm1 so that buf remains aligned on a power of 2

    // for (int32_t i = 0; i < h; ++i)
    //     Atemp[i] = A[i] + A[h + i];
    // for (int32_t i = 0; i < h; ++i)
    //     Btemp[i] = B[i] + B[h + i];

    // // Karatsuba recursivly
    // Karatsuba_aux(R, A, B, h, buf, context, alloc); // (R[0],R[2*h-2]), (A[0],A[h-1]), (B[0],B[h-1])
    // Karatsuba_aux(R + size, A + h, B + h, h, buf, context,
    //               alloc); // (R[2*h],R[4*h-2]), (A[h],A[2*h-1]), (B[h],B[2*h-1])
    // Karatsuba_aux(Rtemp, Atemp, Btemp, h, buf, context, alloc);
    // R[sm1] = 0; //this one needs to be copy manually
    // for (int32_t i = 0; i < sm1; ++i)
    //     Rtemp[i] -= R[i] + R[size + i];
    // for (int32_t i = 0; i < sm1; ++i)
    //     R[h + i] += Rtemp[i];

}

// poly1, poly2 and result are polynomials mod X^N+1
template<>
void TorusPolynomial<BigTorus>::MultKaratsuba(
    TorusPolynomial<BigTorus> *result,
    const IntPolynomial<INT_TYPE> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // BigTorus *R = new BigTorus[2 * N - 1];
    // char *buf = new char[4 * N * sizeof(BigTorus)]; //that's large enough to store every tmp variables (2*2*N*4) TODO: see if there is unused memory (before generic torus byte cnt was 16*N)

    // // Karatsuba
    // Karatsuba_aux(R, poly1->coefs, poly2->coefs, N, buf, context, alloc);

    // // reduction mod X^N+1
    // for (int32_t i = 0; i < N - 1; ++i)
    //     result->coefs[i] = R[i] - R[N + i];
    // result->coefs[N - 1] = R[N - 1];

    // delete[] buf;
    // delete[] R;
}

// poly1, poly2 and result are polynomials mod X^N+1
template<>
void TorusPolynomial<BigTorus>::AddMulRKaratsuba(
    TorusPolynomial<BigTorus> *result,
    const IntPolynomial<INT_TYPE> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // BigTorus *R = new BigTorus[2 * N - 1];
    // char *buf = new char[16 * N]; //that's large enough to store every tmp variables (2*2*N*4)

    // // Karatsuba
    // Karatsuba_aux(R, poly1->coefs, poly2->coefs, N, buf, context, alloc);

    // // reduction mod X^N+1
    // for (int32_t i = 0; i < N - 1; ++i)
    //     result->coefs[i] += R[i] - R[N + i];
    // result->coefs[N - 1] += R[N - 1];

    // delete[] R;
    // delete[] buf;
}

// poly1, poly2 and result are polynomials mod X^N+1
template<>
void TorusPolynomial<BigTorus>::SubMulRKaratsuba(
    TorusPolynomial<BigTorus> *result,
    const IntPolynomial<INT_TYPE> *poly1,
    const TorusPolynomial<BigTorus> *poly2,
    const PolynomialParameters<BigTorus> *params,
    TfheThreadContext *context,
    Allocator alloc)
{
    // const int32_t N = params->N;
    // BigTorus *R = new BigTorus[2 * N - 1];
    // char *buf = new char[16 * N]; //that's large enough to store every tmp variables (2*2*N*4)

    // // Karatsuba
    // Karatsuba_aux(R, poly1->coefs, poly2->coefs, N, buf, context, alloc);

    // // reduction mod X^N+1
    // for (int32_t i = 0; i < N - 1; ++i)
    //     result->coefs[i] -= R[i] - R[N + i];
    // result->coefs[N - 1] -= R[N - 1];

    // delete[] R;
    // delete[] buf;
}

