add_library(lib STATIC)

target_sources(lib PRIVATE      clang_format_lib.h 
                                version.cpp
                                parse_line.cpp
                                generate_reference_file.cpp
                                write_clang_format_file.cpp)

target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

