set(CMAKE_INCLUDE_CURRENT_DIR On)

add_executable(intervals main.cpp
        ${INCLUDE_DIR}/active_segments.h
        ${INCLUDE_DIR}/time_stamp.h
        ${INCLUDE_DIR}/utils.h
        )

target_include_directories(intervals PUBLIC ${INCLUDE_DIR})
target_include_directories(intervals PUBLIC ${Boost_INCLUDE_DIRS})
#target_link_libraries(intervals Boost::date_time)