
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_FROM_VALUE_H)
#define CNL_IMPL_DUPLEX_INTEGER_FROM_VALUE_H

#include "../num_traits/from_value.h"
#include "digits.h"
#include "forward_declaration.h"
#include "instantiate_duplex_integer.h"
#include "is_duplex_integer.h"

/// compositional numeric library
namespace cnl {
    template<class Duplex, class Value>
    requires(_impl::is_duplex_integer<Duplex>::value && !_impl::is_duplex_integer<Value>::value) struct from_value<Duplex, Value> {
        [[nodiscard]] constexpr auto operator()(Value const& value) const
                -> _impl::instantiate_duplex_integer_t<
                        digits<Value>,
                        _impl::duplex_integer_upper_t<Duplex>>
        {
            return value;
        }
    };

    template<_impl::any_duplex_integer Duplex, _impl::any_duplex_integer Value>
    struct from_value<Duplex, Value> {
        [[nodiscard]] constexpr auto operator()(Value const& value) const -> Value
        {
            return value;
        }
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_FROM_VALUE_H
