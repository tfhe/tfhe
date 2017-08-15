cmake_minimum_required(VERSION 3.0)

# This is the nayuki fft processor for the tfhe library
enable_language(ASM)

set(SRCS_AVX
    spqlios-fft-impl.cpp
    spqlios-fft-avx.s
    spqlios-ifft-avx.s
    fft_processor_spqlios.cpp
    lagrangehalfc_impl.cpp
    lagrangehalfc_impl_avx.s
    )
    
set(SRCS_FMA
    spqlios-fft-impl.cpp
    spqlios-fft-fma.s
    spqlios-ifft-fma.s
    fft_processor_spqlios.cpp
    lagrangehalfc_impl.cpp
    lagrangehalfc_impl_fma.s
    )
    
set(HEADERS
    spqlios-fft.h
    lagrangehalfc_impl.h
    )

if (ENABLE_SPQLIOS_AVX) 
    add_library(tfhe-fft-spqlios-avx OBJECT ${SRCS_AVX} ${HEADERS})
    set_property(TARGET tfhe-fft-spqlios-avx PROPERTY POSITION_INDEPENDENT_CODE ON)
endif (ENABLE_SPQLIOS_AVX) 

if (ENABLE_SPQLIOS_FMA) 
    add_library(tfhe-fft-spqlios-fma OBJECT ${SRCS_FMA} ${HEADERS})
    set_property(TARGET tfhe-fft-spqlios-fma PROPERTY POSITION_INDEPENDENT_CODE ON)
endif (ENABLE_SPQLIOS_FMA)
