cmake_minimum_required(VERSION 3.0)

include_directories(${GTEST_INCLUDE_DIRS})
# add_subdirectory(googletest EXCLUDE_FROM_ALL)

# set(GOOGLETEST_SOURCES
#     arithmetic_test.cpp
#     lwe_test.cpp
#     polynomial_test.cpp
#     tlwe_test.cpp
#     tgsw_test.cpp
#     tlwe_fft_test.cpp
#     tgsw_fft_test.cpp
#     lwekeyswitch_test.cpp
#     bootstrapping_test.cpp
#     bootstrapping_test_fft.cpp
#     io_test.cpp
#     lagrangehalfc_test.cpp
#     boots_gates_test.cpp
#     )

set(CPP_ITESTS
    # test-bootstrapping-fft
    # test-decomp-tgsw
    # test-lwe
    # test-multiplication
    # test-tlwe
    # test-gate-bootstrapping
    # test-addition-boot
    test_lut_bootstrapping
    )

set(C_ITESTS
    # test-c-binding
    )

# We build a specific unit and integration test for each fft processor
foreach (FFT_PROCESSOR IN LISTS FFT_PROCESSORS)

if (FFT_PROCESSOR STREQUAL "fftw")
set(RUNTIME_LIBS
    tfhe-fftw
    ${FFTW_LIBRARIES}
    )

else ()
set(RUNTIME_LIBS
    tfhe-${FFT_PROCESSOR}
    )

endif (FFT_PROCESSOR STREQUAL "fftw")

#the unittests are compiled with the google test framework
# add_executable(unittests-${FFT_PROCESSOR} ${GOOGLETEST_SOURCES} ${TFHE_HEADERS})
# target_link_libraries(unittests-${FFT_PROCESSOR} ${RUNTIME_LIBS} ${GTEST_BOTH_LIBRARIES} -lgmock -lpthread)
# add_test(unittests-${FFT_PROCESSOR} unittests-${FFT_PROCESSOR})

#the integration tests must be single source code, and are compiled as a standalone application
#we first compile the C++ tests
foreach (CPP_ITEST ${CPP_ITESTS})
    add_executable(${CPP_ITEST}-${FFT_PROCESSOR} ${CPP_ITEST}.cpp ${TFHE_HEADERS})
    target_link_libraries(${CPP_ITEST}-${FFT_PROCESSOR} ${RUNTIME_LIBS})
endforeach(CPP_ITEST)

#then the C tests
foreach (C_ITEST ${C_ITESTS})
    add_executable(${C_ITEST}-${FFT_PROCESSOR} ${C_ITEST}.c ${TFHE_HEADERS})
    target_link_libraries(${C_ITEST}-${FFT_PROCESSOR} ${RUNTIME_LIBS})
endforeach(C_ITEST)

endforeach (FFT_PROCESSOR IN LISTS FFT_PROCESSORS)

