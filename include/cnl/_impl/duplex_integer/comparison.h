
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_COMPARISON_H)
#define CNL_IMPL_DUPLEX_INTEGER_COMPARISON_H

#include "../operators/custom_operator.h"
#include "../type_traits/common_type.h"
#include "definition.h"
#include "is_duplex_integer.h"
#include "numeric_limits.h"

#include <tuple>

/// compositional numeric library
namespace cnl {
    template<_impl::comparison_op Operator, typename Upper, typename Lower>
    struct custom_operator<
            Operator,
            op_value<_impl::duplex_integer<Upper, Lower>>,
            op_value<_impl::duplex_integer<Upper, Lower>>> {
        using result_tag = _impl::native_tag;

        [[nodiscard]] constexpr auto operator()(
                _impl::duplex_integer<Upper, Lower> const& lhs,
                _impl::duplex_integer<Upper, Lower> const& rhs) const -> bool
        {
            using tuple = std::tuple<Upper const&, Lower const&>;
            return Operator{}(tuple(lhs.upper(), lhs.lower()), tuple(rhs.upper(), rhs.lower()));
        }
    };

    template<_impl::comparison_op Operator, typename Lhs, typename Rhs>
    requires(_impl::is_duplex_integer<Lhs>::value != _impl::is_duplex_integer<Rhs>::value) struct custom_operator<Operator, op_value<Lhs>, op_value<Rhs>> {
        using result_tag = _impl::native_tag;

        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> bool
        {
            using common_type = _impl::common_type_t<Lhs, Rhs>;
            return custom_operator<Operator, op_value<common_type>, op_value<common_type>>{}(lhs, rhs);
        }
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_COMPARISON_H
