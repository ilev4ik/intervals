enable_testing()
set(GTEST_ROOT $ENV{GTEST_ROOT})
find_package(GTest REQUIRED)

message(STATUS ${GTEST_INCLUDE_DIRS})

add_executable(intervals_test intervals_test.cpp)
target_include_directories(intervals_test PRIVATE
        ${INCLUDE_DIR}
        ${GTEST_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS})

target_link_libraries(intervals_test GTest::GTest GTest::Main)
add_test(intervals_test intervals_test)