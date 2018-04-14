cmake_minimum_required(VERSION 3.0)

include_directories(../../libtfhe)

set(TFHE_TESTLIB_SOURCES
        ntl_conversions.cpp
        ntl_conversions.h)

add_library(tfhetestlib ${TFHE_TESTLIB_SOURCES})
