#pragma once
#ifndef STAMPS_ACCUMULATOR_H
#define STAMPS_ACCUMULATOR_H

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <string>

#include <boost/bimap.hpp>
#include <boost/optional.hpp>
#include <boost/icl/split_interval_map.hpp>

#include "active_segments.h"
#include "property_generator.h"

namespace lvn {

	namespace detail {

		using index_t = std::size_t;
		using time_t = int64_t;
        using data_t = std::string;

		template <typename T = time_t, typename D = data_t>
		using stamps_set = std::set<lvn::time_stamp<T, D>>;

		template <typename T = time_t>
		using inf_time_t = boost::optional<T>;

		template <typename T = time_t, typename D = data_t>
		using segment_t = lvn::segment<T, D>;

		template <typename T = time_t, typename D = data_t>
		using segment_vec = std::vector<segment_t<T, D>>;

		template <typename T>
		using bm_type = boost::bimap<index_t, inf_time_t<T>>;

	}

	template <typename T, typename D>
	class stamps_accumulator
    {
        using property_set = std::multiset<D>;
        using timeline_type = boost::icl::split_interval_map<detail::index_t, property_set>;
	public:
        std::vector<detail::segment_vec<T, D>> add(detail::stamps_set<T, D> stamps);
		void print_res();
		void intersect();

	private:
		std::vector<detail::segment_vec<T, D>> multi_segment_vec;
		timeline_type timeline;
		detail::bm_type<T> index_time_bm;
		property_generator mac_generator;
	};

}

#include <stamps_accumulator.inl>

#endif // STAMPS_ACCUMULATOR_H