cmake_minimum_required(VERSION 3.0)

# This is the nayuki fft processor for the tfhe library
project(libtfhe-fft-poc)

enable_language(ASM)

set(SRCS_AVX
    poc-fft-impl.cpp
    poc-fft.s
    poc-ifft.s
    fft_processor_poc.cpp
    )
    
set(HEADERS
    poc-fft.h
    )

#if (ENABLE_POC_AVX) 
    add_library(tfhe-fft-poc-avx OBJECT ${SRCS_AVX} ${HEADERS})
#endif (ENABLE_POC_AVX) 
