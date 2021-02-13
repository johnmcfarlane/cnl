
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OPERATORS_TAGGED_H)
#define CNL_IMPL_OPERATORS_TAGGED_H

#include "../../constant.h"
#include "../config.h"
#include "../num_traits/tag.h"
#include "custom_operator.h"
#include "operators.h"

/// compositional numeric library
namespace cnl {
    /// \brief converts a value to a type
    /// \headerfile cnl/all.h
    ///
    /// \tparam DestTag specifies the destination behavior tag, e.g. \ref cnl::native_overflow_tag
    /// \tparam SrcTag specifies the source behavior tag, e.g. \ref cnl::native_overflow_tag
    /// \param src value to convert from
    /// \return value converted to
    ///
    /// \sa cnl::add, cnl::divide, cnl::shift_right, cnl::multiply, cnl::subtract,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag,
    /// cnl::trapping_overflow_tag, cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<tag DestTag, tag SrcTag, typename Dest, typename Src>
    [[nodiscard]] constexpr auto convert(Src const& src)
    {
        return custom_operator<_impl::convert_op, operand<Src, SrcTag>, operand<Dest, DestTag>>{}(src);
    }

    template<tag DestTag, tag SrcTag, typename Dest, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    [[nodiscard]] constexpr auto convert(constant<Value> const& src)
    {
        return custom_operator<_impl::convert_op, operand<decltype(Value), SrcTag>, operand<Dest, DestTag>>{}(src);
    }

    namespace _impl {
        template<_impl::binary_arithmetic_op Operator, tag Tag, typename Lhs, typename Rhs>
        [[nodiscard]] constexpr auto binary_arithmetic_operate(Lhs const& lhs, Rhs const& rhs)
        {
            return custom_operator<Operator, operand<Lhs, Tag>, operand<Rhs, Tag>>{}(lhs, rhs);
        }
    }

    /// \brief adds two values together
    /// \headerfile cnl/all.h
    ///
    /// \tparam Tag specifies the conversion mode, e.g. \ref cnl::native_overflow_tag
    /// \return the result of `lhs + rhs`
    ///
    /// \sa cnl::convert, cnl::divide, cnl::shift_right, cnl::multiply, cnl::subtract,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag,
    /// cnl::trapping_overflow_tag, cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<tag Tag, typename Lhs, typename Rhs>
    [[nodiscard]] constexpr auto add(Lhs const& lhs, Rhs const& rhs)
    {
        return custom_operator<_impl::add_op, operand<Lhs, Tag>, operand<Rhs, Tag>>{}(lhs, rhs);
    }

    /// \brief subtracts one value from another
    /// \headerfile cnl/all.h
    ///
    /// \tparam Tag specifies the conversion mode, e.g. \ref cnl::native_overflow_tag
    /// \return the result of `lhs - rhs`
    ///
    /// \sa cnl::add, cnl::convert, cnl::divide, cnl::shift_right, cnl::multiply,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag,
    /// cnl::trapping_overflow_tag, cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<tag Tag, typename Lhs, typename Rhs>
    [[nodiscard]] constexpr auto subtract(Lhs const& lhs, Rhs const& rhs)
    {
        return custom_operator<_impl::subtract_op, operand<Lhs, Tag>, operand<Rhs, Tag>>{}(lhs, rhs);
    }

    /// \brief multiplies one value by another
    /// \headerfile cnl/all.h
    ///
    /// \tparam Tag specifies the conversion mode, e.g. \ref cnl::native_overflow_tag
    /// \return the result of `lhs + rhs`
    ///
    /// \sa cnl::add, cnl::convert, cnl::divide, cnl::subtract, cnl::shift_right,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag,
    /// cnl::trapping_overflow_tag, cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<tag Tag, typename Lhs, typename Rhs>
    [[nodiscard]] constexpr auto multiply(Lhs const& lhs, Rhs const& rhs)
    {
        return custom_operator<_impl::multiply_op, operand<Lhs, Tag>, operand<Rhs, Tag>>{}(lhs, rhs);
    }

    /// \brief divides one value by another
    /// \headerfile cnl/all.h
    ///
    /// \tparam Tag specifies the conversion mode, e.g. \ref cnl::native_rounding_tag
    /// \return the result of `lhs / rhs`
    ///
    /// \sa cnl::add, cnl::convert, cnl::shift_right, cnl::multiply, cnl::subtract,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag,
    /// cnl::trapping_overflow_tag, cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<tag Tag, typename Lhs, typename Rhs>
    [[nodiscard]] constexpr auto divide(Lhs const& lhs, Rhs const& rhs)
    {
        return custom_operator<_impl::divide_op, operand<Lhs, Tag>, operand<Rhs, Tag>>{}(lhs, rhs);
    }

    template<tag Tag, typename Lhs, typename Rhs>
    [[nodiscard]] constexpr auto shift_left(Lhs const& lhs, Rhs const& rhs)
    {
        return custom_operator<_impl::shift_left_op, operand<Lhs, Tag>, operand<Rhs, Tag>>{}(lhs, rhs);
    }

    /// \brief bitwise left-shifts one value by another
    /// \headerfile cnl/all.h
    ///
    /// \tparam Tag specifies the conversion mode, e.g. \ref cnl::native_overflow_tag
    /// \return the result of `lhs + rhs`
    ///
    /// \sa cnl::add, cnl::convert, cnl::divide, cnl::multiply, cnl::subtract,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag,
    /// cnl::trapping_overflow_tag, cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<tag Tag, typename Lhs, typename Rhs>
    [[nodiscard]] constexpr auto shift_right(Lhs const& lhs, Rhs const& rhs)
    {
        return custom_operator<_impl::shift_right_op, operand<Lhs, Tag>, operand<Rhs, Tag>>{}(lhs, rhs);
    }
}

#endif  // CNL_IMPL_OPERATORS_TAGGED_H
