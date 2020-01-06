
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_FROM_VALUE_H)
#define CNL_IMPL_ELASTIC_INTEGER_FROM_VALUE_H

#include "../num_traits/from_value.h"
#include "../num_traits/set_width.h"
#include "../num_traits/width.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, class Narrowest, class Value>
    struct from_value<elastic_integer<Digits, Narrowest>, Value>
        : _impl::from_value_simple<
                elastic_integer<
                        digits<Value>::value,
                        _impl::set_width_t<Value, _impl::width<Narrowest>::value>>,
                Value> {
    };

    template<int Digits, typename Narrowest, int ValueDigits, typename ValueNarrowest>
    struct from_value<elastic_integer<Digits, Narrowest>, elastic_integer<ValueDigits, ValueNarrowest>>
        : _impl::from_value_simple<
                elastic_integer<ValueDigits, Narrowest>,
                elastic_integer<ValueDigits, Narrowest>> {
    };

    template<int Digits, class Narrowest, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<elastic_integer<Digits, Narrowest>, constant<Value>>
        : _impl::from_value_simple<elastic_integer<digits<constant<Value>>::value, int>, constant<Value>> {
    };
}

#endif // CNL_IMPL_ELASTIC_INTEGER_FROM_VALUE_H
