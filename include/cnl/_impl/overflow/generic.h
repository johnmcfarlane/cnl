
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_GENERIC_H)
#define CNL_IMPL_OVERFLOW_GENERIC_H

#include "../operators/generic.h"
#include "../polarity.h"
#include "../type_traits/type_identity.h"
#include "builtin_overflow.h"
#include "is_overflow.h"
#include "is_overflow_tag.h"
#include "overflow_operator.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<tag Tag1, tag Tag2>
        struct common_overflow_tag {
        };

        template<tag OverflowTag, tag Tag>
        requires(_impl::is_overflow_tag<OverflowTag>::value && !_impl::is_overflow_tag<Tag>::value) struct common_overflow_tag<OverflowTag, Tag>
            : type_identity<OverflowTag> {
        };

        template<tag Tag, tag OverflowTag>
        requires(!_impl::is_overflow_tag<Tag>::value && _impl::is_overflow_tag<OverflowTag>::value) struct common_overflow_tag<Tag, OverflowTag>
            : type_identity<OverflowTag> {
        };

        template<overflow_tag OverflowTag>
        struct common_overflow_tag<OverflowTag, OverflowTag>
            : type_identity<OverflowTag> {
        };

        template<tag Tag1, tag Tag2>
        using common_overflow_tag_t = typename common_overflow_tag<Tag1, Tag2>::type;
    }

    template<tag DestTag, tag SrcTag, typename Destination, typename Source>
    requires(_impl::is_overflow_tag<DestTag>::value || _impl::is_overflow_tag<SrcTag>::value) struct convert_operator<DestTag, SrcTag, Destination, Source> {
        using overflow_tag = _impl::common_overflow_tag_t<DestTag, SrcTag>;

        CNL_NODISCARD constexpr Destination operator()(Source const& from) const
        {
            return _impl::is_overflow<_impl::convert_op, _impl::polarity::positive>{}
                                   .template operator()<Destination>(from)
                         ? _impl::overflow_operator<
                                   _impl::convert_op, overflow_tag, _impl::polarity::positive>{}
                                   .template operator()<Destination>(from)
                 : _impl::is_overflow<_impl::convert_op, _impl::polarity::negative>{}
                                   .template operator()<Destination>(from)
                         ? _impl::overflow_operator<
                                   _impl::convert_op, overflow_tag, _impl::polarity::negative>{}
                                   .template operator()<Destination>(from)
                         : static_cast<Destination>(from);
        }
    };

    template<class Operator, overflow_tag Tag, typename Operand>
    struct unary_operator<Operator, Tag, Operand> {
        CNL_NODISCARD constexpr auto operator()(Operand const& operand) const
                -> _impl::op_result<Operator, Operand>
        {
            return _impl::is_overflow<Operator, _impl::polarity::positive>{}(operand)
                         ? _impl::overflow_operator<
                                 Operator, Tag, _impl::polarity::positive>{}(operand)
                 : _impl::is_overflow<Operator, _impl::polarity::negative>{}(operand)
                         ? _impl::overflow_operator<
                                 Operator, Tag, _impl::polarity::negative>{}(operand)
                         : Operator{}(operand);
        }
    };

#if defined(CNL_BUILTIN_OVERFLOW_ENABLED)
    template<class Operator, overflow_tag LhsTag, overflow_tag RhsTag, typename Lhs, typename Rhs>
    requires _impl::builtin_overflow_operator<Operator, Lhs, Rhs>::value struct binary_operator<Operator, LhsTag, RhsTag, Lhs, Rhs> {
        using result_type = _impl::op_result<Operator, Lhs, Rhs>;

        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> result_type
        {
            result_type result{};
            if (!_impl::builtin_overflow_operator<Operator, Lhs, Rhs>{}(lhs, rhs, result)) {
                return result;
            }

            switch (_impl::overflow_polarity<Operator>{}(lhs, rhs)) {
            case _impl::polarity::positive:
                return _impl::overflow_operator<
                        Operator, _impl::common_overflow_tag_t<LhsTag, RhsTag>,
                        _impl::polarity::positive>{}(lhs, rhs);
            case _impl::polarity::negative:
                return _impl::overflow_operator<
                        Operator, _impl::common_overflow_tag_t<LhsTag, RhsTag>,
                        _impl::polarity::negative>{}(lhs, rhs);
            default:
                return _impl::unreachable<result_type>("CNL internal error");
            }
        }
    };
#endif

    template<class Operator, overflow_tag LhsTag, overflow_tag RhsTag, typename Lhs, typename Rhs>
    requires(!_impl::builtin_overflow_operator<Operator, Lhs, Rhs>::value) struct binary_operator<Operator, LhsTag, RhsTag, Lhs, Rhs> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
                -> _impl::op_result<Operator, Lhs, Rhs>
        {
            return _impl::is_overflow<Operator, _impl::polarity::positive>{}(lhs, rhs)
                         ? _impl::overflow_operator<
                                 Operator, _impl::common_overflow_tag_t<LhsTag, RhsTag>,
                                 _impl::polarity::positive>{}(lhs, rhs)
                 : _impl::is_overflow<Operator, _impl::polarity::negative>{}(lhs, rhs)
                         ? _impl::overflow_operator<
                                 Operator, _impl::common_overflow_tag_t<LhsTag, RhsTag>,
                                 _impl::polarity::negative>{}(lhs, rhs)
                         : Operator{}(lhs, rhs);
        }
    };

    template<class Operator, overflow_tag LhsTag, tag RhsTag, typename Lhs, typename Rhs>
    struct shift_operator<Operator, LhsTag, RhsTag, Lhs, Rhs> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
                -> _impl::op_result<Operator, Lhs, Rhs>
        {
            return _impl::is_overflow<Operator, _impl::polarity::positive>{}(lhs, rhs)
                         ? _impl::overflow_operator<
                                 Operator, _impl::common_overflow_tag_t<LhsTag, RhsTag>,
                                 _impl::polarity::positive>{}(lhs, rhs)
                 : _impl::is_overflow<Operator, _impl::polarity::negative>{}(lhs, rhs)
                         ? _impl::overflow_operator<
                                 Operator, _impl::common_overflow_tag_t<LhsTag, RhsTag>,
                                 _impl::polarity::negative>{}(lhs, rhs)
                         : Operator{}(lhs, rhs);
        }
    };

    template<class Operator, overflow_tag OverflowTag, typename Rhs>
    struct pre_operator<Operator, OverflowTag, Rhs> {
        constexpr auto operator()(Rhs& rhs) const -> Rhs
        {
            return compound_assignment_operator<
                    typename _impl::pre_to_assign<Operator>::type, OverflowTag, OverflowTag, Rhs,
                    int>{}(rhs, 1);
        }
    };

    template<class Operator, overflow_tag OverflowTag, typename Rhs>
    struct post_operator<Operator, OverflowTag, Rhs> {
        constexpr auto operator()(Rhs& rhs) const -> Rhs
        {
            auto copy = rhs;
            compound_assignment_operator<
                    typename _impl::post_to_assign<Operator>::type, OverflowTag, OverflowTag, Rhs,
                    int>{}(rhs, 1);
            return copy;
        }
    };
}

#endif  // CNL_IMPL_OVERFLOW_GENERIC_H
