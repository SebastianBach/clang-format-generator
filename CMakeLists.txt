cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(clang_format_generator VERSION 0.1 DESCRIPTION "" LANGUAGES CXX)
enable_testing()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "build with GCC")
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

if(CMAKE_COMPILER_IS_GNUCXX)

    option(BUILD_COVERAGE "Coverage Build" OFF)

    if(BUILD_COVERAGE)
        message(STATUS "Build coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    endif()

    option(BUILD_UBSAN "UndefinedBehaviorSanitizer Build" OFF)

    if(BUILD_UBSAN)
        message(STATUS "Build with UBSan")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fno-omit-frame-pointer -fno-sanitize-recover")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
    endif()

    option(BUILD_ASAN "AddressSanitizer Build" OFF)

    if(BUILD_ASAN)
        message(STATUS "Build with ASan")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    endif()
    
endif()

add_subdirectory(src)
