
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_NATIVE_H)
#define CNL_IMPL_OVERFLOW_NATIVE_H

#include "common.h"
#include "overflow_operator.h"
#include "../native_tag.h"
#include "../polarity.h"

/// compositional numeric library
namespace cnl {
    /// \brief tag to match the overflow behavior of fundamental arithmetic types
    ///
    /// Arithmetic operations using this tag should behave the same as equivalent operators.
    ///
    /// \headerfile cnl/overflow.h
    /// \sa cnl::overflow_integer,
    /// cnl::add, cnl::convert, cnl::divide, cnl::left_shift, cnl::multiply, cnl::subtract,
    /// cnl::saturated_overflow_tag, cnl::throwing_overflow_tag, cnl::trapping_overflow_tag, cnl::undefined_overflow_tag
    struct native_overflow_tag : _impl::native_tag {
    };

    namespace _impl {
        template<typename Operator, polarity Polarity>
        struct overflow_operator<Operator, native_overflow_tag, Polarity> : Operator {
        };
    }

    template<typename Destination, typename Source>
    struct convert_operator<native_overflow_tag, Destination, Source>
            : _impl::tagged_convert_overflow_operator<native_overflow_tag, Destination, Source> {
    };

    template<class Operator>
    struct unary_operator<native_overflow_tag, Operator>
            : _impl::tagged_unary_overflow_operator<native_overflow_tag, Operator> {
    };

    template<class Operator>
    struct binary_operator<native_overflow_tag, Operator>
            : binary_operator<_impl::native_tag, Operator> {
    };
}

#endif  // CNL_IMPL_OVERFLOW_NATIVE_H
