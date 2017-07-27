
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief specialization of `std::numeric_limits` for 128-bit integer types

#if !defined(CNL_LIMITS_H)
#define CNL_LIMITS_H 1

#include "config.h"

#include <climits>
#include <cstdint>
#include <limits>

// CNL_NUMERIC_LIMITS_128_PROVIDED defined if
// standard library specializes std::numeric_limits for 128-bit integer
#if !defined(__clang__) && defined(__GNUG__) && (__cplusplus <= 201402L)
#define CNL_NUMERIC_LIMITS_128_PROVIDED
#elif defined(CNL_NUMERIC_LIMITS_128_PROVIDED)
#error CNL_NUMERIC_LIMITS_128_PROVIDED already defined
#endif

#if defined(CNL_INT128_ENABLED) && !defined(CNL_NUMERIC_LIMITS_128_PROVIDED)

namespace std {
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
}

#endif  // CNL_INT128_ENABLED

#endif  // CNL_LIMITS_H
