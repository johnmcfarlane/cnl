
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_INC_DEC_OPERATOR_H)
#define CNL_IMPL_NUMBER_INC_DEC_OPERATOR_H

#include "../operators/generic.h"
#include "../operators/native_tag.h"
#include "definition.h"
#include "rep_of.h"
#include "to_rep.h"

namespace cnl {
    template<_impl::pre_op Operator, _impl::wrapper Number>
    struct pre_operator<Operator, _impl::native_tag, Number> {
        constexpr Number& operator()(Number& rhs) const
        {
            pre_operator<Operator, _impl::tag_of_t<Number>, _impl::rep_of_t<Number>>{}(
                    _impl::to_rep(rhs));
            return rhs;
        }
    };

    template<_impl::post_op Operator, _impl::wrapper Number>
    struct post_operator<Operator, _impl::native_tag, Number> {
        constexpr Number operator()(Number& lhs) const
        {
            return _impl::from_rep<Number>(
                    post_operator<Operator, _impl::tag_of_t<Number>, _impl::rep_of_t<Number>>{}(
                            _impl::to_rep(lhs)));
        }
    };
}

#endif  // CNL_IMPL_NUMBER_INC_DEC_OPERATOR_H
