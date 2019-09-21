
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_NEAREST_ROUNDING_TAG_H)
#define CNL_IMPL_ROUNDING_NEAREST_ROUNDING_TAG_H

#include "is_rounding_tag.h"
#include "../operators/generic.h"
#include "../operators/native_tag.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    /// \brief tag to specify round-to-nearest behavior in arithemtic operations
    ///
    /// Arithmetic operations using this tag round to the nearest representatable value in situations where the
    /// precise value cannot be represented.
    ///
    /// \headerfile cnl/rounding.h
    /// \sa cnl::rounding_integer,
    /// cnl::add, cnl::convert, cnl::divide, cnl::left_shift, cnl::multiply, cnl::subtract,
    /// cnl::native_rounding_tag
    struct nearest_rounding_tag {
    };

    namespace _impl {
        template<>
        struct is_rounding_tag<nearest_rounding_tag> : std::true_type {
        };
    }

    template<class Operator, typename Operand>
    struct unary_operator<Operator, nearest_rounding_tag, Operand>
            : unary_operator<Operator, _impl::native_tag, Operand> {
    };

    template<class Operator, typename Lhs, typename Rhs>
    struct binary_operator<Operator, nearest_rounding_tag, nearest_rounding_tag, Lhs, Rhs>
            : Operator {
    };

    template<typename Lhs, typename Rhs>
    struct binary_operator<_impl::divide_op, nearest_rounding_tag, nearest_rounding_tag, Lhs, Rhs> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(lhs/rhs)
        {
            return (((lhs < 0) ^ (rhs < 0))
                    ? lhs-(rhs/2)
                    : lhs+(rhs/2))/rhs;
        }
    };

    template<class Operator, typename Lhs, typename Rhs>
    struct shift_operator<Operator, nearest_rounding_tag, nearest_rounding_tag, Lhs, Rhs>
            : Operator {
    };

    template<class Operator, typename Rhs>
    struct pre_operator<Operator, nearest_rounding_tag, Rhs>
            : Operator {
    };

    template<class Operator, typename Rhs>
    struct post_operator<Operator, nearest_rounding_tag, Rhs>
            : Operator {
    };
}

#endif  // CNL_IMPL_ROUNDING_NEAREST_ROUNDING_TAG_H
