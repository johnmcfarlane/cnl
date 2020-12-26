
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_BUILTIN_OVERFLOW_H)
#define CNL_IMPL_OVERFLOW_BUILTIN_OVERFLOW_H

#include "../config.h"
#include "../operators/operators.h"
#include "../polarity.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/is_integral.h"
#include "../unreachable.h"
#include "overflow_operator.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_polarity

        template<class Operator>
        struct overflow_polarity;

        template<>
        struct overflow_polarity<convert_op> {
            template<typename Destination, typename Source>
            CNL_NODISCARD constexpr polarity operator()(Source const& from) const
            {
                return measure_polarity(from);
            }
        };

        template<>
        struct overflow_polarity<minus_op> {
            template<typename Rhs>
            CNL_NODISCARD constexpr polarity operator()(Rhs const& rhs) const
            {
                return -measure_polarity(rhs);
            }
        };

        template<>
        struct overflow_polarity<add_op> {
            template<typename Lhs, typename Rhs>
            CNL_NODISCARD constexpr polarity operator()(Lhs const&, Rhs const& rhs) const
            {
                return measure_polarity(rhs);
            }
        };

        template<>
        struct overflow_polarity<subtract_op> {
            template<typename Lhs, typename Rhs>
            CNL_NODISCARD constexpr polarity operator()(Lhs const&, Rhs const& rhs) const
            {
                return -measure_polarity(rhs);
            }
        };

        template<>
        struct overflow_polarity<multiply_op> {
            template<typename Lhs, typename Rhs>
            CNL_NODISCARD constexpr polarity operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return measure_polarity(lhs) * measure_polarity(rhs);
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::are_builtin_operands

        template<typename Lhs, typename Rhs>
        struct are_builtin_operands
            : std::integral_constant<
                      bool, _impl::is_integral<Lhs>::value && _impl::is_integral<Rhs>::value> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::builtin_overflow_operator

        template<class Operator, typename Lhs, typename Rhs>
        struct builtin_overflow_operator : std::false_type {
        };

#if defined(CNL_BUILTIN_OVERFLOW_ENABLED)
        template<typename Lhs, typename Rhs>
        struct builtin_overflow_operator<add_op, Lhs, Rhs> : are_builtin_operands<Lhs, Rhs> {
            template<typename Result>
            CNL_NODISCARD constexpr auto operator()(
                    Lhs const& lhs, Rhs const& rhs, Result& result) const
            {
                return __builtin_add_overflow(lhs, rhs, &result);
            }
        };

        template<typename Lhs, typename Rhs>
        struct builtin_overflow_operator<subtract_op, Lhs, Rhs> : are_builtin_operands<Lhs, Rhs> {
            template<typename Result>
            CNL_NODISCARD constexpr auto operator()(
                    Lhs const& lhs, Rhs const& rhs, Result& result) const
            {
                return __builtin_sub_overflow(lhs, rhs, &result);
            }
        };

        template<typename Lhs, typename Rhs>
        struct builtin_overflow_operator<multiply_op, Lhs, Rhs> : are_builtin_operands<Lhs, Rhs> {
            template<typename Result>
            CNL_NODISCARD constexpr auto operator()(
                    Lhs const& lhs, Rhs const& rhs, Result& result) const
            {
                return __builtin_mul_overflow(lhs, rhs, &result);
            }
        };
#endif
    }
}

#endif  // CNL_IMPL_OVERFLOW_BUILTIN_OVERFLOW_H
