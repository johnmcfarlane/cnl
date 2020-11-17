
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

    namespace _impl
    {
        // compose a 128-bit integer from two 64-bit integers
        template<typename Integer128>
        class join128 {
        public:
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
            constexpr join128(uint64 upper, uint64 lower)
                    :_value(lower+(Integer128{upper} << 64)) { }

            CNL_NODISCARD explicit constexpr operator Integer128() const
            {
                return _value;
            }
        private:
            Integer128 _value;
        };
    }

    template<>
    struct numeric_limits<int128> : numeric_limits<long long> {
        static int const digits = CHAR_BIT*sizeof(int128)-1;
        static int const digits10 = 38;

        CNL_NODISCARD static constexpr int128 min()
        {
            return int128(_impl::join128<int128>(0x8000000000000000, 0x0000000000000000));
        }

        CNL_NODISCARD static constexpr int128 max()
        {
            return int128(_impl::join128<int128>(0x7fffffffffffffff, 0xffffffffffffffff));
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

        CNL_NODISCARD static constexpr int128 min()
        {
            return 0;
        }

        CNL_NODISCARD static constexpr uint128 max()
        {
            return uint128(_impl::join128<uint128>(0xffffffffffffffff, 0xffffffffffffffff));
        }

        CNL_NODISCARD static constexpr uint128 lowest()
        {
            return min();
        }
    };

#endif  // CNL_INT128_ENABLED
}

#endif  // CNL_LIMITS_H
