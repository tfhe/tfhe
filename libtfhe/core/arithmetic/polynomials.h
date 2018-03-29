#ifndef POLYNOMIALS_H
#define POLYNOMIALS_H

///@file
///@brief This file contains the declaration of polynomials structures

#include <cmath>
#include "tfhe_core.h"
#include "torus.h"
#include "../allocator/allocator.h"
#include "threadcontext.h"



/**
 * CLASSES
 */
class PolynomialParameters {
public:
    int N;
};


/**
 * Class of integer polynomials modulo X^N+1
 */
class IntPolynomial {
public:
    int *coefs;


    // constructor
    IntPolynomial(const PolynomialParameters* params,
                  TfheThreadContext &context,
                  Allocator &alloc);

    void destroy(const PolynomialParameters* params,
                 TfheThreadContext &context,
                 Allocator &alloc);

    // destructors
    IntPolynomial(const IntPolynomial &) = delete;

    IntPolynomial *operator=(const IntPolynomial &) = delete;

    ~IntPolynomial() = delete;
};


/**
 * Class of torus polynomials modulo X^N+1
 */
template<typename TORUS>
class TorusPolynomial {
public:
    TORUS *coefsT;


    // constructor
    TorusPolynomial(const PolynomialParameters* params,
                  TfheThreadContext &context,
                  Allocator &alloc);

    void destroy(const PolynomialParameters* params,
                 TfheThreadContext &context,
                 Allocator &alloc);


    // destructors
    TorusPolynomial(const TorusPolynomial<TORUS> &) = delete;

    TorusPolynomial<TORUS> *operator=(const TorusPolynomial<TORUS> &) = delete;

    ~TorusPolynomial() = delete;
};

template
struct TorusPolynomial<Torus32>;
template
struct TorusPolynomial<Torus64>;






/**
 * MEMORY ALLOC - integer polynomials
 */

/**
 * Allocates an integer polynomial
 */
inline IntPolynomial *new_IntPolynomial(const int N,
                                        TfheThreadContext &context,
                                        Allocator &alloc) {
    return alloc->newObject<IntPolynomial>(N);
}

/**
 * Allocates an array of integer polynomials
 */
inline IntPolynomial *new_IntPolynomial_array(size_t nbelts,
                                              const int N,
                                              TfheThreadContext &context,
                                              Allocator &alloc) {
    return alloc->newArray<IntPolynomial>(nbelts, N);
}


/**
 * MEMORY ALLOC - torus polynomials
 */

/**
 * Allocates an torus polynomial
 */
template<typename TORUS>
inline TorusPolynomial<TORUS> *new_TorusPolynomial(const int N,
                                                   TfheThreadContext &context,
                                                   Allocator &alloc) {
    return alloc->newObject<TorusPolynomial<TORUS>>(N);
}

/**
 * Allocates an array of torus polynomials
 */
template<typename TORUS>
inline TorusPolynomial<TORUS> *new_TorusPolynomial_array(size_t nbelts,
                                                         const int N,
                                                         TfheThreadContext &context,
                                                         Allocator &alloc) {
    return alloc->newArray<TorusPolynomial<TORUS>>(nbelts, N);
}

/**
 * Deletes a torus polynomial
 */
template<typename TORUS>
inline void delete_TorusPolynomial(TorusPolynomial<TORUS> obj,
                                   TfheThreadContext &context,
                                   Allocator &alloc) {
    alloc->deleteObject<TorusPolynomial<TORUS>>(&obj);
}

/**
 * Deletes an array of torus polynomials
 */
template<typename TORUS>
inline void delete_TorusPolynomial_array(size_t nbelts,
                                         TorusPolynomial<TORUS> *obj,
                                         TfheThreadContext &context,
                                         Allocator &alloc) {
    alloc->deleteArray<TorusPolynomial<TORUS>>(nbelts, obj);
}


#endif //POLYNOMIALS_H
