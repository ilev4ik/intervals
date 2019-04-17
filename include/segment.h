#pragma once
#ifndef INTERVALS_SEGMENT_H
#define INTERVALS_SEGMENT_H

#include <ostream>
#include <boost/optional.hpp>
#include <string>

namespace lvn {
    static const std::string infty = "\xE2\x88\x9E";

    template<typename T, typename D, class Enable = void>
    struct segment;

    // D should be equality comparable
    template<typename T, typename D>
    struct segment<T, D, typename std::enable_if_t<std::is_integral_v<T>>> {
        segment(T s, boost::optional<T> e, D i = "")
        : start(s), end(e), info(i)
        {}

        T start;
        boost::optional<T> end;

        D info;

        friend bool operator<(const segment<T, D> &lhs, const segment<T, D> &rhs) {
            return lhs.start < rhs.start;
        }

        friend bool operator==(const segment<T, D> &lhs, const segment<T, D> &rhs) {
            return lhs.start == rhs.start && lhs.end == rhs.end;
        }

        friend std::ostream &operator<<(std::ostream &os, const segment<T, D> &s) {
            return os << "[ " << s.start << "; "
                      << (s.end ? std::to_string(s.end.value()) : infty)
                      << " )" << " = " << s.info;
        }
    };
}

#endif //INTERVALS_SEGMENT_H
