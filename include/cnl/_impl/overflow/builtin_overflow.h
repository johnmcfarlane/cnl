
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_OVERFLOW_BUILTIN_OVERFLOW_H
#define CNL_IMPL_OVERFLOW_BUILTIN_OVERFLOW_H

#include "overflow_operator.h"
#include "../config.h"
#include "../operators.h"
#include "../polarity.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/is_integral.h"
#include "../unreachable.h"

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

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#endif

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
                return measure_polarity(lhs)*measure_polarity(rhs);
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::builtin_overflow

        enum class builtin_overflow_result {
            overflow,   // overflow occurred
            ok,   // overflow did not occur
            inconclusive    // there is no built-in test for this
        };

        template<class Operator, typename Lhs, typename Rhs, typename Result>
        CNL_NODISCARD constexpr builtin_overflow_result builtin_overflow(Operator, Lhs const&, Rhs const&, Result&)
        {
            return builtin_overflow_result::inconclusive;
        }

#if defined(CNL_BUILTIN_OVERFLOW_ENABLED)

        template<typename Lhs, typename Rhs, typename Result>
        CNL_NODISCARD constexpr auto builtin_overflow(add_op, Lhs const& lhs, Rhs const& rhs, Result& result)
        -> enable_if_t<
                is_integral<Lhs>::value && is_integral<Rhs>::value && is_integral<Result>::value,
                builtin_overflow_result>
        {
            return __builtin_add_overflow(lhs, rhs, &result)
                    ? builtin_overflow_result::overflow
                    : builtin_overflow_result::ok;
        }

        template<typename Lhs, typename Rhs, typename Result>
        CNL_NODISCARD constexpr auto builtin_overflow(subtract_op, Lhs const& lhs, Rhs const& rhs, Result& result)
        -> enable_if_t<
                is_integral<Lhs>::value && is_integral<Rhs>::value && is_integral<Result>::value,
                builtin_overflow_result>
        {
            return __builtin_sub_overflow(lhs, rhs, &result)
                    ? builtin_overflow_result::overflow
                    : builtin_overflow_result::ok;
        }

        template<typename Lhs, typename Rhs, typename Result>
        CNL_NODISCARD constexpr auto builtin_overflow(multiply_op, Lhs const& lhs, Rhs const& rhs, Result& result)
        -> enable_if_t<
                is_integral<Lhs>::value && is_integral<Rhs>::value && is_integral<Result>::value,
                builtin_overflow_result>
        {
            return __builtin_mul_overflow(lhs, rhs, &result)
                    ? builtin_overflow_result::overflow
                    : builtin_overflow_result::ok;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::builtin_binary_overflow_operator

        template<class OverflowTag, class Operator, class Lhs, class Rhs>
        CNL_NODISCARD constexpr auto builtin_binary_overflow_operator(Lhs const& lhs, Rhs const& rhs)
        -> op_result<Operator, Lhs, Rhs>
        {
            using Result = op_result<Operator, Lhs, Rhs>;
            Result result{};
            switch (builtin_overflow(Operator{}, lhs, rhs, result))
            {
            case builtin_overflow_result::ok:
                return result;
            case builtin_overflow_result::overflow:
                switch (overflow_polarity<Operator>{}(lhs, rhs)) {
                case polarity::positive:
                    return overflow_operator<Operator, OverflowTag, polarity::positive>{}(lhs, rhs);
                case polarity::negative:
                    return overflow_operator<Operator, OverflowTag, polarity::negative>{}(lhs, rhs);
                case polarity::neutral:
                    return unreachable<Result>("CNL internal error");
                }
            case builtin_overflow_result::inconclusive:
                break;
            }
            return unreachable<Result>("CNL internal error");
        }

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::builtin_overflow_supported

        template<class Operator, typename Lhs, typename Rhs>
        CNL_NODISCARD constexpr bool builtin_overflow_supported()
        {
            using Result = op_result<Operator, Lhs, Rhs>;
            Result result{};
            return builtin_overflow(Operator{}, Lhs{}, Rhs{}, result)!=builtin_overflow_result::inconclusive;
        }
#else

        template<class Operator, typename Lhs, typename Rhs>
        CNL_NODISCARD constexpr bool builtin_overflow_supported()
        {
            return false;
        }
#endif  // CNL_BUILTIN_OVERFLOW_ENABLED
    }
}

#endif  // CNL_IMPL_OVERFLOW_BUILTIN_OVERFLOW_H
