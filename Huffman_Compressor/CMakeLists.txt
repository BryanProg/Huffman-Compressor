cmake_minimum_required(VERSION 3.15)
project(Huff_Compressor LANGUAGES C VERSION 0.0.1)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE Generator: ${CMAKE_GENERATOR}")

file(GLOB_RECURSE HCL_SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "source/*.c")
file(GLOB_RECURSE HUFF_COMPRESSOR_SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "tests/Huffmanh_Compressor.c")

add_library(HCL STATIC ${HCL_SOURCES})

add_executable(Huff_Compressor ${HUFF_COMPRESSOR_SOURCES})

target_link_libraries(Huff_Compressor PUBLIC HCL)

set_target_properties(Huff_Compressor HCL PROPERTIES
    C_STANDARD 11
	C_STANDARD_REQUIRED ON
	C_EXTENSIONS OFF
)
