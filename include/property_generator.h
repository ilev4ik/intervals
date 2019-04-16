#pragma once
#ifndef PROPERTY_GENERATOR_H
#define PROPERTY_GENERATOR_H

#include <array>
#include <string>
#include <random>

namespace lvn {
    class property_generator {
        static const int mac_count = 4;
    public:
        property_generator() : engine(std::random_device()()), uniform_dist(0, mac_count - 1) {}

        std::string operator()() {
            return get_random_mac();
        }

        std::string get_random_mac() {
            return mac_store[uniform_dist(engine)];
        }


    private:
        std::default_random_engine engine;
        std::uniform_int_distribution<int> uniform_dist;

        std::array <std::string, mac_count> mac_store = {
                "11::", "22::", "33::", "44::"
        };
    };
}
#endif // PROPERTY_GENERATOR_H
