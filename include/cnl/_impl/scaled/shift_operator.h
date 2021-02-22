
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_SHIFT_OPERATOR_H)
#define CNL_IMPL_SCALED_SHIFT_OPERATOR_H

#include "../num_traits/scale.h"
#include "../operators/custom_operator.h"
#include "../operators/tagged.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    // shift_operator of scaled_integer and scaled_integer
    template<_impl::shift_op Operator, typename Lhs, int LhsExponent, typename Rhs, int Radix>
    struct custom_operator<Operator, operand<Lhs, power<LhsExponent, Radix>>, operand<Rhs>>
        : Operator {
    };

    // shift_operator of scaled_integer and something else
    template<_impl::shift_op Operator, typename Lhs, int LhsExponent, typename Rhs, int Radix>
    struct custom_operator<Operator, operand<Lhs, power<LhsExponent, Radix>>, operand<Rhs, power<0, Radix>>>
        : custom_operator<Operator, operand<Lhs, power<LhsExponent, Radix>>, operand<Rhs>> {
    };
}

#endif  // CNL_IMPL_SCALED_SHIFT_OPERATOR_H
