
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OPERATORS_TAGGED_H)
#define CNL_IMPL_OPERATORS_TAGGED_H

#include "../../constant.h"
#include "../config.h"
#include "generic.h"
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
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag, cnl::trapping_overflow_tag,
    /// cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<class DestTag, typename SrcTag, typename Dest, typename Src>
    CNL_NODISCARD constexpr auto convert(Src const& src)
    {
        return convert_operator<DestTag, SrcTag, Dest, Src>{}(src);
    }

    template<class DestTag, class SrcTag, typename Dest, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    CNL_NODISCARD constexpr auto convert(constant<Value> const& src)
    {
        return convert_operator<DestTag, SrcTag, Dest, decltype(Value)>{}(src);
    }

    namespace _impl {
        template<class Operator, class Tag, typename Lhs, typename Rhs>
        CNL_NODISCARD constexpr auto binary_operate(Lhs const& lhs, Rhs const& rhs)
        {
            return binary_operator<Operator, Tag, Tag, Lhs, Rhs>{}(lhs, rhs);
        }
    }

    /// \brief adds two values together
    /// \headerfile cnl/all.h
    ///
    /// \tparam Tag specifies the conversion mode, e.g. \ref cnl::native_overflow_tag
    /// \return the result of `lhs + rhs`
    ///
    /// \sa cnl::convert, cnl::divide, cnl::shift_right, cnl::multiply, cnl::subtract,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag, cnl::trapping_overflow_tag,
    /// cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<class Tag, typename Lhs, typename Rhs>
    CNL_NODISCARD constexpr auto add(Lhs const& lhs, Rhs const& rhs)
    {
        return binary_operator<_impl::add_op, Tag, Tag, Lhs, Rhs>{}(lhs, rhs);
    }

    /// \brief subtracts one value from another
    /// \headerfile cnl/all.h
    ///
    /// \tparam Tag specifies the conversion mode, e.g. \ref cnl::native_overflow_tag
    /// \return the result of `lhs - rhs`
    ///
    /// \sa cnl::add, cnl::convert, cnl::divide, cnl::shift_right, cnl::multiply,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag, cnl::trapping_overflow_tag,
    /// cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<class Tag, typename Lhs, typename Rhs>
    CNL_NODISCARD constexpr auto subtract(Lhs const& lhs, Rhs const& rhs)
    {
        return binary_operator<_impl::subtract_op, Tag, Tag, Lhs, Rhs>{}(lhs, rhs);
    }

    /// \brief multiplies one value by another
    /// \headerfile cnl/all.h
    ///
    /// \tparam Tag specifies the conversion mode, e.g. \ref cnl::native_overflow_tag
    /// \return the result of `lhs + rhs`
    ///
    /// \sa cnl::add, cnl::convert, cnl::divide, cnl::subtract, cnl::shift_right,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag, cnl::trapping_overflow_tag,
    /// cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<class Tag, typename Lhs, typename Rhs>
    CNL_NODISCARD constexpr auto multiply(Lhs const& lhs, Rhs const& rhs)
    {
        return binary_operator<_impl::multiply_op, Tag, Tag, Lhs, Rhs>{}(lhs, rhs);
    }

    /// \brief divides one value by another
    /// \headerfile cnl/all.h
    ///
    /// \tparam Tag specifies the conversion mode, e.g. \ref cnl::native_rounding_tag
    /// \return the result of `lhs / rhs`
    ///
    /// \sa cnl::add, cnl::convert, cnl::shift_right, cnl::multiply, cnl::subtract,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag, cnl::trapping_overflow_tag,
    /// cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<class Tag, typename Lhs, typename Rhs>
    CNL_NODISCARD constexpr auto divide(Lhs const& lhs, Rhs const& rhs)
    {
        return binary_operator<_impl::divide_op, Tag, Tag, Lhs, Rhs>{}(lhs, rhs);
    }

    template<class Tag, typename Lhs, typename Rhs>
    CNL_NODISCARD constexpr auto shift_left(Lhs const& lhs, Rhs const& rhs)
    {
        return binary_operator<_impl::shift_left_op, Tag, Tag, Lhs, Rhs>{}(lhs, rhs);
    }

    /// \brief bitwise left-shifts one value by another
    /// \headerfile cnl/all.h
    ///
    /// \tparam Tag specifies the conversion mode, e.g. \ref cnl::native_overflow_tag
    /// \return the result of `lhs + rhs`
    ///
    /// \sa cnl::add, cnl::convert, cnl::divide, cnl::multiply, cnl::subtract,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag, cnl::trapping_overflow_tag,
    /// cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<class Tag, typename Lhs, typename Rhs>
    CNL_NODISCARD constexpr auto shift_right(Lhs const& lhs, Rhs const& rhs)
    {
        return binary_operator<_impl::shift_right_op, Tag, Tag, Lhs, Rhs>{}(lhs, rhs);
    }
}

#endif  // CNL_IMPL_OPERATORS_TAGGED_H
