cmake_minimum_required(VERSION 3.0)

set(CORE_SRCS
        #list all .cpp and .h files here, one file per file
        arithmetic/numeric_utils.h
        arithmetic/polynomials.cpp
        arithmetic/polynomials.h
        arithmetic/polynomials_arithmetic.cpp
        arithmetic/polynomials_arithmetic.h
        arithmetic/polynomials_multiplication.cpp
        arithmetic/random_gen.cpp
        arithmetic/random_gen.h
        arithmetic/tfhe_core.h
        arithmetic/torus.h
        arithmetic/threadcontext.h
        tfhe-core.cpp
        allocator/allocator.h
        allocator/ValgrindAllocator.cpp
        allocator/ValgrindAllocator.h
        allocator/TFHEAllocator.cpp
        allocator/TFHEAllocator.h
        arithmetic/BigInt.cpp
        arithmetic/BigInt.h
        arithmetic/BigTorus.cpp
        arithmetic/BigTorus.h)

add_library(tfhe-core OBJECT ${CORE_SRCS})
set_property(TARGET tfhe-core PROPERTY POSITION_INDEPENDENT_CODE ON)

