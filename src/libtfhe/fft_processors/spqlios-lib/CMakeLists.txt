cmake_minimum_required(VERSION 3.0)

# This is the nayuki fft processor for the tfhe library
enable_language(ASM)

set(SRCS
        spqlios-lib-impl.cpp
        spqlios-lib-processor.cpp
        )

set(HEADERS
        spqlios-lib-impl.h
        )

if (ENABLE_SPQLIOS_LIB)
    add_library(tfhe-fft-spqlios-lib OBJECT ${SRCS} ${HEADERS})
    set_property(TARGET tfhe-fft-spqlios-lib PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_link_libraries(tfhe-fft-spqlios-lib ${SPQLIOS_LIB})
    target_include_directories(tfhe-fft-spqlios-lib PRIVATE ${SPQLIOS_INCLUDES})
endif (ENABLE_SPQLIOS_LIB)