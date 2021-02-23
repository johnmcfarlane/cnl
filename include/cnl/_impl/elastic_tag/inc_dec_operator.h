
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_TAG_INC_DEC_OPERATOR_H)
#define CNL_IMPL_ELASTIC_TAG_INC_DEC_OPERATOR_H

#include "../operators/custom_operator.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<_impl::prefix_op Operator, typename Rhs, int Digits, typename Narrowest>
    struct custom_operator<Operator, op_value<Rhs, elastic_tag<Digits, Narrowest>>> : Operator {
    };

    template<_impl::postfix_op Operator, typename Lhs, int Digits, typename Narrowest>
    struct custom_operator<Operator, op_value<Lhs, elastic_tag<Digits, Narrowest>>> : Operator {
    };
}

#endif  // CNL_IMPL_ELASTIC_TAG_INC_DEC_OPERATOR_H
