cmake_minimum_required(VERSION 3.0)

set(CORE_SRCS
        #list all .cpp and .h files here, one file per file
        tfhe-core.cpp
        allocator/allocator.h
        allocator/MallocAllocator.cpp
        allocator/MallocAllocator.h
        allocator/StrictMallocAllocator.cpp
        allocator/StrictMallocAllocator.h
        allocator/StackAllocator.cpp
        allocator/StackAllocator.h
        allocator/FirstStackAllocator.cpp
        allocator/FirstStackAllocator.h)

add_library(tfhe-core OBJECT ${CORE_SRCS})
set_property(TARGET tfhe-core PROPERTY POSITION_INDEPENDENT_CODE ON)

