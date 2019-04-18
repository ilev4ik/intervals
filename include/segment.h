#pragma once
#ifndef INTERVALS_SEGMENT_H
#define INTERVALS_SEGMENT_H

#include <set>
#include <ostream>
#include <boost/optional.hpp>
#include <string>

namespace lvn {

    template<typename T, typename D, class Enable = void>
    struct segment;

    // D should be less comparable
    template<typename T, typename D>
    struct segment<T, D, typename std::enable_if<std::is_integral<T>::value>::type> {
        segment(T s, boost::optional<T> e, const std::multiset<D>& i_mset)
                : start(s), end(e), info_set(i_mset)
        {}

        segment(T s, boost::optional<T> e, const D& i)
            : start(s), end(e)
        {
            info_set.insert(i);
        }

        T start;
        boost::optional<T> end;
        std::multiset<D> info_set;

        std::vector<D> collisions() const {
            std::vector<D> rv;
            for (auto each = info_set.begin(); each != info_set.end(); each = info_set.upper_bound(*each)) {
                decltype(info_set.begin()) from, to;
                std::tie(from, to) = info_set.equal_range(*each);
                if (std::distance(from, to) > 1) {
                    std::copy(from, to, std::back_inserter(rv));
                }
            }
            return rv;
        }

        bool unique() const {
            std::size_t unique_count = 0;

            for (auto each = info_set.begin(); each != info_set.end(); each = info_set.upper_bound(*each)) {
                unique_count++;
            }

            return unique_count == info_set.size();
        }

        friend bool operator<(const segment<T, D>& lhs, const segment<T, D>& rhs) {
            return lhs.start < rhs.start;
        }

        friend bool operator==(const segment<T, D>& lhs, const segment<T, D>& rhs) {
            return lhs.start == rhs.start && lhs.end == rhs.end;
        }

        friend std::ostream &operator<<(std::ostream& os, const segment<T, D>& s) {
            static const std::string infty = "\xE2\x88\x9E";

            os << "[ ";
            os << s.start << "; "
               << (s.end ? std::to_string(s.end.value()) : infty);
            os << " )";

            os << " = { ";
            for (auto&& info : s.info_set) {
                os << info << " ";
            }
            os << "}";

            return os;
        }
    };
}

#endif //INTERVALS_SEGMENT_H
