
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to `cnl::fixed_point` arithmetic

#if !defined(CNL_FIXED_POINT_ARITHMETIC_H)
#define CNL_FIXED_POINT_ARITHMETIC_H 1

#include "type.h"
#include "num_traits.h"

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // implementation-specific definitions

    namespace _impl {
        namespace fp {
            namespace arithmetic {

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

                ////////////////////////////////////////////////////////////////////////////////
                // cnl::_impl::fp::arithmetic::result

                template<class OperationTag, class Lhs, class Rhs>
                struct result;

                template<class Lhs, class Rhs>
                struct result<_impl::multiply_op, Lhs, Rhs> {
                    using lhs_rep = typename Lhs::rep;
                    using rhs_rep = typename Rhs::rep;
                    using rep_op_result = _impl::op_result<_impl::multiply_op, lhs_rep, rhs_rep>;

                    static constexpr int digits = cnl::digits<Lhs>::value+cnl::digits<Rhs>::value;
                    static constexpr bool is_signed =
                            numeric_limits<lhs_rep>::is_signed || numeric_limits<rhs_rep>::is_signed;

                    using prewidened_result_rep = _impl::make_signed_t<rep_op_result, is_signed>;
                    using rep_type = set_digits_t<prewidened_result_rep, digits>;

                    static constexpr int rep_exponent = Lhs::exponent+Rhs::exponent;

                    using type = fixed_point<rep_type, rep_exponent>;
                };

                template<class Lhs, class Rhs>
                struct result<_impl::divide_op, Lhs, Rhs> {
                    using lhs_rep = typename Lhs::rep;
                    using rhs_rep = typename Rhs::rep;
                    using rep_op_result = _impl::op_result<_impl::multiply_op, lhs_rep, rhs_rep>;

                    static constexpr int integer_digits = _impl::integer_digits<Lhs>::value+fractional_digits<Rhs>::value;
                    static constexpr int fractional_digits = _impl::fractional_digits<Lhs>::value+_impl::integer_digits<Rhs>::value;
                    static constexpr int necessary_digits = integer_digits+fractional_digits;
                    static constexpr bool is_signed =
                            numeric_limits<lhs_rep>::is_signed || numeric_limits<rhs_rep>::is_signed;

                    static constexpr int promotion_digits = digits<rep_op_result>::value;
                    static constexpr int digits = _impl::max(necessary_digits, promotion_digits);

                    using prewidened_result_rep = _impl::make_signed_t<rep_op_result, is_signed>;
                    using rep_type = set_digits_t<prewidened_result_rep, digits>;

                    static constexpr int rep_exponent = -fractional_digits;

                    using type = fixed_point<rep_type, rep_exponent>;
                };

                ////////////////////////////////////////////////////////////////////////////////
                // cnl::_impl::fp::arithmetic::intermediate

                // default
                template<class OperationTag, class Lhs, class Rhs>
                struct intermediate;

                template<class Lhs, class Rhs>
                struct intermediate<_impl::multiply_op, Lhs, Rhs> {
                    using _result = result<_impl::multiply_op, Lhs, Rhs>;
                    using result_rep = typename _result::rep_type;
                    using prewidened_result_rep = typename _result::prewidened_result_rep;

                    // If the 'natural' result of the rep op is wide enough, stick with it.
                    // This ensures that auto-widening rep types (e.g. elastic_integer) don't get widened twice
                    // but types that need a little help (e.g. built-ins) get widened going into the op.
                    using rep_type = typename std::conditional<
                            digits<prewidened_result_rep>::value>=_result::digits,
                            typename Lhs::rep, result_rep>::type;

                    using lhs_type = fixed_point<rep_type, Lhs::exponent>;
                    using rhs_type = Rhs;
                };

                template<class Lhs, class Rhs>
                struct intermediate<_impl::divide_op, Lhs, Rhs> {
                    using wide_result = result<_impl::divide_op, Lhs, Rhs>;
                    using rep_type = typename wide_result::rep_type;

                    static constexpr int exponent = Lhs::exponent-digits<Rhs>::value;

                    using lhs_type = fixed_point<rep_type, exponent>;
                    using rhs_type = Rhs;
                };

                ////////////////////////////////////////////////////////////////////////////////
                // cnl::_impl::fp::arithmetic::operate_params

                template<class OperationTag, class Lhs, class Rhs>
                struct operate_params {
                    using lhs_type = typename fixed_point_type<Lhs>::type;
                    using rhs_type = typename fixed_point_type<Rhs>::type;

                    using _intermediate = intermediate<OperationTag, lhs_type, rhs_type>;
                    using intermediate_lhs = typename _intermediate::lhs_type;
                    using intermediate_rhs = typename _intermediate::rhs_type;

                    using _result = result<OperationTag, lhs_type, rhs_type>;
                    using result_type = typename _result::type;
                };
            }

            ////////////////////////////////////////////////////////////////////////////////
            // mappings from named function strategies to public API

            ////////////////////////////////////////////////////////////////////////////////
            // cnl::_impl::fixed_point::operate

            template<class Operation, class Lhs, class Rhs>
            constexpr auto operate(Lhs const& lhs, Rhs const& rhs, Operation)
            -> typename arithmetic::operate_params<Operation, Lhs, Rhs>::result_type
            {
                using params = arithmetic::operate_params<Operation, Lhs, Rhs>;
                using intermediate_lhs = typename params::intermediate_lhs;
                using intermediate_rhs = typename params::intermediate_rhs;
                using result_type = typename params::result_type;
                using result_rep = typename result_type::rep;

                return _impl::from_rep<result_type>(
                        static_cast<result_rep>(
                                Operation()(
                                        _impl::to_rep(static_cast<intermediate_lhs>(lhs)),
                                        _impl::to_rep(static_cast<intermediate_rhs>(rhs)))));
            };
        }
    }
}

#endif  // CNL_FIXED_POINT_ARITHMETIC_H
