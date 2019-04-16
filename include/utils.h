#pragma once
#ifndef INTERVALS_UTILS_H
#define INTERVALS_UTILS_H

#include <iomanip>
#include <iterator>
#include <iostream>
#include <type_traits>

namespace lvn {
    template<typename It>
    void print_iterable(It begin, It end) {
        std::copy(begin, end, std::ostream_iterator<typename std::iterator_traits<It>::value_type>(std::cout, "\n"));
    }

    template<typename Container>
    void print_iterable_range(Container c) {
        print_iterable(c.begin(), c.end());
    }

    void underline(std::size_t count = 20) {
        std::cout << std::setfill('=') << std::setw(count) << "" << std::endl;
    }
}

#endif //INTERVALS_UTILS_H
