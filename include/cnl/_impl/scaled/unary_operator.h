
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_UNARY_OPERATOR_H)
#define CNL_IMPL_SCALED_UNARY_OPERATOR_H

#include "../../integer.h"
#include "../custom_operator/definition.h"
#include "../custom_operator/op.h"
#include "is_scaled_tag.h"

/// compositional numeric library
namespace cnl {
    template<_impl::unary_arithmetic_op Operator, integer Rep, scaled_tag Scale>
    struct custom_operator<
            Operator, op_value<Rep, Scale>> {
        [[nodiscard]] constexpr auto operator()(Rep const& rhs) const
        {
            return Operator{}(rhs);
        }
    };
}

#endif  // CNL_IMPL_SCALED_UNARY_OPERATOR_H
