
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_OPERATORS_H)
#define CNL_IMPL_WIDE_INTEGER_OPERATORS_H

#include "../common.h"
#include "../duplex_integer/operators.h"
#include "../num_traits/to_rep.h"
#include "../operators/generic.h"
#include "../ostream.h"
#include "../type_traits/is_signed.h"
#include "../type_traits/set_signedness.h"
#include "forward_declaration.h"
#include "from_rep.h"
#include "from_value.h"
#include "make_wide_integer.h"
#include "numeric_limits.h"
#include "type.h"

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
    }

    template<typename Operator, int Digits, typename Narrowest>
    struct unary_operator<Operator, _impl::native_tag, _impl::wide_integer<Digits, Narrowest>> {
        CNL_NODISCARD constexpr auto operator()(
                _impl::wide_integer<Digits, Narrowest> const& rhs) const -> _impl::wide_integer<Digits, Narrowest>
        {
            return Operator()(_impl::to_rep(rhs));
        }
    };

    template<class Operator, int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
    struct binary_operator<
            Operator,
            _impl::native_tag, _impl::native_tag,
            _impl::wide_integer<LhsDigits, LhsNarrowest>, _impl::wide_integer<RhsDigits, RhsNarrowest>> {
        using _lhs = _impl::wide_integer<LhsDigits, LhsNarrowest>;
        using _rhs = _impl::wide_integer<RhsDigits, RhsNarrowest>;

        static constexpr auto _max_digits = _impl::max(LhsDigits, RhsDigits);
        static constexpr auto _are_signed = is_signed<LhsNarrowest>::value
                || is_signed<RhsNarrowest>::value;
        using _common_type = typename std::common_type<LhsNarrowest, RhsNarrowest>::type;
        using _narrowest = _impl::set_signedness_t<_common_type, _are_signed>;

        using _result = _impl::wide_integer<_impl::max(LhsDigits, RhsDigits), _narrowest>;

        CNL_NODISCARD constexpr auto operator()(_lhs const& lhs, _rhs const& rhs) const -> _result
        {
            return Operator{}(_impl::to_rep(lhs), _impl::to_rep(rhs));
        }
    };

    template<class Operator, int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
    struct comparison_operator<
            Operator,
            _impl::wide_integer<LhsDigits, LhsNarrowest>, _impl::wide_integer<RhsDigits, RhsNarrowest>> {
        CNL_NODISCARD constexpr auto operator()(
                _impl::wide_integer<LhsDigits, LhsNarrowest> const& lhs,
                _impl::wide_integer<RhsDigits, RhsNarrowest> const& rhs) const
        -> decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)))
        {
            return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
        }
    };

    template<class Operator, int Digits, typename Narrowest>
    struct pre_operator<Operator, _impl::native_tag, _impl::wide_integer<Digits, Narrowest>> {
        CNL_RELAXED_CONSTEXPR auto operator()(_impl::wide_integer<Digits, Narrowest>& rhs) const
        -> _impl::wide_integer<Digits, Narrowest>&
        {
            Operator()(_impl::to_rep(rhs));
            return rhs;
        }
    };

    template<class Operator, int Digits, typename Narrowest>
    struct post_operator<Operator, _impl::native_tag, _impl::wide_integer<Digits, Narrowest>> {
        CNL_RELAXED_CONSTEXPR auto operator()(_impl::wide_integer<Digits, Narrowest>& lhs) const
        -> _impl::wide_integer<Digits, Narrowest>
        {
            auto copy = lhs;
            Operator()(_impl::to_rep(lhs));
            return copy;
        }
    };

    namespace _impl {
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
