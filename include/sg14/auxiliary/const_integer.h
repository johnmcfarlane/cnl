
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `sg14::const_integer` type

#if !defined(SG14_CONST_INTEGER_H)
#define SG14_CONST_INTEGER_H 1

#include <sg14/auxiliary/numeric.h>
#include <sg14/bits/common.h>

#include <cstdint>

#if defined(SG14_EXCEPTIONS_ENABLED)
#include <stdexcept>
#endif

namespace sg14 {
    namespace _const_integer_impl {

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::digits_to_integral_constant
        //
        // return intrgral_constant given string of digits

        constexpr std::intmax_t combine(int, std::intmax_t p)
        {
            return p;
        }

        template<class... TT>
        constexpr std::intmax_t combine(int base, std::intmax_t val, int p0, TT... pp)
        {
            return combine(base, val * base + p0, pp...);
        }

        constexpr int parse_dec(char C)
        {
#if defined(SG14_EXCEPTIONS_ENABLED)
            return (C>='0' && C<='9')
                   ? C-'0'
                   : throw std::out_of_range("only decimal digits are allowed");
#else
            return C-'0';
#endif
        }

        constexpr int parse_hex(char C) {
            return (C >= '0' && C <= '9')
                   ? C - '0'
                   : (C >= 'a' && C <= 'f')
                     ? C - 'a'
                     : (C >= 'A' && C <= 'F')
                       ? C - 'A'
#if defined(SG14_EXCEPTIONS_ENABLED)
                       : throw std::out_of_range("only decimal digits are allowed")
#else
                : 0
#endif
                    ;
        }

        template<char... Digits>
        struct digits_to_integral {
            static constexpr std::intmax_t value = combine(10, 0, parse_dec(Digits)...);
        };

        template<char... Digits>
        struct digits_to_integral<'0', 'x', Digits...> {
            static constexpr std::intmax_t value = combine(16, 0, parse_hex(Digits)...);
        };

        template<char... Digits>
        struct digits_to_integral<'0', 'X', Digits...> {
            static constexpr std::intmax_t value = combine(16, 0, parse_hex(Digits)...);
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::const_integer

    /// \brief a compile-time-only integer type like a std::integral_constant with arithmetic support
    ///
    /// \tparam Integral the type of Value
    /// \tparam Value the integer number to be represented
    /// \tparam Digits automatically calculated value (leave defaulted)
    /// \tparam Exponent automatically calculated value (leave defaulted)

    template<
        class Integral,
        Integral Value,
        int Digits = used_bits(Value),
        int Exponent = trailing_bits(Value)>
    class const_integer {
    public:
        using value_type = Integral;
        static constexpr value_type value = Value;

        template<class T>
        constexpr explicit operator T() const { return value; }

        static constexpr int digits = Digits;
        static_assert(
            digits == used_bits(Value),
            "defaulted non-type template parameters should not be specified explicitly");

        static constexpr int exponent = Exponent;
        static_assert(
            exponent == trailing_bits(Value),
            "defaulted non-type template parameters should not be specified explicitly");
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::const_integer arithmetic operator overloads

    namespace _const_integer_impl {
        template<class Lhs, class Rhs, class Type>
        struct enable_if_op;

        template<
                class LhsIntegral, LhsIntegral LhsValue, int LhsDigits, int LhsExponent,
                class RhsIntegral, RhsIntegral RhsValue, int RhsDigits, int RhsExponent,
                class Type>
        struct enable_if_op<
                const_integer<LhsIntegral, LhsValue, LhsDigits, LhsExponent>,
                const_integer<RhsIntegral, RhsValue, RhsDigits, RhsExponent>,
                Type> {
            using type = Type;
        };

        template<
                class LhsIntegral, LhsIntegral LhsValue, int LhsDigits, int LhsExponent,
                class Rhs,
                class Type>
        struct enable_if_op<
                const_integer<LhsIntegral, LhsValue, LhsDigits, LhsExponent>,
                Rhs,
                Type> {
            using type = Type;
        };

        template<
                class Lhs,
                class RhsIntegral, RhsIntegral RhsValue, int RhsDigits, int RhsExponent,
                class Type>
        struct enable_if_op<
                Lhs,
                const_integer<RhsIntegral, RhsValue, RhsDigits, RhsExponent>,
                Type> {
            using type = Type;
        };

        // Lhs OP const_integer
        template<
                class Operator,
                class Lhs,
                class RhsIntegral, RhsIntegral RhsValue, int RhsDigits, int RhsExponent,
                class = _impl::enable_if_t<std::is_integral<Lhs>::value>>
        constexpr auto operate(
                const Lhs& lhs,
                const const_integer<RhsIntegral, RhsValue, RhsDigits, RhsExponent>&,
                Operator op)
        -> decltype(op(lhs, RhsValue)) {
            return op(lhs, RhsValue);
        }

        // const_integer OP Lhs
        template<
                class Operator,
                class LhsIntegral, LhsIntegral LhsValue, int LhsDigits, int LhsExponent,
                class Rhs,
                class = _impl::enable_if_t<std::is_integral<Rhs>::value, int>>
        constexpr auto operate(
                const const_integer<LhsIntegral, LhsValue, LhsDigits, LhsExponent>&,
                const Rhs& rhs,
                Operator op)
        -> decltype(op(LhsValue, rhs)) {
            return op(LhsValue, rhs);
        }

        // const_integer OP const_integer
        template<
                class Operator,
                class LhsIntegral, LhsIntegral LhsValue, int LhsDigits, int LhsExponent,
                class RhsIntegral, RhsIntegral RhsValue, int RhsDigits, int RhsExponent>
        constexpr auto operate(
                const const_integer<LhsIntegral, LhsValue, LhsDigits, LhsExponent>&,
                const const_integer<RhsIntegral, RhsValue, RhsDigits, RhsExponent>&,
                Operator)
        -> decltype(const_integer<_impl::op_result<Operator, LhsIntegral, RhsIntegral>, Operator()(LhsValue, RhsValue)>{}) {
            return const_integer<_impl::op_result<Operator, LhsIntegral, RhsIntegral>, Operator()(LhsValue, RhsValue)>{};
        }
    }

    template<class Lhs, class Rhs, typename _const_integer_impl::enable_if_op<Lhs, Rhs, int>::type dummy = 0>
    constexpr auto operator+(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_const_integer_impl::operate(lhs, rhs, _impl::add_tag))
    {
        return _const_integer_impl::operate(lhs, rhs, _impl::add_tag);
    }

    template<class Lhs, class Rhs, typename _const_integer_impl::enable_if_op<Lhs, Rhs, int>::type dummy = 0>
    constexpr auto operator-(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_const_integer_impl::operate(lhs, rhs, _impl::subtract_tag))
    {
        return _const_integer_impl::operate(lhs, rhs, _impl::subtract_tag);
    }

    template<class Lhs, class Rhs, typename _const_integer_impl::enable_if_op<Lhs, Rhs, int>::type dummy = 0> 
    constexpr auto operator*(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_const_integer_impl::operate(lhs, rhs, _impl::multiply_tag))
    {
        return _const_integer_impl::operate(lhs, rhs, _impl::multiply_tag);
    }

    template<class Lhs, class Rhs, typename _const_integer_impl::enable_if_op<Lhs, Rhs, int>::type dummy = 0>
    constexpr auto operator/(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_const_integer_impl::operate(lhs, rhs, _impl::divide_tag))
    {
        return _const_integer_impl::operate(lhs, rhs, _impl::divide_tag);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::const_integer comparison operator overloads

    namespace _const_integer_impl {
        // const_integer OP const_integer
        template<
                class Operator,
                class LhsIntegral, LhsIntegral LhsValue, int LhsDigits, int LhsExponent,
                class RhsIntegral, RhsIntegral RhsValue, int RhsDigits, int RhsExponent>
        constexpr auto compare(
                const const_integer<LhsIntegral, LhsValue, LhsDigits, LhsExponent>&,
                const const_integer<RhsIntegral, RhsValue, RhsDigits, RhsExponent>&,
                Operator op)
        -> decltype(op(LhsValue, RhsValue))
        {
            return op(LhsValue, RhsValue);
        }
    }
    
    template<class Lhs, class Rhs, typename _const_integer_impl::enable_if_op<Lhs, Rhs, int>::type dummy = 0>
    constexpr auto operator==(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_const_integer_impl::compare(lhs, rhs, _impl::equal_tag))
    {
        return _const_integer_impl::compare(lhs, rhs, _impl::equal_tag);
    }

#if ! defined(_MSC_VER) || (_MSC_VER > 1900)
    template<class RhsIntegral, RhsIntegral RhsValue>
    constexpr const_integer<decltype(-RhsValue), -RhsValue>
    operator-(const_integer<RhsIntegral, RhsValue>) noexcept
    {
        return const_integer<decltype(-RhsValue), -RhsValue>{};
    }
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::const_integer type traits

    template<class T>
    struct is_const_integer : std::false_type {};

    template<class Integral, Integral Value>
    struct is_const_integer<const_integer<Integral, Value>> : std::true_type {};

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::literals - literal wrapper for std::integral_constant
    //
    // http://codereview.stackexchange.com/a/51576/26421

    namespace literals {
        template<char... Digits>
        constexpr auto operator "" _c()
        -> const_integer<std::intmax_t, _const_integer_impl::digits_to_integral<Digits...>::value>
        {
            return {};
        }
    }
}
namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::common_type<const_integer<>, ...>
    
    template<class Integral, Integral Value, int Digits, int Zeros, class Rhs>
    struct common_type<sg14::const_integer<Integral, Value, Digits, Zeros>, Rhs>
        : common_type<Integral, Rhs> {
    };

    template<class Lhs, class Integral, Integral Value, int Digits, int Zeros>
    struct common_type<Lhs, sg14::const_integer<Integral, Value, Digits, Zeros>>
        : common_type<Lhs, Integral> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits<const_integer<>>

    template<class Integral, Integral Value, int Digits, int Zeros>
    struct numeric_limits<sg14::const_integer<Integral, Value, Digits, Zeros>>
    : numeric_limits<Integral> {
        /// alias to template parameter, \a Digits
        static constexpr int digits = Digits;
    };
}

#endif // SG14_CONST_INTEGER_H
