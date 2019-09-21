
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_OPERATORS_NATIVE_TAG_H
#define CNL_IMPL_OPERATORS_NATIVE_TAG_H

#include "generic.h"
#include "operators.h"
#include "../type_traits/is_integral.h"
#include "../type_traits/remove_signedness.h"
#include "../../constant.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // match the behavior of fundamental arithmetic types
        struct native_tag {
            using identity = native_tag;
        };

        // true iff given type, T, provides its own operators
        template<typename T>
        struct has_native_operators
                : std::integral_constant<
                        bool,
                        is_constant<T>::value
                        || is_integral<T>::value
                        || std::is_floating_point<T>::value> {
        };
    }

    template<typename Destination, typename Source>
    struct convert_operator<_impl::native_tag, _impl::native_tag, Destination, Source> {
        CNL_NODISCARD constexpr auto operator()(Source const& from) const -> Destination
        {
            return _impl::convert_op{}.template operator()<Destination>(from);
        }
    };

    template<class Operator, typename Rhs>
    struct unary_operator<
            Operator,
            _impl::native_tag, Rhs,
            _impl::enable_if_t<_impl::has_native_operators<Rhs>::value>> : Operator {
    };

    template<class Operator, typename Lhs, typename Rhs>
    struct binary_operator<
            Operator,
            _impl::native_tag, _impl::native_tag, Lhs, Rhs,
            _impl::enable_if_t<_impl::has_native_operators<Lhs>::value && _impl::has_native_operators<Rhs>::value>>
        : Operator {
    };
}

#endif  // CNL_IMPL_OPERATORS_NATIVE_TAG_H
