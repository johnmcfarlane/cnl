
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

namespace std {
#if defined(SG14_INT128_ENABLED)
    template<>
    struct numeric_limits<SG14_INT128> : numeric_limits<long long> {
        static const int digits = CHAR_BIT*sizeof(SG14_INT128)-1;
        static const int digits10 = 38;

        struct _s {
            constexpr _s(uint64_t upper, uint64_t lower) : value(lower + (SG14_INT128{upper} << 64)) {}
            constexpr operator SG14_INT128() const { return value; }
            SG14_INT128 value;
        };

        static constexpr SG14_INT128 min()
        {
            return _s(0x8000000000000000, 0x0000000000000000);
        }

        static constexpr SG14_INT128 max()
        {
            return _s(0x7fffffffffffffff, 0xffffffffffffffff);
        }

        static constexpr SG14_INT128 lowest()
        {
            return min();
        }
    };

    template<>
    struct numeric_limits<SG14_UINT128> : numeric_limits<unsigned long long> {
        static const int digits = CHAR_BIT*sizeof(SG14_INT128);
        static const int digits10 = 38;

        struct _s {
            constexpr _s(uint64_t upper, uint64_t lower) : value(lower + (SG14_UINT128{upper} << 64)) {}
            constexpr operator SG14_INT128() const { return value; }
            SG14_UINT128 value;
        };

        static constexpr SG14_INT128 min()
        {
            return 0;
        }

        static constexpr SG14_INT128 max()
        {
            return _s(0xffffffffffffffff, 0xffffffffffffffff);
        }

        static constexpr SG14_INT128 lowest()
        {
            return min();
        }
    };
#endif
}

#endif // SG14_INT128_H
