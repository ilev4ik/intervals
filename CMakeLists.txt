cmake_minimum_required(VERSION 3.13)
project(intervals)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

find_package(Boost REQUIRED)

add_subdirectory(src)
add_subdirectory(test)
