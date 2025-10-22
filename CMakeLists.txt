cmake_minimum_required(VERSION 4.0)
project(graphics_engine C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(glfw3 3.3 REQUIRED)

link_directories(lib)

add_executable(graphics_engine /usr/local/src/glad.c)

target_link_libraries(graphics_engine PRIVATE glfw)
target_link_libraries(graphics_engine PRIVATE GL)
target_link_libraries(graphics_engine PRIVATE cimgui)
target_link_libraries(graphics_engine PRIVATE m)
target_link_libraries(graphics_engine PRIVATE stdc++)

add_compile_definitions(CIMGUI_DEFINE_ENUMS_AND_STRUCTS=1)

include_directories(include)

add_subdirectory(src)