
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_GENERIC_H)
#define CNL_IMPL_OVERFLOW_GENERIC_H

#include "builtin_overflow.h"
#include "is_overflow.h"
#include "is_overflow_tag.h"
#include "overflow_operator.h"
#include "../polarity.h"
#include "../operators/generic.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<class OverflowTag, class SrcTag, typename Destination, typename Source>
    struct convert_operator<OverflowTag, SrcTag, Destination, Source,
            _impl::enable_if_t<_impl::is_overflow_tag<OverflowTag>::value>> {
        CNL_NODISCARD constexpr Destination operator()(Source const& from) const
        {
            return _impl::is_overflow<_impl::convert_op, _impl::polarity::positive>{}.template operator()<Destination>(from)
                    ? _impl::overflow_operator<_impl::convert_op, OverflowTag, _impl::polarity::positive>{}.template operator()<Destination>(from)
                    : _impl::is_overflow<_impl::convert_op, _impl::polarity::negative>{}.template operator()<Destination>(from)
                            ? _impl::overflow_operator<_impl::convert_op, OverflowTag, _impl::polarity::negative>{}.template operator()<Destination>(from)
                            : static_cast<Destination>(from);
        }
    };

    template<class OverflowTag, class Operator, typename Operand>
    struct unary_operator<OverflowTag, Operator, Operand,
            _impl::enable_if_t<_impl::is_overflow_tag<OverflowTag>::value>> {
        CNL_NODISCARD constexpr auto operator()(Operand const& operand) const
        -> _impl::op_result<Operator, Operand>
        {
            return _impl::is_overflow<Operator, _impl::polarity::positive>{}(operand)
                    ? _impl::overflow_operator<Operator, OverflowTag, _impl::polarity::positive>{}(operand)
                    : _impl::is_overflow<Operator, _impl::polarity::negative>{}(operand)
                            ? _impl::overflow_operator<Operator, OverflowTag, _impl::polarity::negative>{}(operand)
                            : Operator{}(operand);
        }
    };

#if defined(CNL_BUILTIN_OVERFLOW_ENABLED)
    template<class OverflowTag, class Operator, typename Lhs, typename Rhs>
    struct binary_operator<OverflowTag, Operator, Lhs, Rhs,
            _impl::enable_if_t<_impl::is_overflow_tag<OverflowTag>::value
                    &&_impl::builtin_overflow_operator<Operator, Lhs, Rhs>::value>> {
        using result_type = _impl::op_result<Operator, Lhs, Rhs>;
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> result_type
        {
            result_type result{};
            if (!_impl::builtin_overflow_operator<Operator, Lhs, Rhs>{}(lhs, rhs, result)) {
                return result;
            }

            switch (_impl::overflow_polarity<Operator>{}(lhs, rhs)) {
            case _impl::polarity::positive:
                return _impl::overflow_operator<Operator, OverflowTag, _impl::polarity::positive>{}(lhs, rhs);
            case _impl::polarity::negative:
                return _impl::overflow_operator<Operator, OverflowTag, _impl::polarity::negative>{}(lhs, rhs);
            default:
                return _impl::unreachable<result_type>("CNL internal error");
            }
        }
    };
#endif

    template<class OverflowTag, class Operator, typename Lhs, typename Rhs>
    struct binary_operator<OverflowTag, Operator, Lhs, Rhs,
            _impl::enable_if_t<_impl::is_overflow_tag<OverflowTag>::value
                    &&!_impl::builtin_overflow_operator<Operator, Lhs, Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> _impl::op_result<Operator, Lhs, Rhs>
        {
            return _impl::is_overflow<Operator, _impl::polarity::positive>{}(lhs, rhs)
                    ? _impl::overflow_operator<Operator, OverflowTag, _impl::polarity::positive>{}(lhs, rhs)
                    : _impl::is_overflow<Operator, _impl::polarity::negative>{}(lhs, rhs)
                            ? _impl::overflow_operator<Operator, OverflowTag, _impl::polarity::negative>{}(lhs, rhs)
                            : Operator{}(lhs, rhs);
        }
    };

    template<class OverflowTag, class Operator, typename Lhs, typename Rhs>
    struct shift_operator<OverflowTag, Operator, Lhs, Rhs,
            _impl::enable_if_t<_impl::is_overflow_tag<OverflowTag>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> _impl::op_result<Operator, Lhs, Rhs>
        {
            return _impl::is_overflow<Operator, _impl::polarity::positive>{}(lhs, rhs)
                    ? _impl::overflow_operator<Operator, OverflowTag, _impl::polarity::positive>{}(lhs, rhs)
                    : _impl::is_overflow<Operator, _impl::polarity::negative>{}(lhs, rhs)
                            ? _impl::overflow_operator<Operator, OverflowTag, _impl::polarity::negative>{}(lhs, rhs)
                            : Operator{}(lhs, rhs);
        }
    };

    template<class OverflowTag, class Operator, typename Rhs>
    struct pre_operator<OverflowTag, Operator, Rhs,
            _impl::enable_if_t<_impl::is_overflow_tag<OverflowTag>::value>> {
        constexpr auto operator()(Rhs& rhs) const
        -> Rhs
        {
            return compound_assignment_operator<
                    OverflowTag, typename _impl::pre_to_assign<Operator>::type,
                    Rhs, int>{}(rhs, 1);
        }
    };

    template<class OverflowTag, class Operator, typename Rhs>
    struct post_operator<OverflowTag, Operator, Rhs,
            _impl::enable_if_t<_impl::is_overflow_tag<OverflowTag>::value>> {
        CNL_RELAXED_CONSTEXPR auto operator()(Rhs& rhs) const
        -> Rhs
        {
            auto copy = rhs;
            compound_assignment_operator<
                    OverflowTag, typename _impl::post_to_assign<Operator>::type,
                    Rhs, int>{}(rhs, 1);
            return copy;
        }
    };
}

#endif  // CNL_IMPL_OVERFLOW_GENERIC_H
