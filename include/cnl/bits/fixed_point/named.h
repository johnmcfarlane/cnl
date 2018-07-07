
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential named definitions related to the `cnl::fixed_point` type

#if !defined(CNL_FIXED_POINT_NAMED_H)
#define CNL_FIXED_POINT_NAMED_H 1

#include <cnl/bits/common.h>
#include "num_traits.h"

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_fixed_point<Value>

    /// \brief makes a fixed_point object from a given value
    ///
    /// \tparam Value the type of the value that is to be made
    /// into a \ref fixed_point value
    ///
    /// \param value the value from which to make the \ref fixed_point object
    ///
    /// \note This function is deprecated after C++17
    /// in favor of class template deduction.
    template<typename Value>
    constexpr auto make_fixed_point(Value const& value)
    -> cnl::from_value_t<fixed_point<Value, 0>, Value>
    {
        return _impl::make_number<fixed_point<Value, 0>>(value);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::multiply with fixed_point operand(s)

    namespace _multiply_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::fp::arithmetic::fixed_point_type

        template<class Operand>
        struct fixed_point_type {
            using type = fixed_point<Operand, 0>;
        };

        template<typename Rep, int Exponent, int Radix>
        struct fixed_point_type<fixed_point<Rep, Exponent, Radix>> {
            using type = fixed_point<Rep, Exponent, Radix>;
        };

        template<class Lhs, class Rhs>
        struct params {
            using lhs_type = typename fixed_point_type<Lhs>::type;
            using rhs_type = typename fixed_point_type<Rhs>::type;
            using lhs_rep = typename lhs_type::rep;
            using rhs_rep = typename rhs_type::rep;

            // Exponent
            static constexpr int rep_exponent = lhs_type::exponent + rhs_type::exponent;

            // Rep
            using rep_op_result = _impl::op_result<_impl::multiply_op, lhs_rep, rhs_rep>;
            static constexpr int sum_digits = digits<lhs_type>::value + digits<rhs_type>::value;
            static constexpr bool is_signed =
                    numeric_limits<lhs_rep>::is_signed || numeric_limits<rhs_rep>::is_signed;
            using prewidened_result_rep = _impl::make_signed_t<rep_op_result, is_signed>;
            using result_rep = set_digits_t<prewidened_result_rep, sum_digits>;

            // If the 'natural' result of the rep op is wide enough, stick with it.
            // This ensures that auto-widening rep types (e.g. elastic_integer) don't get widened twice
            // but types that need a little help (e.g. built-ins) get widened going into the op.
            using rep_type = typename std::conditional<
                    digits<prewidened_result_rep>::value >= sum_digits,
                    lhs_rep, result_rep>::type;

            using result_type = fixed_point<result_rep, rep_exponent>;
            using intermediate_lhs = fixed_point<rep_type, lhs_type::exponent>;
            using intermediate_rhs = Rhs;
        };
    }

    /// \brief calculates the product of two \ref fixed_point factors
    /// \headerfile cnl/fixed_point.h
    ///
    /// \param lhs, rhs the factors
    ///
    /// \return product: lhs * rhs
    ///
    /// \note This function multiplies the values
    /// without performing any additional scaling or conversion.

    template<class Lhs, class Rhs>
    constexpr auto multiply(Lhs const& lhs, Rhs const& rhs)
    -> typename _multiply_impl::params<Lhs, Rhs>::result_type
    {
        using params = _multiply_impl::params<Lhs, Rhs>;
        using intermediate_lhs = typename params::intermediate_lhs;
        using intermediate_rhs = typename params::intermediate_rhs;
        using result_type = typename params::result_type;
        using result_rep = typename result_type::rep;

        return from_rep<result_type>{}(
                static_cast<result_rep>(to_rep(static_cast<intermediate_lhs>(lhs))
                                        * to_rep(static_cast<intermediate_rhs>(rhs))));
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::quotient

    namespace _impl {
        template<typename Number>
        struct fixed_point_rep {
            using type = Number;
        };

        template<typename Rep, int Exponent, int Radix>
        struct fixed_point_rep<fixed_point<Rep, Exponent, Radix>> : fixed_point_rep<Rep> {
        };

        template<typename Number>
        constexpr Number not_fixed_point(Number const& number)
        {
            return number;
        }

        template<typename Rep, int Exponent, int Radix>
        constexpr Rep not_fixed_point(fixed_point<Rep, Exponent, Radix> const& f)
        {
            return to_rep(f);
        }

        template<typename Number>
        struct exponent : constant<0> {};

        template<typename Rep, int Exponent, int Radix>
        struct exponent<fixed_point<Rep, Exponent, Radix>> : constant<Exponent> {
        };

        template<class Quotient, class Dividend, class Divisor>
        struct exponent_shift : std::integral_constant<
                int,
                _impl::exponent<Dividend>::value
                    -_impl::exponent<Divisor>::value
                    -_impl::exponent<Quotient>::value> {
        };

        struct default_quotient_tag {};

        template<class Quotient, class Dividend, class Divisor>
        struct result;

        template<typename Rep, int Exponent, int Radix, typename Dividend, typename Divisor>
        struct result<fixed_point<Rep, Exponent, Radix>, Dividend, Divisor> {
            using type = fixed_point<Rep, Exponent, Radix>;
        };

        template<class Dividend, class Divisor>
        struct result<default_quotient_tag, Dividend, Divisor> {
            using natural_result = _impl::op_result<_impl::divide_op, Dividend, Divisor>;

            static constexpr int integer_digits =
                    _impl::integer_digits<Dividend>::value+_impl::fractional_digits<Divisor>::value;
            static constexpr int fractional_digits =
                    _impl::fractional_digits<Dividend>::value+_impl::integer_digits<Divisor>::value;

            static constexpr auto necessary_digits = integer_digits+fractional_digits;
            static constexpr auto natural_digits = digits<natural_result>::value;
            static constexpr auto result_digits = _impl::max(necessary_digits, natural_digits);

            using rep_type = set_digits_t<natural_result, result_digits>;
            static constexpr int rep_exponent = -fractional_digits;

            using type = fixed_point<typename fixed_point_rep<rep_type>::type, rep_exponent>;
        };
    }

    /// \brief calculates the quotient of two \ref fixed_point values
    /// \headerfile cnl/fixed_point.h
    ///
    /// \tparam Quotient the desired \ref fixed_point type of quotient;
    /// by default, a value which minimizes the chances of overflow or precision loss
    /// \tparam Dividend the dividend (top number) of the division
    /// \tparam Divisor the divisor (bottom number) of the division
    /// \param dividend, divisor dividend and divisor
    ///
    /// \return quotient: dividend / divisor
    ///
    /// \note When specifying the `Quotient`, ensure that there is enough capacity
    /// to perform a *widened division* operation and store the quotient.
    /// \note A *widened division* is one where the quotient has as many integer digits as the
    /// dividend's integer digits plus the divisor's fractional digits and as many fractional
    /// digits as the dividend's fractional digits plus the divisor's integer digits.

    template<
            class Quotient = _impl::default_quotient_tag,
            class Dividend,
            class Divisor>
    constexpr auto quotient(Dividend const& dividend, Divisor const& divisor)
    -> typename _impl::result<Quotient, Dividend, Divisor>::type {
        using result_type = typename _impl::result<Quotient, Dividend, Divisor>::type;
        using result_rep = typename result_type::rep;
        return from_rep<result_type>()(
                static_cast<result_rep>(_impl::scale<_impl::exponent_shift<result_type, Dividend, Divisor>::value>(
                        static_cast<result_rep>(_impl::not_fixed_point(dividend)))
                        /_impl::not_fixed_point(divisor)));
    }
}

#endif  // CNL_FIXED_POINT_NAMED_H
