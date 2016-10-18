
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief trait definitions related to the `sg14::fixed_point` type

#if !defined(SG14_TYPE_TRAITS_H)
#define SG14_TYPE_TRAITS_H 1

#include <type_traits>

/// study group 14 of the C++ working group
namespace sg14 {
    ////////////////////////////////////////////////////////////////////////////////
    // import selected <type_traits> definitions from std namespace

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

    ////////////////////////////////////////////////////////////////////////////////
    // Clang/GCC hacks to get 128-bit integers working with fixed_point

#if defined(SG14_INT128_ENABLED)
    // sg14::make_signed
    template<>
    struct make_signed<SG14_INT128> {
        using type = SG14_INT128;
    };

    template<>
    struct make_signed<SG14_UINT128> {
        using type = SG14_INT128;
    };

    // sg14::make_signed
    template<>
    struct make_unsigned<SG14_INT128> {
        using type = SG14_UINT128;
    };

    template<>
    struct make_unsigned<SG14_UINT128> {
        using type = SG14_UINT128;
    };
#endif
}

#endif	// SG14_TYPE_TRAITS_H
