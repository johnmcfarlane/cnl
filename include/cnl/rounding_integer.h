
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_ROUNDING_INTEGER_H)
#define CNL_ROUNDING_INTEGER_H 1

#include "cnl/bits/num_traits/fixed_width_scale.h"
#include "bits/number_base.h"
#include "bits/rounding.h"
#include "bits/used_digits.h"

/// compositional numeric library
namespace cnl {
    using _impl::native_rounding_tag;
    using _impl::nearest_rounding_tag;

    /// \brief An integer with customized rounding behavior.
    template<class Rep = int, class RoundingTag = _impl::nearest_rounding_tag>
    class rounding_integer;

    namespace _rounding_integer_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_rounding_integer_impl::is_rounding_integer

        template<class T>
        struct is_rounding_integer : std::false_type {
        };

        template<class Rep, class RoundingTag>
        struct is_rounding_integer<rounding_integer<Rep, RoundingTag>> : std::true_type {
        };
    }

    template<class Rep, class RoundingTag>
    class rounding_integer : public _impl::number_base<rounding_integer<Rep, RoundingTag>, Rep> {
        static_assert(!_rounding_integer_impl::is_rounding_integer<Rep>::value,
                "rounding_integer of rounding_integer is not a supported");

        using super = _impl::number_base<rounding_integer<Rep, RoundingTag>, Rep>;
    public:
        using rounding = RoundingTag;

        using _base = _impl::number_base<rounding_integer<Rep, RoundingTag>, Rep>;

        rounding_integer() = default;

        template<class T, _impl::enable_if_t<numeric_limits<T>::is_integer, int> Dummy = 0>
        constexpr rounding_integer(T const& v)
                : super(static_cast<Rep>(v)) { }

        template<class T, _impl::enable_if_t<!numeric_limits<T>::is_integer, int> Dummy = 0>
        constexpr rounding_integer(T const& v)
                : super(_impl::convert<rounding, Rep>{}(v)) { }

        template<class T>
        constexpr explicit operator T() const
        {
            return static_cast<T>(_impl::to_rep(*this));
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // numeric type traits

    template<class Rep, class RoundingTag>
    struct digits<rounding_integer<Rep, RoundingTag>> : digits<Rep> {
    };

    template<class Rep, class RoundingTag, int MinNumBits>
    struct set_digits<rounding_integer<Rep, RoundingTag>, MinNumBits> {
        using type = rounding_integer<set_digits_t<Rep, MinNumBits>, RoundingTag>;
    };

    /// \brief \ref rounding_integer specialization of \ref from_rep
    /// \tparam ArchetypeRep ignored; replaced by \c Rep
    /// \tparam RoundingTag the \c RoundingTag of the generated type
    template<class ArchetypeRep, class RoundingTag>
    struct from_rep<rounding_integer<ArchetypeRep, RoundingTag>> {
        /// \brief generates an \ref rounding_integer equivalent to \c r in type and value
        template<typename Rep>
        constexpr auto operator()(Rep const& r) const
        -> rounding_integer<Rep, RoundingTag>
        {
            return r;
        }
    };

    namespace _impl {
        template<class Rep, class RoundingTag>
        struct get_rep<rounding_integer<Rep, RoundingTag>> {
            using type = Rep;
        };

        template<class OldRep, class RoundingTag, class NewRep>
        struct set_rep<rounding_integer<OldRep, RoundingTag>, NewRep> {
            using type = rounding_integer<NewRep, RoundingTag>;
        };
    }

    template<class Rep, class RoundingTag, class Value>
    struct from_value<rounding_integer<Rep, RoundingTag>, Value> : _impl::from_value_simple<
            rounding_integer<Value, RoundingTag>, Value> {
    };

    template<class Rep, class RoundingTag, class ValueRep, class ValueRoundingTag>
    struct from_value<rounding_integer<Rep, RoundingTag>, rounding_integer<ValueRep, ValueRoundingTag>>
            : _impl::from_value_simple<
            rounding_integer<ValueRep, RoundingTag>, rounding_integer<ValueRep, ValueRoundingTag>> {
    };

    template<class Rep, class RoundingTag, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<rounding_integer<Rep, RoundingTag>, constant<Value>> : _impl::from_value_simple<
            rounding_integer<typename std::conditional<
                    digits<int>::value<_impl::used_digits(Value),
                            decltype(Value),
                            int>::type, RoundingTag>,
                    constant<Value>> {
    };

    template<int Digits, class Rep, class RoundingTag>
    struct scale<Digits, 2, rounding_integer<Rep, RoundingTag>,
            _impl::enable_if_t<Digits<0>>
            : _impl::default_scale<Digits, 2, rounding_integer<Rep, RoundingTag>> {
    };

    template<int Digits, int Radix, class Rep, class RoundingTag>
    struct scale<Digits, Radix, rounding_integer<Rep, RoundingTag>,
            _impl::enable_if_t<0<=Digits>> {
        constexpr auto operator()(rounding_integer<Rep, RoundingTag> const& s) const
        -> decltype(from_rep<rounding_integer<Rep, RoundingTag>>{}(scale<Digits, Radix, Rep>{}(_impl::to_rep(s))))
        {
            return from_rep<rounding_integer<Rep, RoundingTag>>{}(scale<Digits, Radix, Rep>{}(_impl::to_rep(s)));
        }
    };

    template<int Digits, class Rep, class RoundingTag>
    struct fixed_width_scale<Digits, 2, rounding_integer<Rep, RoundingTag>, _impl::enable_if_t<0<=Digits>> {
        constexpr auto operator()(rounding_integer<Rep, RoundingTag> const& s) const
        -> decltype(from_rep<rounding_integer<Rep, RoundingTag>>{}(
                fixed_width_scale<Digits, 2, Rep>{}(_impl::to_rep(s))))
        {
            return from_rep<rounding_integer<Rep, RoundingTag>>{}(
                    fixed_width_scale<Digits, 2, Rep>{}(_impl::to_rep(s)));
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_rounding_integer

    template<class RoundingTag, class Rep>
    constexpr auto make_rounding_integer(Rep const& value)
    -> rounding_integer<Rep, RoundingTag>
    {
        return value;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // unary arithmetic

    namespace _impl {
        // for operands with a common tag
        template<class Operator, class Rep, class RoundingTag>
        struct unary_operator<Operator, rounding_integer<Rep, RoundingTag>> {
            constexpr auto operator()(rounding_integer<Rep, RoundingTag> const& operand) const
            -> decltype(from_rep<rounding_integer<decltype(Operator()(_impl::to_rep(operand))), RoundingTag>>{}(
                    Operator()(_impl::to_rep(operand))))
            {
                using result_type = rounding_integer<decltype(Operator()(_impl::to_rep(operand))), RoundingTag>;
                return from_rep<result_type>{}(Operator()(_impl::to_rep(operand)));
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // binary arithmetic

        // for operands with a common tag
        template<class Operator, class LhsRep, class RhsRep, class RoundingTag>
        struct binary_operator<Operator,
                rounding_integer<LhsRep, RoundingTag>, rounding_integer<RhsRep, RoundingTag>,
                typename Operator::is_not_comparison> {
            constexpr auto operator()(
                    rounding_integer<LhsRep, RoundingTag> const& lhs,
                    rounding_integer<RhsRep, RoundingTag> const& rhs) const
            -> decltype(make_rounding_integer<RoundingTag>(tagged_binary_operator<RoundingTag, Operator>()(
                    _impl::to_rep(lhs), _impl::to_rep(rhs))))
            {
                return make_rounding_integer<RoundingTag>(
                        tagged_binary_operator<RoundingTag, Operator>()(_impl::to_rep(lhs), _impl::to_rep(rhs)));
            }
        };

        template<class LhsRep, class RhsRep, class RoundingTag>
        struct binary_operator<shift_right_op,
                rounding_integer<LhsRep, RoundingTag>, rounding_integer<RhsRep, RoundingTag>> {
            constexpr auto operator()(
                    rounding_integer<LhsRep, RoundingTag> const& lhs,
                    rounding_integer<RhsRep, RoundingTag> const& rhs) const
            -> decltype(from_rep<rounding_integer<decltype(_impl::to_rep(lhs)>>_impl::to_rep(rhs)), RoundingTag>>{}(
                    _impl::to_rep(lhs)>>_impl::to_rep(rhs)))
            {
                return from_rep<rounding_integer<decltype(_impl::to_rep(lhs)>>_impl::to_rep(rhs)), RoundingTag>>{}(
                        _impl::to_rep(lhs)>>_impl::to_rep(rhs));
            }
        };

        // comparison for operands with a common tag
        template<class Operator, class LhsRep, class RhsRep, class RoundingTag>
        struct binary_operator<Operator, rounding_integer<LhsRep, RoundingTag>, rounding_integer<RhsRep, RoundingTag>,
                typename Operator::is_comparison> {
        constexpr auto operator()(
                rounding_integer<LhsRep, RoundingTag> const& lhs,
                rounding_integer<RhsRep, RoundingTag> const& rhs) const
            -> decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)))
            {
                return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
            }
        };

        // for operands with different policies
        template<class Operator, class LhsRep, class LhsRoundingTag, class RhsRep, class RhsRoundingTag>
        struct binary_operator<Operator,
                rounding_integer<LhsRep, LhsRoundingTag>, rounding_integer<RhsRep, RhsRoundingTag>,
                typename Operator::is_comparison> {
            constexpr auto operator()(
                    rounding_integer<LhsRep, LhsRoundingTag> const& lhs,
                    rounding_integer<RhsRep, RhsRoundingTag> const& rhs) const
            -> decltype(binary_operator<Operator, rounding_integer<LhsRep, common_type_t<LhsRoundingTag, RhsRoundingTag>>, rounding_integer<LhsRep, common_type_t<LhsRoundingTag, RhsRoundingTag>>>()(lhs, rhs))
            {
                using common_tag = common_type_t<LhsRoundingTag, RhsRoundingTag>;
                return binary_operator<Operator, rounding_integer<LhsRep, common_tag>, rounding_integer<LhsRep, common_tag>>()(lhs, rhs);
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // pre/post operators

        template<class Operator, typename Rep, class RoundingTag>
        struct pre_operator<Operator, rounding_integer<Rep, RoundingTag>>
                : pre_operator<Operator, typename rounding_integer<Rep, RoundingTag>::_base> {
        };

        template<class Operator, typename Rep, class RoundingTag>
        struct post_operator<Operator, rounding_integer<Rep, RoundingTag>>
                : post_operator<Operator, typename rounding_integer<Rep, RoundingTag>::_base> {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::numeric_limits specialization for rounding_integer

    template<class Rep, class RoundingTag>
    struct numeric_limits<cnl::rounding_integer<Rep, RoundingTag>>
            : numeric_limits<cnl::_impl::number_base<cnl::rounding_integer<Rep, RoundingTag>, Rep>> {
        static constexpr bool is_integer = true;
    };

    template<class Rep, class RoundingTag>
    struct numeric_limits<cnl::rounding_integer<Rep, RoundingTag> const>
            : numeric_limits<cnl::_impl::number_base<cnl::rounding_integer<Rep, RoundingTag>, Rep>> {
        static constexpr bool is_integer = true;
    };
}

namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits specialization for rounding_integer

    template<class Rep, class RoundingTag>
    struct numeric_limits<cnl::rounding_integer<Rep, RoundingTag>>
            : cnl::numeric_limits<cnl::rounding_integer<Rep, RoundingTag>> {
    };

    template<class Rep, class RoundingTag>
    struct numeric_limits<cnl::rounding_integer<Rep, RoundingTag> const>
            : cnl::numeric_limits<cnl::rounding_integer<Rep, RoundingTag>> {
    };
}

#endif  // CNL_ROUNDING_INTEGER_H
