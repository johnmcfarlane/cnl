
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_QUASI_EXACT_CONVERT_OPERATOR_H)
#define CNL_IMPL_SCALED_QUASI_EXACT_CONVERT_OPERATOR_H

#include "../../../floating_point.h"
#include "../../../fraction.h"
#include "../../custom_operator/native_tag.h"
#include "../../num_traits/fixed_width_scale.h"
#include "../../num_traits/scale.h"
#include "../../power_value.h"
#include "../../scaled_integer/definition.h"

/// compositional numeric library
namespace cnl {
    template<number Src, int SrcExponent, number Dest, int DestExponent, int Radix>
    struct custom_operator<
            _impl::convert_op,
            op_value<Src, quasi_exact<SrcExponent, Radix>>,
            op_value<Dest, quasi_exact<DestExponent, Radix>>>
        : custom_operator<
                  _impl::convert_op,
                  op_value<Src, power<SrcExponent, Radix>>,
                  op_value<Dest, power<DestExponent, Radix>>> {
    };

    // shims between equivalent tags
    template<typename Input, typename Result, int DestExponent, int DestRadix>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input>,
            op_value<Result, quasi_exact<DestExponent, DestRadix>>>
        : custom_operator<
                  _impl::convert_op,
                  op_value<Input>,
                  op_value<Result, power<DestExponent, DestRadix>>> {
    };

    template<typename Input, int SrcExponent, int SrcRadix, typename Result>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input, quasi_exact<SrcExponent, SrcRadix>>,
            op_value<Result>>
        : custom_operator<
                  _impl::convert_op,
                  op_value<Input, power<SrcExponent, SrcRadix>>,
                  op_value<Result>> {
    };
}

#endif  // CNL_IMPL_SCALED_QUASI_EXACT_CONVERT_OPERATOR_H
