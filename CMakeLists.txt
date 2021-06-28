# CMakeList.txt : CMake project for RayTracer3, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("RayTracer3")
set(CMAKE_PREFIX_PATH "SDL2")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(src)