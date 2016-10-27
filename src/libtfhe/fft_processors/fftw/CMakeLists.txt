cmake_minimum_required(VERSION 3.0)

# This is the nayuki fft processor for the tfhe library
project(libtfhe-fft-fftw)

#enable_language(ASM)

set(SRCS
    fft_processor_fftw.cpp
    lagrangehalfc_impl.cpp
    )

set(HEADERS
    lagrangehalfc_impl.h
    )

add_library(tfhe-fft-fftw OBJECT ${SRCS} ${HEADERS})
