#include <algorithm>
#include <set>

#include "../include/utils.h"
#include "../include/time_stamp.h"
#include "../include/active_segments.h"

int main()
{
    std::set<lvn::time_stamp<int>> stamps = {
            {0, false}, {10, false}, {20, true},
            {50, true}, {100, true}, {200, false},
            {250, true}, {300, true}, {310, false},
            {320, true}
    };

    auto&& segments = lvn::active_segments(stamps.begin(), stamps.end());
    print_iterable(stamps.begin(), stamps.end());
    lvn::underline();
    print_iterable(segments.begin(), segments.end());
    return 0;
}