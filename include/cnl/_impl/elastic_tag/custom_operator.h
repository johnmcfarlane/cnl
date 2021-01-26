
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_TAG_GENERIC_H)
#define CNL_IMPL_ELASTIC_TAG_GENERIC_H

#include "../../constant.h"
#include "../num_traits/set_digits.h"
#include "../num_traits/width.h"
#include "../operators/overloads.h"
#include "../type_traits/set_signedness.h"
#include "definition.h"
#include "is_elastic_tag.h"
#include "policy.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<
            typename Source, int SrcDigits, class SrcNarrowest,
            typename Destination, int DestDigits, class DestNarrowest>
    struct custom_operator<
            _impl::convert_op,
            operand<Source, cnl::elastic_tag<SrcDigits, SrcNarrowest>>,
            operand<Destination, cnl::elastic_tag<DestDigits, DestNarrowest>>>
        : custom_operator<
                  _impl::convert_op,
                  operand<Source>,
                  operand<Destination>> {
    };

    template<typename Source, typename Destination, int DestDigits, class DestNarrowest>
    struct custom_operator<
            _impl::convert_op,
            operand<Source>,
            operand<Destination, cnl::elastic_tag<DestDigits, DestNarrowest>>> : custom_operator<_impl::convert_op, operand<Source>, operand<Destination>> {
    };

    template<typename Source, int Digits, class Narrowest, typename Destination>
    struct custom_operator<
            _impl::convert_op,
            operand<Source, cnl::elastic_tag<Digits, Narrowest>>,
            operand<Destination>>
        : custom_operator<_impl::convert_op, operand<Source>, operand<Destination>> {
    };

    template<
            _impl::binary_arithmetic_op Operator,
            typename Lhs, int LhsDigits, class LhsNarrowest,
            typename Rhs, int RhsDigits, class RhsNarrowest>
    struct custom_operator<
            Operator,
            operand<Lhs, elastic_tag<LhsDigits, LhsNarrowest>>,
            operand<Rhs, elastic_tag<RhsDigits, RhsNarrowest>>> {
        static_assert(digits<Lhs> >= LhsDigits, "LHS number is not wide enough");
        static_assert(digits<Rhs> >= RhsDigits, "RHS number is not wide enough");

        using result_tag = decltype(Operator{}(
                std::declval<elastic_tag<LhsDigits, LhsNarrowest>>(),
                std::declval<elastic_tag<RhsDigits, RhsNarrowest>>()));
        using result_rep = typename result_tag::_rep;

        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(static_cast<result_rep>(lhs), static_cast<result_rep>(rhs));
        }
    };

    // shift_operator of scaled_integer and scaled_integer
    template<_impl::shift_op Operator, typename Lhs, int LhsDigits, typename LhsNarrowest, typename Rhs>
    requires(!_impl::is_constant<Rhs>::value) struct custom_operator<
            Operator,
            operand<Lhs, elastic_tag<LhsDigits, LhsNarrowest>>,
            operand<Rhs>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator{}(lhs, rhs);
        }
    };

    // shift_operator of scaled_integer and something else
    template<
            _impl::shift_op Operator,
            typename Lhs, int LhsDigits, typename LhsNarrowest, int RhsDigits,
            typename Rhs, typename RhsNarrowest>
    struct custom_operator<
            Operator,
            operand<Lhs, elastic_tag<LhsDigits, LhsNarrowest>>,
            operand<Rhs, elastic_tag<RhsDigits, RhsNarrowest>>>
        : custom_operator<
                  Operator,
                  operand<Lhs, elastic_tag<LhsDigits, LhsNarrowest>>,
                  operand<Rhs, _impl::native_tag>> {
    };
}

#endif  // CNL_IMPL_ELASTIC_TAG_GENERIC_H
