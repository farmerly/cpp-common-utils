cmake_minimum_required (VERSION 3.0)

project(cpp_common_utils)

add_compile_options(-g)

include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX)
	check_cxx_compiler_flag(--std=c++11 SUPPORTS_CXX11)
	if(SUPPORTS_CXX11)
        add_compile_options(--std=c++11)
    else()
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/checksum)
include_directories(${CMAKE_SOURCE_DIR}/include/crypto)
include_directories(${CMAKE_SOURCE_DIR}/include/logutils)
include_directories(${CMAKE_SOURCE_DIR}/include/sockutils)
include_directories(${CMAKE_SOURCE_DIR}/include/threadpool)
include_directories(${CMAKE_SOURCE_DIR}/include/toolutils)
include_directories(${CMAKE_SOURCE_DIR}/include/xmlutils)

add_subdirectory(src)
add_subdirectory(test)