
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `cnl::overflow_integer` type

#if !defined(CNL_OVERFLOW_INT_H)
#define CNL_OVERFLOW_INT_H 1

#include "fixed_point.h"
#include "overflow.h"

#include "bits/number_base.h"

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // forward-declarations

    /// \brief An integer which detects overflow.
    template<class Rep, class OverflowTag>
    class overflow_integer;

    namespace _integer_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_integer_impl::is_overflow_integer - trait to identify cnl::overflow_integer<>

        template<class T>
        struct is_overflow_integer
                : std::false_type {
        };

        template<class Rep, class OverflowTag>
        struct is_overflow_integer<overflow_integer<Rep, OverflowTag>>
                : std::true_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_integer_impl::common_type

        template<class, class, class = void>
        struct common_type;

        // given two overflow_integer<>, produces the type that is best suited to both of them
        template<class LhsRep, class RhsRep, class OverflowTag>
        struct common_type<
                overflow_integer<LhsRep, OverflowTag>,
                overflow_integer<RhsRep, OverflowTag>> {
            using type = overflow_integer<
                    typename std::common_type<LhsRep, RhsRep>::type,
                    OverflowTag>;
        };

        // given a overflow_integer<> and a built-in integer type,
        // generates a overflow_integer<> type that is as big as both of them (or as close as possible)
        template<class LhsRep, class LhsOverflowTag, class RhsInteger>
        struct common_type<
                overflow_integer<LhsRep, LhsOverflowTag>, RhsInteger,
                _impl::enable_if_t<
                        !_integer_impl::is_overflow_integer<RhsInteger>::value && std::is_integral<RhsInteger>::value>> {
            using type = typename cnl::overflow_integer<typename std::common_type<LhsRep, RhsInteger>::type, LhsOverflowTag>;
        };

        // given a overflow_integer<> and a floating-point type,
        // generates a floating-point type that is as big as both of them (or as close as possible)
        template<class LhsRep, class LhsOverflowTag, class Float>
        struct common_type<
                overflow_integer<LhsRep, LhsOverflowTag>, Float,
                _impl::enable_if_t<std::is_floating_point<Float>::value>> {
            using type = typename std::common_type<LhsRep, Float>::type;
        };

        // when first type is not overflow_integer<> and second type is, reverse the order
        template<class Lhs, class RhsRep, class RhsOverflowTag>
        struct common_type<Lhs, overflow_integer<RhsRep, RhsOverflowTag>>
                : common_type<overflow_integer<RhsRep, RhsOverflowTag>, Lhs> {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::overflow_integer<>

    template<class Rep = int, class OverflowTag = throwing_overflow_tag>
    class overflow_integer : public _impl::number_base<overflow_integer<Rep, OverflowTag>, Rep> {
        static_assert(!_integer_impl::is_overflow_integer<Rep>::value,
                "overflow_integer of overflow_integer is not a supported");
    public:
        ////////////////////////////////////////////////////////////////////////////////
        // types

        using rep = Rep;
        using overflow_tag = OverflowTag;

        using _base = _impl::number_base<overflow_integer<Rep, OverflowTag>, Rep>;

        ////////////////////////////////////////////////////////////////////////////////
        // functions

        constexpr overflow_integer() = delete;

        template<class RhsRep, class RhsOverflowTag>
        constexpr overflow_integer(overflow_integer<RhsRep, RhsOverflowTag> const& rhs)
                :overflow_integer(to_rep(rhs))
        {
        }

        template<class Rhs, _impl::enable_if_t<!_integer_impl::is_overflow_integer<Rhs>::value, int> dummy = 0>
        constexpr overflow_integer(Rhs const& rhs)
                :_base(convert<rep>(overflow_tag{}, rhs))
        {
        }

        /// constructor taking an integral constant
        template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
        constexpr overflow_integer(constant<Value>)
                : _base(static_cast<rep>(Value))
        {
            static_assert(Value <= numeric_limits<rep>::max(), "initialization by out-of-range value");
            static_assert(!numeric_limits<decltype(Value)>::is_signed || Value >= numeric_limits<rep>::lowest(), "initialization by out-of-range value");
        }

        template<class T>
        constexpr explicit operator T() const
        {
            return static_cast<T>(to_rep(*this));
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_impl::set_rep<overflow_integer<>>

    namespace _impl {
        template<class Rep, class OverflowTag>
        struct get_rep<overflow_integer<Rep, OverflowTag>> {
            using type = Rep;
        };

        template<class OldRep, class OverflowTag, class NewRep>
        struct set_rep<overflow_integer<OldRep, OverflowTag>, NewRep> {
            using type = overflow_integer<NewRep, OverflowTag>;
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // numeric traits

    template<class Rep, class OverflowTag>
    struct digits<overflow_integer<Rep, OverflowTag>> : digits<Rep> {
    };

    template<class Rep, class OverflowTag, _digits_type MinNumBits>
    struct set_digits<overflow_integer<Rep, OverflowTag>, MinNumBits> {
        using type = overflow_integer<set_digits_t<Rep, MinNumBits>, OverflowTag>;
    };

    /// \brief Overload of \ref to_rep(Number const& number) for \ref overflow_integer.
    template<class Rep, class OverflowTag>
    constexpr Rep to_rep(overflow_integer<Rep, OverflowTag> const& number)
    {
        using base_type = typename overflow_integer<Rep, OverflowTag>::_base;
        return to_rep(static_cast<base_type const&>(number));
    }

    /// \brief \ref overflow_integer specialization of \ref from_rep
    /// \tparam ArchetypeRep ignored; replaced by \c Rep
    /// \tparam OverflowTag the \c OverflowTag of the generated type
    template<class ArchetypeRep, class OverflowTag>
    struct from_rep<overflow_integer<ArchetypeRep, OverflowTag>> {
        /// \brief generates an \ref overflow_integer equivalent to \c r in type and value
        template<typename Rep>
        constexpr auto operator()(Rep const& r) const
        -> overflow_integer<Rep, OverflowTag>
        {
            return r;
        }
    };

    template<class Rep, class OverflowTag, class Value>
    struct from_value<overflow_integer<Rep, OverflowTag>, Value> {
        using type = overflow_integer<Value, OverflowTag>;
    };

    template<class Rep, class OverflowTag, class ValueRep, class ValueOverflowTag>
    struct from_value<overflow_integer<Rep, OverflowTag>, overflow_integer<ValueRep, ValueOverflowTag>> {
    private:
        // the common_type of two overflow tags is the stricter for some sense of strict
        using _overflow_tag = _impl::common_type_t<OverflowTag, ValueOverflowTag>;
        using _rep = from_value_t<Rep, ValueRep>;
    public:
        using type = overflow_integer<_rep, _overflow_tag>;
    };

    template<class Rep, class OverflowTag, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<overflow_integer<Rep, OverflowTag>, constant<Value>> {
        using _rep = typename std::conditional<digits<int>::value<used_bits(Value), decltype(Value), int>::type;
        using type = overflow_integer<_rep, OverflowTag>;
    };

    template<int Digits, int Radix, class Rep, class OverflowTag>
    struct shift<Digits, Radix, overflow_integer<Rep, OverflowTag>>
            : shift<Digits, Radix, _impl::number_base<overflow_integer<Rep, OverflowTag>, Rep>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_overflow_int

    template<class OverflowTag, class Rep>
    constexpr auto make_overflow_int(Rep const& value)
    -> overflow_integer<Rep, OverflowTag>
    {
        return value;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // binary arithmetic

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // arithmetc

        // for arithmetic operands with a common overflow tag
        template<class Operator, class Rep, class OverflowTag>
        struct unary_operator<Operator, overflow_integer<Rep, OverflowTag>> {
            constexpr auto operator()(overflow_integer<Rep, OverflowTag> const& operand) const
            -> decltype(overflow_integer<decltype(Operator()(to_rep(operand))), OverflowTag>(Operator()(to_rep(operand))))
            {
                return overflow_integer<decltype(Operator()(to_rep(operand))), OverflowTag>(Operator()(to_rep(operand)));
            }
        };

        // for arithmetic operands with a common overflow tag and common rep type
        template<class Operator, class Rep, class OverflowTag>
        struct binary_operator<Operator,
                overflow_integer<Rep, OverflowTag>, overflow_integer<Rep, OverflowTag>,
                typename Operator::is_not_comparison> {
            constexpr auto operator()(
                    overflow_integer<Rep, OverflowTag> const& lhs,
                    overflow_integer<Rep, OverflowTag> const& rhs) const
            -> decltype(make_overflow_int<OverflowTag>(_overflow_impl::binary_operator<OverflowTag, Operator>()(to_rep(lhs), to_rep(rhs))))
            {
                return make_overflow_int<OverflowTag>(
                        _overflow_impl::binary_operator<OverflowTag, Operator>()(to_rep(lhs), to_rep(rhs)));
            }
        };

        // for arithmetic operands with a common overflow tag but different rep types
        template<class Operator, class LhsRep, class RhsRep, class OverflowTag>
        struct binary_operator<Operator,
                overflow_integer<LhsRep, OverflowTag>, overflow_integer<RhsRep, OverflowTag>,
                typename Operator::is_not_comparison> {
            constexpr auto operator()(
                    overflow_integer<LhsRep, OverflowTag> const& lhs,
                    overflow_integer<RhsRep, OverflowTag> const& rhs) const
            -> overflow_integer<decltype(Operator{}(std::declval<LhsRep>(), std::declval<RhsRep>())), OverflowTag>
            {
                using result_rep_type = decltype(Operator{}(std::declval<LhsRep>(), std::declval<RhsRep>()));
                using result_type = overflow_integer<result_rep_type, OverflowTag>;
                return _impl::from_value<result_type>(binary_operator<Operator, result_type, result_type>{}(lhs, rhs));
            }
        };

        // for comparison operands with a common overflow tag
        template<class Operator, class LhsRep, class RhsRep, class OverflowTag>
        struct binary_operator<Operator,
                overflow_integer<LhsRep, OverflowTag>, overflow_integer<RhsRep, OverflowTag>,
                typename Operator::is_comparison> {
            constexpr auto operator()(
                    overflow_integer<LhsRep, OverflowTag> const& lhs,
                    overflow_integer<RhsRep, OverflowTag> const& rhs) const
            -> decltype(_overflow_impl::comparison_operator<OverflowTag, Operator>()(to_rep(lhs), to_rep(rhs)))
            {
                return _overflow_impl::comparison_operator<OverflowTag, Operator>()(to_rep(lhs), to_rep(rhs));
            }
        };

        // for arithmetic operands with different overflow tags
        template<class Operator, class LhsRep, class LhsTag, class RhsRep, class RhsTag>
        struct binary_operator<Operator,
                overflow_integer<LhsRep, LhsTag>, overflow_integer<RhsRep, RhsTag>,
                typename Operator::is_comparison> {
            constexpr auto operator()(
                    const overflow_integer<LhsRep, LhsTag>& lhs,
                    const overflow_integer<RhsRep, RhsTag>& rhs) const
            -> decltype(binary_operator<Operator, overflow_integer<LhsRep, cnl::_impl::common_type_t<LhsTag, RhsTag>>, overflow_integer<LhsRep, cnl::_impl::common_type_t<LhsTag, RhsTag>>>()(lhs, rhs))
            {
                using common_tag = cnl::_impl::common_type_t<LhsTag, RhsTag>;
                return binary_operator<Operator, overflow_integer<LhsRep, common_tag>, overflow_integer<LhsRep, common_tag>>()(lhs, rhs);
            }
        };
    }
}

namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits specialization for overflow_integer

    template<class Rep, class OverflowTag>
    struct numeric_limits<cnl::overflow_integer<Rep, OverflowTag>>
            : numeric_limits<cnl::_impl::number_base<cnl::overflow_integer<Rep, OverflowTag>, Rep>> {
        static constexpr bool is_integer = true;
    };

    template<class Rep, class OverflowTag>
    struct numeric_limits<cnl::overflow_integer<Rep, OverflowTag> const>
            : numeric_limits<cnl::_impl::number_base<cnl::overflow_integer<Rep, OverflowTag>, Rep>> {
        static constexpr bool is_integer = true;
    };
}

namespace std {
    // std::common_type<T, cnl::overflow_integer>
    template<
            class Lhs,
            class RhsRep, class RhsOverflowTag>
    struct common_type<
            Lhs,
            cnl::overflow_integer<RhsRep, RhsOverflowTag>>
            : cnl::_integer_impl::common_type<
                    Lhs,
                    cnl::overflow_integer<RhsRep, RhsOverflowTag>> {
    };

    // std::common_type<cnl::overflow_integer, T>
    template<
            class LhsRep, class LhsOverflowTag,
            class Rhs>
    struct common_type<
            cnl::overflow_integer<LhsRep, LhsOverflowTag>,
            Rhs>
            : cnl::_integer_impl::common_type<
                    cnl::overflow_integer<LhsRep, LhsOverflowTag>,
                    Rhs> {
    };

    // std::common_type<cnl::overflow_integer, cnl::fixed_point>
    template<
            class LhsRep, class LhsOverflowTag,
            class RhsRep, int RhsExponent>
    struct common_type<
            cnl::overflow_integer<LhsRep, LhsOverflowTag>,
            cnl::fixed_point<RhsRep, RhsExponent>>
            : std::common_type<
                    cnl::fixed_point<cnl::overflow_integer<LhsRep, LhsOverflowTag>, 0>,
                    cnl::fixed_point<RhsRep, RhsExponent>> {
    };

    // std::common_type<cnl::fixed_point, cnl::overflow_integer>
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, class RhsOverflowTag>
    struct common_type<
            cnl::fixed_point<LhsRep, LhsExponent>,
            cnl::overflow_integer<RhsRep, RhsOverflowTag>>
            : std::common_type<
                    cnl::fixed_point<LhsRep, LhsExponent>,
                    cnl::fixed_point<cnl::overflow_integer<RhsRep, RhsOverflowTag>, 0>> {
    };
    
    // std::common_type<cnl::overflow_integer, cnl::overflow_integer>
    template<
            class LhsRep, class LhsOverflowTag,
            class RhsRep, class RhsOverflowTag>
    struct common_type<
            cnl::overflow_integer<LhsRep, LhsOverflowTag>,
            cnl::overflow_integer<RhsRep, RhsOverflowTag>>
            : cnl::_integer_impl::common_type<
                    cnl::overflow_integer<LhsRep, LhsOverflowTag>,
                    cnl::overflow_integer<RhsRep, RhsOverflowTag>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits specialization for overflow_integer

    template<class Rep, class OverflowTag>
    struct numeric_limits<cnl::overflow_integer<Rep, OverflowTag>>
            : cnl::numeric_limits<cnl::overflow_integer<Rep, OverflowTag>> {
    };

    template<class Rep, class OverflowTag>
    struct numeric_limits<cnl::overflow_integer<Rep, OverflowTag> const>
            : cnl::numeric_limits<cnl::overflow_integer<Rep, OverflowTag>> {
    };
}

#endif  // CNL_OVERFLOW_INT_H
