
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_MODULO_H)
#define CNL_IMPL_DUPLEX_INTEGER_MODULO_H

#include "../../wide_integer.h"
#include "../operators/custom_operator.h"
#include "../operators/operators.h"
#include "definition.h"
#include "numbers.h"

#include <algorithm>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // cnl::_impl::heterogeneous_duplex_modulo_operator
        template<typename Lhs, typename Rhs>
        struct heterogeneous_duplex_modulo_operator {
            using result_tag = _impl::native_tag;

            using common_type = rep_of_t<wide_integer<
                    std::max(digits<Lhs>, digits<Rhs>),
                    numbers::set_signedness_t<int, numbers::signedness_v<Lhs> | numbers::signedness_v<Rhs>>>>;

            [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> Lhs
            {
                return static_cast<Lhs>(
                        static_cast<common_type>(lhs) / static_cast<common_type>(rhs));
            }
        };
    }

    // duplex_integer<> % duplex_integer<>
    template<typename Upper, typename Lower>
    struct custom_operator<
            _impl::modulo_op,
            op_value<_impl::duplex_integer<Upper, Lower>>,
            op_value<_impl::duplex_integer<Upper, Lower>>> {
        using result_tag = _impl::native_tag;

        using _duplex_integer = _impl::duplex_integer<Upper, Lower>;
        using _unsigned_duplex_integer = numbers::set_signedness_t<_duplex_integer, false>;

        [[nodiscard]] constexpr auto operator()(
                _duplex_integer const& lhs, _duplex_integer const& rhs) const -> _duplex_integer
        {
            return lhs - rhs * (lhs / rhs);
        }
    };

    template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
    struct custom_operator<
            _impl::modulo_op,
            op_value<_impl::duplex_integer<LhsUpper, LhsLower>>,
            op_value<_impl::duplex_integer<RhsUpper, RhsLower>>>
        : _impl::heterogeneous_duplex_modulo_operator<
                  _impl::duplex_integer<LhsUpper, LhsLower>,
                  _impl::duplex_integer<RhsUpper, RhsLower>> {
    };

    template<typename Lhs, typename RhsUpper, typename RhsLower>
    struct custom_operator<
            _impl::modulo_op,
            op_value<Lhs>,
            op_value<_impl::duplex_integer<RhsUpper, RhsLower>>>
        : _impl::heterogeneous_duplex_modulo_operator<
                  Lhs, _impl::duplex_integer<RhsUpper, RhsLower>> {
    };

    template<typename LhsUpper, typename LhsLower, typename Rhs>
    struct custom_operator<
            _impl::modulo_op,
            op_value<_impl::duplex_integer<LhsUpper, LhsLower>>,
            op_value<Rhs>>
        : _impl::heterogeneous_duplex_modulo_operator<
                  _impl::duplex_integer<LhsUpper, LhsLower>, Rhs> {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_MODULO_H
