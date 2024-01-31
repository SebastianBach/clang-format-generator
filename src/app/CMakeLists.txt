add_executable(app)

target_sources(app PRIVATE  app.cpp 
                            result.h
                            run.cpp
                            run.h)
set_target_properties(app PROPERTIES OUTPUT_NAME "clang_format_generator") 
target_link_libraries(app PRIVATE lib)
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


add_test(NAME app_no_args COMMAND app)
set_tests_properties(app_no_args PROPERTIES WILL_FAIL TRUE)

add_test(NAME app_invalid_reference_file COMMAND app bad_source.cpp target 123)
set_tests_properties(app_invalid_reference_file PROPERTIES WILL_FAIL TRUE)

add_test(NAME app_create_ref COMMAND app ref.cpp)
add_test(NAME app_create_clang_format_3_7 COMMAND app ref.cpp output_37 37)
add_test(NAME app_create_clang_format_10_0 COMMAND app ref.cpp output_100 100)
add_test(NAME app_create_clang_format_14_0 COMMAND app ref.cpp output_140 140)
add_test(NAME app_create_clang_format_17_0 COMMAND app ref.cpp output_170 170)

add_test(NAME app_invalid_version COMMAND app ref.cpp output_invalid_version abc)
set_tests_properties(app_invalid_version PROPERTIES WILL_FAIL TRUE)
