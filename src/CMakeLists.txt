cmake_minimum_required(VERSION 3.15)

set (CMAKE_CXX_STANDARD 20)

project(JsonEval)

add_subdirectory(JsonLib)
add_subdirectory(tests)

add_executable(json_eval main.cpp)
target_link_libraries(json_eval PRIVATE JsonLib)