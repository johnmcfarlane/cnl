
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_OPERATORS_H)
#define CNL_IMPL_WIDE_INTEGER_OPERATORS_H

#include "common_type.h"
#include "from_rep.h"
#include "from_value.h"
#include "make_wide_integer.h"
#include "numeric_limits.h"
#include "type.h"
#include "../operators.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Operator, int Digits, typename Narrowest>
        struct unary_operator<Operator, wide_integer<Digits, Narrowest>> {
            constexpr auto operator()(wide_integer<Digits, Narrowest> const& rhs) const
            -> decltype(from_rep<wide_integer<Digits, decltype(Operator()(to_rep(rhs)))>>(Operator()(to_rep(rhs))))
            {
                return from_rep<wide_integer<Digits, decltype(Operator()(to_rep(rhs)))>>(Operator()(to_rep(rhs)));
            }
        };

        template<class Operator, int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
        struct binary_operator<
                Operator,
                wide_integer<LhsDigits, LhsNarrowest>, wide_integer<RhsDigits, RhsNarrowest>,
                enable_if_t<!is_comparison_operator<Operator>::value>> {
            using _lhs = wide_integer<LhsDigits, LhsNarrowest>;
            using _rhs = wide_integer<RhsDigits, RhsNarrowest>;
            using _result = typename std::common_type<_lhs, _rhs>::type;
            constexpr auto operator()(_lhs const& lhs, _rhs const& rhs) const -> _result
            {
                return Operator{}(to_rep(lhs), to_rep(rhs));
            }
        };

        template<class Operator, int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
        struct binary_operator<
                Operator,
                wide_integer<LhsDigits, LhsNarrowest>, wide_integer<RhsDigits, RhsNarrowest>,
                enable_if_t<is_comparison_operator<Operator>::value>> {
            constexpr auto operator()(
                    wide_integer<LhsDigits, LhsNarrowest> const& lhs,
                    wide_integer<RhsDigits, RhsNarrowest> const& rhs) const
            -> decltype(Operator()(to_rep(lhs), to_rep(rhs)))
            {
                return Operator()(to_rep(lhs), to_rep(rhs));
            }
        };

        template<class Operator, int Digits, typename Narrowest>
        struct pre_operator<Operator, wide_integer<Digits, Narrowest>> {
            CNL_RELAXED_CONSTEXPR auto operator()(wide_integer<Digits, Narrowest>& rhs) const
            -> wide_integer<Digits, Narrowest>&
            {
                Operator()(_impl::to_rep(rhs));
                return rhs;
            }
        };

        template<class Operator, int Digits, typename Narrowest>
        struct post_operator<Operator, wide_integer<Digits, Narrowest>> {
            CNL_RELAXED_CONSTEXPR auto operator()(wide_integer<Digits, Narrowest>& lhs) const
            -> wide_integer<Digits, Narrowest>
            {
                auto copy = lhs;
                Operator()(_impl::to_rep(lhs));
                return copy;
            }
        };
    }
}

#endif  // CNL_IMPL_WIDE_INTEGER_OPERATORS_H
