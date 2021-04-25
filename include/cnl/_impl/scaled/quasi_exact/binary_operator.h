
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_QUASI_EXACT_BINARY_OPERATOR_H)
#define CNL_IMPL_SCALED_QUASI_EXACT_BINARY_OPERATOR_H

#include "../../custom_operator/definition.h"
#include "../../custom_operator/op.h"
#include "../../num_traits/digits.h"
#include "../binary_operator.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    // template<_impl::binary_op Operator, typename Lhs, int LhsExponent, int RhsExponent, typename Rhs, int Radix>
    // struct custom_operator<
    //         Operator,
    //         op_value<Lhs, quasi_exact<LhsExponent, Radix>>,
    //         op_value<Rhs, quasi_exact<RhsExponent, Radix>>>
    //     : custom_operator<
    //               Operator,
    //               op_value<Lhs, power<LhsExponent, Radix>>,
    //               op_value<Rhs, power<RhsExponent, Radix>>> {
    // };

    namespace _impl {
        template<typename Rep, int Exponent, int Radix>
        [[nodiscard]] constexpr auto not_scaled_integer(
                scaled_integer<Rep, quasi_exact<Exponent, Radix>> const& f)
        {
            return _impl::to_rep(f);
        }

        template<typename Rep, int Exponent, int Radix>
        struct exponent<scaled_integer<Rep, quasi_exact<Exponent, Radix>>> : constant<Exponent> {
        };
    }

    template<typename Lhs, int LhsExponent, int RhsExponent, typename Rhs, int Radix>
    struct custom_operator<
            _impl::divide_op,
            op_value<Lhs, quasi_exact<LhsExponent, Radix>>,
            op_value<Rhs, quasi_exact<RhsExponent, Radix>>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            using natural_result = _impl::op_result<_impl::divide_op, Lhs, Rhs>;

            constexpr int integer_digits = _impl::integer_digits<Lhs> + _impl::fractional_digits<Rhs>;
            constexpr int fractional_digits = _impl::fractional_digits<Lhs> + _impl::integer_digits<Rhs>;

            constexpr auto necessary_digits = integer_digits + fractional_digits;
            constexpr auto natural_digits = digits_v<natural_result>;            CNL_ASSERT(necessary_digits >= natural_digits);     // !!!!!!!!!!!!!!!!!!!
            constexpr auto result_digits = std::max(necessary_digits, natural_digits);

            constexpr int rep_exponent = -fractional_digits;
            using scale = quasi_exact<rep_exponent>;

            using rep_type = set_digits_t<natural_result, result_digits>;
            using rep = decltype(_impl::not_scaled_integer(std::declval<rep_type>()));

            return _impl::from_rep<scaled_integer<rep, scale>>(
                    convert<scale, rep, quasi_exact<>>{}(cnl::fraction{lhs, rhs}));
        }
    };
}

#endif  // CNL_IMPL_SCALED_QUASI_EXACT_BINARY_OPERATOR_H
