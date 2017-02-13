
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `sg14::const_integer` type

#if !defined(SG14_CONST_INTEGER_H)
#define SG14_CONST_INTEGER_H 1

#if ! defined(SG14_GODBOLT_ORG)
#include <sg14/cstdint>
#include <sg14/limits>
#include <sg14/type_traits>
#endif

#include <climits>

#if defined(SG14_EXCEPTIONS_ENABLED)
#include <stdexcept>
#endif

namespace sg14 {
    namespace _const_integer_impl {

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::num_integer_bits - count of bits being 'used'

        template<class Integer>
        constexpr int num_integer_bits_positive(Integer value, int mask_bits = sizeof(Integer)*CHAR_BIT/2)
        {
            return (value>=(Integer{1} << mask_bits))
                   ? mask_bits+num_integer_bits_positive(value/(Integer{1} << mask_bits), mask_bits)
                   : (mask_bits>1)
                     ? num_integer_bits_positive(value, mask_bits/2)
                     : 1;
        }

#if defined(_MSC_VER)
        template<class Integer, class Enable = void>
        struct _num_integer_bits {
            static constexpr int f(Integer value) {
                return value ? num_integer_bits_positive(value) : 0;
            }
        };

        template<class Integer>
        struct _num_integer_bits<Integer, typename std::enable_if<std::numeric_limits<Integer>::is_signed>::type> {
            static constexpr int f(Integer value) {
                // Most negative number is not exploited;
                // thus negating the result or subtracting it from something else
                // will less likely result in overflow.
                return (value > 0)
                    ? num_integer_bits_positive(value)
                    : (value < 0)
                    ? num_integer_bits_positive(-value)
                    : 0;
            }
        };

        template<class Integer>
        constexpr int num_integer_bits(Integer value)
        {
            return _num_integer_bits<Integer>::f(value);
        }
#else
        template<class Integer>
        constexpr int num_integer_bits(Integer value)
        {
            // Most negative number is not exploited;
            // thus negating the result or subtracting it from something else
            // will less likely result in overflow.
            return (value>0)
                   ? num_integer_bits_positive(value)
                   : (value<0)
                     ? num_integer_bits_positive(-value)
                     : 0;
        }
#endif

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::num_integer_zeros - count of bits 'not' being used

        template<class Integer>
        constexpr int num_integer_zeros_positive(Integer value, int mask_bits = sizeof(Integer)*CHAR_BIT/2)
        {
            return ((value&((Integer{1} << mask_bits)-1)) == 0)
                   ? mask_bits+num_integer_zeros_positive(value/(Integer{1} << mask_bits), mask_bits)
                   : (mask_bits>1)
                     ? num_integer_zeros_positive(value, mask_bits/2)
                     : 0;
        }

#if defined(_MSC_VER)
        template<class Integer, class Enable = void>
        struct _num_integer_zeros {
            static constexpr int f(Integer value) {
                return value ? num_integer_zeros_positive(value) : 0;
            }
        };

        template<class Integer>
        struct _num_integer_zeros<Integer, typename std::enable_if<std::numeric_limits<Integer>::is_signed>::type> {
            static constexpr int f(Integer value) {
                // Most negative number is not exploited;
                // thus negating the result or subtracting it from something else
                // will less likely result in overflow.
                return (value > 0)
                    ? num_integer_zeros_positive(value)
                    : (value < 0)
                    ? num_integer_zeros_positive(-value)
                    : 0;
            }
        };

        template<class Integer>
        constexpr int num_integer_zeros(Integer value)
        {
            return _num_integer_zeros<Integer>::f(value);
        }
#else
        template<class Integer>
        constexpr int num_integer_zeros(Integer value)
        {
            // Most negative number is not exploited;
            // thus negating the result or subtracting it from something else
            // will less likely result in overflow.
            return (value>0)
                   ? num_integer_zeros_positive(value)
                   : (value<0)
                     ? num_integer_zeros_positive(-value)
                     : 0;
        }
#endif

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
            static constexpr intmax_t value = combine(10, 0, parse_dec(Digits)...);
        };

        template<char... Digits>
        struct digits_to_integral<'0', 'x', Digits...> {
            static constexpr intmax_t value = combine(16, 0, parse_hex(Digits)...);
        };

        template<char... Digits>
        struct digits_to_integral<'0', 'X', Digits...> {
            static constexpr intmax_t value = combine(16, 0, parse_hex(Digits)...);
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::const_integer

    // a compile-time-only integer type
    // like a std::integral_constant with arithmetic support
    template<
        class Integral,
        Integral Value,
        int Digits = _const_integer_impl::num_integer_bits(Value),
        int Exponent = _const_integer_impl::num_integer_zeros(Value)>
    class const_integer {
    public:
        using value_type = Integral;
        static constexpr value_type value = Value;

        template<class T>
        constexpr explicit operator T() const { return value; }

        static constexpr int digits = Digits;
        static_assert(
            digits == _const_integer_impl::num_integer_bits(Value),
            "defaulted non-type template parameters should not be specified explicitly");

        static constexpr int exponent = Exponent;
        static_assert(
            exponent == _const_integer_impl::num_integer_zeros(Value),
            "defaulted non-type template parameters should not be specified explicitly");
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::const_integer operator overloads

#if ! defined(_MSC_VER) || (_MSC_VER > 1900)
    template<class RhsIntegral, RhsIntegral RhsValue>
    constexpr const_integer<decltype(-RhsValue), -RhsValue>
    operator-(const_integer<RhsIntegral, RhsValue>) noexcept
    {
        return const_integer<decltype(-RhsValue), -RhsValue>{};
    }
#endif

    template<
        class LhsIntegral, LhsIntegral LhsValue,
        class RhsIntegral, RhsIntegral RhsValue>
    constexpr const_integer<decltype(LhsValue + RhsValue), LhsValue + RhsValue>
    operator+(const_integer<LhsIntegral, LhsValue>, const_integer<RhsIntegral, RhsValue>) noexcept {
        return {};
    }

    template<
        class LhsIntegral, LhsIntegral LhsValue,
        class RhsIntegral, RhsIntegral RhsValue>
    constexpr const_integer<decltype(LhsValue * RhsValue), LhsValue * RhsValue>
    operator*(const_integer<LhsIntegral, LhsValue>, const_integer<RhsIntegral, RhsValue>) noexcept {
        return {};
    }

    template<
        class LhsIntegral, LhsIntegral LhsValue,
        class RhsIntegral, RhsIntegral RhsValue>
    constexpr typename std::conditional<LhsValue==RhsValue, std::true_type, std::false_type>::type
    operator==(const_integer<LhsIntegral, LhsValue>, const_integer<RhsIntegral, RhsValue>) noexcept {
        return {};
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::const_integer type traits

    template<class T>
    struct is_const_integer : std::false_type {};

    template<class Integral, Integral Value>
    struct is_const_integer<const_integer<Integral, Value>> : std::true_type {};

    template<class Integral, Integral Value>
    struct make_unsigned<const_integer<Integral, Value>> {
        using type = const_integer<typename std::make_unsigned<Integral>::type, Value>;
    };

    template<class Integral, Integral Value>
    struct make_signed<const_integer<Integral, Value>> {
        using type = const_integer<typename std::make_signed<Integral>::type, Value>;
    };

    template<class Integral, Integral Value, int Digits>
    struct width<const_integer<Integral, Value, Digits>>
    : std::integral_constant<_width_type, Digits + std::numeric_limits<Integral>::is_signed> {};

    template<class Integral, Integral Value, _width_type MinNumBits>
    struct set_width<const_integer<Integral, Value>, MinNumBits> {
        using type = const_integer<typename sg14::set_width<Integral, MinNumBits>::type, Value>;
    };

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

////////////////////////////////////////////////////////////////////////////////
// std::common_type<const_integer<>, ...>

namespace std {
    template<class Integral, Integral Value, int Ditits, int Zeros, class Rhs>
    struct common_type<sg14::const_integer<Integral, Value, Ditits, Zeros>, Rhs>
        : common_type<Integral, Rhs> {
    };

    template<class Lhs, class Integral, Integral Value, int Ditits, int Zeros>
    struct common_type<Lhs, sg14::const_integer<Integral, Value, Ditits, Zeros>>
        : common_type<Lhs, Integral> {
    };
}

#endif // SG14_CONST_INTEGER_H
