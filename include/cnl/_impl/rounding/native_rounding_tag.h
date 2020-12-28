
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_NATIVE_ROUNDING_TAG_H)
#define CNL_IMPL_ROUNDING_NATIVE_ROUNDING_TAG_H

#include "../operators/native_tag.h"
#include "is_rounding_tag.h"
#include "is_tag.h"

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
    struct native_rounding_tag
        : _impl::homogeneous_deduction_tag_base
        , _impl::homogeneous_operator_tag_base {
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

    template<_impl::unary_op Operator, typename Operand>
    struct unary_operator<Operator, native_rounding_tag, Operand>
        : unary_operator<Operator, _impl::native_tag, Operand> {
    };

    template<_impl::binary_op Operator, typename Lhs, typename Rhs>
    struct binary_operator<Operator, native_rounding_tag, native_rounding_tag, Lhs, Rhs>
        : binary_operator<Operator, _impl::native_tag, _impl::native_tag, Lhs, Rhs> {
    };

    template<_impl::shift_op Operator, tag RhsTag, typename Lhs, typename Rhs>
    struct shift_operator<Operator, native_rounding_tag, RhsTag, Lhs, Rhs>
        : shift_operator<Operator, _impl::native_tag, _impl::native_tag, Lhs, Rhs> {
    };

    template<_impl::pre_op Operator, typename Rhs>
    struct pre_operator<Operator, native_rounding_tag, Rhs>
        : pre_operator<Operator, _impl::native_tag, Rhs> {
    };

    template<_impl::post_op Operator, typename Rhs>
    struct post_operator<Operator, native_rounding_tag, Rhs>
        : post_operator<Operator, _impl::native_tag, Rhs> {
    };
}

#endif  // CNL_IMPL_ROUNDING_NATIVE_ROUNDING_TAG_H
