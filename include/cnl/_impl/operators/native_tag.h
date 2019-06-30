
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_OPERATORS_NATIVE_TAG_H
#define CNL_IMPL_OPERATORS_NATIVE_TAG_H

#include "operators.h"
#include "../type_traits/is_integral.h"
#include "../type_traits/remove_signedness.h"
#include "../../constant.h"

#include <type_traits>

////////////////////////////////////////////////////////////////////////////////
// tag error message

template<typename Type>
struct CNL_ERROR___cannot_use {
    // It you see this type in an error message then somewhere,
    // you probably passed a non-tag type as a tag parameter.
    struct as_a_tag;
};

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // match the behavior of fundamental arithmetic types
        struct native_tag {};

        template<typename T>
        struct has_native_operators
                : std::integral_constant<
                        bool,
                        is_constant<T>::value
                        || is_integral<T>::value
                        || std::is_floating_point<T>::value> {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::convert_operator

    template<class Tag, typename Destination, typename Source, typename Enabled=void>
    struct convert_operator : public CNL_ERROR___cannot_use<Tag>::as_a_tag {
    };

    template<typename Destination, typename Source>
    struct convert_operator<_impl::native_tag, Destination, Source> : _impl::convert_op {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::unary_operator

    template<class Tag, class Operator, typename Rhs, class Enabled=void>
    struct unary_operator : public CNL_ERROR___cannot_use<Tag>::as_a_tag {
    };

    template<class Operator, typename Rhs>
    struct unary_operator<
            _impl::native_tag, Operator,
            Rhs,
            _impl::enable_if_t<_impl::has_native_operators<Rhs>::value>> : Operator {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::binary_operator

    template<class Tag, class Operator, typename Lhs, typename Rhs, class Enabled=void>
    struct binary_operator : public CNL_ERROR___cannot_use<Tag>::as_a_tag {
    };

    template<class Operator, typename Lhs, typename Rhs>
    struct binary_operator<
            _impl::native_tag, Operator,
            Lhs, Rhs,
            _impl::enable_if_t<_impl::has_native_operators<Lhs>::value && _impl::has_native_operators<Rhs>::value>>
        : Operator {
    };
}

#endif  // CNL_IMPL_OPERATORS_NATIVE_TAG_H
