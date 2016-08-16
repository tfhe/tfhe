cmake_minimum_required(VERSION 3.0)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RULE_MESSAGES ON)

set(CMAKE_BUILD_TYPE "optim" CACHE STRING "Build Type: Debug or Optim")
set(ENABLE_FFTW ON CACHE BOOL "Enable the FFTW FFT processor (GPL)")
set(ENABLE_NAYUKI_PORTABLE ON CACHE BOOL "Enable the Nayuki portable FFT processor (MIT)")
set(ENABLE_NAYUKI_AVX ON CACHE BOOL "Enable the Nayuki AVX assembly FFT processor (MIT)")
set(DEFAULT_FFT_PROCESSOR "nayuki-portable" CACHE String "Choice of the FFT processor for test files")

set(CMAKE_CXX_FLAGS "-std=gnu++11 -g3 -O0 -Wall -Werror")
set(CMAKE_C_FLAGS "-g3 -O0 -Wall -Werror")

set(CMAKE_CXX_FLAGS_DEBUG "-std=gnu++11 -g3 -O0 -Wall -Werror")
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -Wall -Werror")

set(CMAKE_CXX_FLAGS_OPTIM "-std=gnu++11 -g0 -O2 -DNDEBUG -funroll-loops -Wall -Werror")
set(CMAKE_C_FLAGS_OPTIM "-g0 -O2 -DNDEBUG -funroll-loops -Wall -Werror")

set(CMAKE_CXX_FLAGS_RELEASE "-std=gnu++11 -g0 -O2 -DNDEBUG -funroll-loops -Wall -Werror")
set(CMAKE_C_FLAGS_RELEASE "-g0 -O2 -DNDEBUG -funroll-loops -Wall -Werror")

project(tfhe)

# include the lib and the tests
add_subdirectory(libtfhe)
add_subdirectory(test)

