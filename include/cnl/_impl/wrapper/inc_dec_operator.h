
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_INC_DEC_OPERATOR_H)
#define CNL_IMPL_WRAPPER_INC_DEC_OPERATOR_H

#include "../operators/generic.h"
#include "../operators/native_tag.h"
#include "definition.h"
#include "rep_of.h"
#include "to_rep.h"

namespace cnl {
    template<_impl::prefix_op Operator, _impl::wrapped Number>
    struct prefix_operator<Operator, _impl::native_tag, Number> {
        constexpr Number& operator()(Number& rhs) const
        {
            prefix_operator<Operator, _impl::tag_of_t<Number>, _impl::rep_of_t<Number>>{}(
                    _impl::to_rep(rhs));
            return rhs;
        }
    };

    template<_impl::postfix_op Operator, _impl::wrapped Number>
    struct postfix_operator<Operator, _impl::native_tag, Number> {
        constexpr Number operator()(Number& lhs) const
        {
            return _impl::from_rep<Number>(
                    postfix_operator<Operator, _impl::tag_of_t<Number>, _impl::rep_of_t<Number>>{}(
                            _impl::to_rep(lhs)));
        }
    };
}

#endif  // CNL_IMPL_WRAPPER_INC_DEC_OPERATOR_H
