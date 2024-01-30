add_executable(app)

target_sources(app PRIVATE  app.cpp 
                            result.h)
set_target_properties(app PROPERTIES OUTPUT_NAME "clang_format_generator") 
target_link_libraries(app PRIVATE lib)
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
