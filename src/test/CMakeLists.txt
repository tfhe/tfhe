cmake_minimum_required(VERSION 3.0)

project(libtfhe-tests)

set(HEADERS
    ../include/tlwe.h
    ../include/lwesamples.h
    ../include/numeric_functions.h
    ../include/polynomials.h
    ../include/lweparams.h
    ../include/lagrangehalfc_arithmetic.h
    ../include/lwe-functions.h
    ../include/lwekeyswitch.h
    ../include/lwebootstrappingkey.h
    ../include/polynomials_arithmetic.h
    ../include/tlwe_functions.h
    ../include/tgsw_functions.h
    ../include/tfhe.h
    ../include/tgsw.h
    ../include/tfhe_core.h
    ../include/lwekey.h
    )

set(GOOGLETEST_SOURCES
    arithmetic_test.cpp
    lwe_test.cpp
    )

foreach (FFT_PROCESSOR IN LISTS FFT_PROCESSORS) 

if (FFT_PROCESSOR STREQUAL "fftw")
set(RUNTIME_LIBS
    tfhe-fftw
    -lfftw3
    )

elseif (FFT_PROCESSOR STREQUAL "nayuki-portable")
set(RUNTIME_LIBS
    tfhe-nayuki-portable
    )

elseif (FFT_PROCESSOR STREQUAL "nayuki-avx")
set(RUNTIME_LIBS
    tfhe-nayuki-avx
    )

elseif (FFT_PROCESSOR STREQUAL "spqlios-avx")
set(RUNTIME_LIBS
    tfhe-spqlios-avx
    )

elseif (FFT_PROCESSOR STREQUAL "spqlios-fma")
set(RUNTIME_LIBS
    tfhe-spqlios-fma
    )

endif (FFT_PROCESSOR STREQUAL "fftw") 

add_executable(unittests-${FFT_PROCESSOR} ${GOOGLETEST_SOURCES} ${HEADERS})
add_executable(test-bootstrapping-${FFT_PROCESSOR} test-bootstrapping.cpp ${HEADERS})
add_executable(test-bootstrapping-fft2-${FFT_PROCESSOR} test-bootstrapping-fft2.cpp ${HEADERS})
add_executable(test-deb-boot-${FFT_PROCESSOR} test-deb-boot.cpp ${HEADERS})
add_executable(test-decomp-tgsw-${FFT_PROCESSOR} test-decomp-tgsw.cpp ${HEADERS})
add_executable(test-lwe-${FFT_PROCESSOR} test-lwe.cpp ${HEADERS})
add_executable(test-multiplication-${FFT_PROCESSOR} test-multiplication.cpp ${HEADERS})
add_executable(test-tlwe-${FFT_PROCESSOR} test-tlwe.cpp ${HEADERS})

target_link_libraries(unittests-${FFT_PROCESSOR} ${RUNTIME_LIBS} -lgtest -lgtest_main)
target_link_libraries(test-bootstrapping-${FFT_PROCESSOR} ${RUNTIME_LIBS})
target_link_libraries(test-bootstrapping-fft2-${FFT_PROCESSOR} ${RUNTIME_LIBS})
target_link_libraries(test-deb-boot-${FFT_PROCESSOR} ${RUNTIME_LIBS})
target_link_libraries(test-decomp-tgsw-${FFT_PROCESSOR} ${RUNTIME_LIBS})
target_link_libraries(test-lwe-${FFT_PROCESSOR} ${RUNTIME_LIBS})
target_link_libraries(test-multiplication-${FFT_PROCESSOR} ${RUNTIME_LIBS})
target_link_libraries(test-tlwe-${FFT_PROCESSOR} ${RUNTIME_LIBS})

endforeach (FFT_PROCESSOR IN LISTS FFT_PROCESSORS) 

