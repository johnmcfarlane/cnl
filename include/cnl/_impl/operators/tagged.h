
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
    /// \tparam Tag specifies the conversion mode, e.g. \ref cnl::native_overflow_tag
    /// \param from value to convert from
    /// \return value converted to
    ///
    /// \sa cnl::add, cnl::divide, cnl::shift_right, cnl::multiply, cnl::subtract,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag, cnl::trapping_overflow_tag,
    /// cnl::undefined_overflow_tag, cnl::nearest_rounding_tag
    template<class Tag, typename Result, typename Input>
    CNL_NODISCARD constexpr auto convert(Input const& from)
    -> decltype(convert_operator<Tag, Result, Input>{}(from))
    {
        return convert_operator<Tag, Result, Input>{}(from);
    }

    template<class DestTag, typename Dest, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    CNL_NODISCARD constexpr auto convert(constant<Value> const& src)
    -> decltype(convert_operator<DestTag, Dest, decltype(Value)>{}(src))
    {
        return convert_operator<DestTag, Dest, decltype(Value)>{}(src);
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
    -> decltype(binary_operator<Tag, _impl::add_op, Lhs, Rhs>{}(lhs, rhs))
    {
        return binary_operator<Tag, _impl::add_op, Lhs, Rhs>{}(lhs, rhs);
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
    -> decltype(binary_operator<Tag, _impl::subtract_op, Lhs, Rhs>{}(lhs, rhs))
    {
        return binary_operator<Tag, _impl::subtract_op, Lhs, Rhs>{}(lhs, rhs);
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
    -> decltype(binary_operator<Tag, _impl::multiply_op, Lhs, Rhs>{}(lhs, rhs))
    {
        return binary_operator<Tag, _impl::multiply_op, Lhs, Rhs>{}(lhs, rhs);
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
    -> decltype(binary_operator<Tag, _impl::divide_op, Lhs, Rhs>{}(lhs, rhs))
    {
        return binary_operator<Tag, _impl::divide_op, Lhs, Rhs>{}(lhs, rhs);
    }

    template<class Tag, typename Lhs, typename Rhs>
    CNL_NODISCARD constexpr auto shift_left(Lhs const& lhs, Rhs const& rhs)
    -> decltype(binary_operator<Tag, _impl::shift_left_op, Lhs, Rhs>{}(lhs, rhs))
    {
        return binary_operator<Tag, _impl::shift_left_op, Lhs, Rhs>{}(lhs, rhs);
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
    -> decltype(binary_operator<Tag, _impl::shift_right_op, Lhs, Rhs>{}(lhs, rhs))
    {
        return binary_operator<Tag, _impl::shift_right_op, Lhs, Rhs>{}(lhs, rhs);
    }
}

#endif  // CNL_IMPL_OPERATORS_TAGGED_H
