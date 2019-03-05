
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `cnl::elastic_integer` type

#if !defined(CNL_ELASTIC_INTEGER_H)
#define CNL_ELASTIC_INTEGER_H 1

#include "constant.h"

#include "_impl/limits/lowest.h"
#include "_impl/num_traits/adopt.h"
#include "_impl/num_traits/adopt_signedness.h"
#include "_impl/num_traits/fixed_width_scale.h"
#include "_impl/num_traits/width.h"
#include "_impl/number_base.h"
#include "_impl/type_traits/common_type.h"
#include "_impl/type_traits/is_signed.h"
#include "_impl/type_traits/make_signed.h"
#include "_impl/type_traits/set_signedness.h"

#include <ostream>

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
    /// \sa cnl::elastic_number
    template<int Digits, class Narrowest>
    class elastic_integer;

    ////////////////////////////////////////////////////////////////////////////////
    // numeric traits

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
        // cnl::_elastic_integer_impl::base_class_t

        template<int Digits, class Narrowest>
        using rep_t = typename set_digits<Narrowest, _impl::max(cnl::digits<Narrowest>::value, Digits)>::type;

        template<int Digits, class Narrowest>
        using base_class_t = _impl::number_base<
                elastic_integer<Digits, Narrowest>,
                _elastic_integer_impl::rep_t<Digits, Narrowest>>;
    }

    template<int Digits, class Narrowest>
    struct digits<elastic_integer<Digits, Narrowest>> : std::integral_constant<int, Digits> {
    };

    template<int Digits, class Narrowest, int MinNumBits>
    struct set_digits<elastic_integer<Digits, Narrowest>, MinNumBits> {
        using type = elastic_integer<MinNumBits, Narrowest>;
    };

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
    }

    /// \brief \ref elastic_integer specialization of \ref from_rep
    /// \tparam Digits ignored; replaced by number of digits of \c Rep
    /// \tparam Exponent the \c Exponent parameter of the generated \ref fixed_point type
    template<int Digits, typename Narrowest, typename Rep>
    struct from_rep<elastic_integer<Digits, Narrowest>, Rep> {
        /// \brief generates an \ref elastic_integer equivalent to \c r in type and value
        constexpr auto operator()(Rep const& r) const
        -> elastic_integer<Digits, cnl::_impl::adopt_signedness_t<Narrowest, Rep>>
        {
            return r;
        }
    };

    template<int Digits, class Narrowest, class Value>
    struct from_value<elastic_integer<Digits, Narrowest>, Value>
            : _impl::from_value_simple<
                    elastic_integer<
                            cnl::digits<Value>::value,
                            _impl::set_width_t<Value, _impl::width<Narrowest>::value>>,
                    Value> {
    };

    template<int Digits, typename Narrowest, int ValueDigits, typename ValueNarrowest>
    struct from_value<elastic_integer<Digits, Narrowest>, elastic_integer<ValueDigits, ValueNarrowest>>
            : _impl::from_value_simple<
                    elastic_integer<_impl::max(Digits, ValueDigits), Narrowest>,
                    elastic_integer<ValueDigits, Narrowest>> {
    };

    template<int Digits, class Narrowest, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<elastic_integer<Digits, Narrowest>, constant<Value>> : _impl::from_value_simple<
            elastic_integer<digits<constant<Value>>::value, int>,
            constant<Value>> {
    };

    // cnl::fixed_width_scale<..., cnl::elastic_integer<>>
    template<int ScalePower, int ScaleRadix, int ScalarDigits, class ScalarNarrowest>
    struct fixed_width_scale<
            ScalePower, ScaleRadix, elastic_integer<ScalarDigits, ScalarNarrowest>> {
        constexpr auto operator()(elastic_integer<ScalarDigits, ScalarNarrowest> const& s) const
        -> elastic_integer<ScalarDigits, ScalarNarrowest>
        {
            using result_type = elastic_integer<ScalarDigits, ScalarNarrowest>;
            using result_rep = typename result_type::rep;
            return fixed_width_scale<ScalePower, ScaleRadix, result_rep>()(_impl::to_rep(s));
        }
    };

    // cnl::scale<..., cnl::elastic_integer<>>
    template<int ShiftDigits, int ScaleRadix, int ScalarDigits, class ScalarNarrowest>
    struct scale<ShiftDigits, ScaleRadix, elastic_integer<ScalarDigits, ScalarNarrowest>,
            _impl::enable_if_t<(0<=ShiftDigits)>> {
        constexpr auto operator()(elastic_integer<ScalarDigits, ScalarNarrowest> const& s) const
        -> elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>
        {
            using result_type = elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>;
            using result_rep = typename result_type::rep;
            return _impl::from_rep<result_type>(
                    scale<ShiftDigits, ScaleRadix, result_rep>()(_impl::to_rep(s)));
        }
    };

    template<int ShiftDigits, int ScalarDigits, class ScalarNarrowest>
    struct scale<ShiftDigits, 2, elastic_integer<ScalarDigits, ScalarNarrowest>,
            _impl::enable_if_t<(ShiftDigits<0)>> {
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
    public:
        using _base = _elastic_integer_impl::base_class_t<Digits, Narrowest>;
        static_assert(!_elastic_integer_impl::is_elastic_integer<typename _base::rep>::value,
                "elastic_integer of elastic_integer is not a supported");

        /// the actual type used to store the value; closely related to Narrowest but may be a different width
        using rep = typename _base::rep;

        /// default constructor
        elastic_integer() = default;

        /// construct from numeric type
        template<class Number, _impl::enable_if_t<numeric_limits<Number>::is_specialized, int> = 0>
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
        template<class S, _impl::enable_if_t<std::is_floating_point<S>::value, int> = 0>
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
            static constexpr int value =
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
    -> elastic_integer<digits_v<constant<Value>>>;
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_elastic_integer

    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    constexpr auto make_elastic_integer(constant<Value>)
    -> elastic_integer<digits<constant<Value>>::value>
    {
        return elastic_integer<digits<constant<Value>>::value>{Value};
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

    template<class Narrowest = void, class Integral, _impl::enable_if_t<!_impl::is_constant<Integral>::value, int> = 0>
    constexpr auto make_elastic_integer(Integral const& value)
    -> _elastic_integer_impl::make_type<Narrowest, Integral>
    {
        return _elastic_integer_impl::make_type<Narrowest, Integral>{value};
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
                _impl::enable_if_t<FromDigits!=OtherDigits || !std::is_same<FromNarrowest, OtherNarrowest>::value, std::nullptr_t> = nullptr>
        constexpr auto cast_to_common_type(
                elastic_integer<FromDigits, FromNarrowest> const& from,
                elastic_integer<OtherDigits, OtherNarrowest> const&)
        -> decltype(static_cast<_impl::common_type_t<
                elastic_integer<FromDigits, FromNarrowest>,
                elastic_integer<OtherDigits, OtherNarrowest>>>(from)) {
            return static_cast<_impl::common_type_t<
                    elastic_integer<FromDigits, FromNarrowest>,
                    elastic_integer<OtherDigits, OtherNarrowest>>>(from);
        }

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
            -> decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)))
            {
                return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
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
        struct policy<_impl::modulo_op, LhsTraits, RhsTraits> {
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

            static constexpr int narrowest_width = _impl::max(
                    width<LhsNarrowest>::value,
                    width<RhsNarrowest>::value);
            using narrowest = set_digits_t<
                    _impl::set_signedness_t<rep_result, policy::is_signed>,
                    narrowest_width-policy::is_signed>;
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

        ////////////////////////////////////////////////////////////////////////////////
        // pre/post operators

        template<class Operator, int Digits, typename Narrowest>
        struct pre_operator<Operator, elastic_integer<Digits, Narrowest>>
                : pre_operator<Operator, typename elastic_integer<Digits, Narrowest>::_base> {
        };

        template<class Operator, int Digits, typename Narrowest>
        struct post_operator<Operator, elastic_integer<Digits, Narrowest>>
                : post_operator<Operator, typename elastic_integer<Digits, Narrowest>::_base> {
        };
    }

    template<int LhsDigits, class LhsNarrowest, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    constexpr auto operator<<(elastic_integer<LhsDigits, LhsNarrowest> const& lhs, constant<RhsValue>)
    -> decltype(_impl::from_rep<elastic_integer<LhsDigits + static_cast<int>(RhsValue), LhsNarrowest>>(
            _impl::to_rep(static_cast<elastic_integer<LhsDigits + static_cast<int>(RhsValue), LhsNarrowest>>(lhs)) << RhsValue)) {
        using result_type = elastic_integer<LhsDigits + static_cast<int>(RhsValue), LhsNarrowest>;
        return _impl::from_rep<result_type>(_impl::to_rep(static_cast<result_type>(lhs)) << RhsValue);
    }

    template<int LhsDigits, class LhsNarrowest, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    constexpr auto operator>>(elastic_integer<LhsDigits, LhsNarrowest> const& lhs, constant<RhsValue>)
    -> decltype (_impl::from_rep<elastic_integer<LhsDigits - static_cast<int>(RhsValue), LhsNarrowest>>(
            _impl::to_rep(lhs) >> RhsValue)) {
        return _impl::from_rep<elastic_integer<LhsDigits - static_cast<int>(RhsValue), LhsNarrowest>>(
                _impl::to_rep(lhs) >> RhsValue);
    }

    // unary operator-
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator-(elastic_integer<RhsDigits, RhsNarrowest> const& rhs)
    -> decltype(_impl::from_rep<elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>>(
            -_impl::to_rep(static_cast<elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>>(rhs))))
    {
        using result_type = elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>;
        return _impl::from_rep<result_type>(-_impl::to_rep(static_cast<result_type>(rhs)));
    }

    // unary operator+
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator+(elastic_integer<RhsDigits, RhsNarrowest> const& rhs)
    -> decltype(_impl::from_rep<elastic_integer<RhsDigits, RhsNarrowest>>(
            +_impl::to_rep(static_cast<elastic_integer<RhsDigits, RhsNarrowest>>(rhs))))
    {
        return _impl::from_rep<elastic_integer<RhsDigits, RhsNarrowest>>(
                +_impl::to_rep(static_cast<elastic_integer<RhsDigits, RhsNarrowest>>(rhs)));
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
            return static_cast<_rep>(_rep_numeric_limits::max() >> (_rep_numeric_limits::digits-digits));
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
            return _impl::lowest<_rep, _narrowest_numeric_limits::is_signed>()(_rep_max());
        }
    };

    template<int Digits, class Narrowest>
    struct numeric_limits<elastic_integer<Digits, Narrowest> const>
            : numeric_limits<elastic_integer<Digits, Narrowest>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // operator<<(std::ostream& o, cnl::elastic_integer const& i)

    template<int Digits, class Narrowest>
    std::ostream &operator<<(std::ostream &o, elastic_integer<Digits, Narrowest> const &i) {
        return o << _impl::to_rep(i);
    }
}

#endif  // CNL_ELASTIC_INTEGER_H
