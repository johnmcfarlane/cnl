
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef _SG14_INT128_H
#define _SG14_INT128_H

#include "../type_traits.h"

// This file contains tweaks to standard traits that integers more conducive to generic programming.

#if defined(_MSC_VER)
namespace sg14 {
    // sg14::is_integral
    template<>
    struct is_integral<__int64> : std::true_type {
    };

    template<>
    struct is_integral<unsigned __int64> : std::true_type {
    };
}
#endif

#if defined(_GLIBCXX_USE_INT128)
// sg14 trait specializations required to use 128-bit integers with fixed_point under GCC/Clang
namespace sg14 {
    // sg14::is_integral - related to https://llvm.org/bugs/show_bug.cgi?id=23156
    template<>
    struct is_integral<__int128> : std::true_type {
    };

    template<>
    struct is_integral<unsigned __int128> : std::true_type {
    };

    // sg14::is_signed
    template<>
    struct is_signed<__int128> : std::true_type {
    };

    template<>
    struct is_signed<unsigned __int128> : std::false_type {
    };

    // sg14::is_signed
    template<>
    struct is_unsigned<__int128> : std::false_type {
    };

    template<>
    struct is_unsigned<unsigned __int128> : std::true_type {
    };

    // sg14::make_signed
    template<>
    struct make_signed<__int128> {
        using type = __int128;
    };

    template<>
    struct make_signed<unsigned __int128> {
        using type = __int128;
    };

    // sg14::make_signed
    template<>
    struct make_unsigned<__int128> {
        using type = unsigned __int128;
    };

    template<>
    struct make_unsigned<unsigned __int128> {
        using type = __int128;
    };

    // sg14::resize
    template<std::size_t NumBytes>
    struct resize<__int128, NumBytes> : resize<signed, NumBytes> {
    };

    template<std::size_t NumBytes>
    struct resize<unsigned __int128, NumBytes> : resize<unsigned, NumBytes> {
    };

    // sg14::width
    template<>
    struct width<__int128> : std::integral_constant<int, sizeof(__int128)*CHAR_BIT> {
    };

    template<>
    struct width<unsigned __int128> : std::integral_constant<int, sizeof(unsigned __int128)*CHAR_BIT> {
    };
}
#endif

#endif //_SG14_INT128_H
