//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// definitions that are directly required by more than one header of the API

#if !defined(_SG14_COMMON_H)
#define _SG14_COMMON_H 1

#include <type_traits>

namespace sg14 {
    ////////////////////////////////////////////////////////////////////////////////
    // import std definitions

    // common_type
    template<class ...T>
    struct common_type;

    template<class ...T>
    struct common_type : std::common_type<T...> {
    };

    // is_integral
    template<class T>
    struct is_integral;

    template<class T>
    struct is_integral : std::is_integral<T> {
    };

    // is_signed
    template<class T>
    struct is_signed;

    template<class T>
    struct is_signed : std::is_signed<T> {
    };

    // is_unsigned
    template<class T>
    struct is_unsigned;

    template<class T>
    struct is_unsigned : std::is_unsigned<T> {
    };

    // make_signed
    template<class T>
    struct make_signed;

    template<class T>
    struct make_signed : std::make_signed<T> {
    };

    // make_unsigned
    template<class T>
    struct make_unsigned;

    template<class T>
    struct make_unsigned : std::make_unsigned<T> {
    };

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::max

        template<class T>
        constexpr T max(T a, T b)
        {
            return (a<b) ? b : a;
        }
    }
}

#endif  // _SG14_COMMON_H
