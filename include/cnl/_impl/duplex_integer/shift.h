
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_SHIFT_H)
#define CNL_IMPL_DUPLEX_INTEGER_SHIFT_H

#include "../operators/generic.h"
#include "../operators/operators.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    template<typename Upper, typename Lower, typename Rhs>
    struct shift_operator<
            _impl::shift_left_op,
            _impl::native_tag, _impl::native_tag,
            _impl::duplex_integer<Upper, Lower>, Rhs> {
        using _duplex_integer = _impl::duplex_integer<Upper, Lower>;

        CNL_NODISCARD constexpr auto operator()(_duplex_integer const& lhs, Rhs const& rhs) const
        -> _duplex_integer
        {
            return with_int(lhs, static_cast<int>(rhs));
        }

    private:
        CNL_NODISCARD constexpr auto with_int(_duplex_integer const& lhs, int const& rhs) const
        -> _duplex_integer
        {
            return _duplex_integer(
                    _impl::sensible_left_shift<Upper>(lhs.upper(), rhs)
                            | _impl::extra_sensible_right_shift<Upper>(lhs.lower(), _impl::width<Lower>::value-rhs),
                    _impl::sensible_left_shift<Lower>(lhs.lower(), rhs));
        }
    };

    template<typename Upper, typename Lower, typename Rhs>
    struct shift_operator<
            _impl::shift_right_op,
            _impl::native_tag, _impl::native_tag,
            _impl::duplex_integer<Upper, Lower>, Rhs> {
        using _duplex_integer = _impl::duplex_integer<Upper, Lower>;

        CNL_NODISCARD constexpr auto operator()(_duplex_integer const& lhs, Rhs const& rhs) const
        -> _duplex_integer
        {
            return with_int(lhs, static_cast<int>(rhs));
        }

    private:
        CNL_NODISCARD constexpr auto with_int(_duplex_integer const& lhs, int rhs) const
        -> _duplex_integer
        {
            return _duplex_integer(calculate_upper(lhs, rhs), calculate_lower(lhs, rhs));
        }

        CNL_NODISCARD constexpr auto calculate_upper(_duplex_integer const& lhs, int rhs) const
        -> Upper
        {
            return _impl::sensible_right_shift<Upper>(lhs.upper(), rhs);
        }

        CNL_NODISCARD constexpr auto calculate_lower(_duplex_integer const& lhs, int rhs) const
        -> Lower
        {
            return static_cast<Lower>(
                    _impl::sensible_right_shift<Lower>(lhs.lower(), rhs)
                            | _impl::extra_sensible_right_shift<Lower>(lhs.upper(), rhs-_impl::width<Lower>::value));
        }
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_SHIFT_H
