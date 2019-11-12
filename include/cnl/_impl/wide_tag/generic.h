
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_TAG_GENERIC_H)
#define CNL_IMPL_WIDE_TAG_GENERIC_H

#include "definition.h"
#include "is_wide_tag.h"
#include "../common.h"
//#include "../duplex_integer/operators.h"
#include "../num_traits/digits.h"
#include "../num_traits/set_width.h"
#include "../num_traits/to_rep.h"
#include "../num_traits/width.h"
//#include "../number/is_number.h"
#include "../operators/generic.h"
//#include "../ostream.h"
//#include "../type_traits/enable_if.h"
#include "../type_traits/is_signed.h"
#include "../type_traits/set_signedness.h"
//#include "../wide_integer/definition.h"
//#include "../wide_integer/from_rep.h"
//#include "../wide_integer/from_value.h"
//#include "../wide_integer/make_wide_integer.h"
//#include "../wide_integer/numeric_limits.h"
//#include "../wide_integer/type.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
//    namespace _impl {
//
//        template<int LhsDigits, class LhsNarrowest, class Rhs>
//        CNL_NODISCARD constexpr auto operator<<(wide_integer<LhsDigits, LhsNarrowest> const& lhs, Rhs const& rhs)
//        -> wide_integer<LhsDigits, LhsNarrowest>
//        {
//            return wide_integer<LhsDigits, LhsNarrowest>(
//                    to_rep(lhs) << set_signedness_t<int, is_signed<Rhs>::value>(rhs));
//        }
//
//        template<int LhsDigits, class LhsNarrowest, class Rhs>
//        CNL_NODISCARD constexpr auto operator>>(wide_integer<LhsDigits, LhsNarrowest> const& lhs, Rhs const& rhs)
//        -> wide_integer<LhsDigits, LhsNarrowest>
//        {
//            return wide_integer<LhsDigits, LhsNarrowest>(
//                    to_rep(lhs) >> set_signedness_t<int, is_signed<Rhs>::value>(rhs));
//        }
//    }

    template<int ArchetypeDigits, typename ArchetypeNarrowest, typename Initializer>
    struct deduction<
            wide_tag<ArchetypeDigits, ArchetypeNarrowest>,
            Initializer> {
        // tag associated with deduced type
        using tag = wide_tag<
                digits<Initializer>::value,
                _impl::set_width_t<Initializer, _impl::width<ArchetypeNarrowest>::value>>;

        // deduced type
        using type = Initializer;
    };

    template<
            int DestDigits, class DestNarrowest,
            class SrcTag,
            typename Dest, typename Src>
    struct convert_operator<
            wide_tag<DestDigits, DestNarrowest>,
            SrcTag,
            Dest, Src,
            _impl::enable_if_t<!_impl::is_wide_tag<SrcTag>::value>> {
        CNL_NODISCARD constexpr Dest operator()(Src const& from) const
        {
            return convert_operator<_impl::native_tag, SrcTag, Dest, Src>{}(from);
        }
    };

    template<
            class DestTag,
            int SrcDigits, class SrcNarrowest,
            typename Dest, typename Src>
    struct convert_operator<
            DestTag,
            wide_tag<SrcDigits, SrcNarrowest>,
            Dest, Src> {
        CNL_NODISCARD constexpr Dest operator()(Src const& from) const
        {
            return convert_operator<_impl::native_tag, _impl::native_tag, Dest, Src>{}(from);
        }
    };

    template<
            typename Operator,
            int Digits, typename Narrowest,
            class Rhs>
    struct unary_operator<Operator, wide_tag<Digits, Narrowest>, Rhs>
            : unary_operator<Operator, _impl::native_tag, Rhs> {
    };

    template<
            class Operator,
            int LhsDigits, class LhsNarrowest,
            int RhsDigits, class RhsNarrowest,
            class Lhs, class Rhs>
    struct binary_operator<
            Operator,
            wide_tag<LhsDigits, LhsNarrowest>,
            wide_tag<RhsDigits, RhsNarrowest>,
            Lhs, Rhs> {
        static constexpr auto _max_digits{_impl::max(LhsDigits, RhsDigits)};
        static constexpr auto _are_signed{is_signed<LhsNarrowest>::value
                || is_signed<RhsNarrowest>::value};
        using _common_type = typename std::common_type<LhsNarrowest, RhsNarrowest>::type;
        using _narrowest = _impl::set_signedness_t<_common_type, _are_signed>;

        using _result_tag = wide_tag<_max_digits, _narrowest>;
        using _result = typename _result_tag::_rep;

        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> _result
        {
            return Operator{}(_impl::to_rep(lhs), _impl::to_rep(rhs));
        }
    };

    template<
            class Operator,
            int LhsDigits, typename LhsNarrowest,
            typename Lhs, typename Rhs>
    struct shift_operator<
            Operator,
            wide_tag<LhsDigits, LhsNarrowest>, _impl::native_tag,
            Lhs, Rhs> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(Operator{}(lhs, rhs))
        {
            return Operator{}(lhs, rhs);
        }
    };

//    template<class Operator, class LhsTag, typename Lhs, typename Rhs>
//    struct shift_operator<
//            Operator,
//            LhsTag, _impl::native_tag,
//            Lhs, Rhs,
//            _impl::enable_if_t<_impl::is_wide_tag<LhsTag>::value&&!_impl::is_constant<Rhs>::value>> {
//        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
//        -> decltype(Operator{}(lhs, rhs))
//        {
//            return Operator{}(lhs, rhs);
//        }
//    };
//
//    template<
//            class Operator,
//            int LhsDigits, typename LhsNarrowest,
//            int RhsDigits, typename RhsNarrowest,
//            typename Lhs, typename Rhs>
//    struct shift_operator<
//            Operator,
//            wide_tag<LhsDigits, LhsNarrowest>, wide_tag<RhsDigits, RhsNarrowest>,
//            Lhs, Rhs>
//        : shift_operator<Operator, wide_tag<LhsDigits, LhsNarrowest>, _impl::native_tag, Lhs, Rhs> {
//    };

    template<
            class Operator,
            int LhsDigits, class LhsNarrowest,
            int RhsDigits, class RhsNarrowest>
    struct comparison_operator<
            Operator,
            wide_tag<LhsDigits, LhsNarrowest>,
            wide_tag<RhsDigits, RhsNarrowest>>
            : comparison_operator<Operator, cnl::_impl::native_tag, cnl::_impl::native_tag> {
    };

    template<class Operator, int Digits, typename Narrowest, typename Rhs>
    struct pre_operator<Operator, wide_tag<Digits, Narrowest>, Rhs> : Operator {
    };

    template<class Operator, int Digits, typename Narrowest, typename Lhs>
    struct post_operator<Operator, wide_tag<Digits, Narrowest>, Lhs> : Operator {
    };

//    namespace _impl {
//        ////////////////////////////////////////////////////////////////////////////////
//        // cnl::wide_integer streaming
//
//        template<int Digits, typename Narrowest>
//        ::std::ostream& operator<<(::std::ostream& out, wide_integer<Digits, Narrowest> const& value)
//        {
//            return out << to_rep(value);
//        }
//    }
}

#endif  // CNL_IMPL_WIDE_TAG_GENERIC_H
