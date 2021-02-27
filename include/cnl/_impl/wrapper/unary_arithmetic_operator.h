
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_UNARY_OPERATOR_H)
#define CNL_IMPL_WRAPPER_UNARY_OPERATOR_H

#include "../operators/custom_operator.h"
#include "../operators/native_tag.h"
#include "../operators/overloads.h"
#include "from_rep.h"
#include "is_wrapper.h"
#include "rep_of.h"
#include "tag_of.h"
#include "to_rep.h"

/// compositional numeric library
namespace cnl {
    template<_impl::unary_arithmetic_op Operator, _impl::any_wrapper Rhs>
    struct custom_operator<Operator, op_value<Rhs>> {
        using result_tag = _impl::native_tag;

        [[nodiscard]] constexpr auto operator()(Rhs const& rhs) const
        {
            using rep_type = _impl::rep_of_t<Rhs>;
            using tag_type = _impl::tag_of_t<Rhs>;
            return _impl::from_rep<Rhs>(
                    custom_operator<Operator, op_value<rep_type, tag_type>>{}(_impl::to_rep(rhs)));
        }
    };
}

#endif  // CNL_IMPL_WRAPPER_UNARY_OPERATOR_H
