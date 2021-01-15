
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_BINARY_OPERATOR_H)
#define CNL_IMPL_SCALED_BINARY_OPERATOR_H

#include "../num_traits/scale.h"
#include "../operators/generic.h"
#include "../operators/tagged.h"
#include "../type_traits/enable_if.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    template<_impl::binary_arithmetic_op Operator, typename Lhs, typename Rhs, int Exponent, int Radix>
    struct custom_operator<Operator, operand<Lhs, power<Exponent, Radix>>, operand<Rhs, power<Exponent, Radix>>>
        : Operator {
    };

    namespace _impl {
        template<binary_arithmetic_op Operator>
        struct is_zero_degree : std::true_type {
        };
        template<>
        struct is_zero_degree<multiply_op> : std::false_type {
        };
        template<>
        struct is_zero_degree<divide_op> : std::false_type {
        };
        template<>
        struct is_zero_degree<modulo_op> : std::false_type {
        };
    }

    template<_impl::binary_arithmetic_op Operator, typename Lhs, int LhsExponent, int RhsExponent, typename Rhs, int Radix>
    requires(LhsExponent != RhsExponent && _impl::is_zero_degree<Operator>::value) struct custom_operator<
            Operator,
            operand<Lhs, power<LhsExponent, Radix>>,
            operand<Rhs, power<RhsExponent, Radix>>> {
    private:
        static constexpr int _common_exponent = _impl::min(LhsExponent, RhsExponent);
        using _common_power = power<_common_exponent, Radix>;
        static constexpr int _lhs_left_shift = LhsExponent - _common_exponent;
        static constexpr int _rhs_left_shift = RhsExponent - _common_exponent;

    public:
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return _impl::binary_arithmetic_operate<Operator, _common_power>(
                    _impl::scale<_lhs_left_shift, Radix>(lhs),
                    _impl::scale<_rhs_left_shift, Radix>(rhs));
        }
    };

    template<_impl::binary_arithmetic_op Operator, typename Lhs, int LhsExponent, typename Rhs, int RhsExponent, int Radix>
    requires(LhsExponent != RhsExponent && !_impl::is_zero_degree<Operator>::value) struct custom_operator<
            Operator,
            operand<Lhs, power<LhsExponent, Radix>>,
            operand<Rhs, power<RhsExponent, Radix>>>
        : Operator {
    };
}

#endif  // CNL_IMPL_SCALED_BINARY_OPERATOR_H
