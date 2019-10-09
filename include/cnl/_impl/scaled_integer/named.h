
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential named definitions related to the `cnl::scaled_integer` type

#if !defined(CNL_IMPL_SCALED_INTEGER_NAMED_H)
#define CNL_IMPL_SCALED_INTEGER_NAMED_H

#include "../common.h"
#include "../num_traits/fixed_width_scale.h"
#include "num_traits.h"

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_scaled_integer<Value>

    /// \brief makes a scaled_integer object from a given value
    ///
    /// \tparam Value the type of the value that is to be made
    /// into a \ref scaled_integer value
    ///
    /// \param value the value from which to make the \ref scaled_integer object
    ///
    /// \note This function is deprecated after C++17
    /// in favor of class template deduction.
    template<typename Value>
    CNL_NODISCARD constexpr auto make_scaled_integer(Value const& value)
    -> decltype(_impl::from_value<scaled_integer<>, Value>(value))
    {
        return _impl::from_value<scaled_integer<>, Value>(value);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::quotient

    namespace _impl {
        template<typename Number>
        struct scaled_integer_rep {
            using type = Number;
        };

        template<typename Rep, int Exponent, int Radix>
        struct scaled_integer_rep<scaled_integer<Rep, power<Exponent, Radix>>> : scaled_integer_rep<Rep> {
        };

        template<typename Number>
        CNL_NODISCARD constexpr Number not_scaled_integer(Number const& number)
        {
            return number;
        }

        template<typename Rep, int Exponent, int Radix>
        CNL_NODISCARD constexpr Rep not_scaled_integer(scaled_integer<Rep, power<Exponent, Radix>> const& f)
        {
            return _impl::to_rep(f);
        }

        template<typename Number>
        struct exponent : constant<0> {};

        template<typename Rep, int Exponent, int Radix>
        struct exponent<scaled_integer<Rep, power<Exponent, Radix>>> : constant<Exponent> {
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
        struct result<scaled_integer<Rep, power<Exponent, Radix>>, Dividend, Divisor> {
            using type = scaled_integer<Rep, power<Exponent, Radix>>;
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

            using type = scaled_integer<typename scaled_integer_rep<rep_type>::type, power<rep_exponent>>;
        };
    }

    /// \brief calculates the quotient of two \ref scaled_integer values
    /// \headerfile cnl/scaled_integer.h
    ///
    /// \tparam Quotient the desired \ref scaled_integer type of quotient;
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
    CNL_NODISCARD constexpr auto quotient(Dividend const& dividend, Divisor const& divisor)
    -> typename _impl::result<Quotient, Dividend, Divisor>::type {
        using result_type = typename _impl::result<Quotient, Dividend, Divisor>::type;
        using result_rep = typename result_type::rep;
        return _impl::from_rep<result_type>(
                static_cast<result_rep>(_impl::fixed_width_scale<_impl::exponent_shift<result_type, Dividend, Divisor>::value>(
                        static_cast<result_rep>(_impl::not_scaled_integer(dividend)))
                        /_impl::not_scaled_integer(divisor)));
    }
}

#endif  // CNL_IMPL_SCALED_INTEGER_NAMED_H
