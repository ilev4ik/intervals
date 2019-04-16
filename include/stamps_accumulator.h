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

		template <typename T = time_t>
		using stamps_set = std::set<lvn::time_stamp<T>>;

		template <typename T = time_t>
		using inf_time_t = boost::optional<T>;

		template <typename T = time_t>
		using segment_t = lvn::segment<T>;

		template <typename T = time_t>
		using segment_vec = std::vector<segment_t<T>>;

		using property_set = std::multiset<std::string>; // multiset of ?
		using timeline_type = boost::icl::split_interval_map<index_t, property_set>;

		template <typename T>
		using bm_type = boost::bimap<index_t, inf_time_t<T>>;

	}

	template <typename T = detail::time_t>
	class stamps_accumulator
	{
	public:
		detail::segment_vec<T> add(detail::stamps_set<T> stamps);
		void print_res();
		void intersect();

	private:
		std::vector<detail::segment_vec<T>> multi_segment_vec;
		detail::timeline_type timeline;
		detail::bm_type<T> index_time_bm;
		property_generator mac_generator;
	};

}

#include <stamps_accumulator.inl>

#endif // STAMPS_ACCUMULATOR_H