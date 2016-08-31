cmake_minimum_required(VERSION 3.0)

project(tfhe-fft-processors)

if (ENABLE_FFTW) 
    add_subdirectory(fftw)
endif (ENABLE_FFTW) 

if (ENABLE_NAYUKI_AVX OR ENABLE_NAYUKI_PORTABLE) 
    add_subdirectory(nayuki)
endif (ENABLE_NAYUKI_AVX OR ENABLE_NAYUKI_PORTABLE)

if (ENABLE_POC_AVX OR ENABLE_POC_FMA) 
    add_subdirectory(poc)
endif (ENABLE_POC_AVX OR ENABLE_POC_FMA) 

