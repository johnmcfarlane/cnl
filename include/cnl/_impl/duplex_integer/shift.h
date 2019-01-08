
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_SHIFT_H)
#define CNL_IMPL_DUPLEX_INTEGER_SHIFT_H 1

#include "type.h"
#include "../operators.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Upper, typename Lower, typename Rhs>
        struct binary_operator<shift_left_op, duplex_integer<Upper, Lower>, Rhs> {
            using _duplex_integer = duplex_integer<Upper, Lower>;

            constexpr auto operator()(_duplex_integer const& lhs, Rhs const& rhs) const
            -> _duplex_integer
            {
                return with_int(lhs, static_cast<int>(rhs));
            }

        private:
            constexpr auto with_int(_duplex_integer const& lhs, int const& rhs) const
            -> _duplex_integer
            {
                return _duplex_integer(
                        sensible_left_shift<Upper>(lhs.upper(), rhs)
                                | extra_sensible_right_shift<Upper>(lhs.lower(), width<Lower>::value-rhs),
                        sensible_left_shift<Lower>(lhs.lower(), rhs));
            }
        };

        template<typename Upper, typename Lower, typename Rhs>
        struct binary_operator<shift_right_op, duplex_integer<Upper, Lower>, Rhs> {
            using _duplex_integer = duplex_integer<Upper, Lower>;

            constexpr auto operator()(_duplex_integer const& lhs, Rhs const& rhs) const
            -> _duplex_integer
            {
                return with_int(lhs, static_cast<int>(rhs));
            }

        private:
            constexpr auto with_int(_duplex_integer const& lhs, int rhs) const
            -> _duplex_integer
            {
                return _duplex_integer(calculate_upper(lhs, rhs), calculate_lower(lhs, rhs));
            }

            constexpr auto calculate_upper(_duplex_integer const& lhs, int rhs) const
            -> Upper
            {
                return sensible_right_shift<Upper>(lhs.upper(), rhs);
            }

            constexpr auto calculate_lower(_duplex_integer const& lhs, int rhs) const
            -> Lower
            {
                return static_cast<Lower>(
                        sensible_right_shift<Lower>(lhs.lower(), rhs)
                                | extra_sensible_right_shift<Lower>(lhs.upper(), rhs-width<Lower>::value));
            }
        };
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_SHIFT_H
