add_executable(unit_tests)

target_sources(unit_tests PRIVATE test.cpp)
target_link_libraries(unit_tests PRIVATE lib)
target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME unit_tests COMMAND unit_tests)
