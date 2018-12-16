
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_OPERATORS_H)
#define CNL_IMPL_DUPLEX_INTEGER_OPERATORS_H 1

#include "ctors.h"
#include "digits.h"
#include "is_duplex_integer.h"
#include "make_signed.h"
#include "make_unsigned.h"
#include "numeric_limits.h"
#include "set_digits.h"
#include "set_width.h"
#include "to_rep.h"
#include "type.h"
#include "../common.h"
#include "../num_traits/to_rep.h"
#include "../operators.h"
#include "../type_traits/common_type.h"
#include "../type_traits/set_signedness.h"
#include "../wide_integer/type.h"
#include "../wide_integer/operators.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // long_multiply - T should be same width as operands
        template<typename T>
        struct long_multiply;

        // int64
        template<typename Word>
        struct long_multiply {
            template<typename Lhs, typename Rhs>
            using result_type = set_width_t<Word, width<Lhs>::value+width<Rhs>::value>;

            template<typename Lhs, typename Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> result_type<Lhs, Rhs>
            {
                using result_type = result_type<Lhs, Rhs>;
                return static_cast<result_type>(lhs)*static_cast<result_type>(rhs);
            }
        };

        // duplex_integer<int64, int64>
        template<typename Upper, typename Lower>
        struct long_multiply<duplex_integer<Upper, Lower>> {
            using result_type = duplex_integer<duplex_integer<Upper, Lower>, duplex_integer<Lower, Lower>>;

            template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
            constexpr auto operator()(
                    duplex_integer<LhsUpper, LhsLower> const& lhs,
                    duplex_integer<RhsUpper, RhsLower> const& rhs) const
            -> result_type
            {
                return multiply_components(lhs.upper(), lhs.lower(), rhs.upper(), rhs.lower());
            }

            template<typename Lhs, typename RhsUpper, typename RhsLower>
            constexpr auto operator()(
                    Lhs const& lhs,
                    duplex_integer<RhsUpper, RhsLower> const& rhs) const
            -> result_type
            {
                return multiply_components(0, lhs, rhs.upper(), rhs.lower());
            }

            template<typename LhsUpper, typename LhsLower, typename Rhs>
            constexpr auto operator()(
                    duplex_integer<LhsUpper, LhsLower> const& lhs,
                    Rhs const& rhs) const
            -> result_type
            {
                return multiply_components(lhs.upper(), lhs.lower(), 0, rhs);
            }

            template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
            static constexpr auto multiply_components(
                    LhsUpper const& lhs_upper, LhsLower const& lhs_lower,
                    RhsUpper const& rhs_upper, RhsLower const& rhs_lower)
            -> result_type
            {
                return ((result_type{_impl::long_multiply<Upper>{}(lhs_upper, rhs_upper)})
                        << (digits<Lower>::value+digits<Upper>::value))
                        +((result_type{_impl::long_multiply<Upper>{}(lhs_lower, rhs_upper)}
                                +result_type{_impl::long_multiply<Upper>{}(lhs_upper, rhs_lower)})
                                << digits<Lower>::value)
                        +((result_type{_impl::long_multiply<Lower>{}(lhs_lower, rhs_lower)}));
            }
        };

        // wants_generic_ops
        template<typename Upper, typename Lower>
        struct wants_generic_ops<duplex_integer<Upper, Lower>> : std::true_type {
        };

        // default_binary_operator
        template<class Operator, typename Upper, typename Lower>
        struct default_binary_operator {
            using _duplex_integer = duplex_integer<Upper, Lower>;

            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const
            -> _duplex_integer
            {
                return _duplex_integer(
                        static_cast<Upper>(Operator{}(lhs.upper(), rhs.upper())),
                        static_cast<Lower>(Operator{}(lhs.lower(), rhs.lower())));
            }
        };

        // add_op and subtract_op
        template<class Operator, typename Upper, typename Lower>
        struct first_degree_binary_operator {
            using _duplex_integer = duplex_integer<Upper, Lower>;

            static constexpr auto lower_digits = digits<Lower>::value;
            using wide_lower = set_digits_t<Lower, lower_digits+1>;

            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const
            -> _duplex_integer
            {
                return from_sums(
                        static_cast<Upper>(Operator{}(lhs.upper(), rhs.upper())),
                        wide_lower(Operator{}(wide_lower{lhs.lower()}, wide_lower{rhs.lower()})));
            }

            static constexpr auto from_sums(Upper const& upper_sum, wide_lower const& lower_sum)
            -> _duplex_integer
            {
                return _duplex_integer{
                        static_cast<Upper>(upper_sum+static_cast<Upper>(lower_sum >> constant<lower_digits>{})),
                        static_cast<Lower>(lower_sum)};
            }
        };

        // unary_operator
        template<typename Upper, typename Lower>
        struct unary_operator<bitwise_not_op, duplex_integer<Upper, Lower>> {
            constexpr auto operator()(duplex_integer<Upper, Lower> const& rhs) const
            -> duplex_integer<Upper, Lower>
            {
                return duplex_integer<Upper, Lower>(~rhs.upper(), ~rhs.lower());
            }
        };

        template<typename Upper, typename Lower>
        struct unary_operator<minus_op, duplex_integer<Upper, Lower>> {
            constexpr auto operator()(duplex_integer<Upper, Lower> rhs) const
            -> duplex_integer<Upper, Lower>
            {
                return unary_operator<bitwise_not_op, duplex_integer<Upper, Lower>>{}(--rhs);
            }
        };

        template<typename Upper, typename Lower>
        struct unary_operator<plus_op, duplex_integer<Upper, Lower>> {
            constexpr auto operator()(duplex_integer<Upper, Lower> const& rhs) const
            -> duplex_integer<Upper, Lower>
            {
                return duplex_integer<Upper, Lower>(+rhs.upper(), +rhs.lower());
            }
        };

        // binary_operator
        template<typename Upper, typename Lower>
        struct binary_operator<add_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>>
                : first_degree_binary_operator<add_op, Upper, Lower> {
        };

        template<typename Upper, typename Lower>
        struct binary_operator<subtract_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>>
                : first_degree_binary_operator<subtract_op, Upper, Lower> {
        };

        template<typename Upper, typename Lower>
        struct binary_operator<multiply_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>> {
            using _duplex_integer = duplex_integer<Upper, Lower>;

            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const
            -> _duplex_integer
            {
                return multiply_components(lhs.upper(), lhs.lower(), rhs.upper(), rhs.lower());
            }

            static constexpr auto multiply_components(
                    Upper const& lhs_upper,
                    Lower const& lhs_lower,
                    Upper const& rhs_upper,
                    Lower const& rhs_lower)
            -> _duplex_integer
            {
                using common_result_type = decltype(long_multiply<Upper>{}(lhs_upper, rhs_upper));
                return (long_multiply<Upper>{}(lhs_upper, rhs_upper) << digits<Upper>::value)
                        +((long_multiply<Upper>{}(lhs_upper, rhs_lower)+long_multiply<Upper>{}(lhs_lower, rhs_upper))
                                << digits<Lower>::value)
                        +static_cast<common_result_type>(long_multiply<Lower>{}(lhs_lower, rhs_lower));
            }
        };

        template<typename Upper, typename Lower>
        struct binary_operator<divide_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>> {
            using _duplex_integer = duplex_integer<Upper, Lower>;
            using _unsigned_duplex_integer = make_unsigned_t<_duplex_integer>;

            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const
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

            // lifted from: https://github.com/torvalds/linux/blob/master/lib/div64.c#L142
            static CNL_RELAXED_CONSTEXPR auto non_negative_division(
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

            static CNL_RELAXED_CONSTEXPR auto fls(Upper n) -> int
            {
                auto half_digits = numeric_limits<_unsigned_duplex_integer>::digits/2;

                if (!n) {
                    return 0;
                }
                for (int r = half_digits;; n <<= 1, r--) {
                    if (n & Upper(1ULL << (half_digits-1))) {
                        return r;
                    }
                }
            };

            // from Linux div64_32
            static CNL_RELAXED_CONSTEXPR auto
            div_by_lower(_unsigned_duplex_integer const& dividend, Lower const& divisor)
            -> _unsigned_duplex_integer
            {
                _unsigned_duplex_integer rem = dividend;
                _unsigned_duplex_integer b = divisor;
                _unsigned_duplex_integer d = 1;

                using unsigned_upper = set_signedness_t<Upper, false>;
                auto high = rem.upper();

                _unsigned_duplex_integer quot = 0;
                if (static_cast<unsigned_upper>(high)>=divisor) {
                    high /= divisor;
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

        template<typename Upper, typename Lower>
        struct binary_operator<bitwise_or_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>>
                : default_binary_operator<bitwise_or_op, Upper, Lower> {
        };

        template<typename Upper, typename Lower>
        struct binary_operator<bitwise_and_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>>
                : default_binary_operator<bitwise_and_op, Upper, Lower> {
        };

        template<typename Upper, typename Lower>
        struct binary_operator<bitwise_xor_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>>
                : default_binary_operator<bitwise_xor_op, Upper, Lower> {
        };

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

        // comparison_operator
        template<typename Upper, typename Lower>
        struct comparison_operator<equal_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>> {
            using _duplex_integer = duplex_integer<Upper, Lower>;

            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const -> bool
            {
                return lhs.lower()==rhs.lower() && lhs.upper()==rhs.upper();
            }
        };

        template<typename Upper, typename Lower>
        struct comparison_operator<not_equal_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>> {
            using _duplex_integer = duplex_integer<Upper, Lower>;

            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const -> bool
            {
                return lhs.lower()!=rhs.lower() || lhs.upper()!=rhs.upper();
            }
        };

        template<typename Upper, typename Lower>
        struct comparison_operator<less_than_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>> {
            using _duplex_integer = duplex_integer<Upper, Lower>;

            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const -> bool
            {
                return lhs.upper()<rhs.upper() || (lhs.upper()==rhs.upper() && lhs.lower()<rhs.lower());
            }
        };

        template<typename Upper, typename Lower>
        struct comparison_operator<less_than_or_equal_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>> {
            using _duplex_integer = duplex_integer<Upper, Lower>;

            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const -> bool
            {
                return !comparison_operator<greater_than_op, _duplex_integer, _duplex_integer>{}(lhs, rhs);
            }
        };

        template<typename Upper, typename Lower>
        struct comparison_operator<greater_than_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>> {
            using _duplex_integer = duplex_integer<Upper, Lower>;

            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const -> bool
            {
                return lhs.upper()>rhs.upper() || (lhs.upper()==rhs.upper() && lhs.lower()>rhs.lower());
            }
        };

        template<typename Upper, typename Lower>
        struct comparison_operator<greater_than_or_equal_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>> {
            using _duplex_integer = duplex_integer<Upper, Lower>;

            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const -> bool
            {
                return !comparison_operator<less_than_op, _duplex_integer, _duplex_integer>{}(lhs, rhs);
            }
        };

        template<class Operator, typename Lhs, typename Rhs>
        struct comparison_operator<Operator, Lhs, Rhs,
                enable_if_t<is_duplex_integer<Lhs>::value!=is_duplex_integer<Rhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> bool
            {
                using common_type = common_type_t<Lhs, Rhs>;
                return comparison_operator<Operator, common_type, common_type>{}(lhs, rhs);
            }
        };

        // pre_operator
        template<typename Upper, typename Lower>
        struct pre_operator<pre_increment_op, duplex_integer<Upper, Lower>> {
            constexpr auto operator()(duplex_integer<Upper, Lower>& rhs) const
            -> duplex_integer<Upper, Lower>
            {
                return CNL_UNLIKELY(rhs.lower()==numeric_limits<Lower>::max())
                       ? duplex_integer<Upper, Lower>{++rhs.upper(), numeric_limits<Lower>::lowest()}
                       : duplex_integer<Upper, Lower>{rhs.upper(), ++rhs.lower()};
            }
        };

        template<typename Upper, typename Lower>
        struct pre_operator<pre_decrement_op, duplex_integer<Upper, Lower>> {
            constexpr auto operator()(duplex_integer<Upper, Lower>& rhs) const
            -> duplex_integer<Upper, Lower>
            {
                return CNL_UNLIKELY(rhs.lower()==numeric_limits<Lower>::lowest())
                       ? duplex_integer<Upper, Lower>{static_cast<Upper>(--rhs.upper()), numeric_limits<Lower>::max()}
                       : duplex_integer<Upper, Lower>{rhs.upper(), --rhs.lower()};
            }
        };
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_OPERATORS_H
