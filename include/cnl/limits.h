
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief CNL alternative to <limits> with 128-bit support

#if !defined(CNL_LIMITS_H)
#define CNL_LIMITS_H 1

#include "cnl/bits/config.h"

#include <climits>
#include <cstdint>
#include <limits>

namespace cnl {

    template<class T>
    struct numeric_limits : std::numeric_limits<T> {};

#if defined(CNL_INT128_ENABLED)

    template<>
    struct numeric_limits<CNL_INT128> : numeric_limits<long long> {
        static int const digits = CHAR_BIT*sizeof(CNL_INT128)-1;
        static int const digits10 = 38;

        struct _s {
            constexpr _s(uint64_t upper, uint64_t lower) : value(lower + (CNL_INT128{upper} << 64)) {}
            constexpr operator CNL_INT128() const { return value; }
            CNL_INT128 value;
        };

        static constexpr CNL_INT128 min()
        {
            return _s(0x8000000000000000, 0x0000000000000000);
        }

        static constexpr CNL_INT128 max()
        {
            return _s(0x7fffffffffffffff, 0xffffffffffffffff);
        }

        static constexpr CNL_INT128 lowest()
        {
            return min();
        }
    };

    template<>
    struct numeric_limits<CNL_UINT128> : numeric_limits<unsigned long long> {
        static int const digits = CHAR_BIT*sizeof(CNL_INT128);
        static int const digits10 = 38;

        struct _s {
            constexpr _s(uint64_t upper, uint64_t lower) : value(lower + (CNL_UINT128{upper} << 64)) {}
            constexpr operator CNL_INT128() const { return value; }
            CNL_UINT128 value;
        };

        static constexpr CNL_INT128 min()
        {
            return 0;
        }

        static constexpr CNL_INT128 max()
        {
            return _s(0xffffffffffffffff, 0xffffffffffffffff);
        }

        static constexpr CNL_INT128 lowest()
        {
            return min();
        }
    };

#endif  // CNL_INT128_ENABLED
}

#endif  // CNL_LIMITS_H
