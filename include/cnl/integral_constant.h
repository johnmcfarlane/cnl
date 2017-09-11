
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to use of `std::integral_constant` type

#if !defined(CNL_INTEGRAL_CONSTANT_H)
#define CNL_INTEGRAL_CONSTANT_H 1

#include <cnl/numeric.h>
#include <cnl/bits/common.h>

#include <cstdint>

#if defined(CNL_EXCEPTIONS_ENABLED)
#include <stdexcept>
#endif

namespace cnl {
    namespace _const_integer_impl {

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::digits_to_integral_constant
        //
        // return intrgral_constant given string of digits

        constexpr std::intmax_t combine(int, std::intmax_t p) {
            return p;
        }

        template<class... TT>
        constexpr std::intmax_t combine(int base, std::intmax_t val, int p0, TT... pp) {
            return combine(base, val * base + p0, pp...);
        }

        constexpr int parse_dec(char C) {
#if defined(CNL_EXCEPTIONS_ENABLED)
            return (C >= '0' && C <= '9')
                   ? C - '0'
                   : throw std::out_of_range("only decimal digits are allowed");
#else
            return C-'0';
#endif
        }

        constexpr int parse_hex(char C) {
            return (C >= '0' && C <= '9')
                   ? C - '0'
                   : (C >= 'a' && C <= 'f')
                     ? C - 'a' + 10
                     : (C >= 'A' && C <= 'F')
                       ? C - 'A' + 10
#if defined(CNL_EXCEPTIONS_ENABLED)
                       : throw std::out_of_range("only hexadecimal digits are allowed")
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

    namespace literals {
        template<char... Digits>
        constexpr auto operator "" _c()
        -> std::integral_constant<std::intmax_t, _const_integer_impl::digits_to_integral<Digits...>::value> {
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
