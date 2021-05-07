
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_QUASI_EXACT_INC_DEC_OPERATOR_H)
#define CNL_IMPL_SCALED_QUASI_EXACT_INC_DEC_OPERATOR_H

#include "../../custom_operator/definition.h"
#include "../../power_value.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<_impl::prefix_op Operator, typename Rhs, int Exponent, int Radix>
    struct custom_operator<Operator, op_value<Rhs, quasi_exact<Exponent, Radix>>> {
        constexpr auto operator()(Rhs& rhs) const
        {
            return typename _impl::pre_to_assign<Operator>::type{}(
                    rhs, _impl::power_value<Rhs, -Exponent, Radix>());
        }
    };

    template<_impl::postfix_op Operator, typename Lhs, int Exponent, int Radix>
    struct custom_operator<Operator, op_value<Lhs, quasi_exact<Exponent, Radix>>> {
        constexpr auto operator()(Lhs& lhs) const -> Lhs
        {
            auto copy = lhs;
            typename _impl::post_to_assign<Operator>::type{}(
                    lhs, _impl::power_value<Lhs, -Exponent, Radix>());
            return copy;
        }
    };
}

#endif  // CNL_IMPL_SCALED_QUASI_EXACT_INC_DEC_OPERATOR_H
