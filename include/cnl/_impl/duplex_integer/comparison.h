
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_COMPARISON_H)
#define CNL_IMPL_DUPLEX_INTEGER_COMPARISON_H 1

#include "is_duplex_integer.h"
#include "numeric_limits.h"
#include "type.h"
#include "wants_generic_ops.h"
#include "../operators.h"
#include "../type_traits/common_type.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
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
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_COMPARISON_H
