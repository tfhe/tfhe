cmake_minimum_required(VERSION 3.0)

if (ENABLE_FFTW) 
    add_subdirectory(fftw)
endif (ENABLE_FFTW) 

if (ENABLE_NAYUKI_AVX OR ENABLE_NAYUKI_PORTABLE) 
    add_subdirectory(nayuki)
endif (ENABLE_NAYUKI_AVX OR ENABLE_NAYUKI_PORTABLE)

if (ENABLE_SPQLIOS_AVX OR ENABLE_SPQLIOS_FMA) 
    add_subdirectory(spqlios)
endif (ENABLE_SPQLIOS_AVX OR ENABLE_SPQLIOS_FMA) 

