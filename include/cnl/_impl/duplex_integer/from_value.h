
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_FROM_VALUE_H)
#define CNL_IMPL_DUPLEX_INTEGER_FROM_VALUE_H

#include "../num_traits/from_value.h"
#include "../type_traits/enable_if.h"
#include "digits.h"
#include "forward_declaration.h"
#include "instantiate_duplex_integer.h"
#include "is_duplex_integer.h"

/// compositional numeric library
namespace cnl {
    /// \private specialization for (duplex_integer<>, anything-else)
    template<class Duplex, class Value>
    struct from_value<
            Duplex, Value,
            _impl::enable_if_t<_impl::is_duplex_integer<Duplex>::value && !_impl::is_duplex_integer<Value>::value>> {
        CNL_NODISCARD constexpr auto operator()(Value const& value) const
        -> _impl::instantiate_duplex_integer<digits<Value>::value, Value>
        {
            return value;
        }
    };

    /// \private specialization for (duplex_integer<>, duplex_integer<>)
    template<class Duplex, class Value>
    struct from_value<
            Duplex, Value,
            _impl::enable_if_t<_impl::is_duplex_integer<Duplex>::value && _impl::is_duplex_integer<Value>::value>> {
        CNL_NODISCARD constexpr auto operator()(Value const& value) const
        -> Value
        {
            return value;
        }
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_FROM_VALUE_H
