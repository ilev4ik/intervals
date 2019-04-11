enable_testing()
find_package(GTest REQUIRED)

add_executable(intervals_test intervals_test.cpp)
target_include_directories(intervals_test PRIVATE
        ${INCLUDE_DIR}
        ${GTEST_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS})
target_link_libraries(intervals_test GTest::GTest GTest::Main)
add_test(intervals_test intervals_test)