
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_OPERATORS_H)
#define CNL_IMPL_WIDE_INTEGER_OPERATORS_H

#include "forward_declaration.h"
#include "from_rep.h"
#include "from_value.h"
#include "make_wide_integer.h"
#include "numeric_limits.h"
#include "type.h"
#include "../common.h"
#include "../duplex_integer/operators.h"
#include "../generic_operators.h"
#include "../ostream.h"
#include "../type_traits/is_signed.h"
#include "../type_traits/set_signedness.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {

        template<int LhsDigits, class LhsNarrowest, class Rhs>
        CNL_NODISCARD constexpr auto operator<<(wide_integer<LhsDigits, LhsNarrowest> const& lhs, Rhs const& rhs)
        -> wide_integer<LhsDigits, LhsNarrowest>
        {
            return wide_integer<LhsDigits, LhsNarrowest>(
                    to_rep(lhs) << set_signedness_t<int, is_signed<Rhs>::value>(rhs));
        }

        template<int LhsDigits, class LhsNarrowest, class Rhs>
        CNL_NODISCARD constexpr auto operator>>(wide_integer<LhsDigits, LhsNarrowest> const& lhs, Rhs const& rhs)
        -> wide_integer<LhsDigits, LhsNarrowest>
        {
            return wide_integer<LhsDigits, LhsNarrowest>(
                    to_rep(lhs) >> set_signedness_t<int, is_signed<Rhs>::value>(rhs));
        }

        template<typename Operator, int Digits, typename Narrowest>
        struct unary_operator<Operator, wide_integer<Digits, Narrowest>> {
            CNL_NODISCARD constexpr auto operator()(wide_integer<Digits, Narrowest> const& rhs) const -> wide_integer<Digits, Narrowest>
            {
                return Operator()(to_rep(rhs));
            }
        };

        template<class Operator, int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
        struct binary_operator<
                Operator,
                wide_integer<LhsDigits, LhsNarrowest>, wide_integer<RhsDigits, RhsNarrowest>> {
            using _lhs = wide_integer<LhsDigits, LhsNarrowest>;
            using _rhs = wide_integer<RhsDigits, RhsNarrowest>;

            static constexpr auto _max_digits = cnl::_impl::max(LhsDigits, RhsDigits);
            static constexpr auto _are_signed = cnl::is_signed<LhsNarrowest>::value
                    || cnl::is_signed<RhsNarrowest>::value;
            using _common_type = typename std::common_type<LhsNarrowest, RhsNarrowest>::type;
            using _narrowest = cnl::_impl::set_signedness_t<_common_type, _are_signed>;

            using _result = cnl::_impl::wide_integer<cnl::_impl::max(LhsDigits, RhsDigits), _narrowest>;

            CNL_NODISCARD constexpr auto operator()(_lhs const& lhs, _rhs const& rhs) const -> _result
            {
                return Operator{}(to_rep(lhs), to_rep(rhs));
            }
        };

        template<class Operator, int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
        struct comparison_operator<
                Operator,
                wide_integer<LhsDigits, LhsNarrowest>, wide_integer<RhsDigits, RhsNarrowest>> {
            CNL_NODISCARD constexpr auto operator()(
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

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::wide_integer streaming

        template<int Digits, typename Narrowest>
        ::std::ostream& operator<<(::std::ostream& out, wide_integer<Digits, Narrowest> const& value)
        {
            return out << to_rep(value);
        }
    }
}

#endif  // CNL_IMPL_WIDE_INTEGER_OPERATORS_H
