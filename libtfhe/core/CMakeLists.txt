cmake_minimum_required(VERSION 3.0)

set(CORE_SRCS
        #list all .cpp and .h files here, one file per file
        tfhe-core.cpp
        arithmetic/torus.h
        arithmetic/random_gen.h
        arithmetic/numeric_utils.h
        )

add_library(tfhe-core OBJECT ${CORE_SRCS})
set_property(TARGET tfhe-core PROPERTY POSITION_INDEPENDENT_CODE ON)

