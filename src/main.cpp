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

class stamps_accumulator
{
    class generator
    {
        static const int mac_count = 4;
    public:
        generator() : engine(std::random_device()()), uniform_dist(0, mac_count - 1) {}
        std::string operator()() {
            return get_random_mac();
        }

        std::string get_random_mac() {
            return mac_store[uniform_dist(engine)];
        }


    private:
        std::default_random_engine engine;
        std::uniform_int_distribution<int> uniform_dist;

        std::array<std::string, mac_count> mac_store = {
                "11::", "22::", "33::", "44::"
        };
    };

    using segment_vec = std::vector<lvn::segment<int>>;

    using property_set = std::multiset<std::string>;
    using timeline_type = icl::split_interval_map<int, property_set>;
    using bm_type = bimap<int, optional<int>>;

public:
    segment_vec add(stamps_set stamps) {
        auto active_segments = lvn::active_segments(stamps.begin(), stamps.end());
        for (auto& segment : active_segments) {
            segment.info = mac_generator();
        }

        multi_segment_vec.push_back(active_segments);
        return active_segments;
    }

    void print_res() {
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
    }

    void intersect() {
        std::vector<int> times;
        for (const auto& vec : multi_segment_vec) {
            for (const auto& segment : vec) {
                times.push_back(segment.start);
                if (segment.end.is_initialized()) {
                    times.push_back(segment.end.value());
                }
            }
        }
        auto last_unique_time = std::unique(times.begin(), times.end());
        std::sort(times.begin(), last_unique_time);
        auto& unique_times = times;

        auto& left_bm = index_time_bm.left;
        for (std::size_t i = 0; i < unique_times.size(); ++i) {
            left_bm.insert(bm_type::left_value_type(i, make_optional(unique_times[i])));
        };
        // для бесконечности
        left_bm.insert(bm_type::left_value_type(unique_times.size(), boost::none));


        for (const auto& segments : multi_segment_vec) {
            for (const auto& segment : segments) {
                const int start = index_time_bm.right.at(segment.start);
                const int end = index_time_bm.right.at(segment.end);
                const std::string mac = segment.info;

                timeline.add(std::make_pair(icl::interval<int>::right_open(start, end), property_set{mac}));
            }
        }

    }

private:
    std::vector<segment_vec> multi_segment_vec;
    generator mac_generator;
    timeline_type timeline;
    bm_type index_time_bm;
};

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

    stamps_accumulator stamper;
    lvn::print_iterable_range(stamper.add(stamps1));
    lvn::underline();
    lvn::print_iterable_range(stamper.add(stamps2));

    stamper.intersect();
    stamper.print_res();

    return 0;
}
