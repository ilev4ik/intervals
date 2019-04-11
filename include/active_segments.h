#pragma once
#ifndef INTERVALS_ACTIVE_SEGMENTS_H
#define INTERVALS_ACTIVE_SEGMENTS_H

#include <ostream>
#include <type_traits>
#include <boost/optional.hpp>
#include <vector>

#include <time_stamp.h>

namespace lvn {

    static const std::string infty =  "\xE2\x88\x9E";

    template <typename T, class Enable = void>
    struct segment;

    template <typename T>
    struct segment<T, typename std::enable_if_t<std::is_integral_v<T>>>
    {
        T start;
        boost::optional<T> end;

        friend bool operator==(const segment<T>& lhs, const segment<T>& rhs) {
            return lhs.start == rhs.start && lhs.end == rhs.end;
        }

        friend std::ostream &operator<<(std::ostream &os, const segment<T> &s) {
            return os << "[ " << s.start << "; " << (s.end ? std::to_string(s.end.value()) : infty) << " )";
        }
    };

    template<typename It,
            typename value_type = typename std::iterator_traits<It>::value_type::value_type>
    auto active_segments(It begin, It end) -> std::vector<segment<value_type>>
    {
        std::vector<segment<value_type>> rv;
        rv.reserve(std::distance(begin, end) / 2);

        static auto is_active = [](const time_stamp<value_type>& s) { return s.active; };
        static auto activeness_changed = [](const time_stamp<value_type>& lhs, const time_stamp<value_type>& rhs) {
            return lhs.active != rhs.active;
        };

        auto from = begin;
        while (from != end) {
            from = std::find_if(from, end, is_active);
            if (from != end) {
                auto found_it = std::adjacent_find(from, end, activeness_changed);
                auto to = std::next(found_it, found_it == end ? 0 : 1);
                rv.push_back({
                    from->time,
                    found_it != end ? boost::make_optional(to->time) : boost::none
                });
                from = to;
            }
        }

        rv.shrink_to_fit();
        return rv;
    }

}
#endif //INTERVALS_ACTIVE_SEGMENTS_H
