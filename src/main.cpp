#include <boost/bimap.hpp>
#include <boost/optional.hpp>
#include <boost/icl/split_interval_map.hpp>

#include "../include/active_segments.h"
#include "../include/utils.h"
#include "../include/stamps_accumulator.h"

using namespace lvn;
using namespace lvn::detail;

int main()
{
    stamps_set<> stamps1 = {
            {0, false}, {10, true}, {20, false},
            {50, true}, {100, false}, {200, true},
            {250, false}, {300, true}, {310, false},
            {320, true}
    };


    stamps_set<> stamps2 = {
            {5, false}, {15, true}, {25, false}, {330, true}
    };

    stamps_accumulator stamper;
    lvn::print_iterable_range(stamper.add(stamps1));
    lvn::underline();
    lvn::print_iterable_range(stamper.add(stamps2));

    stamper.intersect();
    stamper.print_res();

    return 0;
}
