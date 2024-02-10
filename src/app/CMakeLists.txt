add_executable(app)

target_sources(app PRIVATE  app.cpp 
                            result.h
                            run.cpp
                            run.h)
set_target_properties(app PROPERTIES OUTPUT_NAME "clang_format_generator") 
target_link_libraries(app PRIVATE lib)
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


# handling invalid arguments 

add_test(NAME app_no_args COMMAND app)
set_tests_properties(app_no_args PROPERTIES WILL_FAIL TRUE)

add_test(NAME app_invalid_reference_file COMMAND app bad_source.cpp target 123)
set_tests_properties(app_invalid_reference_file PROPERTIES WILL_FAIL TRUE)

# generate reference file and compare result

set(GENERATED_REFERENCE_FILE ${CMAKE_CURRENT_BINARY_DIR}/ref.cpp)

add_test(NAME app_create_ref COMMAND app ${GENERATED_REFERENCE_FILE})

set(REFERENCE_FILE ${PROJECT_SOURCE_DIR}/src/test_resources/ref.cpp)

add_test(NAME app_create_ref_compare COMMAND ${CMAKE_COMMAND}
    -DOUTPUT_FILE=${GENERATED_REFERENCE_FILE}
    -DREFERENCE_FILE=${REFERENCE_FILE}
    -P ${PROJECT_SOURCE_DIR}/src/test_resources/compare.cmake
)

# generate clang-format configurations

add_test(NAME app_create_clang_format_3_7 COMMAND app ref.cpp output_37 37)
add_test(NAME app_create_clang_format_10_0 COMMAND app ref.cpp output_100 100)
add_test(NAME app_create_clang_format_14_0 COMMAND app ref.cpp output_140 140)

# generate clang-format configuration and compare

set(OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/output_170)

add_test(NAME app_create_clang_format_17_0 COMMAND app ${GENERATED_REFERENCE_FILE} ${OUTPUT_FILE} 170)

set(REFERENCE_FILE ${PROJECT_SOURCE_DIR}/src/test_resources/output_170)

add_test(NAME app_create_clang_format_17_0_compare COMMAND ${CMAKE_COMMAND}
    -DOUTPUT_FILE=${OUTPUT_FILE}
    -DREFERENCE_FILE=${REFERENCE_FILE}
    -P ${PROJECT_SOURCE_DIR}/src/test_resources/compare.cmake
)

# handling invalid argument

add_test(NAME app_invalid_version COMMAND app ref.cpp output_invalid_version abc)
set_tests_properties(app_invalid_version PROPERTIES WILL_FAIL TRUE)

# empty sourc file

set(EMPTY_SOURCE_FILE ${PROJECT_SOURCE_DIR}/src/test_resources/empty.cpp)
add_test(NAME app_empty_source COMMAND app ${EMPTY_SOURCE_FILE} output_empty_source 140)
set_tests_properties(app_empty_source PROPERTIES WILL_FAIL TRUE)

# unit test

add_executable(app_test)
target_sources(app_test PRIVATE     run.test.cpp
                                    run.cpp
                                    run.h)

target_link_libraries(app_test PRIVATE lib)
target_link_libraries(app_test PRIVATE test_framework)
target_include_directories(app_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME app_test COMMAND app_test)
