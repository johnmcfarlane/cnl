
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential named definitions related to the `cnl::scaled_integer` type

#if !defined(CNL_IMPL_SCALED_INTEGER_NAMED_H)
#define CNL_IMPL_SCALED_INTEGER_NAMED_H

#include "../../fraction.h"
#include "../common.h"
#include "../num_traits/rep.h"
#include "../num_traits/tag.h"
#include "../scaled/power.h"
#include "../type_traits/enable_if.h"

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
    {
        return _impl::from_value<scaled_integer<>, Value>(value);
    }

    namespace _impl {
        template<typename Number>
        struct scaled_integer_rep {
            using type = Number;
        };

        template<typename Rep, int Exponent, int Radix>
        struct scaled_integer_rep<scaled_integer<Rep, power<Exponent, Radix>>> : scaled_integer_rep<Rep> {
        };

        template<class Dividend, class Divisor>
        struct quotient_result {
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
            using scale = power<rep_exponent>;

            using rep = typename scaled_integer_rep<rep_type>::type;
            using type = scaled_integer<rep, scale>;
        };
    }

    template<
            class Dividend,
            class Divisor>
    CNL_NODISCARD constexpr auto make_scaled_integer(fraction<Dividend, Divisor> const& f)
    -> typename _impl::quotient_result<Dividend, Divisor>::type
    {
        using quotient_result = _impl::quotient_result<Dividend, Divisor>;
        return _impl::from_rep<typename quotient_result::type>(
                convert<
                        typename quotient_result::scale,
                        power<>,
                        typename quotient_result::rep>(f));
    }

    /// \brief calculates the quotient of two \ref scaled_integer values
    /// \headerfile cnl/scaled_integer.h
    ///
    /// \tparam Dividend the dividend (top number) of the division
    /// \tparam Divisor the divisor (bottom number) of the division
    /// \param dividend, divisor dividend and divisor
    ///
    /// \return quotient: dividend / divisor
    ///
    /// \note This function provides 'quasi-exact' division as described in [P1368](http://wg21.link/p1368r1).


    template<
            class Dividend,
            class Divisor>
    CNL_NODISCARD constexpr auto quotient(Dividend const& dividend, Divisor const& divisor)
    {
        return make_scaled_integer(make_fraction(dividend, divisor));
    }
}

#endif  // CNL_IMPL_SCALED_INTEGER_NAMED_H
