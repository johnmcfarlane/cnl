
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_GENERIC_H)
#define CNL_IMPL_OVERFLOW_GENERIC_H

#include "../operators/generic.h"
#include "../polarity.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/type_identity.h"
#include "builtin_overflow.h"
#include "is_overflow.h"
#include "is_overflow_tag.h"
#include "overflow_operator.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<class Tag1, class Tag2, class Enable = void>
        struct common_overflow_tag;

        template<class OverflowTag, class Tag>
        struct common_overflow_tag<
                OverflowTag, Tag,
                enable_if_t<_impl::is_overflow_tag<OverflowTag>::value
                        && !_impl::is_overflow_tag<Tag>::value>>
                : type_identity<OverflowTag> {
        };

        template<class Tag, class OverflowTag>
        struct common_overflow_tag<
                Tag, OverflowTag,
                enable_if_t<!_impl::is_overflow_tag<Tag>::value
                        && _impl::is_overflow_tag<OverflowTag>::value>>
                : type_identity<OverflowTag> {
        };

        template<class OverflowTag>
        struct common_overflow_tag<
                OverflowTag, OverflowTag,
                enable_if_t<_impl::is_overflow_tag<OverflowTag>::value>>
                : type_identity<OverflowTag> {
        };

        template<class OverflowTag1, class OverflowTag2>
        struct common_overflow_tag<
                OverflowTag1, OverflowTag2,
                enable_if_t<_impl::is_overflow_tag<OverflowTag1>::value
                        && _impl::is_overflow_tag<OverflowTag2>::value
                        && !std::is_same<OverflowTag1, OverflowTag2>::value>> {
            // TODO: More rules about which overflow tags 'beat' which other overflow tags.
            // TODO: E.g. native is lowest 'ranking' and trapping/contract is probably highest.
        };

        template<class Tag1, class Tag2>
        using common_overflow_tag_t = typename common_overflow_tag<Tag1, Tag2>::type;
    }

    template<class DestTag, class SrcTag, typename Destination, typename Source>
    struct convert_operator<DestTag, SrcTag, Destination, Source,
            _impl::enable_if_t<_impl::is_overflow_tag<DestTag>::value || _impl::is_overflow_tag<SrcTag>::value>> {
        using overflow_tag = _impl::common_overflow_tag_t<DestTag, SrcTag>;

        CNL_NODISCARD constexpr Destination operator()(Source const& from) const
        {
            return _impl::is_overflow<_impl::convert_op, _impl::polarity::positive>{}.template operator()<Destination>(from)
                    ? _impl::overflow_operator<_impl::convert_op, overflow_tag, _impl::polarity::positive>{}.template operator()<Destination>(from)
                    : _impl::is_overflow<_impl::convert_op, _impl::polarity::negative>{}.template operator()<Destination>(from)
                            ? _impl::overflow_operator<_impl::convert_op, overflow_tag, _impl::polarity::negative>{}.template operator()<Destination>(from)
                            : static_cast<Destination>(from);
        }
    };

    template<class Operator, class OverflowTag, typename Operand>
    struct unary_operator<Operator, OverflowTag, Operand,
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
    template<class Operator, class LhsTag, class RhsTag, typename Lhs, typename Rhs>
    struct binary_operator<
            Operator,
            LhsTag, RhsTag,
            Lhs, Rhs,
            _impl::enable_if_t<_impl::is_overflow_tag<LhsTag>::value
                    && _impl::is_overflow_tag<RhsTag>::value
                    && _impl::builtin_overflow_operator<Operator, Lhs, Rhs>::value>> {
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
                return _impl::overflow_operator<
                        Operator, _impl::common_overflow_tag_t<LhsTag, RhsTag>, _impl::polarity::positive>{}(lhs, rhs);
            case _impl::polarity::negative:
                return _impl::overflow_operator<
                        Operator, _impl::common_overflow_tag_t<LhsTag, RhsTag>, _impl::polarity::negative>{}(lhs, rhs);
            default:
                return _impl::unreachable<result_type>("CNL internal error");
            }
        }
    };
#endif

    template<class Operator, class LhsTag, class RhsTag, typename Lhs, typename Rhs>
    struct binary_operator<Operator, LhsTag, RhsTag, Lhs, Rhs,
            _impl::enable_if_t<_impl::is_overflow_tag<LhsTag>::value
                    && _impl::is_overflow_tag<RhsTag>::value
                    && !_impl::builtin_overflow_operator<Operator, Lhs, Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> _impl::op_result<Operator, Lhs, Rhs>
        {
            return _impl::is_overflow<Operator, _impl::polarity::positive>{}(lhs, rhs)
                    ? _impl::overflow_operator<
                            Operator,
                            _impl::common_overflow_tag_t<LhsTag, RhsTag>, _impl::polarity::positive>{}(lhs, rhs)
                    : _impl::is_overflow<Operator, _impl::polarity::negative>{}(lhs, rhs)
                            ? _impl::overflow_operator<
                                    Operator,
                                    _impl::common_overflow_tag_t<LhsTag, RhsTag>, _impl::polarity::negative>{}(lhs, rhs)
                            : Operator{}(lhs, rhs);
        }
    };

    template<class Operator, class LhsTag, class RhsTag, typename Lhs, typename Rhs>
    struct shift_operator<Operator, LhsTag, RhsTag, Lhs, Rhs,
            _impl::enable_if_t<_impl::is_overflow_tag<LhsTag>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> _impl::op_result<Operator, Lhs, Rhs>
        {
            return _impl::is_overflow<Operator, _impl::polarity::positive>{}(lhs, rhs)
                    ? _impl::overflow_operator<
                            Operator,
                            _impl::common_overflow_tag_t<LhsTag, RhsTag>, _impl::polarity::positive>{}(lhs, rhs)
                    : _impl::is_overflow<Operator, _impl::polarity::negative>{}(lhs, rhs)
                            ? _impl::overflow_operator<
                                    Operator,
                                    _impl::common_overflow_tag_t<LhsTag, RhsTag>, _impl::polarity::negative>{}(lhs, rhs)
                            : Operator{}(lhs, rhs);
        }
    };

    template<class Operator, class OverflowTag, typename Rhs>
    struct pre_operator<Operator, OverflowTag, Rhs,
            _impl::enable_if_t<_impl::is_overflow_tag<OverflowTag>::value>> {
        constexpr auto operator()(Rhs& rhs) const
        -> Rhs
        {
            return compound_assignment_operator<
                    typename _impl::pre_to_assign<Operator>::type,
                    OverflowTag, OverflowTag,
                    Rhs, int>{}(rhs, 1);
        }
    };

    template<class Operator, class OverflowTag, typename Rhs>
    struct post_operator<Operator, OverflowTag, Rhs,
            _impl::enable_if_t<_impl::is_overflow_tag<OverflowTag>::value>> {
        constexpr auto operator()(Rhs& rhs) const
        -> Rhs
        {
            auto copy = rhs;
            compound_assignment_operator<
                    typename _impl::post_to_assign<Operator>::type,
                    OverflowTag, OverflowTag,
                    Rhs, int>{}(rhs, 1);
            return copy;
        }
    };
}

#endif  // CNL_IMPL_OVERFLOW_GENERIC_H
