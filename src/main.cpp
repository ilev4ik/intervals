#include <boost/bimap.hpp>
#include <boost/optional.hpp>
#include <boost/icl/split_interval_map.hpp>

#include <set>
#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <random>

#include "../include/active_segments.h"
#include "../include/utils.h"

using namespace boost;

using stamps_set = std::set<lvn::time_stamp<int>>;
using property_set = std::set<std::string>;

const int mac_count = 4;
std::array<std::string, mac_count> mac_store = {
        "11::", "22::", "33::", "44::"
};

std::random_device r;
std::default_random_engine engine(r());
std::uniform_int_distribution<int> uniform_dist(0, mac_count - 1);

std::string get_random_mac()
{
    return mac_store[uniform_dist(engine)];
}

int main()
{
    stamps_set stamps1 = {
            {0, false}, {10, true}, {20, false},
            {50, true}, {100, false}, {200, true},
            {250, false}, {300, true}, {310, false},
            {320, true}
    };


    stamps_set stamps2 = {
            {5, false}, {15, true}, {25, false}, {330, true}
    };

    std::vector<int> unique_times;

    auto fill_unique = [&unique_times](const stamps_set& ss) {
        std::transform(ss.begin(), ss.end(), std::back_inserter(unique_times),
                       [](lvn::time_stamp<int> stamp) {
                            return stamp.time;
                       });
    };

    fill_unique(stamps1);
    fill_unique(stamps2);

    std::sort(unique_times.begin(), unique_times.end());

    // index <--> time value
    using bm_type = bimap<int, optional<int>>;
    bm_type index_time_bm;

    auto& left_bm = index_time_bm.left;
    for (std::size_t i = 0; i < unique_times.size(); ++i) {
        left_bm.insert(bm_type::left_value_type(i, make_optional(unique_times[i])));
    };
    // для бесконечности
    left_bm.insert(bm_type::left_value_type(unique_times.size(), boost::none));

    auto&& segments1 = lvn::active_segments(stamps1.begin(), stamps1.end());
    for (auto& segment : segments1) {
        segment.info = get_random_mac();
    }
    lvn::print_iterable(segments1.begin(), segments1.end());
    lvn::underline();

    auto&& segments2 = lvn::active_segments(stamps2.begin(), stamps2.end());
    for (auto& segment : segments2) {
        segment.info = get_random_mac();
    }
    lvn::print_iterable(segments2.begin(), segments2.end());
    lvn::underline();

    using timeline_type = icl::split_interval_map<int, property_set>;
    timeline_type timeline;

    for (auto&& segment : segments1) {
        const int start = index_time_bm.right.at(segment.start);
        const int end = index_time_bm.right.at(segment.end);
        const std::string mac = segment.info;

        timeline.add(std::make_pair(icl::interval<int>::right_open(start, end), property_set{mac}));
    }

    for (auto&& segment : segments2) {
        const int start = index_time_bm.right.at(segment.start);
        const int end = index_time_bm.right.at(segment.end);
        const std::string mac = segment.info;

        timeline.add(std::make_pair(icl::interval<int>::right_open(start, end), property_set{mac}));
    }

    timeline_type::iterator it = timeline.begin();
    std::cout << "------ intersection result ------\n";
    while (it != timeline.end()) {
        const auto interval = it->first;
        int from = index_time_bm.left.at(interval.lower()).value();
        auto to_opt = index_time_bm.left.at(interval.upper());
        std::cout << lvn::segment<int>{from, to_opt};
        for (auto&& mac : it->second) {
            std::cout << mac << " ";
        }
        std::cout << std::endl;
        it++;
    }

    return 0;
}

// разрешить дубликаты std::set

