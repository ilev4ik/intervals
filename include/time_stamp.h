#pragma once
#ifndef INTERVALS_TIME_STAMP_H
#define INTERVALS_TIME_STAMP_H

#include <ostream>
#include <iomanip>

namespace lvn {

    template <typename T = int>
    struct time_stamp {
        T time;
        bool active;

        using value_type = T;

        friend bool operator<(const time_stamp<T> &lhs, const time_stamp<T> &rhs) {
            return lhs.time < rhs.time;
        }

        friend std::ostream &operator<<(std::ostream &os, const time_stamp<T> &s) {
            os << std::boolalpha;
            return os << "{ " << s.time << ", " << s.active << " }";
        }
    };

}
#endif //INTERVALS_TIME_STAMP_H
