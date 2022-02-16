cmake_minimum_required(VERSION 3.15)
project(Huff_Compressor LANGUAGES C VERSION 0.0.1)

# Mensagens uteis de build
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE Generator: ${CMAKE_GENERATOR}")

# definição de um PATH Relativo Global de um determinado projeto
file(GLOB_RECURSE HCL_SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "source/*.c")
file(GLOB_RECURSE HUFF_COMPRESSOR_SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "tests/Huffmanh_Compressor.c")

# Criação da LIB
add_library(HCL STATIC ${HCL_SOURCES})

# Criação do executavel
add_executable(Huff_Compressor ${HUFF_COMPRESSOR_SOURCES})

# Linkando a lib
target_link_libraries(Huff_Compressor PUBLIC HCL)

set_target_properties(Huff_Compressor HCL PROPERTIES
    C_STANDARD 11
	C_STANDARD_REQUIRED ON
	C_EXTENSIONS OFF
)