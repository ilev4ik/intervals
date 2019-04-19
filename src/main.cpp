#include <boost/variant.hpp>

#include <boost/bimap.hpp>
#include <boost/optional.hpp>
#include <boost/icl/split_interval_map.hpp>

#include "../include/active_segments.h"
#include "../include/utils.h"
#include "../include/stamps_accumulator.h"

using namespace lvn;
using namespace lvn::detail;

struct host_tag {
    friend bool operator== (const host_tag&,const host_tag&) {return true;}

    static std::string as_str() {
        return "host";
    }

};

struct phys_tag {
    friend bool operator== (const phys_tag&, const phys_tag&) {return true;}
    static std::string as_str() {
        return "phys";
    }

};

struct tag_visitor : public boost::static_visitor<std::string>
{
    template <typename T>
    std::string operator()(T) const {
        return T::as_str();
    }
};

template <typename D>
struct tagged_data {
    int id;
    boost::variant<host_tag, phys_tag> tag;
    D data;

    // for multiset
    friend bool operator< (const tagged_data<D>& lhs, const tagged_data<D>& rhs) {
        return lhs.data < rhs.data;
    }

    // for adjacent_find and groupings
    friend bool operator== (const tagged_data<D>& lhs, const tagged_data<D>& rhs) {
        return lhs.tag == rhs.tag && lhs.id == rhs.id && lhs.data == rhs.data;
    }

    // for debugging :)
    friend std::ostream& operator<< (std::ostream& os, const tagged_data<D>& d) {
        return os << "{ " << boost::apply_visitor(tag_visitor{}, d.tag) << ", " << d.id << ", " << d.data << "}";
    }
};

int main()
{
    std::cout << (host_tag{} == host_tag{}) << std::endl;

    using stamper_type = stamps_accumulator<int, tagged_data<std::string>>;
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
