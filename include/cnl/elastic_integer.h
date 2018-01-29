
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `cnl::elastic_integer` type

#if !defined(CNL_ELASTIC_INTEGER_H)
#define CNL_ELASTIC_INTEGER_H 1

#include "constant.h"
#include "numeric.h"

#include "bits/number_base.h"

/// compositional numeric library
namespace cnl {

    /// \brief An integer type with auto-widening operators
    ///
    /// \tparam Digits a count of the number of digits needed to express the number
    /// \tparam Narrowest the most narrow integer type to use for storage
    ///
    /// \note The value is stored in an object of \c Narrowest or
    /// \ref cnl::set_digits_t<Narrowest, Digits>.
    /// \note Arithmetic operations result in types with an adjusted Digits parameter accordingly.
    /// For instance, when two \ref elastic_integer values are multiplied together,
    /// the resultant type has Digits set to the sum of the operands.
    ///
    /// \sa cnl::elastic_fixed_point
    template<int Digits, class Narrowest>
    class elastic_integer;

    ////////////////////////////////////////////////////////////////////////////////
    // numeric traits

    namespace _elastic_integer_impl {
        template<int Digits, class Narrowest>
        struct base_class {
            static constexpr _digits_type digits = Digits;

            static constexpr _digits_type rep_digits = _impl::max(cnl::digits<Narrowest>::value, digits);

            using rep = typename set_digits<Narrowest, rep_digits>::type;
            using type = _impl::number_base<elastic_integer<Digits, Narrowest>, rep>;
        };

        template<int Digits, class Narrowest>
        using base_class_t = typename base_class<Digits, Narrowest>::type;
    }

    template<int Digits, class Narrowest>
    struct digits<elastic_integer<Digits, Narrowest>> : std::integral_constant<_digits_type, Digits> {
    };

    template<int Digits, class Narrowest, _digits_type MinNumBits>
    struct set_digits<elastic_integer<Digits, Narrowest>, MinNumBits> {
        using type = elastic_integer<MinNumBits, Narrowest>;
    };

    namespace _impl {
        template<int Digits, class Narrowest>
        struct get_rep<elastic_integer<Digits, Narrowest>> {
            using type = Narrowest;
        };

        template<int Digits, class OldNarrowest, class NewNarrowest>
        struct set_rep<elastic_integer<Digits, OldNarrowest>, NewNarrowest> {
            using type = elastic_integer<Digits, NewNarrowest>;
        };
    }

    template<int Digits, class Narrowest, class Rep>
    struct from_rep<elastic_integer<Digits, Narrowest>, Rep> {
        constexpr auto operator()(elastic_integer<Digits, Narrowest> const& rep) const
        -> elastic_integer<Digits, cnl::_impl::make_signed_t<Narrowest, cnl::is_signed<Rep>::value>>
        {
            return rep;
        }
    };

    template<int Digits, class Narrowest, class Value>
    struct from_value<elastic_integer<Digits, Narrowest>, Value> {
        using type = elastic_integer<cnl::digits<Value>::value, cnl::_impl::make_signed_t<Narrowest, cnl::is_signed<Value>::value>>;
    };

    namespace _elastic_integer_impl {
        // cnl::_elastic_integer_impl::digits - given a value with which to initialize elastic_integer,
        // returns how many digits the type needs
        template<class Integer>
        constexpr int digits(Integer value) {
            return used_bits((value<0)?-value:value);
        }
    }

    template<int Digits, class Narrowest, intmax Value>
    struct from_value<elastic_integer<Digits, Narrowest>, constant<Value>> {
        static constexpr auto _to_digits = _elastic_integer_impl::digits(Value);
        using type = elastic_integer<_to_digits, Narrowest>;
    };

    // cnl::scale<..., cnl::elastic_integer<>>
    template<int ShiftDigits, int ScalarDigits, class ScalarNarrowest>
    struct scale<ShiftDigits, 2, elastic_integer<ScalarDigits, ScalarNarrowest>> {
        constexpr auto operator()(elastic_integer<ScalarDigits, ScalarNarrowest> const& s) const
        -> elastic_integer<ScalarDigits, ScalarNarrowest>
        {
            using result_type = elastic_integer<ScalarDigits, ScalarNarrowest>;
            using result_rep = typename result_type::rep;
            return _impl::to_rep(s) * (result_rep{1} << ShiftDigits);
        }
    };

    // cnl::shift<..., cnl::elastic_integer<>>
    template<int ShiftDigits, int ScalarDigits, class ScalarNarrowest>
    struct shift<ShiftDigits, 2, elastic_integer<ScalarDigits, ScalarNarrowest>, _impl::enable_if_t<0 <= ShiftDigits>> {
        constexpr auto operator()(elastic_integer<ScalarDigits, ScalarNarrowest> const& s) const
        -> elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>
        {
            using result_type = elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>;
            using result_rep = typename result_type::rep;
            return _impl::to_rep(s) * (result_rep{1} << ShiftDigits);
        }
    };

    template<int ShiftDigits, int ScalarDigits, class ScalarNarrowest>
    struct shift<ShiftDigits, 2, elastic_integer<ScalarDigits, ScalarNarrowest>, _impl::enable_if_t<ShiftDigits < 0>> {
        constexpr auto operator()(elastic_integer<ScalarDigits, ScalarNarrowest> const& s) const
        -> elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>
        {
            using divisor_type = elastic_integer<1-ShiftDigits, ScalarNarrowest>;
            using divisor_rep = typename divisor_type::rep;
            return _impl::to_rep(s) / (divisor_rep{1} << -ShiftDigits);
        }
    };

    template<int Digits = digits<int>::value, class Narrowest = int>
    class elastic_integer : public _elastic_integer_impl::base_class_t<Digits, Narrowest> {
        static_assert(Digits > 0, "type requires positive number of digits");
        using _base = _elastic_integer_impl::base_class_t<Digits, Narrowest>;
    public:
        /// alias to template parameter, \a Digits
        static constexpr int digits = Digits;

        /// alias to template parameter, \a Narrowest
        using narrowest = Narrowest;

        /// the actual type used to store the value; closely related to Narrowest but may be a different width
        using rep = typename _base::rep;

        /// default constructor
        constexpr elastic_integer() = default;
        
        /// common copy constructor
        constexpr elastic_integer(elastic_integer const& rhs)
                :_base(rhs)
        {
        }

        /// construct from numeric type
        template<class Number, _impl::enable_if_t<numeric_limits<Number>::is_specialized, int> Dummy = 0>
        constexpr elastic_integer(Number n)
                : _base(static_cast<rep>(n))
        {
        }

        /// constructor taking an elastic_integer type
        template<int FromWidth, class FromNarrowest>
        explicit constexpr elastic_integer(elastic_integer<FromWidth, FromNarrowest> const& rhs)
                :_base(static_cast<rep>(_impl::to_rep(rhs)))
        {
        }

        /// constructor taking an integral constant
        template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
        constexpr elastic_integer(constant<Value>)
                : _base(static_cast<rep>(Value))
        {
        }

        /// copy assignment operator taking a floating-point type
        template<class S, _impl::enable_if_t<std::is_floating_point<S>::value, int> Dummy = 0>
        elastic_integer& operator=(S s)
        {
            _base::operator=(floating_point_to_rep(s));
            return *this;
        }

        /// returns value
        template<class S>
        explicit constexpr operator S() const
        {
            return static_cast<S>(_impl::to_rep(*this));
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::elastic_integer{constant} deduction guide

    namespace _elastic_integer_impl {
        template<bool Signed>
        struct machine_digits {
            static constexpr _digits_type value =
                    cnl::digits<typename std::conditional<Signed, signed, unsigned>::type>::value;
        };

        template<typename S>
        using narrowest = set_digits_t<S, machine_digits<is_signed<S>::value>::value>;
    }

#if defined(__cpp_deduction_guides)
    template<class S>
    elastic_integer(S const& s)
    -> elastic_integer<digits_v<S>, _elastic_integer_impl::narrowest<S>>;

    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    elastic_integer(constant<Value>)
    -> elastic_integer<_elastic_integer_impl::digits(Value)>;
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_elastic_integer

    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    constexpr auto make_elastic_integer(constant<Value>)
    -> elastic_integer<_elastic_integer_impl::digits(Value)>
    {
        return elastic_integer<_elastic_integer_impl::digits(Value)>{Value};
    }

    namespace _elastic_integer_impl {
        template<class Narrowest, class Integral>
        struct make_narrowest {
            using type = Narrowest;
        };

        template<class Integral>
        struct make_narrowest<void, Integral> {
            using type = narrowest<Integral>;
        };

        template<class Narrowest, class Integral>
        using make_narrowest_t = typename make_narrowest<Narrowest, Integral>::type;

        template<class Narrowest, class Integral>
        using make_type = elastic_integer<cnl::digits<Integral>::value, make_narrowest_t<Narrowest, Integral>>;
    }

    template<class Narrowest = void, class Integral, _impl::enable_if_t<!_impl::is_constant<Integral>::value, int> Dummy = 0>
    constexpr auto make_elastic_integer(Integral const& value)
    -> _elastic_integer_impl::make_type<Narrowest, Integral>
    {
        return _elastic_integer_impl::make_type<Narrowest, Integral>{value};
    }

    namespace _elastic_integer_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_elastic_integer_impl::is_elastic_integer

        template<class ElasticInteger>
        struct is_elastic_integer : std::false_type {
        };

        template<int Digits, class Narrowest>
        struct is_elastic_integer<elastic_integer<Digits, Narrowest>> : std::true_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_elastic_integer_impl::are_integer_class_operands - basically identifies
        // operands that should go into a function defined here; filters out fixed-point

        template<class Lhs, class Rhs>
        struct are_integer_class_operands {
            static constexpr int integer_class = is_elastic_integer<Lhs>::value+is_elastic_integer<Rhs>::value;
            static constexpr int integer_or_float =
                    _impl::is_integer_or_float<Lhs>::value+_impl::is_integer_or_float<Rhs>::value;
            static constexpr bool value = (integer_class>=1) && (integer_or_float==2);
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // bitwise operators

    // operator~
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator~(elastic_integer<RhsDigits, RhsNarrowest> const& rhs)
    -> elastic_integer<RhsDigits, RhsNarrowest>
    {
        using elastic_integer = elastic_integer<RhsDigits, RhsNarrowest>;
        using rep = typename elastic_integer::rep;
        return _impl::from_rep<elastic_integer>(
            static_cast<rep>(
                _impl::to_rep(rhs)
                ^ ((static_cast<rep>(~0)) >> (numeric_limits<rep>::digits - RhsDigits))));
    }

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // comparison operators

        template<int FromDigits, class FromNarrowest, int OtherDigits, class OtherNarrowest,
                _impl::enable_if_t<FromDigits!=OtherDigits || !std::is_same<FromNarrowest, OtherNarrowest>::value, std::nullptr_t> Dummy = nullptr>
        constexpr auto cast_to_common_type(
                elastic_integer<FromDigits, FromNarrowest> const& from,
                elastic_integer<OtherDigits, OtherNarrowest> const&)
        -> decltype(static_cast<_impl::common_type_t<
                elastic_integer<FromDigits, FromNarrowest>,
                elastic_integer<OtherDigits, OtherNarrowest>>>(from)) {
            return static_cast<_impl::common_type_t<
                    elastic_integer<FromDigits, FromNarrowest>,
                    elastic_integer<OtherDigits, OtherNarrowest>>>(from);
        };

        template<class Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
        struct comparison_operator<Operator,
                elastic_integer<LhsDigits, LhsNarrowest>, elastic_integer<RhsDigits, RhsNarrowest>> {
            constexpr auto operator()(
                    elastic_integer<LhsDigits, LhsNarrowest> const& lhs,
                    elastic_integer<RhsDigits, RhsNarrowest> const& rhs) const
            -> decltype(Operator()(cast_to_common_type(lhs, rhs), cast_to_common_type(rhs, lhs)))
            {
                return Operator()(cast_to_common_type(lhs, rhs), cast_to_common_type(rhs, lhs));
            }
        };

        template<class Operator, int Digits, class Narrowest>
        struct comparison_operator<Operator, elastic_integer<Digits, Narrowest>, elastic_integer<Digits, Narrowest>> {
            constexpr auto operator()(
                    elastic_integer<Digits, Narrowest> const& lhs,
                    elastic_integer<Digits, Narrowest> const& rhs) const
            -> decltype(Operator()(to_rep(lhs), to_rep(rhs)))
            {
                return Operator()(to_rep(lhs), to_rep(rhs));
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // arithmetic operators

        ////////////////////////////////////////////////////////////////////////////////
        // policies

        template<class Operation, class LhsTraits, class RhsTraits>
        struct policy;

        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::add_op, LhsTraits, RhsTraits> {
            static constexpr int digits = _impl::max(LhsTraits::digits, RhsTraits::digits)+1;
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed;
        };

        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::subtract_op, LhsTraits, RhsTraits> {
            static constexpr int digits = _impl::max(LhsTraits::digits, RhsTraits::digits) + (LhsTraits::is_signed | RhsTraits::is_signed);
            static constexpr bool is_signed = true;
        };

        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::multiply_op, LhsTraits, RhsTraits> {
            static constexpr int contribution(int operand_digits) { return operand_digits == 1 ? 0 : operand_digits; }
            static constexpr int digits = max(1, contribution(LhsTraits::digits)+contribution(RhsTraits::digits));
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed;
        };

        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::divide_op, LhsTraits, RhsTraits> {
            static constexpr int digits = LhsTraits::digits;
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed;
        };

        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::bitwise_or_op, LhsTraits, RhsTraits> {
            static constexpr int digits = _impl::max(LhsTraits::digits, RhsTraits::digits);
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed; 
        };

        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::bitwise_and_op, LhsTraits, RhsTraits> {
            static constexpr int digits = _impl::min(LhsTraits::digits, RhsTraits::digits);
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed; 
        };

        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::bitwise_xor_op, LhsTraits, RhsTraits> {
            static constexpr int digits = _impl::max(LhsTraits::digits, RhsTraits::digits);
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed;
        };

        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::shift_left_op, LhsTraits, RhsTraits> {
            static constexpr int digits = LhsTraits::digits;
            static constexpr bool is_signed = LhsTraits::is_signed;
        };

        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::shift_right_op, LhsTraits, RhsTraits> {
            static constexpr int digits = LhsTraits::digits;
            static constexpr bool is_signed = LhsTraits::is_signed;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // operate_params

        template<class OperationTag, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
        struct operate_params {
            using lhs = elastic_integer<LhsDigits, LhsNarrowest>;
            using rhs = elastic_integer<RhsDigits, RhsNarrowest>;
            using lhs_traits = numeric_limits<lhs>;
            using rhs_traits = numeric_limits<rhs>;

            using policy = typename _impl::policy<OperationTag, lhs_traits, rhs_traits>;

            using lhs_rep = typename lhs::rep;
            using rhs_rep = typename rhs::rep;
            using rep_result = typename _impl::op_result<OperationTag, lhs_rep, rhs_rep>;

            static constexpr _digits_type narrowest_width = _impl::max(
                    digits<LhsNarrowest>::value + cnl::is_signed<LhsNarrowest>::value,
                    digits<RhsNarrowest>::value + cnl::is_signed<RhsNarrowest>::value);
            using narrowest = set_digits_t<_impl::make_signed_t<rep_result, policy::is_signed>, narrowest_width-policy::is_signed>;
            using result_type = elastic_integer<policy::digits, narrowest>;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::binary_operator

        template<class Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
        struct binary_operator<Operator,
                elastic_integer<LhsDigits, LhsNarrowest>, elastic_integer<RhsDigits, RhsNarrowest>> {
            constexpr auto operator()(
                    elastic_integer<LhsDigits, LhsNarrowest> const& lhs,
                    elastic_integer<RhsDigits, RhsNarrowest> const& rhs) const
            -> typename operate_params<Operator, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::result_type
            {
                using result_type = typename operate_params<Operator, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::result_type;
                return from_rep<result_type>(
                        static_cast<typename result_type::rep>(Operator()(
                                to_rep(static_cast<result_type>(lhs)),
                                to_rep(static_cast<result_type>(rhs)))));
            }
        };
    }

    template<int LhsDigits, class LhsNarrowest, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    constexpr auto operator<<(elastic_integer<LhsDigits, LhsNarrowest> const& lhs, constant<RhsValue>)
    -> decltype(_impl::from_rep<elastic_integer<LhsDigits + RhsValue, LhsNarrowest>>(_impl::to_rep(static_cast<elastic_integer<LhsDigits + RhsValue, LhsNarrowest>>(lhs)) << RhsValue)) {
        using result_type = elastic_integer<LhsDigits + RhsValue, LhsNarrowest>;
        return _impl::from_rep<result_type>(_impl::to_rep(static_cast<result_type>(lhs)) << RhsValue);
    }

    template<int LhsDigits, class LhsNarrowest, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    constexpr auto operator>>(elastic_integer<LhsDigits, LhsNarrowest> const& lhs, constant<RhsValue>)
    -> decltype (_impl::from_rep<elastic_integer<LhsDigits - RhsValue, LhsNarrowest>>(_impl::to_rep(lhs) >> RhsValue)) {
        return _impl::from_rep<elastic_integer<LhsDigits - RhsValue, LhsNarrowest>>(_impl::to_rep(lhs) >> RhsValue);
    }

    // unary operator-
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator-(elastic_integer<RhsDigits, RhsNarrowest> const& rhs)
    -> decltype(_impl::from_rep<elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>>(-_impl::to_rep(static_cast<elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>>(rhs))))
    {
        using result_type = elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>;
        return _impl::from_rep<result_type>(-_impl::to_rep(static_cast<result_type>(rhs)));
    }

    // unary operator+
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator+(elastic_integer<RhsDigits, RhsNarrowest> const& rhs)
    -> decltype(_impl::from_rep<elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>>(_impl::to_rep(static_cast<elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>>(rhs))))
    {
        using result_type = elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>;
        return _impl::from_rep<result_type>(_impl::to_rep(static_cast<result_type>(rhs)));
    }
}

namespace std {
    template<int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
    struct common_type<cnl::elastic_integer<LhsDigits, LhsNarrowest>, cnl::elastic_integer<RhsDigits, RhsNarrowest>> {
        using type = cnl::elastic_integer<
                cnl::_impl::max(LhsDigits, RhsDigits),
                cnl::_impl::common_signedness_t<LhsNarrowest, RhsNarrowest>>;
    };

    template<int LhsDigits, class LhsNarrowest, class Rhs>
    struct common_type<cnl::elastic_integer<LhsDigits, LhsNarrowest>, Rhs>
            : common_type<cnl::elastic_integer<LhsDigits, LhsNarrowest>, cnl::elastic_integer<numeric_limits<Rhs>::digits, Rhs>> {
    };

    template<class Lhs, int RhsDigits, class RhsNarrowest>
    struct common_type<Lhs, cnl::elastic_integer<RhsDigits, RhsNarrowest>>
            : common_type<cnl::elastic_integer<numeric_limits<Lhs>::digits, Lhs>, cnl::elastic_integer<RhsDigits, RhsNarrowest>> {
    };
}

namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::numeric_limits for cnl::elastic_integer

    namespace _elastic_integer_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_elastic_integer_impl::lowest

        // cnl::helper for numeric_limits<cnl::elastic_integer<>>::lowest()
        template<class Rep, bool IsSigned>
        struct lowest;

        template<class Rep>
        struct lowest<Rep, true> {
            constexpr Rep operator()(Rep const& max) const noexcept
            {
                return -max;
            }
        };

        template<class Rep>
        struct lowest<Rep, false> {
            constexpr Rep operator()(Rep const&) const noexcept
            {
                return 0;
            }
        };
    };

    template<int Digits, class Narrowest>
    struct numeric_limits<elastic_integer<Digits, Narrowest>>
            : numeric_limits<Narrowest> {
        // elastic integer-specific helpers
        using _narrowest_numeric_limits = numeric_limits<Narrowest>;
        using _value_type = elastic_integer<Digits, Narrowest>;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;

        static constexpr _rep _rep_max() noexcept
        {
            return _rep_numeric_limits::max() >> (_rep_numeric_limits::digits-digits);
        }

        // standard members
        static constexpr int digits = Digits;

        static constexpr _value_type min() noexcept
        {
            return _impl::from_rep<_value_type>(1);
        }

        static constexpr _value_type max() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_max());
        }

        static constexpr _value_type lowest() noexcept
        {
            return _elastic_integer_impl::lowest<_rep, _narrowest_numeric_limits::is_signed>()(_rep_max());
        }
    };

    template<int Digits, class Narrowest>
    struct numeric_limits<elastic_integer<Digits, Narrowest> const>
            : numeric_limits<elastic_integer<Digits, Narrowest>> {
    };
}

#endif  // CNL_ELASTIC_INTEGER_H
