
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_POWER_BINARY_OPERATOR_H)
#define CNL_IMPL_SCALED_POWER_BINARY_OPERATOR_H

#include "../custom_operator/definition.h"
#include "../custom_operator/tagged.h"
#include "../num_traits/scale.h"
// #include "definition.h"
#include "is_scaled_tag.h"

#include <algorithm>

/// compositional numeric library
namespace cnl {
    template<_impl::binary_op Operator, integer LhsRep, integer RhsRep, scaled_tag Scale>
    struct custom_operator<Operator, op_value<LhsRep, Scale>, op_value<RhsRep, Scale>>
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

    template<
            _impl::binary_op Operator,
            integer LhsRep, scaled_tag LhsScale,
            integer RhsRep, scaled_tag RhsScale>
    requires(_impl::is_zero_degree<Operator>::value&& _impl::exponent_v<LhsScale> != _impl::exponent_v<RhsScale> && _impl::radix_v<LhsScale> == _impl::radix_v<RhsScale>) struct custom_operator<
            Operator,
            op_value<LhsRep, LhsScale>,
            op_value<RhsRep, RhsScale>> {
    private:
    public:
        [[nodiscard]] constexpr auto operator()(LhsRep const& lhs, RhsRep const& rhs) const
        {
            constexpr auto lhs_exponent = _impl::exponent_v<LhsScale>;
            constexpr auto rhs_exponent = _impl::exponent_v<RhsScale>;
            constexpr auto common_exponent = std::min(lhs_exponent, rhs_exponent);
            using common_scale = std::conditional_t<(lhs_exponent<rhs_exponent), LhsScale, RhsScale>;
            constexpr auto common_radix = _impl::radix_v<common_scale>;
            constexpr int lhs_left_shift = lhs_exponent - common_exponent;
            constexpr int rhs_left_shift = rhs_exponent - common_exponent;

            return _impl::operate<Operator, common_scale>{}(
                    _impl::scale<lhs_left_shift, common_radix>(lhs),
                    _impl::scale<rhs_left_shift, common_radix>(rhs));
        }
    };

    template<
            _impl::binary_op Operator,
            integer LhsRep, scaled_tag LhsScale,
            integer RhsRep, scaled_tag RhsScale>
    requires(!_impl::is_zero_degree<Operator>::value && _impl::exponent_v<LhsScale> != _impl::exponent_v<RhsScale> && _impl::radix_v<LhsScale> == _impl::radix_v<RhsScale>) struct custom_operator<
            Operator,
            op_value<LhsRep, LhsScale>,
            op_value<RhsRep, RhsScale>>
        : Operator {
    };

    template<_impl::shift_op Operator, integer LhsRep, scaled_tag LhsTag, integer RhsRep>
    requires(!_impl::is_constant<RhsRep>::value) struct custom_operator<
            Operator,
            op_value<LhsRep, LhsTag>,
            op_value<RhsRep>> {
        [[nodiscard]] constexpr auto operator()(LhsRep const& lhs, RhsRep const& rhs) const
        {
            return Operator{}(lhs, rhs);
        }
    };
}

#endif  // CNL_IMPL_SCALED_POWER_BINARY_OPERATOR_H
