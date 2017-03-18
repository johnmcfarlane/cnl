
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `sg14::elastic_integer` type

#if !defined(SG14_ELASTIC_INTEGER_H)
#define SG14_ELASTIC_INTEGER_H 1

#if ! defined(SG14_GODBOLT_ORG)
#include <sg14/auxiliary/const_integer.h>
#include <sg14/bits/common.h>
#include <sg14/bits/number_base.h>
#include <sg14/cstdint>
#include <sg14/limits>
#endif

/// study group 14 of the C++ working group
namespace sg14 {

    ////////////////////////////////////////////////////////////////////////////////
    // forward-declaration

    template<int Digits, class Narrowest>
    class elastic_integer;

    ////////////////////////////////////////////////////////////////////////////////
    // traits

    template<int Digits, class Narrowest>
    struct make_signed<elastic_integer<Digits, Narrowest>> {
        using type = elastic_integer<Digits, typename make_signed<Narrowest>::type>;
    };

    template<int Digits, class Narrowest>
    struct make_unsigned<elastic_integer<Digits, Narrowest>> {
        using type = elastic_integer<Digits, typename make_unsigned<Narrowest>::type>;
    };

    template<int Digits, class Narrowest>
    struct width<elastic_integer<Digits, Narrowest>>
            : std::integral_constant<_width_type, Digits+std::numeric_limits<Narrowest>::is_signed> {
    };

    template<int Digits, class Narrowest, _width_type MinNumBits>
    struct set_width<elastic_integer<Digits, Narrowest>, MinNumBits> {
        using type = elastic_integer<MinNumBits-std::numeric_limits<Narrowest>::is_signed, Narrowest>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // implementation details

    namespace _elastic_integer_impl {
        template<int Digits, class Narrowest>
        constexpr _width_type rep_width() noexcept
        {
            return _impl::max(
                    sg14::width<Narrowest>::value,
                    width<elastic_integer<Digits, Narrowest>>::value);
        }

        template<int Digits, class Narrowest>
        using rep = set_width_t<Narrowest, rep_width<Digits, Narrowest>()>;

        template<int Digits, class Narrowest>
        using base = _impl::number_base<
                elastic_integer<Digits, Narrowest>,
                _elastic_integer_impl::rep<Digits, Narrowest>>;
    }

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
    class elastic_integer : public _elastic_integer_impl::base<Digits, Narrowest> {
        using _base = _elastic_integer_impl::base<Digits, Narrowest>;
    public:
        /// alias to template parameter, \a Digits
        static constexpr int digits = Digits;

        /// alias to template parameter, \a Narrowest
        using narrowest = Narrowest;

        /// the actual type used to store the value; closely related to Narrowest but may be a different width
        using rep = typename _base::rep;

        /// common copy constructor
        constexpr elastic_integer(const elastic_integer& rhs)
                :_base(rhs)
        {
        }

        /// construct from integer type
        template<class Number, typename std::enable_if<std::numeric_limits<Number>::is_specialized, int>::type Dummy = 0>
        constexpr elastic_integer(Number n)
                : _base(static_cast<rep>(n))
        {
        }

        /// constructor taking an elastic_integer type
        template<int FromWidth, class FromNarrowest>
        explicit constexpr elastic_integer(const elastic_integer<FromWidth, FromNarrowest>& rhs)
                :_base(rhs)
        {
        }

        /// constructor taking an integral constant
        template<typename Integral, Integral Value, int Exponent>
        constexpr elastic_integer(const_integer<Integral, Value, Digits, Exponent>)
                : _base(Value)
        {
        }

        /// copy assignment operator taking a floating-point type
        template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
        elastic_integer& operator=(S s)
        {
            _base::operator=(floating_point_to_rep(s));
            return *this;
        }

        /// returns value
        template<class S>
        explicit constexpr operator S() const
        {
            return static_cast<S>(to_rep(*this));
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_elastic_integer

    template<
            class Integral, Integral Value>
    constexpr auto make_elastic_integer(const_integer<Integral, Value>)
    -> elastic_integer<_const_integer_impl::num_integer_bits(Value)>
    {
        return elastic_integer<_const_integer_impl::num_integer_bits(Value)>{Value};
    }

    template<class Narrowest = int, class Integral, typename std::enable_if<!is_const_integer<Integral>::value, int>::type Dummy = 0>
    constexpr auto make_elastic_integer(const Integral& value)
    -> decltype(elastic_integer<std::numeric_limits<Integral>::digits, Narrowest>{value})
    {
        return elastic_integer<std::numeric_limits<Integral>::digits, Narrowest>{value};
    }

    namespace _elastic_integer_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_elastic_integer_impl::is_elastic_integer

        template<class ElasticInteger>
        struct is_elastic_integer;

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

    ////////////////////////////////////////////////////////////////////////////////
    // comparison operators

#define SG14_ELASTIC_INTEGER_COMPARISON_OP(OP) \
    template<int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>\
    constexpr auto \
    operator OP (const elastic_integer<LhsDigits, LhsNarrowest>& lhs, const elastic_integer<RhsDigits, RhsNarrowest>& rhs) \
    -> decltype(lhs.data() OP rhs.data()) \
    { \
        return lhs.data() OP rhs.data(); \
    } \
 \
    template<int LhsDigits, class LhsNarrowest, class RhsInteger> \
    constexpr auto operator OP (const elastic_integer<LhsDigits, LhsNarrowest>& lhs, const RhsInteger& rhs) \
    -> typename std::enable_if<std::numeric_limits<RhsInteger>::is_integer, decltype(lhs.data() OP rhs)>::type \
    { \
        return lhs.data() OP rhs; \
    } \
 \
    template<int LhsDigits, class LhsNarrowest, class RhsFloat> \
    constexpr auto operator OP (const elastic_integer<LhsDigits, LhsNarrowest>& lhs, const RhsFloat& rhs) \
    -> typename std::enable_if<std::is_floating_point<RhsFloat>::value, decltype(lhs.data() OP rhs)>::type \
    { \
        return lhs.data() OP rhs; \
    } \
 \
    template<class LhsInteger, int RhsDigits, class RhsNarrowest> \
    constexpr auto operator OP (const LhsInteger& lhs, const elastic_integer<RhsDigits, RhsNarrowest>& rhs) \
    -> typename std::enable_if<std::numeric_limits<LhsInteger>::is_integer, decltype(lhs OP rhs.data())>::type \
    { \
        return lhs OP rhs.data(); \
    } \
 \
    template<class LhsFloat, int RhsDigits, class RhsNarrowest> \
    constexpr auto operator OP (const LhsFloat& lhs, const elastic_integer<RhsDigits, RhsNarrowest>& rhs) \
    -> typename std::enable_if<std::is_floating_point<LhsFloat>::value, decltype(lhs OP rhs.data())>::type \
    { \
        return lhs OP rhs.data(); \
    }

    SG14_ELASTIC_INTEGER_COMPARISON_OP(==);

    SG14_ELASTIC_INTEGER_COMPARISON_OP(!=);

    SG14_ELASTIC_INTEGER_COMPARISON_OP(<);

    SG14_ELASTIC_INTEGER_COMPARISON_OP(>);

    SG14_ELASTIC_INTEGER_COMPARISON_OP(<=);

    SG14_ELASTIC_INTEGER_COMPARISON_OP(>=);

    ////////////////////////////////////////////////////////////////////////////////
    // arithmetic operators

    namespace _elastic_integer_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // policies

        template<class Operation, class LhsTraits, class RhsTraits>
        struct policy;

        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::add_tag, LhsTraits, RhsTraits> {
            static constexpr int digits = _impl::max(LhsTraits::digits, RhsTraits::digits)+1;
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed;
        };

        // TODO: if both operands are unsigned and LhsTraits::digits>=RhsTraits::digits,
        // TODO: then digits=LhsTraits::digits and is_signed=false
        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::subtract_tag, LhsTraits, RhsTraits> {
            static constexpr int digits = _impl::max(LhsTraits::digits, RhsTraits::digits);
            static constexpr bool is_signed = true;
        };

        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::multiply_tag, LhsTraits, RhsTraits> {
            static constexpr int digits = LhsTraits::digits+RhsTraits::digits;
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed;
        };

        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::divide_tag, LhsTraits, RhsTraits> {
            static constexpr int digits = LhsTraits::digits;
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // traits

        template<class Param>
        struct traits {
            static constexpr int digits = std::numeric_limits<Param>::digits;
            static constexpr bool is_signed = std::numeric_limits<Param>::is_signed;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // operate_params

        template<class OperationTag, class Lhs, class Rhs>
        struct operate_params {
            using lhs_traits = traits<Lhs>;
            using rhs_traits = traits<Rhs>;

            using policy = typename _elastic_integer_impl::policy<OperationTag, lhs_traits, rhs_traits>;

            using lhs_rep = typename Lhs::rep;
            using rhs_rep = typename Rhs::rep;
            using rep_result = typename _impl::op_result<OperationTag, lhs_rep, rhs_rep>;

            static constexpr _width_type narrowest_width = _impl::max(width<typename Lhs::narrowest>::value,
                    width<typename Rhs::narrowest>::value);
            using narrowest = set_width_t<_impl::make_signed_t<rep_result, policy::is_signed>, narrowest_width>;
            using result_type = elastic_integer<policy::digits, narrowest>;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_elastic_integer_impl::operate

        template<class OperationTag, class Lhs, class Rhs>
        constexpr auto operate(const Lhs& lhs, const Rhs& rhs)
        -> typename operate_params<OperationTag, Lhs, Rhs>::result_type
        {
            using result_type = typename operate_params<OperationTag, Lhs, Rhs>::result_type;
            return result_type::from_data(
                    static_cast<typename result_type::rep>(_impl::op_fn<OperationTag>(
                            static_cast<result_type>(lhs).data(),
                            static_cast<result_type>(rhs).data())));
        };
    }

    // unary operator-
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator-(const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> elastic_integer<RhsDigits, typename sg14::make_signed<RhsNarrowest>::type>
    {
        using result_type = elastic_integer<RhsDigits, typename sg14::make_signed<RhsNarrowest>::type>;
        return result_type::from_data(-static_cast<result_type>(rhs).data());
    }

    // binary operator+
    template<int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
    constexpr auto
    operator+(const elastic_integer<LhsDigits, LhsNarrowest>& lhs, const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> decltype(_elastic_integer_impl::operate<_impl::add_tag>(lhs, rhs))
    {
        return _elastic_integer_impl::operate<_impl::add_tag>(lhs, rhs);
    }

    // binary operator-
    template<int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
    constexpr auto
    operator-(const elastic_integer<LhsDigits, LhsNarrowest>& lhs, const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> decltype(_elastic_integer_impl::operate<_impl::subtract_tag>(lhs, rhs))
    {
        return _elastic_integer_impl::operate<_impl::subtract_tag>(lhs, rhs);
    }

    // operator*
    template<int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
    constexpr auto
    operator*(const elastic_integer<LhsDigits, LhsNarrowest>& lhs, const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> decltype(_elastic_integer_impl::operate<_impl::multiply_tag>(lhs, rhs))
    {
        return _elastic_integer_impl::operate<_impl::multiply_tag>(lhs, rhs);
    }

    template<int LhsDigits, class LhsNarrowest, class Rhs, typename std::enable_if<!_elastic_integer_impl::is_elastic_integer<Rhs>::value, int>::type Dummy = 0>
    constexpr auto
    operator*(const elastic_integer<LhsDigits, LhsNarrowest>& lhs, const Rhs& rhs)
    -> decltype(lhs*make_elastic_integer(rhs))
    {
        return lhs*make_elastic_integer(rhs);
    }

    template<class Lhs, int RhsDigits, class RhsNarrowest, typename std::enable_if<!_elastic_integer_impl::is_elastic_integer<Lhs>::value, int>::type Dummy = 0>
    constexpr auto
    operator*(const Lhs& lhs, const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> decltype(make_elastic_integer(lhs)*rhs)
    {
        return make_elastic_integer(lhs)*rhs;
    }

    // operator/
    template<int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
    constexpr auto
    operator/(const elastic_integer<LhsDigits, LhsNarrowest>& lhs, const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> decltype(_elastic_integer_impl::operate<_impl::divide_tag>(lhs, rhs))
    {
        return _elastic_integer_impl::operate<_impl::divide_tag>(lhs, rhs);
    }

    template<
        int LhsDigits, class LhsNarrowest,
        class RhsIntegral, RhsIntegral RhsValue>
    constexpr auto
    operator/(const elastic_integer<LhsDigits, LhsNarrowest>& lhs, const const_integer<RhsIntegral, RhsValue>& rhs)
    -> decltype(lhs/make_elastic_integer(rhs))
    {
        return lhs/make_elastic_integer(rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::scale<elastic_integer>

    template<int Digits, class Narrowest>
    struct scale<elastic_integer<Digits, Narrowest>> {
        using Integer = elastic_integer<Digits, Narrowest>;

        constexpr Integer operator()(const Integer& i, int base, int exp) const {
            return Integer{scale<typename Integer::rep>()(i.data(), base, exp)};
        }
    };
}

namespace std {
    template<int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
    struct common_type<sg14::elastic_integer<LhsDigits, LhsNarrowest>, sg14::elastic_integer<RhsDigits, RhsNarrowest>> {
        using type = typename std::conditional<RhsDigits
                <LhsDigits, sg14::elastic_integer<LhsDigits, LhsNarrowest>, sg14::elastic_integer<RhsDigits, RhsNarrowest>>::type;
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

    // note: some members are guessed,
    // some are temporary (assuming rounding style, traps etc.)
    // and some are undefined
    template<int Digits, class Narrowest>
    struct numeric_limits<sg14::elastic_integer<Digits, Narrowest>> : numeric_limits<Narrowest> {
        // elastic integer-specific helpers
        using _narrowest_numeric_limits = numeric_limits<Narrowest>;
        using _value_type = sg14::elastic_integer<Digits, Narrowest>;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;

        // standard members
        static constexpr int digits = Digits;

        static constexpr _value_type lowest() noexcept
        {
            return _value_type::from_data(_rep_numeric_limits::lowest() >> (_rep_numeric_limits::digits-digits));
        }

        static constexpr _value_type min() noexcept
        {
            return _value_type::from_data(1);
        }

        static constexpr _value_type max() noexcept
        {
            return _value_type::from_data(_rep_numeric_limits::max() >> (_rep_numeric_limits::digits-digits));
        }
    };
}

#endif // SG14_ELASTIC_INTEGER_H
