
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief CNL alternative to \verbatim<limits>\endverbatim with 128-bit support

#if !defined(CNL_LIMITS_H)
#define CNL_LIMITS_H

#include "cstdint.h"

#include <climits>
#include <limits>

namespace cnl {

    template<class T>
    struct numeric_limits : std::numeric_limits<T> {};

#if defined(CNL_INT128_ENABLED)

    template<>
    struct numeric_limits<int128> : numeric_limits<long long> {
        static int const digits = CHAR_BIT*sizeof(int128)-1;
        static int const digits10 = 38;

        class _s {
        public:
            constexpr _s(uint64 upper, uint64 lower)  // NOLINT(cppcoreguidelines-pro-type-member-init)
                    :value(lower+(int128{upper} << 64)) { }

            CNL_NODISCARD explicit constexpr operator int128() const
            {
                return value;
            }
        private:
            int128 value;
        };

        CNL_NODISCARD static constexpr int128 min()
        {
            return int128(_s(0x8000000000000000, 0x0000000000000000));
        }

        CNL_NODISCARD static constexpr int128 max()
        {
            return int128(_s(0x7fffffffffffffff, 0xffffffffffffffff));
        }

        CNL_NODISCARD static constexpr int128 lowest()
        {
            return min();
        }
    };

    template<>
    struct numeric_limits<uint128> : numeric_limits<unsigned long long> {
        static int const digits = CHAR_BIT*sizeof(int128);
        static int const digits10 = 38;

        class _s {
        public:
            constexpr _s(uint64 upper, uint64 lower)  // NOLINT(cppcoreguidelines-pro-type-member-init)
                    :value(lower+(uint128{upper} << 64)) { }

            CNL_NODISCARD explicit constexpr operator uint128() const
            {
                return value;
            }
        private:
            uint128 value;
        };

        CNL_NODISCARD static constexpr int128 min()
        {
            return 0;
        }

        CNL_NODISCARD static constexpr uint128 max()
        {
            return uint128(_s(0xffffffffffffffff, 0xffffffffffffffff));
        }

        CNL_NODISCARD static constexpr int128 lowest()
        {
            return min();
        }
    };

#endif  // CNL_INT128_ENABLED
}

#endif  // CNL_LIMITS_H
