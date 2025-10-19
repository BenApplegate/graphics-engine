cmake_minimum_required(VERSION 4.0)
project(graphics_engine C)

set(CMAKE_C_STANDARD 23)

find_package(glfw3 3.3 REQUIRED)

add_executable(graphics_engine /usr/local/src/glad.c)

target_link_libraries(graphics_engine glfw)
target_link_libraries(graphics_engine GL)

add_subdirectory(src)