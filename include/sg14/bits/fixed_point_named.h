
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential named definitions related to the `sg14::fixed_point` type

#if !defined(SG14_FIXED_POINT_NAMED_H)
#define SG14_FIXED_POINT_NAMED_H 1

#include "common.h"

#include "fixed_point_arithmetic.h"

/// study group 14 of the C++ working group
namespace sg14 {

    /// \brief calculates the negative of a \ref fixed_point value
    /// \headerfile sg14/fixed_point
    ///
    /// \param rhs input value
    ///
    /// \return negative: - rhs
    ///
    /// \note This function negates the value
    /// without performing any additional scaling or conversion.
    ///
    /// \sa add, subtract, multiply, divide

    template<class RhsRep, int RhsExponent>
    constexpr auto negate(const fixed_point<RhsRep, RhsExponent>& rhs)
    -> fixed_point<decltype(-rhs.data()), RhsExponent>
    {
        using result_type = fixed_point<decltype(-rhs.data()), RhsExponent>;
        return result_type::from_data(-rhs.data());
    }

    /// \brief calculates the sum of two \ref fixed_point values
    /// \headerfile sg14/fixed_point
    ///
    /// \param lhs, rhs augend and addend
    ///
    /// \return sum: lhs + rhs
    ///
    /// \note This function add the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa negate, subtract, multiply, divide

    template<class Lhs, class Rhs>
    constexpr auto add(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::named_function_tag>(lhs, rhs, _impl::add_tag))
    {
        return _impl::fp::operate<_impl::fp::named_function_tag>(lhs, rhs, _impl::add_tag);
    }

    /// \brief calculates the difference of two \ref fixed_point values
    /// \headerfile sg14/fixed_point
    ///
    /// \param lhs, rhs minuend and subtrahend
    ///
    /// \return difference: lhs - rhs
    ///
    /// \note This function subtracts the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa negate, add, multiply, divide

    template<class Lhs, class Rhs>
    constexpr auto subtract(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::named_function_tag>(lhs, rhs, _impl::subtract_tag))
    {
        return _impl::fp::operate<_impl::fp::named_function_tag>(lhs, rhs, _impl::subtract_tag);
    }

    /// \brief calculates the product of two \ref fixed_point factors
    /// \headerfile sg14/fixed_point
    ///
    /// \param lhs, rhs the factors
    ///
    /// \return product: lhs * rhs
    ///
    /// \note This function multiplies the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa negate, add, subtract, divide

    template<class Lhs, class Rhs>
    constexpr auto multiply(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::named_function_tag>(lhs, rhs, _impl::multiply_tag))
    {
        return _impl::fp::operate<_impl::fp::named_function_tag>(lhs, rhs, _impl::multiply_tag);
    }

    /// \brief calculates the quotient of two \ref fixed_point values
    /// \headerfile sg14/fixed_point
    ///
    /// \param lhs, rhs dividend and divisor
    ///
    /// \return quotient: lhs / rhs
    ///
    /// \note This function divides the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa negate, add, subtract, multiply

    template<class Lhs, class Rhs>
    constexpr auto divide(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::division_named_function_tag>(lhs, rhs, _impl::divide_tag))
    {
        return _impl::fp::operate<_impl::fp::division_named_function_tag>(lhs, rhs, _impl::divide_tag);
    }
}

#endif	// SG14_FIXED_POINT_NAMED_H
