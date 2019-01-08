#include "shift.h"

//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_OPERATORS_H)
#define CNL_IMPL_DUPLEX_INTEGER_OPERATORS_H 1

#include "comparison.h"
#include "ctors.h"
#include "digits.h"
#include "divide.h"
#include "make_signed.h"
#include "multiply.h"
#include "numeric_limits.h"
#include "set_digits.h"
#include "type.h"
#include "../common.h"
#include "../num_traits/width.h"
#include "../operators.h"
#include "../wide_integer/type.h"
#include "../wide_integer/operators.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
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
            using wide_lower = set_digits_t<set_signedness_t<Lower, true>, lower_digits+1>;

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
            constexpr auto operator()(duplex_integer<Upper, Lower> const& rhs) const
            -> duplex_integer<Upper, Lower>
            {
                return unary_operator<bitwise_not_op, duplex_integer<Upper, Lower>>{}(
                        rhs-duplex_integer<Upper, Lower>{1});
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

        template<typename Operator, typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
        struct comparison_operator<Operator, duplex_integer<LhsUpper, LhsLower>, duplex_integer<RhsUpper, RhsLower>> {
            constexpr auto operator()(
                    duplex_integer<LhsUpper, LhsLower> const& lhs,
                    duplex_integer<RhsUpper, RhsLower> const& rhs) const -> bool
            {
                using common_type = duplex_integer<
                        common_type_t<LhsUpper, RhsUpper>,
                        common_type_t<LhsLower, RhsLower>>;
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
