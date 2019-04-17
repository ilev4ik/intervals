#pragma once
#ifndef INTERVALS_ACTIVE_SEGMENTS_H
#define INTERVALS_ACTIVE_SEGMENTS_H

#include "segment.h"

#include <string>
#include <ostream>
#include <type_traits>
#include <boost/optional.hpp>
#include <vector>

#include <time_stamp.h>

namespace lvn {

    template<typename It,
            typename stamp_type = typename std::iterator_traits<It>::value_type::stamp_type,
            typename data_type = typename std::iterator_traits<It>::value_type::data_type>
    auto active_segments(It begin, It end) -> std::vector<segment<stamp_type, data_type>>
    {
        const data_type info = begin->data;
        using stamp_t = time_stamp<stamp_type, data_type>;
        std::vector<segment<stamp_type, data_type>> rv;
        rv.reserve(std::distance(begin, end) / 2);

        static auto is_active = [](const stamp_t& s) { return s.active; };
        static auto activeness_changed = [](const stamp_t& lhs, const stamp_t& rhs) {
            return lhs.active != rhs.active;
        };

        auto from = begin;
        while (from != end) {
            from = std::find_if(from, end, is_active);
            if (from != end) {
                auto found_it = std::adjacent_find(from, end, activeness_changed);
                auto to = std::next(found_it, found_it == end ? 0 : 1);
                rv.emplace_back(
                    from->time,
                    found_it != end ? boost::make_optional(to->time) : boost::none,
                    info
                );
                from = to;
            }
        }

        rv.shrink_to_fit();
        return rv;
    }

}
#endif //INTERVALS_ACTIVE_SEGMENTS_H
