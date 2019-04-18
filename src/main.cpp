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
    using stamps_t = stamps_set_t<int, std::string>;
    stamps_t stamps1 = {
            {0, true, "11::"},
            {10, true, "22::"},
            {20, false, "11::"}
    };

    stamps_accumulator<int, std::string> stamper;
    for (auto&& i : stamper.add(stamps1)) {
        lvn::print_iterable_range(i);
    }

    lvn::underline();

    stamps_t stamps2 = {
            {0, true, "22::"},
            {10, true, "44::"},
            {20, false, "44::"},
            {50, true, "11::"},
            {100, false, "22::"}
    };
    for (auto&& i : stamper.add(stamps2)) {
        lvn::print_iterable_range(i);
    }
    lvn::underline();

    stamper.intersect();
    for (auto&& segment : stamper.result()) {
        std::cout << segment << std::endl;
    }

    return 0;
}
