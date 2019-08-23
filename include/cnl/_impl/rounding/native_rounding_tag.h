
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_NATIVE_ROUNDING_TAG_H)
#define CNL_IMPL_ROUNDING_NATIVE_ROUNDING_TAG_H

#include "../operators/native_tag.h"
#include "is_rounding_tag.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    /// \brief tag to match the rounding behavior of fundamental arithmetic types
    ///
    /// Arithmetic operations using this tag should behave the same as equivalent operators.
    ///
    /// \headerfile cnl/rounding.h
    /// \sa cnl::rounding_integer,
    /// cnl::add, cnl::convert, cnl::divide, cnl::left_shift, cnl::multiply, cnl::subtract,
    /// cnl::nearest_rounding_tag
    struct native_rounding_tag : public _impl::native_tag {
        native_rounding_tag() = default;
        native_rounding_tag(native_tag) {}  // NOLINT(hicpp-explicit-conversions, google-explicit-constructor)
    };

    namespace _impl {
        template<>
        struct is_rounding_tag<native_rounding_tag> : std::true_type {
        };
    }

    template<typename Destination, typename Source>
    struct convert_operator<native_rounding_tag, _impl::native_tag, Destination, Source>
            : convert_operator<_impl::native_tag, _impl::native_tag, Destination, Source> {
    };

    template<class Operator, typename Operand>
    struct unary_operator<native_rounding_tag, Operator, Operand>
            : unary_operator<_impl::native_tag, Operator, Operand> {
    };

    template<class Operator, typename Lhs, typename Rhs>
    struct binary_operator<native_rounding_tag, Operator, Lhs, Rhs>
            : binary_operator<_impl::native_tag, Operator, Lhs, Rhs> {
    };

    template<class Operator, typename Lhs, typename Rhs>
    struct shift_operator<native_rounding_tag, Operator, Lhs, Rhs>
            : shift_operator<_impl::native_tag, Operator, Lhs, Rhs> {
    };

    template<class Operator, typename Rhs>
    struct pre_operator<native_rounding_tag, Operator, Rhs>
            : pre_operator<_impl::native_tag, Operator, Rhs> {
    };

    template<class Operator, typename Rhs>
    struct post_operator<native_rounding_tag, Operator, Rhs>
            : post_operator<_impl::native_tag, Operator, Rhs> {
    };
}

#endif  // CNL_IMPL_ROUNDING_NATIVE_ROUNDING_TAG_H
