#ifndef TFHE_CORE_H
#define TFHE_CORE_H

///@file
///@brief This file declares only the structures names
#include <cstdint>

enum OptimTypes {
    PORTABLE=0,
    AVX2=1
};

void die_dramatically(const char *message);

/**
 * @brief prevents copy by removing the copy constructor and assignment operator
 * this should be the default for allocators and for global and thread contexts
 * Note: by default, it also disables moving: declare a move constructor explicitly if yo need it.
 */
#define PREVENT_COPY_ALLOW_STACK(TYPENAME) \
    TYPENAME(const TYPENAME &) = delete; \
    TYPENAME & operator=(const TYPENAME &)= delete;

/**
 * @brief prevents copy and classical allocation on the stack
 * by removing copy and move constructors/assignments, as well as the destructor
 * this should be the default behaviour for all objects of the library
 */
#define PREVENT_STACK_COPY(TYPENAME) \
    PREVENT_COPY_ALLOW_STACK(TYPENAME)\
    TYPENAME(TYPENAME &&) = delete; \
    TYPENAME & operator=(TYPENAME &&)= delete; \
    ~TYPENAME() = delete

#endif //TFHE_CORE_H
