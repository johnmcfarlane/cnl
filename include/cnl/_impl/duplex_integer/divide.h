
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_DIVIDE_H)
#define CNL_IMPL_DUPLEX_INTEGER_DIVIDE_H

#include "../operators/generic.h"
#include "../operators/native_tag.h"
#include "../operators/operators.h"
#include "../type_traits/set_signedness.h"
#include "../wide_integer/definition.h"
#include "ctors.h"
#include "numeric_limits.h"
#include "remove_signedness.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // cnl::_impl::heterogeneous_duplex_divide_operator
        template<typename Lhs, typename Rhs>
        struct heterogeneous_duplex_divide_operator {
            using common_type = rep_t<
                    wide_integer<
                        max(digits<Lhs>::value, digits<Rhs>::value),
                        set_signedness_t<int, is_signed<Lhs>::value|is_signed<Rhs>::value>>>;

            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> Lhs
            {
                return static_cast<Lhs>(static_cast<common_type>(lhs) / static_cast<common_type>(rhs));
            }
        };
    }

    // cnl::_impl::binary_operator<divide_op, duplex_integer<>, duplex_integer<>
    template<typename Upper, typename Lower>
    struct binary_operator<
            _impl::divide_op,
            _impl::native_tag, _impl::native_tag,
            _impl::duplex_integer<Upper, Lower>, _impl::duplex_integer<Upper, Lower>> {
        using _duplex_integer = _impl::duplex_integer<Upper, Lower>;
        using _unsigned_duplex_integer = remove_signedness_t<_duplex_integer>;

        CNL_NODISCARD constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const
        -> _duplex_integer
        {
            return (lhs<_duplex_integer{0})
                   ? (rhs<_duplex_integer{0})
                     ? non_negative_division(-lhs, -rhs)
                     : -non_negative_division(-lhs, rhs)
                   : (rhs<_duplex_integer{0})
                     ? -non_negative_division(lhs, -rhs)
                     : non_negative_division(lhs, rhs);
        }

        // lifted from: https://github.com/torvalds/linux/blob/5ac94332248ee017964ba368cdda4ce647e3aba7/lib/math/div64.c#L142
        static constexpr auto non_negative_division(
                _unsigned_duplex_integer const& dividend,
                _unsigned_duplex_integer const& divisor)
        -> _unsigned_duplex_integer
        {
            auto const high = divisor.upper();
            if (!high) {
                return div_by_lower(dividend, divisor.lower());
            }

            int n = fls(high);
            auto quot = div_by_lower(dividend >> n, (divisor >> n).lower());

            if (quot) {
                --quot;
            }
            if ((dividend-quot*divisor)>=divisor) {
                ++quot;
            }

            return quot;
        }

        static constexpr auto fls(Upper n) -> int
        {
            auto half_digits = numeric_limits<_duplex_integer>::digits/2;

            if (!n) {
                return 0;
            }
            auto const msd = Upper{1} << (half_digits-1);
            for (int r = half_digits;; n <<= 1, r--) {
                if (n & msd) {
                    return r;
                }
            }
        };

        // from Linux div64_32
        static constexpr auto
        div_by_lower(_unsigned_duplex_integer const& dividend, Lower const& divisor)
        -> _unsigned_duplex_integer
        {
            _unsigned_duplex_integer rem = dividend;
            _unsigned_duplex_integer b = divisor;
            _unsigned_duplex_integer d = 1;

            using unsigned_upper = _impl::set_signedness_t<Upper, false>;
            auto high = rem.upper();

            _unsigned_duplex_integer quot = 0;
            if (static_cast<unsigned_upper>(high)>=divisor) {
                high /= divisor;  // NOLINT(clang-analyzer-core.DivideZero)
                quot = _unsigned_duplex_integer{high, 0};
                rem -= _unsigned_duplex_integer(high*divisor, 0);
            }

            while (b<rem) {
                b <<= 1;
                d <<= 1;
            }

            do {
                if (rem>=b) {
                    rem -= b;
                    quot += d;
                }
                b >>= 1;
                d >>= 1;
            }
            while (d);

            return quot;
        };
    };

        template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
        struct binary_operator<
                _impl::divide_op,
                _impl::native_tag, _impl::native_tag,
                _impl::duplex_integer<LhsUpper, LhsLower>, _impl::duplex_integer<RhsUpper, RhsLower>>
                : _impl::heterogeneous_duplex_divide_operator<
                        _impl::duplex_integer<LhsUpper, LhsLower>, _impl::duplex_integer<RhsUpper, RhsLower>> {
        };

    template<typename Lhs, typename RhsUpper, typename RhsLower>
    struct binary_operator<
            _impl::divide_op,
            _impl::native_tag, _impl::native_tag,
            Lhs, _impl::duplex_integer<RhsUpper, RhsLower>>
            : _impl::heterogeneous_duplex_divide_operator<Lhs, _impl::duplex_integer<RhsUpper, RhsLower>> {
    };

    template<typename LhsUpper, typename LhsLower, typename Rhs>
    struct binary_operator<
            _impl::divide_op,
            _impl::native_tag, _impl::native_tag,
            _impl::duplex_integer<LhsUpper, LhsLower>, Rhs>
            : _impl::heterogeneous_duplex_divide_operator<_impl::duplex_integer<LhsUpper, LhsLower>, Rhs> {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_DIVIDE_H
