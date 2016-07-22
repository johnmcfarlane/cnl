
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SG14_INT128_H
#define SG14_INT128_H

#include <limits>

#include "config.h"

// This file contains tweaks to standard traits that integers more conducive to generic programming.

namespace sg14 {
    ////////////////////////////////////////////////////////////////////////////////
    // MSVC++ hacks to get 64-bit integers working a little better

#if defined(_MSC_VER)
    // sg14::is_integral
    // TODO: is this really necessary? https://msdn.microsoft.com/en-us/library/bb983099.aspx
    template<>
    struct is_integral<__int64> : std::true_type {
    };

    template<>
    struct is_integral<unsigned __int64> : std::true_type {
    };
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // Clang/GCC hacks to get 128-bit integers working with fixed_point

#if defined(SG14_INT128_ENABLED)
    // sg14::is_integral - related to https://llvm.org/bugs/show_bug.cgi?id=23156
    template<>
    struct is_integral<SG14_INT128> : std::true_type {
    };

    template<>
    struct is_integral<SG14_UINT128> : std::true_type {
    };

    // sg14::is_signed
    template<>
    struct is_signed<SG14_INT128> : std::true_type {
    };

    template<>
    struct is_signed<SG14_UINT128> : std::false_type {
    };

    // sg14::is_signed
    template<>
    struct is_unsigned<SG14_INT128> : std::false_type {
    };

    template<>
    struct is_unsigned<SG14_UINT128> : std::true_type {
    };

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

    // sg14::resize
    template<_width_type MinNumBits>
    struct set_width<SG14_INT128, MinNumBits> : set_width<signed, MinNumBits> {
    };

    template<_width_type MinNumBits>
    struct set_width<SG14_UINT128, MinNumBits> : set_width<unsigned, MinNumBits> {
    };

    // sg14::width
    template<>
    struct width<SG14_INT128> : std::integral_constant<_width_type, sizeof(SG14_INT128)*CHAR_BIT> {
    };

    template<>
    struct width<SG14_UINT128> : std::integral_constant<_width_type, sizeof(SG14_UINT128)*CHAR_BIT> {
    };
#endif
}

#endif // SG14_INT128_H
