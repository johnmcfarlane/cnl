
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief specialization of `std::numeric_limits` for 128-bit integer types

#if !defined(SG14_LIMITS_H)
#define SG14_LIMITS_H 1

#include "config.h"

#include <climits>
#include <cstdint>
#include <limits>

// SG14_NUMERIC_LIMITS_128_PROVIDED defined if
// standard library specializes std::numeric_limits for 128-bit integer
#if !defined(__clang__) && defined(__GNUG__) && (__cplusplus <= 201402L)
#define SG14_NUMERIC_LIMITS_128_PROVIDED
#elif defined(SG14_NUMERIC_LIMITS_128_PROVIDED)
#error SG14_NUMERIC_LIMITS_128_PROVIDED already defined
#endif

#if defined(SG14_INT128_ENABLED) && !defined(SG14_NUMERIC_LIMITS_128_PROVIDED)

namespace std {
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
}

#endif  // SG14_INT128_ENABLED

#endif	// SG14_LIMITS_H
