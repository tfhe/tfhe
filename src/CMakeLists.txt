cmake_minimum_required(VERSION 3.0)

#set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_RULE_MESSAGES ON)

set(CMAKE_CONFIGURATION_TYPES Debug Optim CACHE TYPE INTERNAL FORCE)
set(CMAKE_BUILD_TYPE "optim" CACHE STRING "Build Type: Debug or Optim")
set(ENABLE_FFTW OFF CACHE BOOL "Enable the FFTW FFT processor (GPL)")
set(ENABLE_NAYUKI_PORTABLE ON CACHE BOOL "Enable the Nayuki portable FFT processor (MIT)")
set(ENABLE_NAYUKI_AVX ON CACHE BOOL "Enable the Nayuki AVX assembly FFT processor (MIT)")
set(ENABLE_SPQLIOS_AVX ON CACHE BOOL "Enable the SPQLIOS AVX assembly FFT processor")
set(ENABLE_SPQLIOS_FMA ON CACHE BOOL "Enable the SPQLIOS FMA assembly FFT processor")
set(ENABLE_TESTS OFF CACHE BOOL "Build the tests (requires googletest)")

project(tfhe)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "AppleClang")
  # https://stackoverflow.com/a/16229679
  set(CLANG_FLAGS "-stdlib=libc++")
endif()

# -std=c99 seems to be required in Travis tests for whatever reason
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -std=c99")

set(CMAKE_CXX_FLAGS_DEBUG "${CLANG_FLAGS} -std=gnu++11 -g3 -O0 -Wall -Werror")
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -Wall -Werror")

set(CMAKE_CXX_FLAGS_OPTIM "${CLANG_FLAGS} -std=gnu++11 -g3 -march=native -O2 -DNDEBUG -funroll-loops -Wall -Werror")
set(CMAKE_C_FLAGS_OPTIM "-g3 -march=native -O3 -DNDEBUG -funroll-loops -Wall -Werror")

set(CMAKE_CXX_FLAGS_RELEASE "${CLANG_FLAGS} -std=gnu++11 -g0 -march=native -O2 -DNDEBUG -funroll-loops -Wall -Werror")
set(CMAKE_C_FLAGS_RELEASE "-g0 -march=native -O3 -DNDEBUG -funroll-loops -Wall -Werror")

if (ENABLE_NAYUKI_PORTABLE)
list(APPEND FFT_PROCESSORS "nayuki-portable")
endif(ENABLE_NAYUKI_PORTABLE)

if (ENABLE_FFTW)
list(APPEND FFT_PROCESSORS "fftw")
## find fftw
find_package(PkgConfig)
pkg_check_modules(PC_FFTW QUIET fftw3)
set(FFTW_DEFINITIONS ${PC_FFTW_CFLAGS_OTHER})
find_path(FFTW_INCLUDES fftw3.h
    HINTS ${PC_FFTW_INCLUDEDIR} ${PC_FFTW_INCLUDE_DIRS})
find_library (FFTW_LIBRARIES NAMES fftw3 
    HINTS ${PC_FFTW_LIBDIR} ${PC_FFTW_LIBRARY_DIRS})
# handle the QUIETLY and REQUIRED arguments and set FFTW_FOUND to
# TRUE if all listed variables are TRUE
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (FFTW DEFAULT_MSG FFTW_LIBRARIES FFTW_INCLUDES)
mark_as_advanced (FFTW_LIBRARIES FFTW_INCLUDES)
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

include_directories("include")
file(GLOB TFHE_HEADERS include/*.h)

install(FILES ${TFHE_HEADERS}
    DESTINATION include/tfhe
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

# include the lib and the tests
add_subdirectory(libtfhe)
if (ENABLE_TESTS)
enable_testing()
add_subdirectory(test)
endif (ENABLE_TESTS)
