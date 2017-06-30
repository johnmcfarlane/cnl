
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SG14_PRECISE_INTEGER_H)
#define SG14_PRECISE_INTEGER_H 1

#include <sg14/bits/number_base.h>
#include <sg14/bits/limits.h>

namespace sg14 {

    struct closest_rounding_tag {
        template<class To, class From>
        static constexpr To convert(const From& from)
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

        template<class T, _impl::enable_if_t<std::numeric_limits<T>::is_integer, int> Dummy = 0>
        constexpr precise_integer(const T& v)
                : super(static_cast<Rep>(v)) { }

        template<class T, _impl::enable_if_t<!std::numeric_limits<T>::is_integer, int> Dummy = 0>
        constexpr precise_integer(const T& v)
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
                const precise_integer<LhsRep, RoundingTag>& lhs,
                const precise_integer<RhsRep, RoundingTag>& rhs)
        -> decltype(from_rep<precise_integer<op_result<Operator, LhsRep, RhsRep>, RoundingTag>>(Operator()(lhs.data(), rhs.data())))
        {
            using result_type = precise_integer<op_result<Operator, LhsRep, RhsRep>, RoundingTag>;
            return from_rep<result_type>(Operator()(lhs.data(), rhs.data()));
        }

        // for arithmetic operands with different policies
        template<class Operator, class RoundingTag, class LhsRep, class RhsRep, enable_if_t<Operator::is_comparison, int> = 0>
        constexpr auto operate_common_tag(
                const precise_integer<LhsRep, RoundingTag>& lhs,
                const precise_integer<RhsRep, RoundingTag>& rhs)
        -> decltype(Operator()(lhs.data(), rhs.data()))
        {
            return Operator()(lhs.data(), rhs.data());
        }

        // for arithmetic operands with different policies
        template<class Operator, class LhsRep, class LhsRoundingTag, class RhsRep, class RhsRoundingTag>
        constexpr auto operate(
                const precise_integer<LhsRep, LhsRoundingTag>& lhs,
                const precise_integer<RhsRep, RhsRoundingTag>& rhs,
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
            const precise_integer<LhsRep, LhsRoundingTag>& lhs,
            const RhsInteger& rhs)
    -> decltype(from_rep<precise_integer<decltype(_impl::to_rep(lhs) << rhs), LhsRoundingTag>>(_impl::to_rep(lhs) << rhs))
    {
        return from_rep<precise_integer<
                decltype(_impl::to_rep(lhs) << rhs),
                LhsRoundingTag>>(_impl::to_rep(lhs) << rhs);
    }
}

namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits specialization for precise_integer

    template<class Rep, class RoundingTag>
    struct numeric_limits<sg14::precise_integer<Rep, RoundingTag>>
            : numeric_limits<sg14::_impl::number_base<sg14::precise_integer<Rep, RoundingTag>, Rep>> {};
}

#endif
