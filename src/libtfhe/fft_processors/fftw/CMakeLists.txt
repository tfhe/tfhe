cmake_minimum_required(VERSION 3.0)

# This is the nayuki fft processor for the tfhe library
project(libtfhe-fft-fftw)

include_directories(../../../include/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#enable_language(ASM)

set(SRCS
    fft_processor_fftw.cpp
    )
    
set(HEADERS
    )

add_library(tfhe-fft-fftw OBJECT ${SRCS} ${HEADERS})
