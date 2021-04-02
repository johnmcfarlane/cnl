
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OPERATORS_TAGGED_H)
#define CNL_IMPL_OPERATORS_TAGGED_H

#include "../../constant.h"
#include "../config.h"
#include "../custom_operator/tag.h"
#include "definition.h"
#include "op.h"

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
    /// \sa cnl::divide, cnl::shift_right,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag,
    /// cnl::trapping_overflow_tag, cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<tag DestTag, tag SrcTag, typename Dest, typename Src>
    [[nodiscard]] constexpr auto convert(Src const& src)
    {
        return custom_operator<_impl::convert_op, op_value<Src, SrcTag>, op_value<Dest, DestTag>>{}(src);
    }

    template<tag DestTag, tag SrcTag, typename Dest, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    [[nodiscard]] constexpr auto convert(constant<Value> const& src)
    {
        return custom_operator<_impl::convert_op, op_value<decltype(Value), SrcTag>, op_value<Dest, DestTag>>{}(src);
    }

    namespace _impl {
        template<op Operator, tag Tag, typename... Operands>
        [[nodiscard]] constexpr auto custom_operate(Operands const&... operands)
        {
            return custom_operator<Operator, op_value<Operands, Tag>...>{}(operands...);
        }
    }

    /// \brief divides one value by another
    /// \headerfile cnl/all.h
    ///
    /// \tparam Tag specifies the conversion mode, e.g. \ref cnl::native_rounding_tag
    /// \return the result of `lhs / rhs`
    ///
    /// \sa cnl::convert, cnl::shift_right,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag,
    /// cnl::trapping_overflow_tag, cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<tag Tag, typename Lhs, typename Rhs>
    [[nodiscard]] constexpr auto divide(Lhs const& lhs, Rhs const& rhs)
    {
        return custom_operator<_impl::divide_op, op_value<Lhs, Tag>, op_value<Rhs, Tag>>{}(lhs, rhs);
    }

    template<tag Tag, typename Lhs, typename Rhs>
    [[nodiscard]] constexpr auto shift_left(Lhs const& lhs, Rhs const& rhs)
    {
        return custom_operator<_impl::shift_left_op, op_value<Lhs, Tag>, op_value<Rhs, Tag>>{}(lhs, rhs);
    }

    /// \brief bitwise left-shifts one value by another
    /// \headerfile cnl/all.h
    ///
    /// \tparam Tag specifies the conversion mode, e.g. \ref cnl::native_overflow_tag
    /// \return the result of `lhs + rhs`
    ///
    /// \sa cnl::convert, cnl::divide,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag,
    /// cnl::trapping_overflow_tag, cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<tag Tag, typename Lhs, typename Rhs>
    [[nodiscard]] constexpr auto shift_right(Lhs const& lhs, Rhs const& rhs)
    {
        return custom_operator<_impl::shift_right_op, op_value<Lhs, Tag>, op_value<Rhs, Tag>>{}(lhs, rhs);
    }
}

#endif  // CNL_IMPL_OPERATORS_TAGGED_H
