cmake_minimum_required(VERSION 3.0)

# This is the nayuki fft processor for the tfhe library
project(libtfhe-fft-poc)

enable_language(ASM)

set(SRCS_AVX
    poc-fft-impl.cpp
    poc-fft-avx.s
    poc-ifft-avx.s
    fft_processor_poc.cpp
    lagrangehalfc_impl.cpp
    lagrangehalfc_impl_avx.s
    )
    
set(SRCS_FMA
    poc-fft-impl.cpp
    poc-fft-fma.s
    poc-ifft-fma.s
    fft_processor_poc.cpp
    lagrangehalfc_impl.cpp
    lagrangehalfc_impl_fma.s
    )
    
set(HEADERS
    poc-fft.h
    lagrangehalfc_impl.h
    )

if (ENABLE_POC_AVX) 
    add_library(tfhe-fft-poc-avx OBJECT ${SRCS_AVX} ${HEADERS})
endif (ENABLE_POC_AVX) 

if (ENABLE_POC_FMA) 
    add_library(tfhe-fft-poc-fma OBJECT ${SRCS_FMA} ${HEADERS})
endif (ENABLE_POC_FMA) 
