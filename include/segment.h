#pragma once
#ifndef INTERVALS_SEGMENT_H
#define INTERVALS_SEGMENT_H

namespace lvn {
    static const std::string infty = "\xE2\x88\x9E";

    template<typename T, class Enable = void>
    struct segment;

    template<typename T>
    struct segment<T, typename std::enable_if_t<std::is_integral_v<T>>> {
        T start;
        boost::optional<T> end;

        std::string info;

        friend bool operator<(const segment<T> &lhs, const segment<T> &rhs) {
            return lhs.start < rhs.start;
        }

        friend bool operator==(const segment<T> &lhs, const segment<T> &rhs) {
            return lhs.start == rhs.start && lhs.end == rhs.end;
        }

        friend std::ostream &operator<<(std::ostream &os, const segment<T> &s) {
            return os << "[ " << s.start << "; "
                      << (s.end ? std::to_string(s.end.value()) : infty)
                      << " )" << " = " << s.info;
        }
    };
}

#endif //INTERVALS_SEGMENT_H
