
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `sg14::elastic_integer` type

#if !defined(SG14_ELASTIC_INTEGER_H)
#define SG14_ELASTIC_INTEGER_H 1

#include <sg14/bits/number_base.h>

/// study group 14 of the C++ working group
namespace sg14 {

    ////////////////////////////////////////////////////////////////////////////////
    // forward-declaration

    template<int Digits, class Narrowest>
    class elastic_integer;

    ////////////////////////////////////////////////////////////////////////////////
    // numeric traits

    namespace _elastic_integer_impl {
        template<int Digits, class Narrowest>
        struct base_class {
            static constexpr _digits_type digits = Digits;

            static constexpr _digits_type rep_digits = _impl::max(sg14::digits<Narrowest>::value, digits);

            using rep = typename set_digits<Narrowest, rep_digits>::type;
            using type = _impl::number_base<elastic_integer<Digits, Narrowest>, rep>;
        };

        template<int Digits, class Narrowest>
        using base_class_t = typename base_class<Digits, Narrowest>::type;
    }

    template<int Digits, class Narrowest>
    struct digits<elastic_integer<Digits, Narrowest>> : std::integral_constant<_digits_type, Digits> {
        static constexpr _digits_type value = Digits;
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

    template<int Digits, class Narrowest, class Value>
    struct from_value<elastic_integer<Digits, Narrowest>, Value> {
        using type = elastic_integer<sg14::digits<Value>::value, sg14::_impl::make_signed_t<Narrowest, sg14::is_signed<Value>::value>>;
    };

    template<int Digits, class Narrowest>
    struct scale<elastic_integer<Digits, Narrowest>> {
        using _value_type = elastic_integer<Digits, Narrowest>;

        constexpr _value_type operator()(const _value_type& i, int base, int exp) const {
            using _rep = typename _value_type::rep;
            return _value_type{ _impl::scale(i.data(), base, exp) };
        }
    };

    /// \brief literal integer type that encodes its width in bits within its type
    ///
    /// \tparam Digits a count of the number of digits needed to express the number
    /// \tparam Narrowest the most narrow integer type to use for storage
    ///
    /// \note Arithmetic operations result in types with an adjusted Digits parameter accordingly.
    /// For instance, when two \ref elastic_integer values are multiplied together,
    /// the resultant type has Digits set to the sum of the operands.
    ///
    /// \sa elastic_fixed_point

    template<int Digits, class Narrowest = int>
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
        constexpr elastic_integer(const elastic_integer& rhs)
                :_base(rhs)
        {
        }

        /// construct from numeric type
        template<class Number, _impl::enable_if_t<std::numeric_limits<Number>::is_specialized, int> Dummy = 0>
        constexpr elastic_integer(Number n)
                : _base(static_cast<rep>(n))
        {
        }

        /// constructor taking an elastic_integer type
        template<int FromWidth, class FromNarrowest>
        explicit constexpr elastic_integer(const elastic_integer<FromWidth, FromNarrowest>& rhs)
                :_base(static_cast<rep>(rhs.data()))
        {
        }

        /// constructor taking an integral constant
        template<class Integral, Integral Value, int Exponent>
        constexpr elastic_integer(const_integer<Integral, Value, Digits, Exponent>)
                : _base(static_cast<rep>(Value))
        {
            static_assert(!sg14::is_signed<Integral>::value || sg14::is_signed<rep>::value, "initialization by out-of-range value");
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
            return static_cast<S>(_base::data());
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_elastic_integer

    template<
            class Integral, Integral Value>
    constexpr auto make_elastic_integer(const_integer<Integral, Value>)
    -> elastic_integer<used_bits(Value)>
    {
        return elastic_integer<used_bits(Value)>{Value};
    }

    template<class Narrowest = int, class Integral, _impl::enable_if_t<!is_const_integer<Integral>::value, int> Dummy = 0>
    constexpr auto make_elastic_integer(const Integral& value)
    -> decltype(elastic_integer<std::numeric_limits<Integral>::digits, Narrowest>{value})
    {
        return elastic_integer<std::numeric_limits<Integral>::digits, Narrowest>{value};
    }

    namespace _elastic_integer_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_elastic_integer_impl::is_elastic_integer

        template<class ElasticInteger>
        struct is_elastic_integer : std::false_type {
        };

        template<int Digits, class Narrowest>
        struct is_elastic_integer<elastic_integer<Digits, Narrowest>> : std::true_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_elastic_integer_impl::are_integer_class_operands - basically identifies
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
    constexpr auto operator~(const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> elastic_integer<RhsDigits, RhsNarrowest>
    {
        using elastic_integer = elastic_integer<RhsDigits, RhsNarrowest>;
        using rep = typename elastic_integer::rep;
        return elastic_integer::from_data(
            static_cast<rep>(
                rhs.data()
                ^ ((static_cast<rep>(~0)) >> (std::numeric_limits<rep>::digits - RhsDigits))));
    }

    // operator<<
    template<int LhsDigits, class LhsNarrowest, class Rhs>
    constexpr auto operator<<(const elastic_integer<LhsDigits, LhsNarrowest>& lhs, const Rhs& rhs)
    -> elastic_integer<LhsDigits, LhsNarrowest>
    {
        return elastic_integer<LhsDigits, LhsNarrowest>::from_data(lhs.data() << rhs);
    }

    template<class Lhs, int RhsDigits, class RhsNarrowest>
    constexpr auto operator<<(const Lhs& lhs, const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> decltype(lhs << 0)
    {
        return lhs << rhs.data();
    }

    // operator>>
    template<int LhsDigits, class LhsNarrowest, class Rhs>
    constexpr auto operator>>(const elastic_integer<LhsDigits, LhsNarrowest>& lhs, const Rhs& rhs)
    -> elastic_integer<LhsDigits, LhsNarrowest>
    {
        return elastic_integer<LhsDigits, LhsNarrowest>::from_data(lhs.data() >> rhs);
    }

    template<class Lhs, int RhsDigits, class RhsNarrowest>
    constexpr auto operator>>(const Lhs& lhs, const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> decltype(lhs >> 0)
    {
        return lhs >> rhs.data();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // comparison operators

    namespace _impl {
        template<int FromDigits, class FromNarrowest, int OtherDigits, class OtherNarrowest,
                _impl::enable_if_t<FromDigits!=OtherDigits || !std::is_same<FromNarrowest, OtherNarrowest>::value, std::nullptr_t> Dummy = nullptr>
        constexpr auto cast_to_common_type(
                const elastic_integer<FromDigits, FromNarrowest>& from,
                const elastic_integer<OtherDigits, OtherNarrowest>&)
        -> decltype(static_cast<_impl::common_type_t<
                elastic_integer<FromDigits, FromNarrowest>,
                elastic_integer<OtherDigits, OtherNarrowest>>>(from)) {
            return static_cast<_impl::common_type_t<
                    elastic_integer<FromDigits, FromNarrowest>,
                    elastic_integer<OtherDigits, OtherNarrowest>>>(from);
        };

        template<class Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest,
#if defined(__GNUG__)
        bool Enable = Operator::is_comparison>
#else
        enable_if_t<Operator::is_comparison>...>
#endif
        constexpr auto operate(
                const elastic_integer<LhsDigits, LhsNarrowest>& lhs,
                const elastic_integer<RhsDigits, RhsNarrowest>& rhs,
                Operator op)
        -> decltype(op(cast_to_common_type(lhs, rhs), cast_to_common_type(rhs, lhs)))
        {
            return op(cast_to_common_type(lhs, rhs), cast_to_common_type(rhs, lhs));
        }

        template<class Operator, int Digits, class Narrowest,
                class = enable_if_t<Operator::is_comparison>>
        constexpr auto
        operate(const elastic_integer<Digits, Narrowest>& lhs, const elastic_integer<Digits, Narrowest>& rhs, Operator op)
        -> decltype(op(lhs.data(), rhs.data()))
        {
            return op(lhs.data(), rhs.data());
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // arithmetic operators

    namespace _impl {
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

        ////////////////////////////////////////////////////////////////////////////////
        // operate_params

        template<class OperationTag, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest,
                class = enable_if_t<OperationTag::is_arithmetic>>
        struct operate_params {
            using lhs = elastic_integer<LhsDigits, LhsNarrowest>;
            using rhs = elastic_integer<RhsDigits, RhsNarrowest>;
            using lhs_traits = std::numeric_limits<lhs>;
            using rhs_traits = std::numeric_limits<rhs>;

            using policy = typename _impl::policy<OperationTag, lhs_traits, rhs_traits>;

            using lhs_rep = typename lhs::rep;
            using rhs_rep = typename rhs::rep;
            using rep_result = typename _impl::op_result<OperationTag, lhs_rep, rhs_rep>;

            static constexpr _digits_type narrowest_width = _impl::max(
                    digits<LhsNarrowest>::value + sg14::is_signed<LhsNarrowest>::value,
                    digits<RhsNarrowest>::value + sg14::is_signed<RhsNarrowest>::value);
            using narrowest = set_digits_t<_impl::make_signed_t<rep_result, policy::is_signed>, narrowest_width-policy::is_signed>;
            using result_type = elastic_integer<policy::digits, narrowest>;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::operate

        template<class Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest,
#if defined(__GNUG__)
                bool Enable = Operator::is_arithmetic>
#else
                enable_if_t<Operator::is_arithmetic>...>
#endif
        constexpr auto operate(
                const elastic_integer<LhsDigits, LhsNarrowest>& lhs,
                const elastic_integer<RhsDigits, RhsNarrowest>& rhs,
                Operator op)
        -> typename operate_params<Operator, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::result_type
        {
            using result_type = typename operate_params<Operator, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::result_type;
            return result_type::from_data(
                    static_cast<typename result_type::rep>(op(
                            static_cast<result_type>(lhs).data(),
                            static_cast<result_type>(rhs).data())));
        }
    }

    // unary operator-
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator-(const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>
    {
        using result_type = elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>;
        return result_type::from_data(-static_cast<result_type>(rhs).data());
    }

    // unary operator+
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator+(const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>
    {
        using result_type = elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>;
        return result_type::from_data(static_cast<result_type>(rhs).data());
    }
}

namespace std {
    template<int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
    struct common_type<sg14::elastic_integer<LhsDigits, LhsNarrowest>, sg14::elastic_integer<RhsDigits, RhsNarrowest>> {
        using type = sg14::elastic_integer<
                sg14::_impl::max(LhsDigits, RhsDigits), 
                sg14::_impl::common_signedness_t<LhsNarrowest, RhsNarrowest>>;
    };

    template<int LhsDigits, class LhsNarrowest, class Rhs>
    struct common_type<sg14::elastic_integer<LhsDigits, LhsNarrowest>, Rhs>
            : common_type<sg14::elastic_integer<LhsDigits, LhsNarrowest>, sg14::elastic_integer<std::numeric_limits<Rhs>::digits, Rhs>> {
    };

    template<class Lhs, int RhsDigits, class RhsNarrowest>
    struct common_type<Lhs, sg14::elastic_integer<RhsDigits, RhsNarrowest>>
            : common_type<sg14::elastic_integer<std::numeric_limits<Lhs>::digits, Lhs>, sg14::elastic_integer<RhsDigits, RhsNarrowest>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits for sg14::elastic_integer

    namespace _elastic_integer_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_elastic_integer_impl::lowest

        // helper for std::numeric_limits<sg14::elastic_integer<>>::lowest()
        template<class Rep, bool IsSigned>
        struct lowest;

        template<class Rep>
        struct lowest<Rep, true> {
            constexpr Rep operator()(const Rep& max) const noexcept
            {
                return -max;
            }
        };

        template<class Rep>
        struct lowest<Rep, false> {
            constexpr Rep operator()(const Rep&) const noexcept
            {
                return 0;
            }
        };
    };

    template<int Digits, class Narrowest>
    struct numeric_limits<sg14::elastic_integer<Digits, Narrowest>>
            : numeric_limits<Narrowest> {
        // elastic integer-specific helpers
        using _narrowest_numeric_limits = numeric_limits<Narrowest>;
        using _value_type = sg14::elastic_integer<Digits, Narrowest>;
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
            return _value_type::from_data(1);
        }

        static constexpr _value_type max() noexcept
        {
            return _value_type::from_data(_rep_max());
        }

        static constexpr _value_type lowest() noexcept
        {
            return _elastic_integer_impl::lowest<_rep, _narrowest_numeric_limits::is_signed>()(_rep_max());
        }
    };
}

#endif // SG14_ELASTIC_INTEGER_H
