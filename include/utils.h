#pragma once
#ifndef INTERVALS_UTILS_H
#define INTERVALS_UTILS_H

#include <iomanip>
#include <iterator>
#include <iostream>
#include <type_traits>

#include <boost/io/ios_state.hpp>

namespace lvn {
    template<typename It>
    void print_iterable(It begin, It end, const char* sep = "\n") {
        std::copy(begin, end, std::ostream_iterator<typename std::iterator_traits<It>::value_type>(std::cout, sep));
    }

    template<typename Range>
    void print_iterable_range(Range c, const char* sep = "\n") {
        print_iterable(c.begin(), c.end(), sep);
    }

    void underline(char symb = '=', std::size_t count = 20) {
        boost::io::ios_all_saver guard(std::cout);
        std::cout << std::setfill(symb) << std::setw(count) << "" << std::endl;
    }
}

#endif //INTERVALS_UTILS_H
