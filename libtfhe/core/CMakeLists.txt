cmake_minimum_required(VERSION 3.0)

set(CORE_SRCS
        #list all .cpp and .h files here, one file per file
        arithmetic/torus_utils.h
        arithmetic/torus_utils_big.h
        arithmetic/generic/polynomial_param.h
        arithmetic/generic/polynomial.h
        arithmetic/generic/polynomial.cpp
        arithmetic/generic/polynomial_big.cpp
        arithmetic/generic/polynomial_int.h
        arithmetic/generic/polynomial_int.cpp
        arithmetic/generic/polynomial_int_big.cpp
        arithmetic/generic/polynomial_torus.h
        arithmetic/generic/polynomial_torus.cpp
        arithmetic/generic/polynomial_torus_sml.cpp
        arithmetic/generic/polynomial_torus_big.cpp
        arithmetic/random_gen.cpp
        arithmetic/random_gen.h
        arithmetic/tfhe_core.h
        arithmetic/torus.h
        arithmetic/torus_utils_primitive.h
        arithmetic/torus_utils_primitive.cpp
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
        allocator/allocator-shortcut-functions.h
        contexts/tfhe_thread_context.h
        contexts/tfhe_global_context.cpp
        contexts/tfhe_global_context.h
        contexts/castable_unique_ptr.cpp
        contexts/castable_unique_ptr.h
        contexts/context_object_creator.cpp
        contexts/context_object_creator.h
        contexts/tfhe_thread_context.cpp
        arithmetic/big_real.cpp
        arithmetic/big_real.h
        arithmetic/real_param.h
        arithmetic/real_param.cpp
        arithmetic/torus_utils_primitive.h arithmetic/torus_utils_primitive.cpp arithmetic/torus_utils_big.cpp)

add_library(tfhe-core OBJECT ${CORE_SRCS})
set_property(TARGET tfhe-core PROPERTY POSITION_INDEPENDENT_CODE ON)

