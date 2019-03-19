
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `cnl::overflow_integer` type

#if !defined(CNL_OVERFLOW_INT_H)
#define CNL_OVERFLOW_INT_H 1

#include "overflow.h"

#include "_impl/native_tag.h"
#include "_impl/number_base.h"
#include "_impl/operators.h"
#include "_impl/type_traits/common_type.h"
#include "_impl/type_traits/identical.h"

#include <ostream>
#include <type_traits>

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
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::overflow_integer<>

    template<class Rep = int, class OverflowTag = undefined_overflow_tag>
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

        overflow_integer() = default;

        template<class RhsRep, class RhsOverflowTag>
        constexpr overflow_integer(overflow_integer<RhsRep, RhsOverflowTag> const& rhs)
                :overflow_integer(_impl::to_rep(rhs))
        {
        }

        template<class Rhs, _impl::enable_if_t<!_integer_impl::is_overflow_integer<Rhs>::value, int> dummy = 0>
        constexpr overflow_integer(Rhs const& rhs)
                :_base(_impl::convert<overflow_tag, rep>(rhs))
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
            return _impl::convert<overflow_tag, T>(_impl::to_rep(*this));
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // numeric traits

    template<class Rep, class OverflowTag>
    struct digits<overflow_integer<Rep, OverflowTag>> : digits<Rep> {
    };

    template<class Rep, class OverflowTag, int MinNumBits>
    struct set_digits<overflow_integer<Rep, OverflowTag>, MinNumBits> {
        using type = overflow_integer<set_digits_t<Rep, MinNumBits>, OverflowTag>;
    };

    /// \brief \ref overflow_integer specialization of \ref from_rep
    /// \tparam ArchetypeRep ignored; replaced by \c Rep
    /// \tparam OverflowTag the \c OverflowTag of the generated type
    template<typename ArchetypeRep, class OverflowTag, typename Rep>
    struct from_rep<overflow_integer<ArchetypeRep, OverflowTag>, Rep> {
        /// \brief generates an \ref overflow_integer equivalent to \c r in type and value
        constexpr auto operator()(Rep const& r) const
        -> overflow_integer<Rep, OverflowTag>
        {
            return r;
        }
    };

    template<class Rep, class OverflowTag, class Value>
    struct from_value<overflow_integer<Rep, OverflowTag>, Value>
            : _impl::from_value_simple<overflow_integer<Value, OverflowTag>, Value> {
    };

    template<class Rep, class OverflowTag, class ValueRep, class ValueOverflowTag>
    struct from_value<overflow_integer<Rep, OverflowTag>, overflow_integer<ValueRep, ValueOverflowTag>>
            : _impl::from_value_simple<
                    overflow_integer<
                            from_value_t<Rep, ValueRep>,
                            _impl::common_type_t<OverflowTag, ValueOverflowTag>>,
                    overflow_integer<ValueRep, ValueOverflowTag>> {
    };

    template<class Rep, class OverflowTag, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<overflow_integer<Rep, OverflowTag>, constant<Value>>
            : _impl::from_value_simple<overflow_integer<
                    typename std::conditional<
                            digits<int>::value<_impl::used_digits(Value), decltype(Value), int>::type, OverflowTag>,
                    constant<Value>>{
    };

    template<int Digits, class Rep, class OverflowTag>
    struct scale<Digits, 2, overflow_integer<Rep, OverflowTag>,
            _impl::enable_if_t<(Digits>=0)>> {
        using _value_type = overflow_integer<Rep, OverflowTag>;
        constexpr auto operator()(_value_type const& s) const
        -> decltype(_impl::from_rep<_value_type>(shift_left(OverflowTag{}, _impl::to_rep(s), constant<Digits>{})))
        {
            return _impl::from_rep<_value_type>(shift_left(OverflowTag{}, _impl::to_rep(s), constant<Digits>{}));
        }
    };

    template<int Digits, int Radix, class Rep, class OverflowTag>
    struct scale<Digits, Radix, overflow_integer<Rep, OverflowTag>,
            _impl::enable_if_t<(Digits<0||Radix!=2)>>
            : scale<Digits, Radix, _impl::number_base<overflow_integer<Rep, OverflowTag>, Rep>> {
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
            -> decltype(overflow_integer<decltype(Operator()(_impl::to_rep(operand))), OverflowTag>(Operator()(_impl::to_rep(operand))))
            {
                return overflow_integer<decltype(Operator()(_impl::to_rep(operand))), OverflowTag>(Operator()(_impl::to_rep(operand)));
            }
        };

        // for arithmetic operands with a common overflow tag
        template<class Operator, class LhsRep, class RhsRep, class OverflowTag>
        struct binary_operator<Operator,
                overflow_integer<LhsRep, OverflowTag>, overflow_integer<RhsRep, OverflowTag>> {
            constexpr auto operator()(
                    overflow_integer<LhsRep, OverflowTag> const& lhs,
                    overflow_integer<RhsRep, OverflowTag> const& rhs) const
            -> overflow_integer<op_result<Operator, LhsRep, RhsRep>, OverflowTag>
            {
                return from_rep<overflow_integer<op_result<Operator, LhsRep, RhsRep>, OverflowTag>>(
                        _impl::tagged_binary_operator<OverflowTag, Operator>{}(_impl::to_rep(lhs), _impl::to_rep(rhs)));
            }
        };

        // for comparison operands with a common overflow tag
        template<class Operator, class LhsRep, class RhsRep, class OverflowTag>
        struct comparison_operator<Operator,
                overflow_integer<LhsRep, OverflowTag>, overflow_integer<RhsRep, OverflowTag>> {
            constexpr auto operator()(
                    overflow_integer<LhsRep, OverflowTag> const& lhs,
                    overflow_integer<RhsRep, OverflowTag> const& rhs) const
            -> decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)))
            {
                return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
            }
        };

        // for arithmetic operands with different overflow tags or rep types
        template<class Operator, class LhsRep, class LhsTag, class RhsRep, class RhsTag>
        struct comparison_operator<Operator,
                overflow_integer<LhsRep, LhsTag>, overflow_integer<RhsRep, RhsTag>> {
            using common_tag = cnl::_impl::common_type_t<LhsTag, RhsTag>;
            using operator_type = comparison_operator<
                    Operator,
                    overflow_integer<LhsRep, common_tag>,
                    overflow_integer<RhsRep, common_tag>>;

            constexpr auto operator()(
                    const overflow_integer<LhsRep, LhsTag>& lhs,
                    const overflow_integer<RhsRep, RhsTag>& rhs) const
            -> decltype(operator_type{}(lhs, rhs))
            {
                return operator_type{}(lhs, rhs);
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // pre/post operators

        template<class Operator, typename Rep, class OverflowTag>
        struct pre_operator<Operator, overflow_integer<Rep, OverflowTag>> {
            constexpr auto operator()(overflow_integer<Rep, OverflowTag>& rhs) const
            -> decltype(typename pre_to_assign<Operator>::type{}(rhs, 1))
            {
                return typename pre_to_assign<Operator>::type{}(rhs, 1);
            }
        };

        template<class Operator, typename Rep, class OverflowTag>
        struct post_operator<Operator, overflow_integer<Rep, OverflowTag>> {
            CNL_RELAXED_CONSTEXPR auto operator()(overflow_integer<Rep, OverflowTag>& rhs) const
            -> overflow_integer<Rep, OverflowTag>
            {
                auto copy = rhs;
                typename post_to_assign<Operator>::type{}(rhs, 1);
                return copy;
            }
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::numeric_limits specialization for overflow_integer

    template<class Rep, class OverflowTag>
    struct numeric_limits<overflow_integer<Rep, OverflowTag>>
            : numeric_limits<_impl::number_base<overflow_integer<Rep, OverflowTag>, Rep>> {
        static constexpr bool is_integer = true;
    };

    template<class Rep, class OverflowTag>
    struct numeric_limits<overflow_integer<Rep, OverflowTag> const>
            : numeric_limits<_impl::number_base<overflow_integer<Rep, OverflowTag>, Rep>> {
        static constexpr bool is_integer = true;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // operator<<(std::ostream& o, cnl::overflow_integer const& i)

    template<class Rep, class OverflowTag>
    std::ostream &operator<<(std::ostream &o, overflow_integer<Rep, OverflowTag> const &i) {
        return o << _impl::to_rep(i);
    }
}

namespace std {
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
