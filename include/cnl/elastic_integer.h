
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `cnl::elastic_integer` type

#if !defined(CNL_ELASTIC_INTEGER_H)
#define CNL_ELASTIC_INTEGER_H

#include "constant.h"

#include "_impl/limits/lowest.h"
#include "_impl/num_traits/adopt_width.h"
#include "_impl/num_traits/digits.h"
#include "_impl/num_traits/fixed_width_scale.h"
#include "_impl/num_traits/rep.h"
#include "_impl/num_traits/rounding.h"
#include "_impl/num_traits/set_width.h"
#include "_impl/num_traits/width.h"
#include "_impl/number_base.h"
#include "_impl/type_traits/add_signedness.h"
#include "_impl/type_traits/is_signed.h"
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
    /// \sa cnl::elastic_scaled_integer
    template<int Digits = digits<int>::value, class Narrowest = int>
    class elastic_integer;

    ////////////////////////////////////////////////////////////////////////////////
    // numeric traits

    template<int Digits, class Narrowest>
    struct rep<elastic_integer<Digits, Narrowest>>
            : set_digits<Narrowest, _impl::max(cnl::digits<Narrowest>::value, Digits)> {
    };

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_elastic_integer

        template<class ElasticInteger>
        struct is_elastic_integer : std::false_type {
        };

        template<int Digits, class Narrowest>
        struct is_elastic_integer<elastic_integer<Digits, Narrowest>> : std::true_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::base_class_t

        template<int Digits, class Narrowest>
        using base_class_t = _impl::number_base<
                elastic_integer<Digits, Narrowest>,
                _impl::rep_t<elastic_integer<Digits, Narrowest>>>;
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
    /// \tparam Exponent the \c Exponent parameter of the generated \ref scaled_integer type
    template<int Digits, typename Narrowest, typename Rep>
    struct set_rep<elastic_integer<Digits, Narrowest>, Rep>
            : _impl::type_identity<elastic_integer<Digits, _impl::adopt_width_t<Rep, Narrowest>>> {
    };

    /// \brief \ref elastic_integer specialization of \ref from_rep
    /// \tparam Digits ignored; replaced by number of digits of \c Rep
    /// \tparam Exponent the \c Exponent parameter of the generated \ref scaled_integer type
    template<int Digits, typename Narrowest, typename Rep>
    struct from_rep<elastic_integer<Digits, Narrowest>, Rep> {
        /// \brief generates an \ref elastic_integer equivalent to \c r in type and value
        CNL_NODISCARD constexpr auto operator()(Rep const& r) const
        -> _impl::set_rep_t<elastic_integer<Digits, Narrowest>, Rep>
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

    template<int Digits, class Narrowest>
    struct rounding<elastic_integer<Digits, Narrowest>> : rounding<_impl::rep_t<elastic_integer<Digits, Narrowest>>> {
    };

    // cnl::scale<..., cnl::elastic_integer<>>
    template<int ShiftDigits, int ScaleRadix, int ScalarDigits, class ScalarNarrowest>
    struct scale<ShiftDigits, ScaleRadix, elastic_integer<ScalarDigits, ScalarNarrowest>,
            _impl::enable_if_t<(0<=ShiftDigits)>> {
        CNL_NODISCARD constexpr auto operator()(elastic_integer<ScalarDigits, ScalarNarrowest> const& s) const
        -> elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>
        {
            using result_type = elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>;
            using result_rep = _impl::rep_t<result_type>;
            return _impl::from_rep<result_type>(
                    scale<ShiftDigits, ScaleRadix, result_rep>()(_impl::to_rep(s)));
        }
    };

    template<int ShiftDigits, int ScalarDigits, class ScalarNarrowest>
    struct scale<ShiftDigits, 2, elastic_integer<ScalarDigits, ScalarNarrowest>,
            _impl::enable_if_t<(ShiftDigits<0)>> {
        CNL_NODISCARD constexpr auto operator()(elastic_integer<ScalarDigits, ScalarNarrowest> const& s) const
        -> elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>
        {
            using divisor_type = elastic_integer<1-ShiftDigits, ScalarNarrowest>;
            using divisor_rep = _impl::rep_t<divisor_type>;
            return _impl::to_rep(s) / (divisor_rep{1} << -ShiftDigits);
        }
    };

    template<int Digits, class Narrowest>
    class elastic_integer : public _impl::base_class_t<Digits, Narrowest> {
    public:
        using _base = _impl::base_class_t<Digits, Narrowest>;

        using _rep_type = typename _impl::rep_t<elastic_integer>;
        static_assert(!_impl::is_elastic_integer<_rep_type>::value,
                "elastic_integer of elastic_integer is not a supported");

        /// default constructor
        elastic_integer() = default;

        /// construct from numeric type
        template<class Number, _impl::enable_if_t<numeric_limits<Number>::is_specialized, int> = 0>
        constexpr elastic_integer(Number n)  // NOLINT(hicpp-explicit-conversions, google-explicit-constructor)
                : _base(static_cast<_rep_type>(n))
        {
        }

        /// constructor taking an elastic_integer type
        template<int FromWidth, class FromNarrowest>
        explicit constexpr elastic_integer(elastic_integer<FromWidth, FromNarrowest> const& rhs)
                :_base(static_cast<_rep_type>(_impl::to_rep(rhs)))
        {
        }

        /// constructor taking an integral constant
        template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
        explicit constexpr elastic_integer(constant<Value>)
                : _base(static_cast<_rep_type>(Value))
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
        CNL_NODISCARD explicit constexpr operator S() const
        {
            return static_cast<S>(_impl::to_rep(*this));
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::elastic_integer{constant} deduction guide

    namespace _impl {
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
    -> elastic_integer<digits_v<S>, _impl::narrowest<S>>;

    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    elastic_integer(constant<Value>)
    -> elastic_integer<digits_v<constant<Value>>>;
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_elastic_integer

    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    CNL_NODISCARD constexpr auto make_elastic_integer(constant<Value>)
    -> elastic_integer<digits<constant<Value>>::value>
    {
        return elastic_integer<digits<constant<Value>>::value>{Value};
    }

    namespace _impl {
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
    CNL_NODISCARD constexpr auto make_elastic_integer(Integral const& value)
    -> _impl::make_type<Narrowest, Integral>
    {
        return _impl::make_type<Narrowest, Integral>{value};
    }

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // comparison operators

        template<typename T1, typename T2>
        struct common_elastic_type;

        template<int Digits1, class Narrowest1, int Digits2, class Narrowest2>
        struct common_elastic_type<
                cnl::elastic_integer<Digits1, Narrowest1>,
                cnl::elastic_integer<Digits2, Narrowest2>> {
            using type = cnl::elastic_integer<
                    cnl::_impl::max(Digits1, Digits2),
                    cnl::_impl::common_signedness_t<Narrowest1, Narrowest2>>;
        };

        template<int Digits1, class Narrowest1, class Rhs>
        struct common_elastic_type<cnl::elastic_integer<Digits1, Narrowest1>, Rhs>
                : common_elastic_type<
                        cnl::elastic_integer<Digits1, Narrowest1>,
                        cnl::elastic_integer<numeric_limits<Rhs>::digits, Rhs>> {
        };

        template<class Lhs, int Digits2, class Narrowest2>
        struct common_elastic_type<Lhs, cnl::elastic_integer<Digits2, Narrowest2>>
                : common_elastic_type<
                        cnl::elastic_integer<numeric_limits<Lhs>::digits, Lhs>,
                        cnl::elastic_integer<Digits2, Narrowest2>> {
        };

        template<int FromDigits, class FromNarrowest, int OtherDigits, class OtherNarrowest,
                _impl::enable_if_t<FromDigits!=OtherDigits || !std::is_same<FromNarrowest, OtherNarrowest>::value, std::nullptr_t> = nullptr>
        CNL_NODISCARD constexpr auto cast_to_common_type(
                elastic_integer<FromDigits, FromNarrowest> const& from,
                elastic_integer<OtherDigits, OtherNarrowest> const&)
        -> decltype(static_cast<typename common_elastic_type<
                elastic_integer<FromDigits, FromNarrowest>,
                elastic_integer<OtherDigits, OtherNarrowest>>::type>(from)) {
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
        -> decltype(Operator()(cast_to_common_type(lhs, rhs), cast_to_common_type(rhs, lhs)))
        {
            return Operator()(cast_to_common_type(lhs, rhs), cast_to_common_type(rhs, lhs));
        }
    };

    template<class Operator, int Digits, class Narrowest>
    struct comparison_operator<Operator, elastic_integer<Digits, Narrowest>, elastic_integer<Digits, Narrowest>> {
        CNL_NODISCARD constexpr auto operator()(
                elastic_integer<Digits, Narrowest> const& lhs,
                elastic_integer<Digits, Narrowest> const& rhs) const
        -> decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)))
        {
            return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
        }
    };

    namespace _impl {
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

        ////////////////////////////////////////////////////////////////////////////////
        // operate_params

        template<class OperationTag, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
        struct operate_params {
            using lhs = elastic_integer<LhsDigits, LhsNarrowest>;
            using rhs = elastic_integer<RhsDigits, RhsNarrowest>;
            using lhs_traits = numeric_limits<lhs>;
            using rhs_traits = numeric_limits<rhs>;

            using policy = typename _impl::policy<OperationTag, lhs_traits, rhs_traits>;

            using rep_result = typename _impl::op_result<OperationTag, rep_t<lhs>, rep_t<rhs>>;

            static constexpr int narrowest_width = _impl::max(
                    width<LhsNarrowest>::value,
                    width<RhsNarrowest>::value);
            using narrowest = set_digits_t<
                    _impl::set_signedness_t<rep_result, policy::is_signed>,
                    narrowest_width-policy::is_signed>;
            using result_type = elastic_integer<policy::digits, narrowest>;
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::binary_operator

    template<class Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
    struct binary_operator<
            Operator,
            _impl::native_tag, _impl::native_tag,
            elastic_integer<LhsDigits, LhsNarrowest>, elastic_integer<RhsDigits, RhsNarrowest>> {
        CNL_NODISCARD constexpr auto operator()(
                elastic_integer<LhsDigits, LhsNarrowest> const& lhs,
                elastic_integer<RhsDigits, RhsNarrowest> const& rhs) const
        -> typename _impl::operate_params<
                Operator,
                LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::result_type
        {
            using result_type = typename _impl::operate_params<
                    Operator, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::result_type;
            return _impl::from_rep<result_type>(
                    static_cast<_impl::rep_t<result_type>>(Operator()(
                            _impl::to_rep(static_cast<result_type>(lhs)),
                            _impl::to_rep(static_cast<result_type>(rhs)))));
        }
    };

#if defined(CNL_OVERLOAD_RESOLUTION_HACK)
    namespace _impl {
        template<int LhsDigits, class LhsNarrowest, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
        struct excluded_from_specialization<elastic_integer<LhsDigits, LhsNarrowest>, constant<RhsValue>>
                : std::true_type {
        };
    }

    template<int LhsDigits, class LhsNarrowest, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    struct shift_operator<
            _impl::shift_left_op,
            _impl::native_tag, _impl::native_tag,
            elastic_integer<LhsDigits, LhsNarrowest>, constant<RhsValue>> {
        CNL_NODISCARD constexpr auto operator()(
                elastic_integer<LhsDigits, LhsNarrowest> const& lhs,
                constant<RhsValue>) const
        -> decltype(_impl::from_rep<elastic_integer<LhsDigits+int{RhsValue}, LhsNarrowest>>(
                _impl::to_rep(static_cast<elastic_integer<LhsDigits+int{RhsValue}, LhsNarrowest>>(
                        lhs)) << RhsValue))
        {
            return _impl::from_rep<elastic_integer<LhsDigits+int{RhsValue}, LhsNarrowest>>(
                    _impl::to_rep(static_cast<elastic_integer<LhsDigits+int{RhsValue}, LhsNarrowest>>(
                            lhs)) << RhsValue);
        }
    };
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // pre/post operators

    template<class Operator, int Digits, typename Narrowest>
    struct pre_operator<Operator, _impl::native_tag, elastic_integer<Digits, Narrowest>>
            : pre_operator<Operator, _impl::native_tag, typename elastic_integer<Digits, Narrowest>::_base> {
    };

    template<class Operator, int Digits, typename Narrowest>
    struct post_operator<Operator, _impl::native_tag, elastic_integer<Digits, Narrowest>>
            : post_operator<Operator, _impl::native_tag, typename elastic_integer<Digits, Narrowest>::_base> {
    };

#if ! defined(CNL_OVERLOAD_RESOLUTION_HACK)
    template<int LhsDigits, class LhsNarrowest, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    CNL_NODISCARD constexpr auto operator<<(elastic_integer<LhsDigits, LhsNarrowest> const& lhs, constant<RhsValue>)
    -> decltype(_impl::from_rep<elastic_integer<LhsDigits + static_cast<int>(RhsValue), LhsNarrowest>>(
            _impl::to_rep(static_cast<elastic_integer<LhsDigits + static_cast<int>(RhsValue), LhsNarrowest>>(lhs)) << RhsValue)) {
        using result_type = elastic_integer<LhsDigits + static_cast<int>(RhsValue), LhsNarrowest>;
        return _impl::from_rep<result_type>(_impl::to_rep(static_cast<result_type>(lhs)) << RhsValue);
    }
#endif

    template<int LhsDigits, class LhsNarrowest, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    CNL_NODISCARD constexpr auto operator>>(elastic_integer<LhsDigits, LhsNarrowest> const& lhs, constant<RhsValue>)
    -> decltype (_impl::from_rep<elastic_integer<LhsDigits - static_cast<int>(RhsValue), LhsNarrowest>>(
            _impl::to_rep(lhs) >> RhsValue)) {
        return _impl::from_rep<elastic_integer<LhsDigits - static_cast<int>(RhsValue), LhsNarrowest>>(
                _impl::to_rep(lhs) >> RhsValue);
    }

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

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::numeric_limits for cnl::elastic_integer

    template<int Digits, class Narrowest>
    struct numeric_limits<elastic_integer<Digits, Narrowest>>
            : numeric_limits<Narrowest> {
        // elastic integer-specific helpers
        using _narrowest_numeric_limits = numeric_limits<Narrowest>;
        using _value_type = elastic_integer<Digits, Narrowest>;
        using _rep = _impl::rep_t<_value_type>;
        using _rep_numeric_limits = numeric_limits<_rep>;

        CNL_NODISCARD static constexpr _rep _rep_max() noexcept
        {
            return static_cast<_rep>(_rep_numeric_limits::max() >> (_rep_numeric_limits::digits-digits));
        }

        // standard members
        static constexpr int digits = Digits;

        CNL_NODISCARD static constexpr _value_type min() noexcept
        {
            return _impl::from_rep<_value_type>(1);
        }

        CNL_NODISCARD static constexpr _value_type max() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_max());
        }

        CNL_NODISCARD static constexpr _value_type lowest() noexcept
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
