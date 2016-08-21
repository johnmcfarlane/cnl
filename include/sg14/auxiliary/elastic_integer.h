
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `sg14::elastic_integer` type

#if !defined(SG14_ELASTIC_INTEGER_H)
#define SG14_ELASTIC_INTEGER_H 1

#include <sg14/type_traits.h>
#include <sg14/bits/common.h>

#include <limits>

/// study group 14 of the C++ working group
namespace sg14 {
    /// \brief literal integer type that encodes its width in bits within its type
    ///
    /// \tparam Digits a count of the number of digits needed to express the number
    /// \tparam Archetype an integer type (typically `signed` or `unsigned`)
    /// that acts as a hint about what integer type should actually be used to represent the value
    ///
    /// \note Arithmetic operations result in types with an adjusted Digits parameter accordingly.
    /// For instance, when two \ref elastic_integer values are multiplied together,
    /// the resultant type has Digits set to the sum of the operands.

    template<int Digits, class Archetype = int>
    class elastic_integer {
    public:
        /// alias to template parameter, \a Digits
        static constexpr int digits = Digits;

        /// alias to template parameter, \a Archetype
        using archetype = Archetype;

        /// width of value
        static constexpr int width = digits+std::numeric_limits<Archetype>::is_signed;

    private:
        static constexpr int _min_width = sg14::width<Archetype>::value;
        static constexpr int _rep_width = _impl::max(_min_width, width);
    public:
        /// the actual type used to store the value; closely related to Archetype but may be a different width
        using rep = set_width_t<Archetype, _rep_width>;

        /// common copy constructor
        template<class S>
        explicit constexpr elastic_integer(S s)
                :_r(static_cast<rep>(s))
        {
        }

        /// constructor taking an elastic_integer type
        template<int FromWidth, class FromArchetype>
        explicit constexpr elastic_integer(const elastic_integer<FromWidth, FromArchetype>& rhs)
                :_r(rhs)
        {
        }

        /// copy assignment operator taking a floating-point type
        template<class S, typename std::enable_if<std::is_floating_point<S>::value, int>::type Dummy = 0>
        elastic_integer& operator=(S s)
        {
            _r = floating_point_to_rep(s);
            return *this;
        }

        /// returns value
        template<class S>
        explicit constexpr operator S() const
        {
            return static_cast<S>(_r);
        }

        /// returns internal representation of value
        constexpr rep data() const
        {
            return _r;
        }

        /// creates an instance given the underlying representation value
        static constexpr elastic_integer from_data(rep r)
        {
            return elastic_integer(r);
        }

    private:
        ////////////////////////////////////////////////////////////////////////////////
        // variables

        rep _r;
    };

    namespace _elastic_integer_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_elastic_integer_impl::is_elastic_integer

        template<class ElasticInteger>
        struct is_elastic_integer;

        template<class ElasticInteger>
        struct is_elastic_integer : std::false_type {
        };

        template<int Digits, class Archetype>
        struct is_elastic_integer<elastic_integer<Digits, Archetype>> : std::true_type {
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

    // operator<<
    template<int LhsDigits, class LhsArchetype, class Rhs>
    constexpr auto operator<<(const elastic_integer<LhsDigits, LhsArchetype>& lhs, const Rhs& rhs)
    -> elastic_integer<LhsDigits, LhsArchetype>
    {
        return elastic_integer<LhsDigits, LhsArchetype>::from_data(lhs.data() << rhs);
    }

    template<class Lhs, int RhsDigits, class RhsArchetype>
    constexpr auto operator<<(const Lhs& lhs, const elastic_integer<RhsDigits, RhsArchetype>& rhs)
    -> decltype(lhs << 0)
    {
        return lhs << rhs.data();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // comparison operators

#define SG14_ELASTIC_INTEGER_COMPARISON_OP(OP) \
    template<int LhsDigits, class LhsArchetype, int RhsDigits, class RhsArchetype>\
    constexpr auto \
    operator OP (const elastic_integer<LhsDigits, LhsArchetype>& lhs, const elastic_integer<RhsDigits, RhsArchetype>& rhs) \
    -> decltype(lhs.data() OP rhs.data()) \
    { \
        return lhs.data() OP rhs.data(); \
    } \
 \
    template<int LhsDigits, class LhsArchetype, class RhsInteger> \
    constexpr auto operator OP (const elastic_integer<LhsDigits, LhsArchetype>& lhs, const RhsInteger& rhs) \
    -> typename std::enable_if<std::numeric_limits<RhsInteger>::is_integer, decltype(lhs.data() OP rhs)>::type \
    { \
        return lhs.data() OP rhs; \
    } \
 \
    template<int LhsDigits, class LhsArchetype, class RhsFloat> \
    constexpr auto operator OP (const elastic_integer<LhsDigits, LhsArchetype>& lhs, const RhsFloat& rhs) \
    -> typename std::enable_if<std::is_floating_point<RhsFloat>::value, decltype(lhs.data() OP rhs)>::type \
    { \
        return lhs.data() OP rhs; \
    } \
 \
    template<class LhsInteger, int RhsDigits, class RhsArchetype> \
    constexpr auto operator OP (const LhsInteger& lhs, const elastic_integer<RhsDigits, RhsArchetype>& rhs) \
    -> typename std::enable_if<std::numeric_limits<LhsInteger>::is_integer, decltype(lhs OP rhs.data())>::type \
    { \
        return lhs OP rhs.data(); \
    } \
 \
    template<class LhsFloat, int RhsDigits, class RhsArchetype> \
    constexpr auto operator OP (const LhsFloat& lhs, const elastic_integer<RhsDigits, RhsArchetype>& rhs) \
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

            // width of result archetype is greater of widths of operand archetypes
            static constexpr _width_type archetype_width = _impl::max(width<typename Lhs::archetype>::value,
                    width<typename Rhs::archetype>::value);
            using archetype = set_width_t<_impl::make_signed_t<rep_result, policy::is_signed>, archetype_width>;
            using result_type = elastic_integer<policy::digits, archetype>;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_elastic_integer_impl::operate

        template<class OperationTag, class Lhs, class Rhs>
        constexpr auto operate(const Lhs& lhs, const Rhs& rhs)
        -> typename operate_params<OperationTag, Lhs, Rhs>::result_type
        {
            using result_type = typename operate_params<OperationTag, Lhs, Rhs>::result_type;
            return result_type::from_data(
                    _impl::op_fn<OperationTag>(
                            static_cast<result_type>(lhs).data(),
                            static_cast<result_type>(rhs).data()));
        };
    }

    // unary operator-
    template<int RhsDigits, class RhsArchetype>
    constexpr auto operator-(const elastic_integer<RhsDigits, RhsArchetype>& rhs)
    -> elastic_integer<RhsDigits, typename sg14::make_signed<RhsArchetype>::type>
    {
        using result_type = elastic_integer<RhsDigits, typename sg14::make_signed<RhsArchetype>::type>;
        return result_type::from_data(-static_cast<result_type>(rhs).data());
    }

    // binary operator+
    template<int LhsDigits, class LhsArchetype, int RhsDigits, class RhsArchetype>
    constexpr auto
    operator+(const elastic_integer<LhsDigits, LhsArchetype>& lhs, const elastic_integer<RhsDigits, RhsArchetype>& rhs)
    -> decltype(_elastic_integer_impl::operate<_impl::add_tag>(lhs, rhs))
    {
        return _elastic_integer_impl::operate<_impl::add_tag>(lhs, rhs);
    }

    // binary operator-
    template<int LhsDigits, class LhsArchetype, int RhsDigits, class RhsArchetype>
    constexpr auto
    operator-(const elastic_integer<LhsDigits, LhsArchetype>& lhs, const elastic_integer<RhsDigits, RhsArchetype>& rhs)
    -> decltype(_elastic_integer_impl::operate<_impl::subtract_tag>(lhs, rhs))
    {
        return _elastic_integer_impl::operate<_impl::subtract_tag>(lhs, rhs);
    }

    // operator*
    template<int LhsDigits, class LhsArchetype, int RhsDigits, class RhsArchetype>
    constexpr auto
    operator*(const elastic_integer<LhsDigits, LhsArchetype>& lhs, const elastic_integer<RhsDigits, RhsArchetype>& rhs)
    -> decltype(_elastic_integer_impl::operate<_impl::multiply_tag>(lhs, rhs))
    {
        return _elastic_integer_impl::operate<_impl::multiply_tag>(lhs, rhs);
    }

    // operator/
    template<int LhsDigits, class LhsArchetype, int RhsDigits, class RhsArchetype>
    constexpr auto
    operator/(const elastic_integer<LhsDigits, LhsArchetype>& lhs, const elastic_integer<RhsDigits, RhsArchetype>& rhs)
    -> decltype(_elastic_integer_impl::operate<_impl::divide_tag>(lhs, rhs))
    {
        return _elastic_integer_impl::operate<_impl::divide_tag>(lhs, rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // traits

    template<int Digits, class Archetype>
    struct is_signed<elastic_integer<Digits, Archetype>> : is_signed<Archetype> {
    };

    template<int Digits, class Archetype>
    struct is_unsigned<elastic_integer<Digits, Archetype>> : is_unsigned<Archetype> {
    };

    template<int Digits, class Archetype>
    struct make_signed<elastic_integer<Digits, Archetype>> {
        using type = elastic_integer<Digits, typename make_signed<Archetype>::type>;
    };

    template<int Digits, class Archetype>
    struct make_unsigned<elastic_integer<Digits, Archetype>> {
        using type = elastic_integer<Digits, typename make_unsigned<Archetype>::type>;
    };

    template<int Digits, class Archetype>
    struct width<elastic_integer<Digits, Archetype>>
            : std::integral_constant<_width_type, elastic_integer<Digits, Archetype>::width> {
    };

    template<int Digits, class Archetype, _width_type MinNumBits>
    struct set_width<elastic_integer<Digits, Archetype>, MinNumBits> {
        using type = elastic_integer<MinNumBits - std::numeric_limits<Archetype>::is_signed, Archetype>;
    };
}

namespace std {
    template<int LhsDigits, class LhsArchetype, int RhsDigits, class RhsArchetype>
    struct common_type<sg14::elastic_integer<LhsDigits, LhsArchetype>, sg14::elastic_integer<RhsDigits, RhsArchetype>> {
        using type = typename std::conditional<RhsDigits
                <LhsDigits, sg14::elastic_integer<LhsDigits, LhsArchetype>, sg14::elastic_integer<RhsDigits, RhsArchetype>>::type;
    };

    template<int LhsDigits, class LhsArchetype, class Rhs>
    struct common_type<sg14::elastic_integer<LhsDigits, LhsArchetype>, Rhs>
            : common_type<sg14::elastic_integer<LhsDigits, LhsArchetype>, sg14::elastic_integer<std::numeric_limits<Rhs>::digits, Rhs>> {
    };

    template<class Lhs, int RhsDigits, class RhsArchetype>
    struct common_type<Lhs, sg14::elastic_integer<RhsDigits, RhsArchetype>>
            : common_type<sg14::elastic_integer<std::numeric_limits<Lhs>::digits, Lhs>, sg14::elastic_integer<RhsDigits, RhsArchetype>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits for sg14::elastic_integer

    // note: some members are guessed,
    // some are temporary (assuming rounding style, traps etc.)
    // and some are undefined
    template<int Digits, class Archetype>
    struct numeric_limits<sg14::elastic_integer<Digits, Archetype>> : numeric_limits<Archetype> {
        // elastic integer-specific helpers
        using _archetype_numeric_limits = numeric_limits<Archetype>;
        using _value_type = sg14::elastic_integer<Digits, Archetype>;
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
