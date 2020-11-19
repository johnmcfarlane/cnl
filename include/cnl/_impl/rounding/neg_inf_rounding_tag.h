
//          Copyright Heikki Berg 2020
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_FLOOR_ROUNDING_TAG_H)
#define CNL_IMPL_ROUNDING_FLOOR_ROUNDING_TAG_H

#include "../cmath/abs.h"
#include "../operators/generic.h"
#include "../operators/native_tag.h"
#include "is_rounding_tag.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    /// \brief tag to specify floor or round towards minus infinity rounding behavior in arithmetic operations
    ///
    /// Arithmetic operations using this tag round by truncating 2's complement binary representation, thus
    /// round all values towards minus infinity.
    ///
    /// \headerfile cnl/rounding.h
    /// \sa cnl::rounding_integer,
    /// cnl::add, cnl::convert, cnl::divide, cnl::left_shift, cnl::multiply, cnl::subtract,
    /// cnl::neg_inf_rounding_tag
    struct neg_inf_rounding_tag
            : _impl::homogeneous_deduction_tag_base, _impl::homogeneous_operator_tag_base {
    };

    namespace _impl {
        template<>
        struct is_rounding_tag<neg_inf_rounding_tag> : std::true_type {
        };
    }

    template<class Operator, typename Operand>
    struct unary_operator<Operator, neg_inf_rounding_tag, Operand>
            : unary_operator<Operator, _impl::native_tag, Operand> {
    };

    template<class Operator, typename Lhs, typename Rhs>
    struct binary_operator<Operator, neg_inf_rounding_tag, neg_inf_rounding_tag, Lhs, Rhs>
            : Operator {
    };

    template<typename Lhs, typename Rhs>
    struct binary_operator<_impl::divide_op, neg_inf_rounding_tag, neg_inf_rounding_tag, Lhs, Rhs> {
    private:
        using result_type = decltype(std::declval<Lhs>()/std::declval<Rhs>());
        CNL_NODISCARD constexpr auto remainder(Lhs const& lhs, Rhs const& rhs) const
        -> result_type
        {
            return lhs%rhs;
        }
        CNL_NODISCARD constexpr auto division(Lhs const& lhs, Rhs const& rhs) const
        -> result_type
        {
            return lhs/rhs;
        }
        CNL_NODISCARD constexpr auto division_neg(Lhs const& lhs, Rhs const& rhs) const
        -> result_type
        {
            return lhs/rhs - 1;
        }
    public:
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> result_type
        {
            return ((remainder(lhs,rhs)!=0) && ((remainder(lhs,rhs)<0) != (rhs<0))) ? division_neg(lhs,rhs): division(lhs,rhs);
        }
    };

    template<class Operator, class RhsTag, typename Lhs, typename Rhs>
    struct shift_operator<Operator, neg_inf_rounding_tag, RhsTag, Lhs, Rhs>
            : Operator {
    };

    template<class Operator, typename Rhs>
    struct pre_operator<Operator, neg_inf_rounding_tag, Rhs>
            : Operator {
    };

    template<class Operator, typename Rhs>
    struct post_operator<Operator, neg_inf_rounding_tag, Rhs>
            : Operator {
    };
}

#endif  // CNL_IMPL_ROUNDING_FLOOR_ROUNDING_TAG_H


