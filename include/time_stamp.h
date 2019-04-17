#pragma once
#ifndef INTERVALS_TIME_STAMP_H
#define INTERVALS_TIME_STAMP_H

#include <ostream>
#include <iomanip>

namespace lvn {

    template <typename T, typename D>
    struct time_stamp {
        using stamp_type = T;
        using data_type = D;

        stamp_type time;
        bool active;
        data_type data;

        friend bool operator<(const time_stamp<T, D> &lhs, const time_stamp<T, D> &rhs) {
            return lhs.time < rhs.time;
        }

        friend std::ostream &operator<<(std::ostream &os, const time_stamp<T, D> &s) {
            os << std::boolalpha;
            return os << "{ " << s.time << ", " << s.active << " }";
        }
    };

}
#endif //INTERVALS_TIME_STAMP_H
