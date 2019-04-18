#pragma once
#ifndef STAMPS_ACCUMULATOR_H
#define STAMPS_ACCUMULATOR_H

#include <set>
#include <vector>

#include <boost/bimap.hpp>
#include <boost/optional.hpp>
#include <boost/icl/split_interval_map.hpp>

#include "active_segments.h"
#include "property_generator.h"

namespace lvn {

    namespace detail {
        using index_t = std::size_t;

        template <typename T, typename D>
        using stamps_set_t = std::multiset<lvn::time_stamp<T, D>>;

        template <typename T>
        using inf_time_t = boost::optional<T>;

        template <typename T>
        using bm_type = boost::bimap<index_t, inf_time_t<T>>;

        template <typename T, typename D>
        using segment_t = lvn::segment<T, D>;

        template <typename T, typename D>
        using segment_vec_t = std::vector<segment_t<T, D>>;

        template <typename D>
        using property_set_t = std::multiset<D>;

        template <typename D>
        using timeline_type = boost::icl::split_interval_map<index_t, property_set_t<D>>;
    }

	template <typename T, typename D>
	class stamps_accumulator
    {
        using segment_vec_t = detail::segment_vec_t<T, D>;
        using timeline_type = detail::timeline_type<D>;
        using bm_type = detail::bm_type<T>;

	public:
        using stamps_set_t = detail::stamps_set_t<T, D>;

        std::vector<segment_vec_t> add(stamps_set_t stamps);
        segment_vec_t result() const;
		void intersect();

	private:
		std::vector<segment_vec_t> multi_segment_vec;
		timeline_type timeline;
		bm_type index_time_bm;
		property_generator mac_generator;
	};

}

#include <stamps_accumulator.inl>

#endif // STAMPS_ACCUMULATOR_H