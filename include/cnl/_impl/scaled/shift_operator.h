
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_SHIFT_OPERATOR_H)
#define CNL_IMPL_SCALED_SHIFT_OPERATOR_H

#include "../num_traits/scale.h"
#include "../operators/generic.h"
#include "../operators/tagged.h"
#include "../type_traits/enable_if.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    // shift_operator of scaled_integer and scaled_integer
    template<class Operator, int LhsExponent, int Radix, typename Lhs, typename Rhs>
    struct shift_operator<
            Operator,
            power<LhsExponent, Radix>, _impl::native_tag,
            Lhs, Rhs> : Operator {
    };

    // shift_operator of scaled_integer and something else
    template<class Operator, int LhsExponent, int Radix, typename Lhs, typename Rhs>
    struct shift_operator<Operator, power<LhsExponent, Radix>, power<0, Radix>, Lhs, Rhs>
            : shift_operator<Operator, power<LhsExponent, Radix>, _impl::native_tag, Lhs, Rhs> {
    };
}

#endif // CNL_IMPL_SCALED_SHIFT_OPERATOR_H
