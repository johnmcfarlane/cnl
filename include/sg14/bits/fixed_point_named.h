
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential named definitions related to the `sg14::fixed_point` type

#if !defined(SG14_FIXED_POINT_NAMED_H)
#define SG14_FIXED_POINT_NAMED_H 1

#include "common.h"

#include "fixed_point_type.h"

/// study group 14 of the C++ working group
namespace sg14 {

    ////////////////////////////////////////////////////////////////////////////////
    // file-local implementation-specific definitions

    namespace _fixed_point_named_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // binary_pair

        template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
        struct binary_pair_base {
            using lhs_type = fixed_point<LhsRep, LhsExponent>;
            using rhs_type = fixed_point<RhsRep, RhsExponent>;
        };

        template<class Lhs, class Rhs>
        struct binary_pair;

        template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
        struct binary_pair<fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>>
                : binary_pair_base<LhsRep, LhsExponent, RhsRep, RhsExponent> {
        };

        template<class LhsRep, int LhsExponent, class Rhs>
        struct binary_pair<fixed_point<LhsRep, LhsExponent>, Rhs> : binary_pair_base<LhsRep, LhsExponent, Rhs, 0> {
            static_assert(std::numeric_limits<Rhs>::is_integer,
                    "named arithmetic functions take only fixed_point and integral types");
        };

        template<class Lhs, class RhsRep, int RhsExponent>
        struct binary_pair<Lhs, fixed_point<RhsRep, RhsExponent>> : binary_pair_base<Lhs, 0, RhsRep, RhsExponent> {
            static_assert(std::numeric_limits<Lhs>::is_integer,
                    "named arithmetic functions take only fixed_point and integral types");
        };

        ////////////////////////////////////////////////////////////////////////////////
        // tags

        // strategy
        struct lean_tag;    // like-for-like interger arithmetic
        struct wide_tag;    // effort is made to widen to accommodate results of multiplication and division

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_named_impl::rep_op_exponent

        template<class OperationTag, class ... Operands>
        struct rep_op_exponent;

        template<class Rhs>
        struct rep_op_exponent<_impl::negate_tag, Rhs> : public std::integral_constant<int, Rhs::exponent> {
        };

        template<class Lhs, class Rhs>
        struct rep_op_exponent<_impl::add_tag, Lhs, Rhs> : public std::integral_constant<int, _impl::min<int>(
                Lhs::exponent,
                Rhs::exponent)> {
        };

        template<class Lhs, class Rhs>
        struct rep_op_exponent<_impl::subtract_tag, Lhs, Rhs> : public std::integral_constant<int, _impl::min<int>(
                Lhs::exponent,
                Rhs::exponent)> {
        };

        template<class Lhs, class Rhs>
        struct rep_op_exponent<_impl::multiply_tag, Lhs, Rhs> : public std::integral_constant<int,
                Lhs::exponent+Rhs::exponent> {
        };

        template<class Lhs, class Rhs>
        struct rep_op_exponent<_impl::divide_tag, Lhs, Rhs> : public std::integral_constant<int,
                Lhs::exponent-Rhs::exponent> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_named_impl::result

        template<class PolicyTag, class OperationTag, class Lhs, class Rhs>
        struct result;

        // result<lean_tag>
        template<class OperationTag, class Lhs, class Rhs>
        struct result<lean_tag, OperationTag, Lhs, Rhs> {
            using lhs_rep = typename Lhs::rep;
            using rhs_rep = typename Rhs::rep;
            using rep_op_result = _impl::op_result<OperationTag, lhs_rep, rhs_rep>;

            static constexpr int exponent = rep_op_exponent<OperationTag, Lhs, Rhs>::value;

            using type = fixed_point<rep_op_result, exponent>;
        };

        // result<wide_tag>
        template<class OperationTag, class Lhs, class Rhs>
        struct result<wide_tag, OperationTag, Lhs, Rhs> {
            using lhs_rep = typename Lhs::rep;
            using rhs_rep = typename Rhs::rep;
            using rep_op_result = _impl::op_result<OperationTag, lhs_rep, rhs_rep>;

            // 'Wide' doesn't guarantee avoiding overflow. Adding a single bit to add/subtract results would often lead to double the width being used.
            static constexpr int sufficient_sign_bits = std::is_signed<rep_op_result>::value;
            static constexpr int sufficient_integer_digits = _impl::max(Lhs::integer_digits,
                    Rhs::integer_digits);
            static constexpr int sufficient_fractional_digits = _impl::max(Lhs::fractional_digits,
                    Rhs::fractional_digits);
            static constexpr _width_type sufficient_width =
                    sufficient_sign_bits+sufficient_integer_digits+sufficient_fractional_digits;
            static constexpr int result_width = _impl::max(sufficient_width, width<rep_op_result>::value);

            using rep_type = set_width_t<rep_op_result, result_width>;
            using type = fixed_point<rep_type, -sufficient_fractional_digits>;
        };
        template<class Lhs, class Rhs>
        struct result<wide_tag, _impl::multiply_tag, Lhs, Rhs> {
            using lhs_rep = typename Lhs::rep;
            using rhs_rep = typename Rhs::rep;
            using rep_op_result = _impl::op_result<_impl::multiply_tag, lhs_rep, rhs_rep>;

            static constexpr int digits = Lhs::digits+Rhs::digits;
            static constexpr bool is_signed =
                    std::numeric_limits<lhs_rep>::is_signed || std::numeric_limits<rhs_rep>::is_signed;
            static constexpr int width = digits+is_signed;

            using prewidened_result_rep = _impl::make_signed_t<rep_op_result, is_signed>;
            using rep_type = set_width_t<prewidened_result_rep, width>;

            static constexpr int rep_exponent = rep_op_exponent<_impl::multiply_tag, Lhs, Rhs>::value;

            using type = fixed_point<rep_type, rep_exponent>;
        };
        template<class Lhs, class Rhs>
        struct result<wide_tag, _impl::divide_tag, Lhs, Rhs> {
            using lhs_rep = typename Lhs::rep;
            using rhs_rep = typename Rhs::rep;
            using rep_op_result = _impl::op_result<_impl::multiply_tag, lhs_rep, rhs_rep>;

            static constexpr int integer_digits = Lhs::integer_digits+Rhs::fractional_digits;
            static constexpr int fractional_digits = Lhs::fractional_digits+Rhs::integer_digits;
            static constexpr int digits = integer_digits+fractional_digits;
            static constexpr bool is_signed =
                    std::numeric_limits<lhs_rep>::is_signed || std::numeric_limits<rhs_rep>::is_signed;

            static constexpr int necessary_width = digits+is_signed;
            static constexpr int promotion_width = width<rep_op_result>::value;
            static constexpr int width = _impl::max(necessary_width, promotion_width);

            using prewidened_result_rep = _impl::make_signed_t<rep_op_result, is_signed>;
            using rep_type = set_width_t<prewidened_result_rep, width>;

            static constexpr int rep_exponent = -fractional_digits;

            using type = fixed_point<rep_type, rep_exponent>;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_named_impl::intermediate

        template<class PolicyTag, class OperationTag, class Lhs, class Rhs>
        struct intermediate;

        // bare add / subtract

        template<class OperationTag, class Lhs, class Rhs>
        struct intermediate<lean_tag, OperationTag, Lhs, Rhs> {
            using _result = result<lean_tag, OperationTag, Lhs, Rhs>;
            using lhs_type = typename _result::type;
            using rhs_type = lhs_type;
        };

        template<class Lhs, class Rhs>
        struct intermediate<lean_tag, _impl::multiply_tag, Lhs, Rhs> {
            using lhs_type = Lhs;
            using rhs_type = Rhs;
        };

        template<class Lhs, class Rhs>
        struct intermediate<lean_tag, _impl::divide_tag, Lhs, Rhs> {
            using lhs_type = Lhs;
            using rhs_type = Rhs;
        };

        // intermediate - wide

        // wide - add/subtract
        template<class OperationTag, class Lhs, class Rhs>
        struct intermediate<wide_tag, OperationTag, Lhs, Rhs> {
            using _result = result<wide_tag, OperationTag, Lhs, Rhs>;
            using lhs_type = typename _result::type;
            using rhs_type = lhs_type;
        };

        template<class Lhs, class Rhs>
        struct intermediate<wide_tag, _impl::multiply_tag, Lhs, Rhs> {
            using _result = result<wide_tag, _impl::multiply_tag, Lhs, Rhs>;
            using result_rep = typename _result::rep_type;
            using prewidened_result_rep = typename _result::prewidened_result_rep;

            // If the 'natural' result of the rep op is wide enough, stick with it.
            // This ensures that auto-widening rep types (e.g. elastic_integer) don't get widened twice
            // but types that need a little help (e.g. built-ins) get widened going into the op.
            using rep_type = typename std::conditional<
                    width<prewidened_result_rep>::value>=_result::width,
                    typename Lhs::rep, result_rep>::type;

            using lhs_type = fixed_point<rep_type, Lhs::exponent>;
            using rhs_type = Rhs;
        };

        template<class Lhs, class Rhs>
        struct intermediate<wide_tag, _impl::divide_tag, Lhs, Rhs> {
            using wide_result = result<wide_tag, _impl::divide_tag, Lhs, Rhs>;
            using rep_type = typename wide_result::rep_type;

            static constexpr int exponent = Lhs::exponent-Rhs::digits;

            using lhs_type = fixed_point<rep_type, exponent>;
            using rhs_type = Rhs;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_named_impl::operate_params

        template<class PolicyTag, class OperationTag, class Lhs, class Rhs>
        struct operate_params;

        template<class PolicyTag, class OperationTag, class Lhs, class Rhs>
        struct operate_params {
            using _binary_pair = binary_pair<Lhs, Rhs>;
            using lhs_type = typename _binary_pair::lhs_type;
            using rhs_type = typename _binary_pair::rhs_type;

            using _intermediate = intermediate<PolicyTag, OperationTag, lhs_type, rhs_type>;
            using intermediate_lhs = typename _intermediate::lhs_type;
            using intermediate_rhs = typename _intermediate::rhs_type;

            using _result = result<PolicyTag, OperationTag, lhs_type, rhs_type>;
            using result_type = typename _result::type;
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // general-purpose implementation-specific definitions

    namespace _fixed_point_impl {

        ////////////////////////////////////////////////////////////////////////////////
        // mappings from operator strategies to public API

        // strategy aliases - for ease of flip-flopping
        using named_function_tag = _fixed_point_named_impl::lean_tag;
        using arithmetic_operator_tag = _fixed_point_named_impl::wide_tag;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::operate

        template<class PolicyTag, class OperationTag, class Lhs, class Rhs>
        constexpr auto operate(const Lhs& lhs, const Rhs& rhs)
        -> typename _fixed_point_named_impl::operate_params<PolicyTag, OperationTag, Lhs, Rhs>::result_type
        {
            using params = _fixed_point_named_impl::operate_params<PolicyTag, OperationTag, Lhs, Rhs>;
            using intermediate_lhs = typename params::intermediate_lhs;
            using intermediate_rhs = typename params::intermediate_rhs;
            using result_type = typename params::result_type;
            using result_rep = typename result_type::rep;

            return result_type::from_data(
                    static_cast<result_rep>(
                            _impl::op_fn<OperationTag>(
                                    static_cast<intermediate_lhs>(lhs).data(),
                                    static_cast<intermediate_rhs>(rhs).data())));
        };
    }

    /// \brief calculates the negative of a \ref fixed_point value
    /// \headerfile sg14/fixed_point.h
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
    /// \headerfile sg14/fixed_point.h
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
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::add_tag>(lhs, rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::add_tag>(lhs, rhs);
    }

    /// \brief calculates the difference of two \ref fixed_point values
    /// \headerfile sg14/fixed_point.h
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
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::subtract_tag>(lhs,
            rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::subtract_tag>(lhs,
                rhs);
    }

    /// \brief calculates the product of two \ref fixed_point factors
    /// \headerfile sg14/fixed_point.h
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
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::multiply_tag>(lhs, rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::multiply_tag>(lhs, rhs);
    }

    /// \brief calculates the quotient of two \ref fixed_point values
    /// \headerfile sg14/fixed_point.h
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
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::divide_tag>(lhs,
            rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::named_function_tag, _impl::divide_tag>(lhs,
                rhs);
    }
}

#endif	// SG14_FIXED_POINT_NAMED_H
