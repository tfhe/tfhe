cmake_minimum_required(VERSION 3.0)

# This is the nayuki fft processor for the tfhe library
enable_language(ASM)

set(SRCS_PORTABLE
    fft-x8664-avx-aux.c
    fft-model-of-x8664-avx.c
    fft_processor_nayuki.cpp
    lagrangehalfc_impl.cpp
    )
    
set(SRCS_AVX
    fft-x8664-avx-aux.c
    fft-x8664-avx.s
    fft-x8664-avx-reverse.s
    fft_processor_nayuki.cpp
    lagrangehalfc_impl.cpp
    )
    
set(HEADERS
    fft.h
    lagrangehalfc_impl.h
    )

if (ENABLE_NAYUKI_PORTABLE) 
    add_library(tfhe-fft-nayuki-portable OBJECT ${SRCS_PORTABLE} ${HEADERS})
    set_property(TARGET tfhe-fft-nayuki-portable PROPERTY POSITION_INDEPENDENT_CODE ON)
endif (ENABLE_NAYUKI_PORTABLE)

if (ENABLE_NAYUKI_AVX) 
    add_library(tfhe-fft-nayuki-avx OBJECT ${SRCS_AVX} ${HEADERS})
    set_property(TARGET tfhe-fft-nayuki-avx PROPERTY POSITION_INDEPENDENT_CODE ON)
endif (ENABLE_NAYUKI_AVX) 
