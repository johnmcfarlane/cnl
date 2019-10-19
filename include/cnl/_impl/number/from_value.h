
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_FROM_VALUE_H)
#define CNL_IMPL_NUMBER_FROM_VALUE_H

#include "../../numeric.h"
#include "../num_traits/from_value.h"
#include "../num_traits/from_value_recursive.h"
#include "../type_traits/common_type.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, class Tag, typename Value>
    struct from_value<_impl::number<Rep, Tag>, Value>
            : _impl::from_value_simple<_impl::number<Value, Tag>, Value> {
    };

    template<class Rep, class Tag, class ValueRep, class ValueTag>
    struct from_value<_impl::number<Rep, Tag>, _impl::number<ValueRep, ValueTag>>
            : _impl::from_value_simple<
                    _impl::number<
                            from_value_t<Rep, ValueRep>,
                            Tag>,
                    _impl::number<ValueRep, ValueTag>> {
    };

    template<typename Rep, class Tag, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<_impl::number<Rep, Tag>, constant<Value>>
            : _impl::from_value_recursive<
                    _impl::number<
                    set_digits_t<Rep, _impl::max(digits<int>::value, _impl::used_digits(Value)-trailing_bits(Value))>,
                    Tag>,
            constant<Value>> {
    };
}

#endif  // CNL_IMPL_NUMBER_FROM_VALUE_H
