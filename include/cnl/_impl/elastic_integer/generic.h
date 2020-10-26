
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_GENERIC_H)
#define CNL_IMPL_ELASTIC_INTEGER_GENERIC_H

#include "../operators/generic.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::common_signedness

        template<class T1, class T2>
        struct common_signedness {
            static constexpr bool _are_signed = numeric_limits<T1>::is_signed | numeric_limits<T2>::is_signed;

            using type = typename std::common_type<set_signedness_t<T1, _are_signed>,
            set_signedness_t<T2, _are_signed>>::type;
        };

        template<class T1, class T2>
        using common_signedness_t = typename common_signedness<T1, T2>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::common_elastic_type

        template<typename T1, typename T2>
        struct common_elastic_type;

        template<int Digits1, class Narrowest1, int Digits2, class Narrowest2>
        struct common_elastic_type<
                elastic_integer<Digits1, Narrowest1>,
                elastic_integer<Digits2, Narrowest2>> {
            using type = elastic_integer<
                    max(Digits1, Digits2),
                    common_signedness_t<Narrowest1, Narrowest2>>;
        };

        template<int Digits1, class Narrowest1, class Rhs>
        struct common_elastic_type<elastic_integer<Digits1, Narrowest1>, Rhs>
                : common_elastic_type<
                        elastic_integer<Digits1, Narrowest1>,
                        elastic_integer<numeric_limits<Rhs>::digits, Rhs>> {
        };

        template<class Lhs, int Digits2, class Narrowest2>
        struct common_elastic_type<Lhs, elastic_integer<Digits2, Narrowest2>>
                : common_elastic_type<
                        elastic_integer<numeric_limits<Lhs>::digits, Lhs>,
                        elastic_integer<Digits2, Narrowest2>> {
        };

        template<int FromDigits, class FromNarrowest, int OtherDigits, class OtherNarrowest,
                enable_if_t<FromDigits!=OtherDigits
                        || !std::is_same<FromNarrowest, OtherNarrowest>::value, std::nullptr_t> = nullptr>
        CNL_NODISCARD constexpr auto cast_to_common_type(
                elastic_integer<FromDigits, FromNarrowest> const& from,
                elastic_integer<OtherDigits, OtherNarrowest> const&) {
            return static_cast<typename common_elastic_type<
                    elastic_integer<FromDigits, FromNarrowest>,
                    elastic_integer<OtherDigits, OtherNarrowest>>::type>(from);
        }
    }

    template<class Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
    struct comparison_operator<Operator,
            elastic_integer<LhsDigits, LhsNarrowest>, elastic_integer<RhsDigits, RhsNarrowest>> {
        CNL_NODISCARD constexpr auto operator()(
                elastic_integer<LhsDigits, LhsNarrowest> const& lhs,
                elastic_integer<RhsDigits, RhsNarrowest> const& rhs) const
        {
            return Operator()(cast_to_common_type(lhs, rhs), cast_to_common_type(rhs, lhs));
        }
    };

    template<class Operator, int Digits, class Narrowest>
    struct comparison_operator<Operator, elastic_integer<Digits, Narrowest>, elastic_integer<Digits, Narrowest>> {
        CNL_NODISCARD constexpr auto operator()(
                elastic_integer<Digits, Narrowest> const& lhs,
                elastic_integer<Digits, Narrowest> const& rhs) const
        {
            return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
        }
    };

    // elastic_integer << non-constant
    // elastic_integer >> non-constant
    template<class Operator, int LhsDigits, class LhsRep, typename Rhs>
    struct shift_operator<
            Operator,
            _impl::native_tag, _impl::native_tag,
            elastic_integer<LhsDigits, LhsRep>, Rhs,
            _impl::enable_if_t<!_impl::is_constant<Rhs>::value>> {
        using lhs_type = elastic_integer<LhsDigits, LhsRep>;

        CNL_NODISCARD constexpr auto operator()(lhs_type const& lhs, Rhs const& rhs) const
        {
            return _impl::from_rep<lhs_type>(Operator{}(_impl::to_rep(lhs), rhs));
        }
    };

    template<int LhsDigits, class LhsNarrowest, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    struct shift_operator<
            _impl::shift_left_op,
            _impl::native_tag, _impl::native_tag,
            elastic_integer<LhsDigits, LhsNarrowest>, constant<RhsValue>> {
        CNL_NODISCARD constexpr auto operator()(
                elastic_integer<LhsDigits, LhsNarrowest> const& lhs,
                constant<RhsValue>) const
        {
            return _impl::from_rep<elastic_integer<LhsDigits+int{RhsValue}, LhsNarrowest>>(
                    _impl::to_rep(static_cast<elastic_integer<LhsDigits+int{RhsValue}, LhsNarrowest>>(
                            lhs)) << RhsValue);
        }
    };

    template<
            int LhsDigits, class LhsNarrowest,
            CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    struct shift_operator<
            _impl::shift_right_op,
            _impl::native_tag, _impl::native_tag,
            elastic_integer<LhsDigits, LhsNarrowest>, constant<RhsValue>> {
        CNL_NODISCARD constexpr auto operator()(
                elastic_integer<LhsDigits, LhsNarrowest> const& lhs,
                constant<RhsValue>) const
        {
            return _impl::from_rep<elastic_integer<LhsDigits-int{RhsValue}, LhsNarrowest>>(
                    _impl::to_rep(static_cast<elastic_integer<LhsDigits, LhsNarrowest>>(
                            lhs)) >> RhsValue);
        }
    };

    // unary operator-
    template<int RhsDigits, class RhsNarrowest>
    struct unary_operator<_impl::minus_op, _impl::native_tag, elastic_integer<RhsDigits, RhsNarrowest>> {
        CNL_NODISCARD constexpr auto operator()(elastic_integer<RhsDigits, RhsNarrowest> const& rhs) const
        -> elastic_integer<RhsDigits, typename add_signedness<RhsNarrowest>::type>
        {
            using result_type = elastic_integer<RhsDigits, typename add_signedness<RhsNarrowest>::type>;
            return _impl::from_rep<result_type>(-_impl::to_rep(static_cast<result_type>(rhs)));
        }
    };

    // unary operator+
    template<int RhsDigits, class RhsNarrowest>
    struct unary_operator<_impl::plus_op, _impl::native_tag, elastic_integer<RhsDigits, RhsNarrowest>> {
        CNL_NODISCARD constexpr auto operator()(elastic_integer<RhsDigits, RhsNarrowest> const& rhs) const
        -> elastic_integer<RhsDigits, RhsNarrowest>
        {
            return _impl::from_rep<elastic_integer<RhsDigits, RhsNarrowest>>(
                    +_impl::to_rep(static_cast<elastic_integer<RhsDigits, RhsNarrowest>>(rhs)));
        }
    };

    // unary operator~
    template<int RhsDigits, class RhsNarrowest>
    struct unary_operator<_impl::bitwise_not_op, _impl::native_tag, elastic_integer<RhsDigits, RhsNarrowest>> {
        CNL_NODISCARD constexpr auto operator()(elastic_integer<RhsDigits, RhsNarrowest> const& rhs) const
        -> elastic_integer<RhsDigits, RhsNarrowest>
        {
            using elastic_integer = elastic_integer<RhsDigits, RhsNarrowest>;
            using rep = _impl::rep_t<elastic_integer>;
            return _impl::from_rep<elastic_integer>(
                    static_cast<rep>(
                            _impl::to_rep(rhs)
                                    ^ ((static_cast<rep>(~0)) >> (numeric_limits<rep>::digits - RhsDigits))));
        }
    };
}

#endif  // CNL_IMPL_ELASTIC_INTEGER_GENERIC_H
