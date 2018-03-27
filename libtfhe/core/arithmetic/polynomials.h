#ifndef POLYNOMIALS_H
#define POLYNOMIALS_H

///@file
///@brief This file contains the declaration of polynomials structures

#include "tfhe_core.h"
#include "tfhe_alloc.h"


/**
 * This structure represents an integer polynomial modulo X^N+1
 */
struct IntPolynomial {
    const int N;
    int *coefs;

    IntPolynomial(const int N);

    ~IntPolynomial();

    IntPolynomial(const IntPolynomial &) = delete; //forbidden
    IntPolynomial *operator=(const IntPolynomial &) = delete; //forbidden
};


/**
 * This structure represents an torus polynomial modulo X^N+1
 */
template<typename TORUS>
struct TorusPolynomial {
    const int N;
    TORUS *coefsT;

    TorusPolynomial(const int N);

    ~TorusPolynomial();

    TorusPolynomial(const TorusPolynomial<TORUS> &) = delete; //forbidden
    TorusPolynomial<TORUS> &operator=(const TorusPolynomial<TORUS> &) = delete; //forbidden
};

template
struct TorusPolynomial<Torus32>;
template
struct TorusPolynomial<Torus64>;

/**
 * This structure is used for FFT operations, and is a representation
 * over C of a polynomial in R[X]/X^N+1
 * This type is meant to be specialized, and all implementations of the structure must be compatible
 * (reinterpret_cast) with this one. Namely, they should contain at most 2 pointers
 */
struct LagrangeHalfCPolynomial {
    void *data;
    void *precomp;

    static void init_obj(LagrangeHalfCPolynomial *obj, const int N);

    static void destroy_obj(LagrangeHalfCPolynomial *obj);
};

//allocates and initialize the IntPolynomial structure
//(equivalent of the C++ new)
inline IntPolynomial *new_IntPolynomial(const int N) {
    return new_obj<IntPolynomial>(N);
}

inline IntPolynomial *new_IntPolynomial_array(int nbelts, const int N) {
    return new_obj_array<IntPolynomial>(nbelts, N);
}

//destroys and frees the IntPolynomial structure
//(equivalent of the C++ delete)
inline void delete_IntPolynomial(IntPolynomial *obj) {
    del_obj<IntPolynomial>(obj);
}

inline void delete_IntPolynomial_array(int nbelts, IntPolynomial *obj) {
    del_obj_array<IntPolynomial>(nbelts, obj);
}


//allocates and initialize the TorusPolynomial structure
//(equivalent of the C++ new)
template<typename TORUS>
inline TorusPolynomial<TORUS> *new_TorusPolynomial(const int N) {
    return new_obj<TorusPolynomial<TORUS>>(N);
}

template<typename TORUS>
inline TorusPolynomial<TORUS> *new_TorusPolynomial_array(int nbelts, const int N) {
    return new_obj_array<TorusPolynomial<TORUS>>(nbelts, N);
}

//destroys and frees the TorusPolynomial structure
//(equivalent of the C++ delete)
template<typename TORUS>
inline void delete_TorusPolynomial(TorusPolynomial<TORUS> *obj) {
    del_obj<TorusPolynomial<TORUS>>(obj);
}

template<typename TORUS>
inline void delete_TorusPolynomial_array(int nbelts, TorusPolynomial<TORUS> *obj) {
    del_obj_array<TorusPolynomial<TORUS>>(nbelts, obj);
}

//allocates and initialize the LagrangeHalfCPolynomial structure
//(equivalent of the C++ new)
inline LagrangeHalfCPolynomial *new_LagrangeHalfCPolynomial(const int N) {
    return new_obj<LagrangeHalfCPolynomial>(N);
}

inline LagrangeHalfCPolynomial *new_LagrangeHalfCPolynomial_array(int nbelts, const int N) {
    return new_obj_array<LagrangeHalfCPolynomial>(nbelts, N);
}

//destroys and frees the LagrangeHalfCPolynomial structure
//(equivalent of the C++ delete)
inline void delete_LagrangeHalfCPolynomial(LagrangeHalfCPolynomial *obj) {
    del_obj(obj);
}

inline void delete_LagrangeHalfCPolynomial_array(int nbelts, LagrangeHalfCPolynomial *obj) {
    del_obj_array(nbelts, obj);
}

/* wrapper to LagrangeHalfCPolynomial initialization function */
inline void init_obj(LagrangeHalfCPolynomial *obj, const int N) {
    LagrangeHalfCPolynomial::init_obj(obj, N);
}

/* wrapper to LagrangeHalfCPolynomial destroy function */
inline void destroy_obj(LagrangeHalfCPolynomial *obj) {
    LagrangeHalfCPolynomial::destroy_obj(obj);
}

#endif //POLYNOMIALS_H
