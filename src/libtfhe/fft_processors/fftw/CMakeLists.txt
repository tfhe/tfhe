cmake_minimum_required(VERSION 3.0)

# This is the nayuki fft processor for the tfhe library
#enable_language(ASM)

set(SRCS
    fft_processor_fftw.cpp
    lagrangehalfc_impl.cpp
    )

set(HEADERS
    lagrangehalfc_impl.h
    )

include_directories(${FFTW_INCLUDES})
add_library(tfhe-fft-fftw OBJECT ${SRCS} ${HEADERS})
set_property(TARGET tfhe-fft-fftw PROPERTY POSITION_INDEPENDENT_CODE ON)
