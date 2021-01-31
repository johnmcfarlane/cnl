
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_MODULO_H)
#define CNL_IMPL_DUPLEX_INTEGER_MODULO_H

#include "../../wide_integer.h"
#include "../operators/custom_operator.h"
#include "../operators/operators.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // cnl::_impl::heterogeneous_duplex_modulo_operator
        template<typename Lhs, typename Rhs>
        struct heterogeneous_duplex_modulo_operator {
            using common_type = rep_of_t<wide_integer<
                    max(digits<Lhs>, digits<Rhs>),
                    set_signedness_t<int, is_signed<Lhs> | is_signed<Rhs>>>>;

            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> Lhs
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
            operand<_impl::duplex_integer<Upper, Lower>>,
            operand<_impl::duplex_integer<Upper, Lower>>> {
        using _duplex_integer = _impl::duplex_integer<Upper, Lower>;
        using _unsigned_duplex_integer = remove_signedness_t<_duplex_integer>;

        CNL_NODISCARD constexpr auto operator()(
                _duplex_integer const& lhs, _duplex_integer const& rhs) const -> _duplex_integer
        {
            return lhs - rhs * (lhs / rhs);
        }
    };

    template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
    struct custom_operator<
            _impl::modulo_op,
            operand<_impl::duplex_integer<LhsUpper, LhsLower>>,
            operand<_impl::duplex_integer<RhsUpper, RhsLower>>>
        : _impl::heterogeneous_duplex_modulo_operator<
                  _impl::duplex_integer<LhsUpper, LhsLower>,
                  _impl::duplex_integer<RhsUpper, RhsLower>> {
    };

    template<typename Lhs, typename RhsUpper, typename RhsLower>
    struct custom_operator<
            _impl::modulo_op,
            operand<Lhs>,
            operand<_impl::duplex_integer<RhsUpper, RhsLower>>>
        : _impl::heterogeneous_duplex_modulo_operator<
                  Lhs, _impl::duplex_integer<RhsUpper, RhsLower>> {
    };

    template<typename LhsUpper, typename LhsLower, typename Rhs>
    struct custom_operator<
            _impl::modulo_op,
            operand<_impl::duplex_integer<LhsUpper, LhsLower>>,
            operand<Rhs>>
        : _impl::heterogeneous_duplex_modulo_operator<
                  _impl::duplex_integer<LhsUpper, LhsLower>, Rhs> {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_MODULO_H
