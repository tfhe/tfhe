cmake_minimum_required(VERSION 3.0)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "No benchmark framework testing" FORCE)
add_subdirectory(benchmark EXCLUDE_FROM_ALL)

set(GOOGLEBENCH_SOURCES
        bench_bigreal.cpp
        bench_bigint.cpp
        )

#the benchmark applications are compiled with google benchmark framework
add_executable(bench ${GOOGLEBENCH_SOURCES})
target_link_libraries(bench benchmark tfhe)
target_include_directories(bench
        PRIVATE benchmark/include
        PRIVATE ../libtfhe
        )
