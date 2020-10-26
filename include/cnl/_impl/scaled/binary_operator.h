
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
    template<
            class Operator,
            int Exponent, int Radix,
            typename Lhs, typename Rhs>
    struct binary_operator<
            Operator,
            power<Exponent, Radix>, power<Exponent, Radix>,
            Lhs, Rhs> : Operator {
    };

    namespace _impl {
        template<class Operator>
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
            class Operator,
            int LhsExponent, int RhsExponent, int Radix,
            typename Lhs, typename Rhs>
    struct binary_operator<
            Operator,
            power<LhsExponent, Radix>, power<RhsExponent, Radix>,
            Lhs, Rhs,
            _impl::enable_if_t<LhsExponent!=RhsExponent && _impl::is_zero_degree<Operator>::value>> {
    private:
        static constexpr int _common_exponent = _impl::min(LhsExponent, RhsExponent);
        using _common_power = power<_common_exponent, Radix>;
        static constexpr int _lhs_left_shift = LhsExponent-_common_exponent;
        static constexpr int _rhs_left_shift = RhsExponent-_common_exponent;
    public:
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return _impl::binary_operate<
                    Operator,
                    _common_power>(
                    _impl::scale<_lhs_left_shift, Radix>(lhs),
                    _impl::scale<_rhs_left_shift, Radix>(rhs));
        }
    };

    template<
            class Operator,
            int LhsExponent, int RhsExponent, int Radix,
            typename Lhs, typename Rhs>
    struct binary_operator<
            Operator,
            power<LhsExponent, Radix>, power<RhsExponent, Radix>,
            Lhs, Rhs,
            _impl::enable_if_t<LhsExponent!=RhsExponent && !_impl::is_zero_degree<Operator>::value>>
            : Operator {
    };
}

#endif // CNL_IMPL_SCALED_BINARY_OPERATOR_H
