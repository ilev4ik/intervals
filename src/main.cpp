#include "../include/utils.h"
#include "../include/stamps_accumulator.h"
#include "tagged_data.h"

struct host_tag : lvn::tag_base<host_tag> {
    constexpr static auto str = "host";
};

struct phys_tag : lvn::tag_base<phys_tag> {
    constexpr static auto str = "phys";
};

int main()
{
    using namespace lvn;

    using stamper_type = stamps_accumulator<int, tagged_data<std::string, tags<host_tag, phys_tag>>>;
    using stamps_t = typename stamper_type::stamps_set_t;

    stamps_t stamps1 = {
            {0, true, {1, host_tag{}, "11::"}},
            {0, true, {1, phys_tag{}, "11::"}},
            {10, true, {2, host_tag{}, "22::"}},
            {20, false, {1, host_tag{}, "11::"}}
    };

    stamper_type stamper;
    for (auto&& i : stamper.add(stamps1)) {
        lvn::print_iterable_range(i);
    }

    lvn::underline();

    stamps_t stamps2 = {
            {0, true, {2, host_tag{}, "22::"}},
            {10, true, {4, host_tag{}, "44::"}},
            {20, false, {4, host_tag{}, "44::"}},
            {50, true, {1, host_tag{}, "11::"}},
            {100, false, {2, host_tag{}, "22::"}}
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
