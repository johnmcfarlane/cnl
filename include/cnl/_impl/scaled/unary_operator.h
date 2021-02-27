
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_UNARY_OPERATOR_H)
#define CNL_IMPL_SCALED_UNARY_OPERATOR_H

#include "../operators/custom_operator.h"
#include "../operators/operators.h"
#include "definition.h"

// #include <algorithm>

/// compositional numeric library
namespace cnl {
    template<_impl::unary_arithmetic_op Operator, typename Rhs, int Exponent, int Radix>
    struct custom_operator<Operator, op_value<Rhs, power<Exponent, Radix>>>
        : Operator {
    };
}

#endif  // CNL_IMPL_SCALED_UNARY_OPERATOR_H
