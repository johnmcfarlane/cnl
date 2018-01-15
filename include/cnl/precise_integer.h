
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_PRECISE_INTEGER_H)
#define CNL_PRECISE_INTEGER_H 1

#include "cnlint.h"
#include "limits.h"

#include "bits/number_base.h"
#include "numeric.h"

namespace cnl {

    struct closest_rounding_tag {
        template<class To, class From>
        static constexpr To convert(From const& from)
        {
            return static_cast<To>(intmax(from+((from>=0) ? .5 : -.5)));
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
            return static_cast<T>(_impl::to_rep(*this));
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

    template<class FromRep, class RoundingTag, class Rep>
    struct from_rep<precise_integer<FromRep, RoundingTag>, Rep> {
        constexpr auto operator()(precise_integer<FromRep, RoundingTag> const& rep) const
        -> precise_integer<Rep, RoundingTag>
        {
            return rep;
        }
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

    template<class Rep, class RoundingTag, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<precise_integer<Rep, RoundingTag>, constant<Value>> {
        using _rep = typename std::conditional<digits<int>::value<used_bits(Value),
                decltype(Value),
                int>::type;
        using type = precise_integer<_rep, RoundingTag>;
    };

    template<int Digits, int Radix, class Rep, class RoundingTag>
    struct shift<Digits, Radix, precise_integer<Rep, RoundingTag>>
            : shift<Digits, Radix, _impl::number_base<precise_integer<Rep, RoundingTag>, Rep>> {
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
    // unary arithmetic

    namespace _impl {
        // for operands with a common tag
        template<class Operator, class Rep, class RoundingTag>
        struct unary_operator<Operator, precise_integer<Rep, RoundingTag>> {
            constexpr auto operator()(precise_integer<Rep, RoundingTag> const& operand) const
            -> decltype(from_rep<precise_integer<decltype(Operator()(to_rep(operand))), RoundingTag>>(Operator()(to_rep(operand))))
            {
                using result_type = precise_integer<decltype(Operator()(to_rep(operand))), RoundingTag>;
                return from_rep<result_type>(Operator()(to_rep(operand)));
            }
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // binary arithmetic

    namespace _impl {
        // for operands with a common tag
        template<class Operator, class LhsRep, class RhsRep, class RoundingTag>
        struct binary_operator<Operator, precise_integer<LhsRep, RoundingTag>, precise_integer<RhsRep, RoundingTag>> {
            constexpr auto operator()(
                    precise_integer<LhsRep, RoundingTag> const& lhs,
                    precise_integer<RhsRep, RoundingTag> const& rhs) const
            -> decltype(from_rep<precise_integer<op_result<Operator, LhsRep, RhsRep>, RoundingTag>>(Operator()(to_rep(lhs), to_rep(rhs))))
            {
                using result_type = precise_integer<op_result<Operator, LhsRep, RhsRep>, RoundingTag>;
                return from_rep<result_type>(Operator()(to_rep(lhs), to_rep(rhs)));
            }
        };

        // for arithmetic operands with different policies
        template<class Operator, class LhsRep, class LhsRoundingTag, class RhsRep, class RhsRoundingTag>
        struct binary_operator<Operator,
                precise_integer<LhsRep, LhsRoundingTag>, precise_integer<RhsRep, RhsRoundingTag>> {
            constexpr auto operator()(
                    precise_integer<LhsRep, LhsRoundingTag> const& lhs,
                    precise_integer<RhsRep, RhsRoundingTag> const& rhs) const
            -> decltype(binary_operator<Operator, precise_integer<LhsRep, common_type_t<LhsRoundingTag, RhsRoundingTag>>, precise_integer<LhsRep, common_type_t<LhsRoundingTag, RhsRoundingTag>>>()(lhs, rhs))
            {
                using common_tag = common_type_t<LhsRoundingTag, RhsRoundingTag>;
                return binary_operator<Operator, precise_integer<LhsRep, common_tag>, precise_integer<LhsRep, common_tag>>()(lhs, rhs);
            }
        };

        // for arithmetic operands with different policies
        template<class Operator, class LhsRep, class RhsRep, class RoundingTag>
        struct comparison_operator<Operator, precise_integer<LhsRep, RoundingTag>, precise_integer<RhsRep, RoundingTag>> {
        constexpr auto operator()(
                precise_integer<LhsRep, RoundingTag> const& lhs,
                precise_integer<RhsRep, RoundingTag> const& rhs) const
            -> decltype(Operator()(to_rep(lhs), to_rep(rhs)))
            {
                return Operator()(to_rep(lhs), to_rep(rhs));
            }
        };

        // for operands with different policies
        template<class Operator, class LhsRep, class LhsRoundingTag, class RhsRep, class RhsRoundingTag>
        struct comparison_operator<Operator,
                precise_integer<LhsRep, LhsRoundingTag>, precise_integer<RhsRep, RhsRoundingTag>> {
            constexpr auto operator()(
                    precise_integer<LhsRep, LhsRoundingTag> const& lhs,
                    precise_integer<RhsRep, RhsRoundingTag> const& rhs) const
            -> decltype(comparison_operator<Operator, precise_integer<LhsRep, common_type_t<LhsRoundingTag, RhsRoundingTag>>, precise_integer<LhsRep, common_type_t<LhsRoundingTag, RhsRoundingTag>>>()(lhs, rhs))
            {
                using common_tag = common_type_t<LhsRoundingTag, RhsRoundingTag>;
                return comparison_operator<Operator, precise_integer<LhsRep, common_tag>, precise_integer<LhsRep, common_tag>>()(lhs, rhs);
            }
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // binary bitwise

    template<class LhsRep, class LhsRoundingTag, class RhsInteger>
    constexpr auto operator<<(
            precise_integer<LhsRep, LhsRoundingTag> const& lhs,
            RhsInteger const& rhs)
    -> decltype(_impl::from_rep<precise_integer<decltype(_impl::to_rep(lhs) << rhs), LhsRoundingTag>>(_impl::to_rep(lhs) << rhs))
    {
        return _impl::from_rep<precise_integer<
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
