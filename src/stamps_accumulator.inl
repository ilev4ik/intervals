#include <algorithm>

using namespace boost;

namespace lvn {

	using namespace detail;

	template <typename T, typename D>
    std::vector<segment_vec_t<T, D>> stamps_accumulator<T, D>::add(stamps_set_t stamps) {

        static auto by_prop = [](const time_stamp<T,D>& lhs, const time_stamp<T,D>& rhs) -> bool {
            return lhs.data == rhs.data;
        };

        // prepare data - get unique properties
        std::vector<time_stamp<T,D>> stamps_vec;
        std::copy(stamps.begin(), stamps.end(), std::back_inserter(stamps_vec));
        std::sort(stamps_vec.begin(), stamps_vec.end());
        auto last = std::unique(stamps_vec.begin(), stamps_vec.end(), by_prop);
        auto& unique_props = stamps_vec;

	    // for each unique property get its timestamps in sorted order
	    // then get active segments for each propery
	    // then save
        std::vector<segment_vec_t> rv;
        auto from = unique_props.begin();
        while (from != last) {
            const auto prop = *from;
            stamps_set_t prop_stamps;
            std::copy_if(stamps.begin(), stamps.end(), std::inserter(prop_stamps, prop_stamps.begin()),
                    std::bind(by_prop, prop, std::placeholders::_1));
            const auto& actives = active_segments(prop_stamps.begin(), prop_stamps.end());
            multi_segment_vec.push_back(actives);
            rv.push_back(actives);
            from++;
        };

		return rv;
	}

	template <typename T, typename D>
	segment_vec_t<T, D> stamps_accumulator<T, D>::result() const {
	    // get reverse mapping result: time_stamp index -> time_stamp
        segment_vec_t rv;
		auto it = timeline.begin();
		while (it != timeline.end()) {
			const auto interval = it->first;
			T from = index_time_bm.left.at(interval.lower()).value();
			inf_time_t<T> to_opt = index_time_bm.left.at(interval.upper());
            rv.emplace_back(from, to_opt, it->second);

			it++;
		}

		return rv;
	}

	template <typename T, typename D>
	void stamps_accumulator<T, D>::intersect() {
		std::vector<T> times;

		// defining unique time_stamps
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

		// creating mapping: time_stamp -> time index
		// initializing bimap
		auto& left_bm = index_time_bm.left;
		for (std::size_t i = 0; i < unique_times.size(); ++i) {
			left_bm.insert(typename bm_type::left_value_type(i, make_optional(unique_times[i])));
		};
		// infinity is marked as max(time_index) + 1
		// for infinity right endpoint
		left_bm.insert(typename bm_type::left_value_type(unique_times.size(), boost::none));

		// filling with the segments the timeline object preparing result data
		for (const auto& segments : multi_segment_vec) {
			for (const auto& segment : segments) {
				const index_t start = index_time_bm.right.at(segment.start);
				const index_t end = index_time_bm.right.at(segment.end);

				timeline.add(std::make_pair(icl::interval<index_t>::right_open(start, end), segment.info_set));
			}
		}

	}

}
