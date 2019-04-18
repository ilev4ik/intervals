#include <boost/bimap.hpp>
#include <boost/optional.hpp>
#include <boost/icl/split_interval_map.hpp>

#include "../include/active_segments.h"
#include "../include/utils.h"
#include "../include/stamps_accumulator.h"

using namespace lvn;
using namespace lvn::detail;

template <typename D>
struct tagged_data {
    int id;
    D data;

    // for multiset
    friend bool operator< (const tagged_data<D>& lhs, const tagged_data<D>& rhs) {
        return lhs.data < rhs.data;
    }

    // for adjacent_find and groupings
    friend bool operator== (const tagged_data<D>& lhs, const tagged_data<D>& rhs) {
        return lhs.id == rhs.id && lhs.data == rhs.data;
    }

    // for debugging :)
    friend std::ostream& operator<< (std::ostream& os, const tagged_data<D>& d) {
        return os << "{" << d.id << ", " << d.data << "}";
    }
};

int main()
{
    using stamper_type = stamps_accumulator<int, tagged_data<std::string>>;
    using stamps_t = typename stamper_type::stamps_set_t;

    stamps_t stamps1 = {
            {0, true, {1, "11::"}},
            {10, true, {2, "22::"}},
            {10, true, {4, "44::"}},
            {20, false, {1, "11::"}}
    };


    stamper_type stamper;
    for (auto&& i : stamper.add(stamps1)) {
        lvn::print_iterable_range(i);
    }

    lvn::underline();

    stamps_t stamps2 = {
            {0, true, {2, "22::"}},
            {10, true, {4, "44::"}},
            {20, false, {4, "44::"}},
            {50, true, {1, "11::"}},
            {100, false, {2, "22::"}}
    };


    for (auto&& i : stamper.add(stamps2)) {
        lvn::print_iterable_range(i);
    }
    lvn::underline();

    stamper.intersect();

    for (auto&& segment : stamper.result()) {
        std::cout << segment << " => \n";
        std::cout << "\t collisions: ";
        for (auto&& collision : segment.collisions()) {
            std::cout << collision << " ";
        }
        std::cout << "\n\n";
    }

    return 0;
}
