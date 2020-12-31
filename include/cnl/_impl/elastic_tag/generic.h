
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_TAG_GENERIC_H)
#define CNL_IMPL_ELASTIC_TAG_GENERIC_H

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
            int DestDigits, class DestNarrowest, int SrcDigits, class SrcNarrowest,
            typename Destination, typename Source>
    struct convert_operator<
            cnl::elastic_tag<DestDigits, DestNarrowest>, cnl::elastic_tag<SrcDigits, SrcNarrowest>,
            Destination, Source>
        : convert_operator<_impl::native_tag, _impl::native_tag, Destination, Source> {
    };

    template<int DestDigits, class DestNarrowest, typename Destination, typename Source>
    struct convert_operator<
            cnl::elastic_tag<DestDigits, DestNarrowest>, cnl::_impl::native_tag, Destination,
            Source> : convert_operator<_impl::native_tag, _impl::native_tag, Destination, Source> {
    };

    template<int Digits, class Narrowest, typename Destination, typename Source>
    struct convert_operator<
            cnl::_impl::native_tag, cnl::elastic_tag<Digits, Narrowest>, Destination, Source>
        : convert_operator<_impl::native_tag, _impl::native_tag, Destination, Source> {
    };

    template<
            class Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest,
            typename Lhs, typename Rhs>
    struct binary_operator<
            Operator, elastic_tag<LhsDigits, LhsNarrowest>, elastic_tag<RhsDigits, RhsNarrowest>,
            Lhs, Rhs> {
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
    template<class Operator, int LhsDigits, typename LhsNarrowest, typename Lhs, typename Rhs>
    struct shift_operator<
            Operator, elastic_tag<LhsDigits, LhsNarrowest>, _impl::native_tag, Lhs, Rhs,
            _impl::enable_if_t<!_impl::is_constant<Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator{}(lhs, rhs);
        }
    };

    // shift_operator of scaled_integer and something else
    template<
            class Operator, int LhsDigits, typename LhsNarrowest, int RhsDigits,
            typename RhsNarrowest, typename Lhs, typename Rhs>
    struct shift_operator<
            Operator, elastic_tag<LhsDigits, LhsNarrowest>, elastic_tag<RhsDigits, RhsNarrowest>,
            Lhs, Rhs>
        : shift_operator<
                  Operator, elastic_tag<LhsDigits, LhsNarrowest>, _impl::native_tag, Lhs, Rhs> {
    };

    template<int Digits, class Narrowest, class Operator, typename Rhs>
    struct pre_operator<elastic_tag<Digits, Narrowest>, Operator, Rhs> : Operator {
    };

    template<int Digits, class Narrowest, class Operator, typename Rhs>
    struct post_operator<elastic_tag<Digits, Narrowest>, Operator, Rhs> : Operator {
    };
}

#endif  // CNL_IMPL_ELASTIC_TAG_GENERIC_H
