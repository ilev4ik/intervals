cmake_minimum_required(VERSION 3.13)
project(intervals)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(BOOST_ROOT $ENV{BOOST_ROOT})
set(BOOST_LIBRARYDIR $ENV{BOOST_LIBRARYDIR})

message(STATUS ${CMAKE_BUILD_TYPE})

set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_${CMAKE_BUILD_TYPE} ON)
set(Boost_ARCHITECTURE -x64)
set(Boost_COMPILER -gcc7)
set(Boost_MULTITHREADED TRUE)

find_package(Boost 1.60.0 REQUIRED
        #COMPONENTS date_time
        )

add_subdirectory(src)
add_subdirectory(test)
