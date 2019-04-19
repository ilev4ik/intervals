cmake_minimum_required(VERSION 3.13)
project(intervals)

#if ("${TARGET_CPU}" STREQUAL "amd64")
#    set (CMAKE_SIZEOF_VOID_P 8)
#
#    set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)
#    set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS FALSE)
#elseif ("${TARGET_CPU}" STREQUAL "x86")
#    set (CMAKE_SIZEOF_VOID_P 4)
#
#    set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS FALSE)
#    set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS TRUE)
#
#    if (GCC)
#        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
#        set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m32")
#    endif ()
#else ()
#    message (FATAL_ERROR "Unsupported CPU: ${TARGET_CPU}")
#endif ()

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
