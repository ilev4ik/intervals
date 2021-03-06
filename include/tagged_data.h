#pragma once
#ifndef INTERVALS_TAGGED_DATA_H
#define INTERVALS_TAGGED_DATA_H

#include <string>
#include <boost/variant.hpp>

namespace lvn {

    template<typename T>
    struct tag_base {
        friend bool operator==(const T &, const T &) { return true; }

        static std::string as_str() {
            return T::str;
        }
    };

    template<typename...>
    struct pack { };

    template <typename... T>
    struct tags {
        using Tags = pack<T...>;
    };

    template <template <typename, typename> class B, typename D, typename H>
    struct tagged_data_base;

    template <typename D, typename... T>
    struct tagged_data_packed;

    template <typename D, typename... T>
    struct tagged_data_packed<D, pack<T...>> {
        int id;
        boost::variant<T...> tag;
        D data;

        template <typename P>
        tagged_data_packed(int id_, const P& tag_, const D& data_ = {})
                : id(id_), tag(tag_), data(data_)
        {}
    };

    template <template <typename, typename> class B, typename D, typename H>
    struct tagged_data_base : tagged_data_packed<D, typename H::Tags> {

        template <typename V>
        struct tag_visitor : public boost::static_visitor<V>
        {
            template <typename T>
            V operator()(T) const {
                return T::as_str();
            }
        };

        using tagged_data_packed<D, typename H::Tags>::tagged_data_packed;

        std::string as_str() const {
            std::stringstream ss;
            ss << "{ ";
            ss << boost::apply_visitor(tag_visitor<D>{}, this->tag) << ", " << this->id << ", " << this->data;
            ss << "}";
            return ss.str();
        }

        // for multiset
        friend bool operator< (const B<D, H>& lhs, const B<D, H>& rhs) {
            return lhs.data < rhs.data;
        }

        // for adjacent_find and groupings
        friend bool operator== (const B<D, H>& lhs, const B<D, H>& rhs) {
            return lhs.tag == rhs.tag && lhs.id == rhs.id && lhs.data == rhs.data;
        }

        // for debugging :)
        friend std::ostream& operator<< (std::ostream& os, const B<D, H>& d) {
            return os << d.as_str();
        }

    };

    template <typename D, typename H>
    struct tagged_data : tagged_data_base<tagged_data, D, H> {
        using tagged_data_base<tagged_data, D, H>::tagged_data_base;
        using data_type = D;
    };

}


#endif //INTERVALS_TAGGED_DATA_H
