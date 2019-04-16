#include "../include/stamps_accumulator.h"

using namespace boost;

namespace lvn {

	using namespace detail;

	template <typename T>
	segment_vec<T> stamps_accumulator<T>::add(stamps_set<T> stamps) {
		auto actives = active_segments(stamps.begin(), stamps.end());

		for (auto& segment : actives) {
		    segment.info = mac_generator();
		}

		multi_segment_vec.push_back(actives);
		return actives;
	}

	template <typename T>
	void stamps_accumulator<T>::print_res() {
		timeline_type::iterator it = timeline.begin();
		std::cout << "------ intersection result ------\n";
		while (it != timeline.end()) {
			const auto interval = it->first;
			T from = index_time_bm.left.at(interval.lower()).value();
			inf_time_t<T> to_opt = index_time_bm.left.at(interval.upper());
			std::cout << segment_t<T>{ from, to_opt };
			for (auto&& mac : it->second) {
				std::cout << mac << " ";
			}
			std::cout << std::endl;
			it++;
		}
	}

	template <typename T>
	void stamps_accumulator<T>::intersect() {
		std::vector<T> times;
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
			left_bm.insert(bm_type<T>::left_value_type(i, make_optional(unique_times[i])));
		};
		// для бесконечности
		left_bm.insert(bm_type<T>::left_value_type(unique_times.size(), boost::none));

		for (const auto& segments : multi_segment_vec) {
			for (const auto& segment : segments) {
				const index_t start = index_time_bm.right.at(segment.start);
				const index_t end = index_time_bm.right.at(segment.end);
				const std::string mac = segment.info;

				timeline.add(std::make_pair(icl::interval<index_t>::right_open(start, end), property_set{ mac }));
			}
		}

	}

}
