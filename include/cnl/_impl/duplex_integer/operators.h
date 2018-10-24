
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_OPERATORS_H)
#define CNL_IMPL_DUPLEX_INTEGER_OPERATORS_H 1

#include "../num_traits/to_rep.h"
#include "../operators.h"
#include "numeric_limits.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Lower, typename Upper>
        struct wants_generic_ops<duplex_integer<Lower, Upper>> : std::true_type {
        };

        template<typename Lower, typename Upper>
        struct comparison_operator<equal_op, duplex_integer<Lower, Upper>, duplex_integer<Lower, Upper>> {
            using duplex_integer = _impl::duplex_integer<Lower, Upper>;

            constexpr auto operator()(duplex_integer const& lhs, duplex_integer const& rhs) const -> bool
            {
                return lhs.lower()==rhs.lower() && lhs.upper()==rhs.upper();
            }
        };

        template<typename Lower, typename Upper>
        struct comparison_operator<not_equal_op, duplex_integer<Lower, Upper>, duplex_integer<Lower, Upper>> {
            using duplex_integer = _impl::duplex_integer<Lower, Upper>;

            constexpr auto operator()(duplex_integer const& lhs, duplex_integer const& rhs) const -> bool
            {
                return lhs.lower()!=rhs.lower() || lhs.upper()!=rhs.upper();
            }
        };

        template<typename Lower, typename Upper>
        struct comparison_operator<less_than_op, duplex_integer<Lower, Upper>, duplex_integer<Lower, Upper>> {
            using duplex_integer = _impl::duplex_integer<Lower, Upper>;

            constexpr auto operator()(duplex_integer const& lhs, duplex_integer const& rhs) const -> bool
            {
                return lhs.upper()<rhs.upper() || (lhs.upper()==rhs.upper() && lhs.lower()<rhs.lower());
            }
        };

        template<typename Lower, typename Upper>
        struct comparison_operator<less_than_or_equal_op, duplex_integer<Lower, Upper>, duplex_integer<Lower, Upper>> {
            using duplex_integer = _impl::duplex_integer<Lower, Upper>;

            constexpr auto operator()(duplex_integer const& lhs, duplex_integer const& rhs) const -> bool
            {
                return lhs.upper()<=rhs.upper() || (lhs.upper()==rhs.upper() && lhs.lower()<=rhs.lower());
            }
        };

        template<typename Lower, typename Upper>
        struct comparison_operator<greater_than_op, duplex_integer<Lower, Upper>, duplex_integer<Lower, Upper>> {
            using duplex_integer = _impl::duplex_integer<Lower, Upper>;

            constexpr auto operator()(duplex_integer const& lhs, duplex_integer const& rhs) const -> bool
            {
                return lhs.upper()>rhs.upper() || (lhs.upper()==rhs.upper() && lhs.lower()>rhs.lower());
            }
        };

        template<typename Lower, typename Upper>
        struct comparison_operator<greater_than_or_equal_op, duplex_integer<Lower, Upper>, duplex_integer<Lower, Upper>> {
            using duplex_integer = _impl::duplex_integer<Lower, Upper>;

            constexpr auto operator()(duplex_integer const& lhs, duplex_integer const& rhs) const -> bool
            {
                return lhs.upper()>=rhs.upper() || (lhs.upper()==rhs.upper() && lhs.lower()>=rhs.lower());
            }
        };
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_OPERATORS_H
