cmake_minimum_required(VERSION 3.0)

set(CORE_SRCS
        #list all .cpp and .h files here, one file per file
        arithmetic/torus_utils.h
        arithmetic/torus_utils_big.h
        arithmetic/polynomial_param.h
        arithmetic/polynomial.h
        arithmetic/polynomial.cpp
        arithmetic/polynomial_big.cpp
        arithmetic/polynomial_int.h
        arithmetic/polynomial_int.cpp
        arithmetic/polynomial_int_big.cpp
        arithmetic/polynomial_torus.h
        arithmetic/polynomial_torus.cpp
        arithmetic/polynomial_torus_big.cpp
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
        arithmetic/big_int.cpp
        arithmetic/big_int.h
        arithmetic/big_torus.cpp
        arithmetic/big_torus.h
        arithmetic/zmodule_param.h
        arithmetic/zmodule_param.cpp
        allocator/allocator-shortcut-functions.h)

add_library(tfhe-core OBJECT ${CORE_SRCS})
set_property(TARGET tfhe-core PROPERTY POSITION_INDEPENDENT_CODE ON)

