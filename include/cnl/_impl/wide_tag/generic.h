
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_TAG_GENERIC_H)
#define CNL_IMPL_WIDE_TAG_GENERIC_H

#include "../common.h"
#include "../num_traits/digits.h"
#include "../num_traits/set_width.h"
#include "../num_traits/to_rep.h"
#include "../num_traits/width.h"
#include "../operators/generic.h"
#include "../operators/native_tag.h"
#include "../type_traits/is_signed.h"
#include "../type_traits/set_signedness.h"
#include "definition.h"
#include "is_wide_tag.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<int ArchetypeDigits, typename ArchetypeNarrowest, typename Initializer>
    struct deduction<wide_tag<ArchetypeDigits, ArchetypeNarrowest>, Initializer> {
        // tag associated with deduced type
        using tag = wide_tag<
                digits<Initializer>,
                _impl::set_width_t<Initializer, _impl::width<ArchetypeNarrowest>::value>>;

        // deduced type
        using type = Initializer;
    };

    template<_impl::wide_tag DestTag, tag SrcTag, typename Dest, typename Src>
    requires(!_impl::is_wide_tag<SrcTag>) struct convert_operator<DestTag, SrcTag, Dest, Src> {
        CNL_NODISCARD constexpr Dest operator()(Src const& from) const
        {
            return convert_operator<_impl::native_tag, SrcTag, Dest, Src>{}(from);
        }
    };

    template<tag DestTag, _impl::wide_tag SrcTag, typename Dest, typename Src>
    struct convert_operator<DestTag, SrcTag, Dest, Src> {
        CNL_NODISCARD constexpr Dest operator()(Src const& from) const
        {
            return convert_operator<_impl::native_tag, _impl::native_tag, Dest, Src>{}(from);
        }
    };

    template<_impl::unary_arithmetic_op Operator, int Digits, typename Narrowest, class Rhs>
    struct custom_operator<Operator, operand<Rhs, wide_tag<Digits, Narrowest>>>
        : custom_operator<Operator, operand<Rhs, _impl::native_tag>> {
    };

    template<
            _impl::binary_arithmetic_op Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest,
            class Lhs, class Rhs>
    struct custom_operator<
            Operator,
            operand<Lhs, wide_tag<LhsDigits, LhsNarrowest>>,
            operand<Rhs, wide_tag<RhsDigits, RhsNarrowest>>> {
    private:
        static constexpr auto _max_digits{_impl::max(LhsDigits, RhsDigits)};
        static constexpr auto _are_signed{
                is_signed<LhsNarrowest>::value || is_signed<RhsNarrowest>::value};
        using common_type = typename std::common_type<LhsNarrowest, RhsNarrowest>::type;
        using narrowest = _impl::set_signedness_t<common_type, _are_signed>;

        using result_tag = wide_tag<_max_digits, narrowest>;
        using result = typename result_tag::rep;

    public:
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> result
        {
            return static_cast<result>(Operator{}(lhs, rhs));
        }
    };

    template<_impl::shift_op Operator, typename Lhs, int LhsDigits, typename LhsNarrowest, typename Rhs>
    struct custom_operator<
            Operator,
            operand<Lhs, wide_tag<LhsDigits, LhsNarrowest>>,
            operand<Rhs>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator{}(lhs, rhs);
        }
    };

    template<_impl::comparison_op Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
    struct custom_operator<
            Operator, wide_tag<LhsDigits, LhsNarrowest>, wide_tag<RhsDigits, RhsNarrowest>>
        : custom_operator<
                  Operator,
                  operand<_impl::native_tag>,
                  operand<_impl::native_tag>> {
    };

    template<_impl::prefix_op Operator, int Digits, typename Narrowest, typename Rhs>
    struct prefix_operator<Operator, wide_tag<Digits, Narrowest>, Rhs> : Operator {
    };

    template<_impl::postfix_op Operator, int Digits, typename Narrowest, typename Lhs>
    struct postfix_operator<Operator, wide_tag<Digits, Narrowest>, Lhs> : Operator {
    };
}

#endif  // CNL_IMPL_WIDE_TAG_GENERIC_H
