
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief CNL alternative to \verbatim<limits>\endverbatim with 128-bit support

#if !defined(CNL_NUMERIC_LIMITS_H)
#define CNL_NUMERIC_LIMITS_H

#include "_impl/config.h"
#include "_impl/cstdint/types.h"

#include <climits>
#include <limits>

namespace cnl {

    template<class T>
    struct numeric_limits : std::numeric_limits<T> {
    };

    template<class T>
    struct numeric_limits<T const> : numeric_limits<T> {
    };

#if defined(CNL_INT128_ENABLED)

    namespace _impl {
        // compose a 128-bit integer from two 64-bit integers
        template<typename Integer128>
        class join128 {
        public:
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
            constexpr join128(std::uint64_t upper, std::uint64_t lower)
                : _value(lower + (Integer128{upper} << 64))
            {
            }

            [[nodiscard]] explicit constexpr operator Integer128() const
            {
                return _value;
            }

        private:
            Integer128 _value;
        };
    }

    template<>
    struct numeric_limits<int128_t> : numeric_limits<long long> {
        static int const digits = CHAR_BIT * sizeof(int128_t) - 1;
        static int const digits10 = 38;

        [[nodiscard]] static constexpr auto min()
        {
            return int128_t(_impl::join128<int128_t>(0x8000000000000000, 0x0000000000000000));
        }

        [[nodiscard]] static constexpr auto max()
        {
            return int128_t(_impl::join128<int128_t>(0x7fffffffffffffff, 0xffffffffffffffff));
        }

        [[nodiscard]] static constexpr auto lowest()
        {
            return min();
        }
    };

    template<>
    struct numeric_limits<uint128_t> : numeric_limits<unsigned long long> {
        static int const digits = CHAR_BIT * sizeof(int128_t);
        static int const digits10 = 38;

        [[nodiscard]] static constexpr auto min()
        {
            return 0;
        }

        [[nodiscard]] static constexpr auto max()
        {
            return uint128_t(_impl::join128<uint128_t>(0xffffffffffffffff, 0xffffffffffffffff));
        }

        [[nodiscard]] static constexpr auto lowest()
        {
            return min();
        }
    };

#endif  // CNL_INT128_ENABLED
}

#endif  // CNL_NUMERIC_LIMITS_H
