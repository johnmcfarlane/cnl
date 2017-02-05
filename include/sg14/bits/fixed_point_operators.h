
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief `sg14::fixed_point` operators

#if !defined(SG14_FIXED_POINT_OPERATORS_H)
#define SG14_FIXED_POINT_OPERATORS_H 1

#if ! defined(SG14_GODBOLT_ORG)
#include "fixed_point_arithmetic.h"
#endif

/// study group 14 of the C++ working group
namespace sg14 {

    ////////////////////////////////////////////////////////////////////////////////
    // (fixed_point @ fixed_point) comparison operators

    template<class Rep, int Exponent>
    constexpr bool operator==(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()==rhs.data();
    }

    template<class Rep, int Exponent>
    constexpr bool operator!=(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()!=rhs.data();
    }

    template<class Rep, int Exponent>
    constexpr bool operator<(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()<rhs.data();
    }

    template<class Rep, int Exponent>
    constexpr bool operator>(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()>rhs.data();
    }

    template<class Rep, int Exponent>
    constexpr bool operator>=(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()>=rhs.data();
    }

    template<class Rep, int Exponent>
    constexpr bool operator<=(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()<=rhs.data();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // (fixed_point @ fixed_point) arithmetic operators

    // negate
    template<class RhsRep, int RhsExponent>
    constexpr auto operator-(const fixed_point<RhsRep, RhsExponent>& rhs)
    -> fixed_point<decltype(-rhs.data()), RhsExponent>
    {
        using result_type = fixed_point<decltype(-rhs.data()), RhsExponent>;
        return result_type::from_data(-rhs.data());
    }

    // add
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator+(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag, _impl::add_tag>(lhs,
            rhs))
    {
        return _impl::fp::operate<_impl::fp::arithmetic_operator_tag, _impl::add_tag>(lhs,
                rhs);
    }

    // subtract
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator-(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag, _impl::subtract_tag>(
            lhs, rhs))
    {
        return _impl::fp::operate<_impl::fp::arithmetic_operator_tag, _impl::subtract_tag>(
                lhs, rhs);
    }

    // multiply
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator*(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag, _impl::multiply_tag>(
            lhs, rhs))
    {
        return _impl::fp::operate<_impl::fp::arithmetic_operator_tag, _impl::multiply_tag>(
                lhs, rhs);
    }

    // divide
    template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
    constexpr auto operator/(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::division_arithmetic_operator_tag, _impl::divide_tag>(
            lhs, rhs))
    {
        return _impl::fp::operate<_impl::fp::division_arithmetic_operator_tag, _impl::divide_tag>(
                lhs, rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // heterogeneous operator overloads
    //
    // compare two objects of different fixed_point specializations

    template<class Lhs, class Rhs>
    constexpr auto operator==(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)==static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator!=(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)!=static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator<(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)<static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator>(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)>static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator>=(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)>=static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator<=(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)<=static_cast<common_type>(rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // (fixed_point @ non-fixed_point) arithmetic operators

    // fixed-point, integer -> fixed-point
    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = typename std::enable_if<std::numeric_limits<RhsInteger>::is_integer>::type>
    constexpr auto operator+(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs + fixed_point<RhsInteger, 0>{rhs})
    {
        return lhs + fixed_point<RhsInteger, 0>{rhs};
    }

    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = typename std::enable_if<std::numeric_limits<RhsInteger>::is_integer>::type>
    constexpr auto operator-(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs - fixed_point<RhsInteger, 0>{rhs})
    {
        return lhs - fixed_point<RhsInteger, 0>{rhs};
    }

    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = typename std::enable_if<std::numeric_limits<RhsInteger>::is_integer>::type>
    constexpr auto operator*(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs*fixed_point<RhsInteger>(rhs))
    {
        return lhs*fixed_point<RhsInteger>(rhs);
    }

    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = typename std::enable_if<std::numeric_limits<RhsInteger>::is_integer>::type>
    constexpr auto operator/(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs/fixed_point<RhsInteger>{rhs})
    {
        return lhs/fixed_point<RhsInteger>{rhs};
    }

    // integer. fixed-point -> fixed-point
    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = typename std::enable_if<std::numeric_limits<LhsInteger>::is_integer>::type>
    constexpr auto operator+(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger, 0>{lhs} + rhs)
    {
        return fixed_point<LhsInteger, 0>{lhs} + rhs;
    }

    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = typename std::enable_if<std::numeric_limits<LhsInteger>::is_integer>::type>
    constexpr auto operator-(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}-rhs)
    {
        return fixed_point<LhsInteger>{lhs}-rhs;
    }

    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = typename std::enable_if<std::numeric_limits<LhsInteger>::is_integer>::type>
    constexpr auto operator*(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}*rhs)
    {
        return fixed_point<LhsInteger>{lhs}*rhs;
    }

    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = typename std::enable_if<std::numeric_limits<LhsInteger>::is_integer>::type>
    constexpr auto operator/(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}/rhs)
    {
        return fixed_point<LhsInteger>{lhs}/rhs;
    }

    // fixed-point, floating-point -> floating-point
    template<class LhsRep, int LhsExponent, class RhsFloat, typename = typename std::enable_if<std::is_floating_point<RhsFloat>::value>::type>
    constexpr auto operator+(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsFloat& rhs)-> _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)+static_cast<result_type>(rhs);
    }

    template<class LhsRep, int LhsExponent, class RhsFloat, typename = typename std::enable_if<std::is_floating_point<RhsFloat>::value>::type>
    constexpr auto operator-(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsFloat& rhs)-> _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)-static_cast<result_type>(rhs);
    }

    template<class LhsRep, int LhsExponent, class RhsFloat>
    constexpr auto operator*(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const RhsFloat& rhs)
    -> _impl::common_type_t<
            fixed_point<LhsRep, LhsExponent>,
            typename std::enable_if<std::is_floating_point<RhsFloat>::value, RhsFloat>::type>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)*rhs;
    }

    template<class LhsRep, int LhsExponent, class RhsFloat>
    constexpr auto operator/(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const RhsFloat& rhs)
    -> _impl::common_type_t<
            fixed_point<LhsRep, LhsExponent>,
            typename std::enable_if<std::is_floating_point<RhsFloat>::value, RhsFloat>::type>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)/rhs;
    }

    // floating-point, fixed-point -> floating-point
    template<class LhsFloat, class RhsRep, int RhsExponent, typename = typename std::enable_if<std::is_floating_point<LhsFloat>::value>::type>
    constexpr auto operator+(const LhsFloat& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)-> _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>;
        return static_cast<result_type>(lhs)+static_cast<result_type>(rhs);
    }

    template<class LhsFloat, class RhsRep, int RhsExponent, typename = typename std::enable_if<std::is_floating_point<LhsFloat>::value>::type>
    constexpr auto operator-(const LhsFloat& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)-> _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>;
        return static_cast<result_type>(lhs)-static_cast<result_type>(rhs);
    }

    template<class LhsFloat, class RhsRep, int RhsExponent>
    constexpr auto operator*(
            const LhsFloat& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> _impl::common_type_t<
            typename std::enable_if<std::is_floating_point<LhsFloat>::value, LhsFloat>::type,
            fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<fixed_point<RhsRep, RhsExponent>, LhsFloat>;
        return lhs*static_cast<result_type>(rhs);
    }

    template<class LhsFloat, class RhsRep, int RhsExponent>
    constexpr auto operator/(
            const LhsFloat& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> _impl::common_type_t<
            typename std::enable_if<std::is_floating_point<LhsFloat>::value, LhsFloat>::type,
            fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<fixed_point<RhsRep, RhsExponent>, LhsFloat>;
        return lhs/
                static_cast<result_type>(rhs);
    }

    template<class LhsRep, int Exponent, class Rhs>
    fixed_point<LhsRep, Exponent>& operator+=(fixed_point<LhsRep, Exponent>& lhs, const Rhs& rhs)
    {
        return lhs = static_cast<fixed_point<LhsRep, Exponent>>(lhs+rhs);
    }

    template<class LhsRep, int Exponent, class Rhs>
    fixed_point<LhsRep, Exponent>& operator-=(fixed_point<LhsRep, Exponent>& lhs, const Rhs& rhs)
    {
        return lhs = static_cast<fixed_point<LhsRep, Exponent>>(lhs-rhs);
    }

    template<class LhsRep, int Exponent>
    template<class Rhs>
    fixed_point<LhsRep, Exponent>&
    fixed_point<LhsRep, Exponent>::operator*=(const Rhs& rhs)
    {
        _r *= static_cast<rep>(rhs);
        return *this;
    }

    template<class LhsRep, int Exponent>
    template<class Rhs>
    fixed_point<LhsRep, Exponent>&
    fixed_point<LhsRep, Exponent>::operator/=(const Rhs& rhs)
    {
        _r /= static_cast<rep>(rhs);
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // shift operators

    template<class LhsRep, int LhsExponent, class Rhs>
    constexpr fixed_point<LhsRep, LhsExponent>
    operator<<(const fixed_point<LhsRep, LhsExponent>& lhs, const Rhs& rhs)
    {
        return fixed_point<LhsRep, LhsExponent>::from_data(lhs.data() << rhs);
    };

    template<class LhsRep, int LhsExponent, class RhsIntegral, RhsIntegral RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent+RhsValue>
    operator<<(const fixed_point<LhsRep, LhsExponent>& lhs, const_integer<RhsIntegral, RhsValue> const&)
    {
        return fixed_point<LhsRep, LhsExponent+RhsValue>::from_data(lhs.data());
    }

    template<class LhsRep, int LhsExponent, class Rhs>
    constexpr auto
    operator>>(const fixed_point<LhsRep, LhsExponent>& lhs, const Rhs& rhs)
    -> typename std::enable_if<!is_const_integer<Rhs>::value, fixed_point<LhsRep, LhsExponent>>::type
    {
        return fixed_point<LhsRep, LhsExponent>::from_data(lhs.data() >> rhs);
    };

    template<class LhsRep, int LhsExponent, class RhsIntegral, RhsIntegral RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent-static_cast<int>(RhsValue)>
    operator>>(const fixed_point<LhsRep, LhsExponent>& lhs, const_integer<RhsIntegral, RhsValue> const&)
    {
        return fixed_point<LhsRep, LhsExponent-RhsValue>::from_data(lhs.data());
    }
}

#endif	// SG14_FIXED_POINT_OPERATORS_H
