#include <active_segments.h>
#include <time_stamp.h>

#include <boost/optional.hpp>
#include <gtest/gtest.h>

namespace {

    template <typename T, typename D>
    using empty_segment_vector = const std::vector<lvn::segment<T, D>>;

    template <typename T = int, typename D = std::string>
    using default_stamp = lvn::time_stamp<T, D>;

    template <typename T = int, typename D = std::string>
    using default_empty_segments = empty_segment_vector<T, D>;

    template <typename T, typename D>
    auto active_segments_range(const std::set<default_stamp<T, D>>& stamps)
    {
        return lvn::active_segments(stamps.begin(), stamps.end());
    }
}

TEST(TestActiveSegments, Empty)
{
    std::set<default_stamp<>> stamps;
    ASSERT_EQ(active_segments_range(stamps), default_empty_segments<>{});
}

TEST(TestActiveSegments, SingleActiveStamp)
{
    std::set<default_stamp<>> stamps = {{0, true}};
    ASSERT_TRUE((active_segments_range(stamps) == default_empty_segments<>{{0, boost::none}}));
}

TEST(TestActiveSegments, AllActiveStamps)
{
    std::set<default_stamp<>> stamps = {{0, true}, {10, true}, {20, true}};
    ASSERT_TRUE((active_segments_range(stamps) == default_empty_segments<>{{0, boost::none}}));
}

TEST(TestInactiveSegments, SingleInactiveStamp)
{
    std::set<default_stamp<>> stamps = {{0, false}};
    ASSERT_EQ(active_segments_range(stamps), default_empty_segments<>{});
}

TEST(TestInactiveSegments, AllInactiveStamps)
{
    std::set<default_stamp<>> stamps = {{0, false}, {10, false}, {20, false}};
    ASSERT_EQ(active_segments_range(stamps), default_empty_segments<>{});
}

TEST(TestActiveSegments, MultipleActiveStampWithOffset)
{
    std::set<default_stamp<>> stamps = {{0, false}, {5, true}, {10, true}};
    ASSERT_TRUE((active_segments_range(stamps) == default_empty_segments<>{{5, boost::none}}));
}

TEST(TestComplex, OffsetSingleActive)
{
    std::set<default_stamp<>> stamps = {
            {0, false}, {10, false}, {20, true}, {25, false}
    };

    ASSERT_TRUE((active_segments_range(stamps) == default_empty_segments<>{{20, 25}}));
}

TEST(TestComplex, OffsetSingleInactive)
{
    std::set<default_stamp<>> stamps = {
            {0, true}, {10, true}, {20, false}, {25, true}
    };

    ASSERT_TRUE((active_segments_range(stamps) == default_empty_segments<>{
        {0, 20},
        {25, boost::none}
    }));
}

TEST(TestComplex, Alternating)
{
    std::set<default_stamp<>> stamps = {
            {0, true}, {10, false}, {20, true},
            {50, false}, {100, true}, {200, false},
            {250, true}, {300, false}, {310, true},
            {320, false}
    };

    ASSERT_TRUE((active_segments_range(stamps) == default_empty_segments<>{
            {0, 10},
            {20, 50},
            {100, 200},
            {250, 300},
            {310, 320}
    }));
}

TEST(TestComplex, AlternatingWithOffset)
{
    std::set<default_stamp<>> stamps = {
            {0, false}, {10, true}, {20, false},
            {50, true}, {100, false}, {200, true},
            {250, false}, {300, true}, {310, false},
            {320, true}
    };

    ASSERT_TRUE((active_segments_range(stamps) == default_empty_segments<>{
            {10, 20},
            {50, 100},
            {200, 250},
            {300, 310},
            {320, boost::none}
    }));
}

TEST(TestComplex, MixActiveWithInactive)
{
    std::set<default_stamp<>> stamps = {
            {0, false}, {10, false}, {20, true},
            {50, true}, {100, true}, {200, false},
            {250, true}, {300, true}, {310, false},
            {320, true}
    };

    ASSERT_TRUE((active_segments_range(stamps) == default_empty_segments<>{
        {20, 200},
        {250, 310},
        {320, boost::none}
    }));
}