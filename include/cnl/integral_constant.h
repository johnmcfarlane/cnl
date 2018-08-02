#error this file has been replaced with constant.h
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to use of `std::integral_constant` type

#if !defined(CNL_INTEGRAL_CONSTANT_H)
#define CNL_INTEGRAL_CONSTANT_H 1

#include "_impl/common.h"

#include <cstdint>

////////////////////////////////////////////////////////////////////////////////
// CNL_INTMAX_C and CNL_UINTMAX_C

#if defined(CNL_INT128_ENABLED)
#define CNL_INTMAX_C(N) (::cnl::_const_integer_impl::parse(CNL_STR(N)))
#define CNL_UINTMAX_C (::cnl::_const_integer_impl::parse(CNL_STR(N)))
#else
#define CNL_INTMAX_C INTMAX_C
#define CNL_UINTMAX_C UINTMAX_C
#endif

namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_integral_constant type traits

        template<class T>
        struct is_integral_constant : std::false_type {
        };

        template<class Integral, Integral Value>
        struct is_integral_constant<std::integral_constant<Integral, Value>> : std::true_type {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::literals - literal wrapper for std::integral_constant
    //
    // http://codereview.stackexchange.com/a/51576/26421

    namespace _const_integer_impl {

        template<typename ParseDigit>
        constexpr intmax_t parse(char const* s, int base, ParseDigit parse_digit, intmax_t value = 0)
        {
            return (*s) ? parse(s+1, base, parse_digit, parse_digit(*s)+value*base) : value;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // digit parsers

        constexpr int parse_bin_char(char c) {
            return (c == '0') ? 0 : (c == '1') ? 1 : int{};
        }

        constexpr int parse_dec_char(char c) {
            return (c >= '0' && c <= '9') ? c - '0' : int{};
        }

        constexpr int parse_oct_char(char c) {
            return (c >= '0' && c <= '7') ? c - '0' : int{};
        }

        constexpr int parse_hex_char(char c) {
            return (c >= '0' && c <= '9')
                   ? c - '0'
                   : (c >= 'a' && c <= 'z')
                     ? c + 10 - 'a'
                     : (c >= 'A' && c <= 'Z')
                       ? c + 10 - 'A'
                       : int{};
        }

        template<int NumChars>
        constexpr intmax_t parse(const char (& s)[NumChars])
        {
            return (s[0]!='0')
                   ? parse(s, 10, parse_dec_char)
                   : (s[1]=='x' || s[1]=='X')
                     ? parse(s+2, 16, parse_hex_char)
                     : (s[1]=='b' || s[1]=='B')
                       ? parse(s+2, 2, parse_bin_char)
                       : parse(s+1, 8, parse_oct_char);
        }

        template<char... Chars>
        constexpr intmax_t parse() {
            return parse<sizeof...(Chars) + 1>({Chars...,'\0'});
        }
    }

    namespace literals {
        template<char... Chars>
        constexpr auto operator "" _c()
        -> std::integral_constant<intmax_t, _const_integer_impl::parse<Chars...,'\0'>()>
        {
            return {};
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// unary operator-(std::integral_constant)

#if ! defined(_MSC_VER) || (_MSC_VER > 1900)
template<class RhsIntegral, RhsIntegral RhsValue>
constexpr std::integral_constant<decltype(-RhsValue), -RhsValue>
operator-(std::integral_constant<RhsIntegral, RhsValue>) noexcept
{
    return std::integral_constant<decltype(-RhsValue), -RhsValue>{};
}
#endif

#endif  // CNL_INTEGRAL_CONSTANT_H
