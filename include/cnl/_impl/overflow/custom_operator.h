
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_GENERIC_H)
#define CNL_IMPL_OVERFLOW_GENERIC_H

#include "../operators/custom_operator.h"
#include "../polarity.h"
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
            : std::type_identity<OverflowTag> {
        };

        template<tag Tag, tag OverflowTag>
        requires(!_impl::is_overflow_tag<Tag>::value && _impl::is_overflow_tag<OverflowTag>::value) struct common_overflow_tag<Tag, OverflowTag>
            : std::type_identity<OverflowTag> {
        };

        template<overflow_tag OverflowTag>
        struct common_overflow_tag<OverflowTag, OverflowTag>
            : std::type_identity<OverflowTag> {
        };

        template<tag Tag1, tag Tag2>
        using common_overflow_tag_t = typename common_overflow_tag<Tag1, Tag2>::type;
    }

    template<typename Source, tag SrcTag, typename Destination, tag DestTag>
    /// \cond
    requires(_impl::is_overflow_tag<DestTag>::value || _impl::is_overflow_tag<SrcTag>::value)
    /// \endcond
    struct custom_operator<_impl::convert_op, operand<Source, SrcTag>, operand<Destination, DestTag>> {
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

    template<_impl::unary_arithmetic_op Operator, typename Operand, overflow_tag Tag>
    struct custom_operator<Operator, operand<Operand, Tag>> {
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
    template<_impl::binary_arithmetic_op Operator, typename Lhs, overflow_tag LhsTag, typename Rhs, overflow_tag RhsTag>
    requires _impl::builtin_overflow_operator<Operator, Lhs, Rhs>::value struct custom_operator<Operator, operand<Lhs, LhsTag>, operand<Rhs, RhsTag>> {
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

    template<_impl::binary_arithmetic_op Operator, typename Lhs, overflow_tag LhsTag, typename Rhs, overflow_tag RhsTag>
    requires(!_impl::builtin_overflow_operator<Operator, Lhs, Rhs>::value) struct custom_operator<Operator, operand<Lhs, LhsTag>, operand<Rhs, RhsTag>> {
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

    template<_impl::shift_op Operator, typename Lhs, overflow_tag LhsTag, typename Rhs, tag RhsTag>
    struct custom_operator<Operator, operand<Lhs, LhsTag>, operand<Rhs, RhsTag>> {
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

    template<_impl::prefix_op Operator, typename Rhs, overflow_tag OverflowTag>
    struct custom_operator<Operator, operand<Rhs, OverflowTag>> {
        constexpr auto operator()(Rhs& rhs) const -> Rhs
        {
            return custom_operator<
                    typename _impl::pre_to_assign<Operator>::type,
                    operand<Rhs, OverflowTag>,
                    operand<int, OverflowTag>>{}(rhs, 1);
        }
    };

    template<_impl::postfix_op Operator, typename Rhs, overflow_tag OverflowTag>
    struct custom_operator<Operator, operand<Rhs, OverflowTag>> {
        constexpr auto operator()(Rhs& rhs) const -> Rhs
        {
            auto copy = rhs;
            custom_operator<
                    typename _impl::post_to_assign<Operator>::type,
                    operand<Rhs, OverflowTag>,
                    operand<int, OverflowTag>>{}(rhs, 1);
            return copy;
        }
    };
}

#endif  // CNL_IMPL_OVERFLOW_GENERIC_H
