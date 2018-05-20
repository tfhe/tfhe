cmake_minimum_required(VERSION 3.0)

include_directories(.)

# include the fft processors in the build process
add_subdirectory(core)

#add_library(tfhe-core OBJECT ${SRCS})
add_library(tfhe SHARED $<TARGET_OBJECTS:tfhe-core>)
target_link_libraries(tfhe -lgmp)
#set_property(TARGET tfhe PROPERTY POSITION_INDEPENDENT_CODE ON)

# foreach (FFT_PROCESSOR IN LISTS FFT_PROCESSORS) 
#     add_library(tfhe-${FFT_PROCESSOR} SHARED
# 	$<TARGET_OBJECTS:tfhe-core>
#         $<TARGET_OBJECTS:tfhe-fft-${FFT_PROCESSOR}>)
#     set_property(TARGET tfhe-${FFT_PROCESSOR} PROPERTY POSITION_INDEPENDENT_CODE ON)
# 
#     if (FFT_PROCESSOR STREQUAL "fftw")
#         target_link_libraries(tfhe-fftw ${FFTW_LIBRARIES})
#     endif (FFT_PROCESSOR STREQUAL "fftw")
# 
#     install(TARGETS tfhe-${FFT_PROCESSOR}
#         RUNTIME DESTINATION bin
#         LIBRARY DESTINATION lib
#         ARCHIVE DESTINATION lib)
# endforeach (FFT_PROCESSOR IN LISTS FFT_PROCESSORS) 
# 
