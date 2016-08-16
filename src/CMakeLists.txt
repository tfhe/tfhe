cmake_minimum_required(VERSION 3.0)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RULE_MESSAGES ON)

project(tfhe)

# include the lib and the tests
add_subdirectory(libtfhe)
add_subdirectory(test)

