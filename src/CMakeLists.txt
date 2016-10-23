cmake_minimum_required(VERSION 3.0)

#set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_RULE_MESSAGES ON)

set(CMAKE_BUILD_TYPE "optim" CACHE STRING "Build Type: Debug or Optim")
set(ENABLE_FFTW OFF CACHE BOOL "Enable the FFTW FFT processor (GPL)")
set(ENABLE_NAYUKI_PORTABLE ON CACHE BOOL "Enable the Nayuki portable FFT processor (MIT)")
set(ENABLE_NAYUKI_AVX ON CACHE BOOL "Enable the Nayuki AVX assembly FFT processor (MIT)")
set(ENABLE_SPQLIOS_AVX ON CACHE BOOL "Enable the SPQLIOS AVX assembly FFT processor")
set(ENABLE_SPQLIOS_FMA ON CACHE BOOL "Enable the SPQLIOS FMA assembly FFT processor")
set(ENABLE_TESTS OFF CACHE BOOL "Build the tests (requires googletest)")

set(CMAKE_CXX_FLAGS_DEBUG "-std=gnu++11 -g3 -O0 -Wall -Werror")
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -Wall -Werror")

set(CMAKE_CXX_FLAGS_OPTIM "-std=gnu++11 -g3 -march=native -O2 -DNDEBUG -funroll-loops -Wall -Werror")
set(CMAKE_C_FLAGS_OPTIM "-g3 -march=native -O2 -DNDEBUG -funroll-loops -Wall -Werror")

set(CMAKE_CXX_FLAGS_RELEASE "-std=gnu++11 -g0 -march=native -O2 -DNDEBUG -funroll-loops -Wall -Werror")
set(CMAKE_C_FLAGS_RELEASE "-g0 -march=native -O2 -DNDEBUG -funroll-loops -Wall -Werror")


if (ENABLE_NAYUKI_PORTABLE)
list(APPEND FFT_PROCESSORS "nayuki-portable")
endif(ENABLE_NAYUKI_PORTABLE)

if (ENABLE_FFTW)
list(APPEND FFT_PROCESSORS "fftw")
endif(ENABLE_FFTW)

if (ENABLE_NAYUKI_AVX)
list(APPEND FFT_PROCESSORS "nayuki-avx")
endif(ENABLE_NAYUKI_AVX)

if (ENABLE_SPQLIOS_AVX)
list(APPEND FFT_PROCESSORS "spqlios-avx")
endif(ENABLE_SPQLIOS_AVX)

if (ENABLE_SPQLIOS_FMA)
list(APPEND FFT_PROCESSORS "spqlios-fma")
endif(ENABLE_SPQLIOS_FMA)

project(tfhe)

include_directories("include")
file(GLOB TFHE_HEADERS include/*.h)

# include the lib and the tests
add_subdirectory(libtfhe)
if (ENABLE_TESTS)
enable_testing()
find_package(GTest REQUIRED)
add_subdirectory(test)
endif (ENABLE_TESTS)
