cmake_minimum_required(VERSION 3.0)

include_directories(../../libtfhe)

set(TFHE_TESTLIB_SOURCES
        ntl_utils.cpp
        ntl_utils.h
        random_sample.cpp
        random_sample.h
        )

add_library(tfhetestlib ${TFHE_TESTLIB_SOURCES})
