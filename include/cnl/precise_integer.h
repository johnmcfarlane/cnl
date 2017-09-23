
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_PRECISE_INTEGER_H)
#define CNL_PRECISE_INTEGER_H 1

#include <cnl/bits/number_base.h>
#include <cnl/limits.h>

namespace cnl {

    struct closest_rounding_tag {
        template<class To, class From>
        static constexpr To convert(From const& from)
        {
            return static_cast<To>(std::intmax_t(from+((from>=0) ? .5 : -.5)));
        }
    };

    template<class Rep = int, class RoundingTag = closest_rounding_tag>
    class precise_integer : public _impl::number_base<precise_integer<Rep, RoundingTag>, Rep> {
        using super = _impl::number_base<precise_integer<Rep, RoundingTag>, Rep>;
    public:
        using rounding = RoundingTag;

        constexpr precise_integer() = default;

        template<class T, _impl::enable_if_t<numeric_limits<T>::is_integer, int> Dummy = 0>
        constexpr precise_integer(T const& v)
                : super(static_cast<Rep>(v)) { }

        template<class T, _impl::enable_if_t<!numeric_limits<T>::is_integer, int> Dummy = 0>
        constexpr precise_integer(T const& v)
                : super(rounding::template convert<Rep>(v)) { }

        template<class T>
        constexpr explicit operator T() const
        {
            return static_cast<T>(super::data());
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // numeric type traits

    template<class Rep, class RoundingTag>
    struct digits<precise_integer<Rep, RoundingTag>> : digits<Rep> {
    };

    template<class Rep, class RoundingTag, _digits_type MinNumBits>
    struct set_digits<precise_integer<Rep, RoundingTag>, MinNumBits> {
        using type = precise_integer<set_digits_t<Rep, MinNumBits>, RoundingTag>;
    };

    namespace _impl {
        template<class Rep, class RoundingTag>
        struct get_rep<precise_integer<Rep, RoundingTag>> {
            using type = Rep;
        };

        template<class OldRep, class RoundingTag, class NewRep>
        struct set_rep<precise_integer<OldRep, RoundingTag>, NewRep> {
            using type = precise_integer<NewRep, RoundingTag>;
        };
    }

    template<class Rep, class RoundingTag, class Value>
    struct from_value<precise_integer<Rep, RoundingTag>, Value> {
        using type = precise_integer<Value, RoundingTag>;
    };

    template<class Rep, class RoundingTag>
    struct scale<precise_integer<Rep, RoundingTag>>
    : scale<_impl::number_base<precise_integer<Rep, RoundingTag>, Rep>> {
    };

    namespace _precise_integer_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // comparison operators

        template<class T>
        struct is_precise_integer : std::false_type {
        };

        template<class Rep, class RoundingTag>
        struct is_precise_integer<precise_integer<Rep, RoundingTag>> : std::true_type {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // binary arithmetic

    namespace _impl {
        // for operands with a common tag
        template<class Operator, class RoundingTag, class LhsRep, class RhsRep, class = enable_if_t<Operator::is_arithmetic>>
        constexpr auto operate_common_tag(
                precise_integer<LhsRep, RoundingTag> const& lhs,
                precise_integer<RhsRep, RoundingTag> const& rhs)
        -> decltype(from_rep<precise_integer<op_result<Operator, LhsRep, RhsRep>, RoundingTag>>(Operator()(lhs.data(), rhs.data())))
        {
            using result_type = precise_integer<op_result<Operator, LhsRep, RhsRep>, RoundingTag>;
            return from_rep<result_type>(Operator()(lhs.data(), rhs.data()));
        }

        // for arithmetic operands with different policies
        template<class Operator, class RoundingTag, class LhsRep, class RhsRep, enable_if_t<Operator::is_comparison, int> = 0>
        constexpr auto operate_common_tag(
                precise_integer<LhsRep, RoundingTag> const& lhs,
                precise_integer<RhsRep, RoundingTag> const& rhs)
        -> decltype(Operator()(lhs.data(), rhs.data()))
        {
            return Operator()(lhs.data(), rhs.data());
        }

        // for arithmetic operands with different policies
        template<class Operator, class LhsRep, class LhsRoundingTag, class RhsRep, class RhsRoundingTag>
        constexpr auto operate(
                precise_integer<LhsRep, LhsRoundingTag> const& lhs,
                precise_integer<RhsRep, RhsRoundingTag> const& rhs,
                Operator)
        -> decltype(operate_common_tag<Operator, common_type_t<LhsRoundingTag, RhsRoundingTag>>(lhs, rhs))
        {
            return operate_common_tag<Operator, common_type_t<LhsRoundingTag, RhsRoundingTag>>(lhs, rhs);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // binary bitwise

    template<class LhsRep, class LhsRoundingTag, class RhsInteger>
    constexpr auto operator<<(
            precise_integer<LhsRep, LhsRoundingTag> const& lhs,
            RhsInteger const& rhs)
    -> decltype(from_rep<precise_integer<decltype(_impl::to_rep(lhs) << rhs), LhsRoundingTag>>(_impl::to_rep(lhs) << rhs))
    {
        return from_rep<precise_integer<
                decltype(_impl::to_rep(lhs) << rhs),
                LhsRoundingTag>>(_impl::to_rep(lhs) << rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::numeric_limits specialization for precise_integer

    template<class Rep, class RoundingTag>
    struct numeric_limits<cnl::precise_integer<Rep, RoundingTag>>
            : numeric_limits<cnl::_impl::number_base<cnl::precise_integer<Rep, RoundingTag>, Rep>> {
        static constexpr bool is_integer = true;
    };

    template<class Rep, class RoundingTag>
    struct numeric_limits<cnl::precise_integer<Rep, RoundingTag> const>
            : numeric_limits<cnl::_impl::number_base<cnl::precise_integer<Rep, RoundingTag>, Rep>> {
        static constexpr bool is_integer = true;
    };
}

namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits specialization for precise_integer

    template<class Rep, class RoundingTag>
    struct numeric_limits<cnl::precise_integer<Rep, RoundingTag>>
            : cnl::numeric_limits<cnl::precise_integer<Rep, RoundingTag>> {
    };

    template<class Rep, class RoundingTag>
    struct numeric_limits<cnl::precise_integer<Rep, RoundingTag> const>
            : cnl::numeric_limits<cnl::precise_integer<Rep, RoundingTag>> {
    };
}

#endif  // CNL_PRECISE_INTEGER_H
