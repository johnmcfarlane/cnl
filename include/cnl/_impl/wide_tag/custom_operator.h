
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_TAG_GENERIC_H)
#define CNL_IMPL_WIDE_TAG_GENERIC_H

#include "../custom_operator/definition.h"
#include "../custom_operator/native_tag.h"
#include "../num_traits/digits.h"
#include "../num_traits/set_width.h"
#include "../num_traits/to_rep.h"
#include "../num_traits/width.h"
#include "../numbers/set_signedness.h"
#include "../numbers/signedness.h"
#include "definition.h"
#include "is_wide_tag.h"

#include <algorithm>
#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<int ArchetypeDigits, typename ArchetypeNarrowest, typename Initializer>
    struct deduction<wide_tag<ArchetypeDigits, ArchetypeNarrowest>, Initializer> {
        // tag associated with deduced type
        using tag = wide_tag<
                digits_v<Initializer>,
                _impl::set_width_t<Initializer, _impl::width<ArchetypeNarrowest>>>;

        // deduced type
        using type = Initializer;
    };

    /// \cond
    template<typename Src, tag SrcTag, typename Dest, _impl::any_wide_tag DestTag>
    requires(!_impl::is_wide_tag<SrcTag>) struct custom_operator<
            _impl::convert_op,
            op_value<Src, SrcTag>, op_value<Dest, DestTag>> {
        [[nodiscard]] constexpr auto operator()(Src const& from) const
        {
            return custom_operator<_impl::convert_op, op_value<Src, SrcTag>, op_value<Dest>>{}(from);
        }
    };
    /// \endcond

    template<typename Src, _impl::any_wide_tag SrcTag, typename Dest, tag DestTag>
    struct custom_operator<_impl::convert_op, op_value<Src, SrcTag>, op_value<Dest, DestTag>> {
        [[nodiscard]] constexpr auto operator()(Src const& from) const
        {
            return custom_operator<_impl::convert_op, op_value<Src>, op_value<Dest>>{}(from);
        }
    };

    template<_impl::unary_arithmetic_op Operator, int Digits, typename Narrowest, class Rhs>
    struct custom_operator<Operator, op_value<Rhs, wide_tag<Digits, Narrowest>>>
        : Operator {
    };

    template<
            _impl::binary_arithmetic_op Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest,
            class Lhs, class Rhs>
    struct custom_operator<
            Operator,
            op_value<Lhs, wide_tag<LhsDigits, LhsNarrowest>>,
            op_value<Rhs, wide_tag<RhsDigits, RhsNarrowest>>> {
    private:
        static constexpr auto _max_digits{std::max(LhsDigits, RhsDigits)};
        static constexpr auto _are_signed{
                numbers::signedness_v<LhsNarrowest> || numbers::signedness_v<RhsNarrowest>};
        using common_type = typename std::common_type<LhsNarrowest, RhsNarrowest>::type;
        using narrowest = numbers::set_signedness_t<common_type, _are_signed>;

        using result_tag = wide_tag<_max_digits, narrowest>;
        using result = typename result_tag::rep;

    public:
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> result
        {
            return static_cast<result>(Operator{}(lhs, rhs));
        }
    };

    template<_impl::shift_op Operator, typename Lhs, int LhsDigits, typename LhsNarrowest, typename Rhs>
    struct custom_operator<
            Operator,
            op_value<Lhs, wide_tag<LhsDigits, LhsNarrowest>>,
            op_value<Rhs>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator{}(lhs, rhs);
        }
    };

    template<_impl::comparison_op Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
    struct custom_operator<
            Operator, wide_tag<LhsDigits, LhsNarrowest>, wide_tag<RhsDigits, RhsNarrowest>>
        : custom_operator<
                  Operator,
                  op_value<_impl::native_tag>,
                  op_value<_impl::native_tag>> {
    };

    template<_impl::prefix_op Operator, typename Rhs, int Digits, typename Narrowest>
    struct custom_operator<Operator, op_value<Rhs, wide_tag<Digits, Narrowest>>> : Operator {
    };

    template<_impl::postfix_op Operator, typename Lhs, int Digits, typename Narrowest>
    struct custom_operator<Operator, op_value<Lhs, wide_tag<Digits, Narrowest>>> : Operator {
    };
}

#endif  // CNL_IMPL_WIDE_TAG_GENERIC_H
