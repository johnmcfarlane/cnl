
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_BINARY_OPERATOR_H)
#define CNL_IMPL_SCALED_BINARY_OPERATOR_H

#include "../custom_operator/definition.h"
#include "../custom_operator/tagged.h"
#include "../num_traits/scale.h"
#include "definition.h"
#include "is_scaled_tag.h"

#include <algorithm>

/// compositional numeric library
namespace cnl {
    template<_impl::binary_op Operator, typename Lhs, typename Rhs, int Exponent, int Radix>
    struct custom_operator<Operator, op_value<Lhs, power<Exponent, Radix>>, op_value<Rhs, power<Exponent, Radix>>>
        : Operator {
    };

    namespace _impl {
        template<binary_op Operator>
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

    template<_impl::binary_op Operator, typename Lhs, int LhsExponent, int RhsExponent, typename Rhs, int Radix>
    requires(LhsExponent != RhsExponent && _impl::is_zero_degree<Operator>::value) struct custom_operator<
            Operator,
            op_value<Lhs, power<LhsExponent, Radix>>,
            op_value<Rhs, power<RhsExponent, Radix>>> {
    private:
        static constexpr int _common_exponent = std::min(LhsExponent, RhsExponent);
        using _common_power = power<_common_exponent, Radix>;
        static constexpr int _lhs_left_shift = LhsExponent - _common_exponent;
        static constexpr int _rhs_left_shift = RhsExponent - _common_exponent;

    public:
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return _impl::custom_operate<Operator, _common_power>(
                    _impl::scale<_lhs_left_shift, Radix>(lhs),
                    _impl::scale<_rhs_left_shift, Radix>(rhs));
        }
    };

    template<_impl::binary_op Operator, typename Lhs, int LhsExponent, typename Rhs, int RhsExponent, int Radix>
    requires(LhsExponent != RhsExponent && !_impl::is_zero_degree<Operator>::value) struct custom_operator<
            Operator,
            op_value<Lhs, power<LhsExponent, Radix>>,
            op_value<Rhs, power<RhsExponent, Radix>>>
        : Operator {
    };

    template<_impl::shift_op Operator, typename LhsRep, scaled_tag LhsTag, typename Rhs>
    requires(!_impl::is_constant<Rhs>::value) struct custom_operator<
            Operator,
            op_value<LhsRep, LhsTag>,
            op_value<Rhs>> {
        [[nodiscard]] constexpr auto operator()(LhsRep const& lhs, Rhs const& rhs) const
        {
            return Operator{}(lhs, rhs);
        }
    };
}

#endif  // CNL_IMPL_SCALED_BINARY_OPERATOR_H
