
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_NATIVE_TAG_H
#define CNL_IMPL_NATIVE_TAG_H

#include "type_traits/remove_signedness.h"

#include "operators.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // match the behavior of fundamental arithmetic types
        struct native_tag {};

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::tagged_convert_operator

        template<class Tag, typename Destination, typename Source, typename Enabled=void>
        struct tagged_convert_operator;

        template<typename Destination, typename Source>
        struct tagged_convert_operator<native_tag, Destination, Source> : convert_op {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::tagged_unary_operator

        template<class Tag, class Operator>
        struct tagged_unary_operator;

        template<class Operator>
        struct tagged_unary_operator<native_tag, Operator> : Operator {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::tagged_binary_operator

        template<class Tag, class Operator>
        struct tagged_binary_operator;

        template<class Operator>
        struct tagged_binary_operator<native_tag, Operator> : Operator {};

    }
}

#endif  // CNL_IMPL_NATIVE_TAG_H
