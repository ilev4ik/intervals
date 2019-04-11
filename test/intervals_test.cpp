#include <active_segments.h>
#include <time_stamp.h>

#include <boost/optional.hpp>
#include <gtest/gtest.h>

namespace {

    template <typename T>
    using empty_segment_vector = const std::vector<lvn::segment<T>>;

    template <typename T>
    auto active_segments_range(const std::set<lvn::time_stamp<T>>& stamps)
    {
        return lvn::active_segments(stamps.begin(), stamps.end());
    }
}

TEST(TestActiveSegments, Empty)
{
    std::set<lvn::time_stamp<int>> stamps;
    ASSERT_EQ(active_segments_range(stamps), empty_segment_vector<int>{});
}

TEST(TestActiveSegments, SingleActiveStamp)
{
    std::set<lvn::time_stamp<int>> stamps = {{0, true}};
    ASSERT_TRUE((active_segments_range(stamps) == empty_segment_vector<int>{{0, boost::none}}));
}

TEST(TestActiveSegments, AllActiveStamps)
{
    std::set<lvn::time_stamp<int>> stamps = {{0, true}, {10, true}, {20, true}};
    ASSERT_TRUE((active_segments_range(stamps) == empty_segment_vector<int>{{0, boost::none}}));
}

TEST(TestInactiveSegments, SingleInactiveStamp)
{
    std::set<lvn::time_stamp<int>> stamps = {{0, false}};
    ASSERT_EQ(active_segments_range(stamps), empty_segment_vector<int>{});
}

TEST(TestInactiveSegments, AllInactiveStamps)
{
    std::set<lvn::time_stamp<int>> stamps = {{0, false}, {10, false}, {20, false}};
    ASSERT_EQ(active_segments_range(stamps), empty_segment_vector<int>{});
}

TEST(TestActiveSegments, MultipleActiveStampWithOffset)
{
    std::set<lvn::time_stamp<int>> stamps = {{0, false}, {5, true}, {10, true}};
    ASSERT_TRUE((active_segments_range(stamps) == empty_segment_vector<int>{{5, boost::none}}));
}

TEST(TestComplex, OffsetSingleActive)
{
    std::set<lvn::time_stamp<int>> stamps = {
            {0, false}, {10, false}, {20, true}, {25, false}
    };

    ASSERT_TRUE((active_segments_range(stamps) == empty_segment_vector<int>{{20, 25}}));
}

TEST(TestComplex, OffsetSingleInactive)
{
    std::set<lvn::time_stamp<int>> stamps = {
            {0, true}, {10, true}, {20, false}, {25, true}
    };

    ASSERT_TRUE((active_segments_range(stamps) == empty_segment_vector<int>{
        {0, 20},
        {25, boost::none}
    }));
}

TEST(TestComplex, Alternating)
{
    std::set<lvn::time_stamp<int>> stamps = {
            {0, true}, {10, false}, {20, true},
            {50, false}, {100, true}, {200, false},
            {250, true}, {300, false}, {310, true},
            {320, false}
    };

    ASSERT_TRUE((active_segments_range(stamps) == empty_segment_vector<int>{
            {0, 10},
            {20, 50},
            {100, 200},
            {250, 300},
            {310, 320}
    }));
}

TEST(TestComplex, AlternatingWithOffset)
{
    std::set<lvn::time_stamp<int>> stamps = {
            {0, false}, {10, true}, {20, false},
            {50, true}, {100, false}, {200, true},
            {250, false}, {300, true}, {310, false},
            {320, true}
    };

    ASSERT_TRUE((active_segments_range(stamps) == empty_segment_vector<int>{
            {10, 20},
            {50, 100},
            {200, 250},
            {300, 310},
            {320, boost::none}
    }));
}

TEST(TestComplex, MixActiveWithInactive)
{
    std::set<lvn::time_stamp<int>> stamps = {
            {0, false}, {10, false}, {20, true},
            {50, true}, {100, true}, {200, false},
            {250, true}, {300, true}, {310, false},
            {320, true}
    };

    ASSERT_TRUE((active_segments_range(stamps) == empty_segment_vector<int>{
        {20, 200},
        {250, 310},
        {320, boost::none}
    }));
}