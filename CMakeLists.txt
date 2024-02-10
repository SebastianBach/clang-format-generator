cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(clang_format_generator VERSION 0.1 DESCRIPTION "" LANGUAGES CXX)
enable_testing()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message("build with GCC")
    add_compile_options(-Wall 
                        -Wextra 
                        -Wshadow 
                        -Wnon-virtual-dtor 
                        -pedantic 
                        -Wold-style-cast 
                        -Wunused 
                        -Wpedantic 
                        -Wconversion 
                        -Wcast-align 
                        -Wsign-conversion 
                        -Wmisleading-indentation
                        -Wduplicated-cond
                        -Wduplicated-branches
                        -Wlogical-op
                        -Wnull-dereference
                        -Wuseless-cast
                        -Wdouble-promotion
                        -Wformat=2
                        -Werror)
endif()

option(BUILD_COVERAGE "Coverage Build" OFF)

if(BUILD_COVERAGE)
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "--coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS "--coverage -fprofile-arcs -ftest-coverage")
    endif()
endif()

add_subdirectory(src)
