cmake_minimum_required(VERSION 3.0)

set(CORE_SRCS
        #list all .cpp and .h files here, one file per file
        arithmetic/lagrangehalfc_arithmetic.h
        arithmetic/lagrangehalfc_impl.cpp
        arithmetic/lagrangehalfc_impl.h
        arithmetic/multiplication.cpp
        arithmetic/numeric_utils.h
        arithmetic/polynomials.cpp
        arithmetic/polynomials.h
        arithmetic/polynomials_arithmetic.h
        arithmetic/random_gen.cpp
        arithmetic/random_gen.h
        arithmetic/tfhe_core.h
        arithmetic/torus.h
        arithmetic/toruspolynomial-functions.cpp
        tfhe-core.cpp
        allocator/allocator.h
        allocator/MallocAllocator.cpp
        allocator/MallocAllocator.h
        allocator/ValgrindAllocator.cpp
        allocator/ValgrindAllocator.h
        allocator/StackAllocator.cpp
        allocator/StackAllocator.h
        allocator/FirstStackAllocator.cpp
        allocator/FirstStackAllocator.h)

add_library(tfhe-core OBJECT ${CORE_SRCS})
set_property(TARGET tfhe-core PROPERTY POSITION_INDEPENDENT_CODE ON)

