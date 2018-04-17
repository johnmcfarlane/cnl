
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
        return _impl::from_value<fixed_point<Value, 0>>(value);
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

        template<class Rep, int Exponent>
        struct fixed_point_type<fixed_point<Rep, Exponent>> {
            using type = fixed_point<Rep, Exponent>;
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
    ///
    /// \sa divide

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
    // cnl::divide with fixed_point operand(s)

    namespace _divide_impl {
        template<class Lhs, class Rhs>
        struct divide;
    }

    /// \brief calculates the quotient of two \ref fixed_point values
    /// \headerfile cnl/fixed_point.h
    ///
    /// \param lhs, rhs dividend and divisor
    ///
    /// \return quotient: lhs / rhs
    ///
    /// \note This function divides the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa multiply

    template<class Lhs, class Rhs>
    constexpr auto divide(Lhs const& lhs, Rhs const& rhs)
    -> decltype(_divide_impl::divide<Lhs, Rhs>()(lhs, rhs)) {
        return _divide_impl::divide<Lhs, Rhs>()(lhs, rhs);
    }

    namespace _divide_impl {
        template<class Lhs, class Rhs>
        struct params {
            using rep_op_result = _impl::op_result<_impl::divide_op, typename Lhs::rep, typename Rhs::rep>;

            static constexpr int integer_digits =
                    _impl::integer_digits<Lhs>::value + _impl::fractional_digits<Rhs>::value;
            static constexpr int fractional_digits =
                    _impl::fractional_digits<Lhs>::value + _impl::integer_digits<Rhs>::value;
            static constexpr int necessary_digits = integer_digits + fractional_digits;

            using rep_type = set_digits_t<rep_op_result, necessary_digits>;
            static constexpr int rep_exponent = -fractional_digits;

            using result_type = fixed_point<rep_type, rep_exponent>;
        };

        template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
        struct divide<fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>> {
            constexpr auto operator()(fixed_point<LhsRep, LhsExponent> const& lhs, fixed_point<RhsRep, RhsExponent> const& rhs) const
            -> typename params<fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>>::result_type {
                using params = params<fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>>;
                using result_type = typename params::result_type;
                using result_rep = typename result_type::rep;

                return from_rep<result_type>{}(static_cast<result_rep>(_impl::scale<digits<RhsRep>::value>(
                        static_cast<typename params::rep_type>(to_rep(lhs)))/to_rep(rhs)));
            }
        };

        template<class Lhs, class RhsRep, int RhsExponent>
        struct divide<Lhs, fixed_point<RhsRep, RhsExponent>> {
            constexpr auto operator()(Lhs const& lhs, fixed_point<RhsRep, RhsExponent> const& rhs) const
            -> decltype(cnl::divide(make_fixed_point(lhs), rhs))
            {
                return cnl::divide(make_fixed_point(lhs), rhs);
            }
        };

        template<class LhsRep, int LhsExponent, class Rhs>
        struct divide<fixed_point<LhsRep, LhsExponent>, Rhs> {
            constexpr auto operator()(fixed_point<LhsRep, LhsExponent> const& lhs, Rhs const& rhs) const
            -> decltype(cnl::divide(lhs, make_fixed_point(rhs)))
            {
                return cnl::divide(lhs, make_fixed_point(rhs));
            }
        };

        template<class Lhs, class Rhs>
        struct divide {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(cnl::divide(make_fixed_point(lhs), make_fixed_point(rhs)))
            {
                return cnl::divide(make_fixed_point(lhs), make_fixed_point(rhs));
            }
        };
    }
}

#endif  // CNL_FIXED_POINT_NAMED_H
