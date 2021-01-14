
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_TAG_INC_DEC_OPERATOR_H)
#define CNL_IMPL_ELASTIC_TAG_INC_DEC_OPERATOR_H

#include "../operators/generic.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<_impl::prefix_op Operator, int Digits, typename Narrowest, typename Rhs>
    struct prefix_operator<Operator, elastic_tag<Digits, Narrowest>, Rhs> : Operator {
    };

    template<_impl::postfix_op Operator, int Digits, typename Narrowest, typename Lhs>
    struct postfix_operator<Operator, elastic_tag<Digits, Narrowest>, Lhs> : Operator {
    };
}

#endif  // CNL_IMPL_ELASTIC_TAG_INC_DEC_OPERATOR_H
