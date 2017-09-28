
//          Copyright John McFarlane 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing definitions equivalent to those in <cstdint>

#ifndef CNL_CNLINT_H
#define CNL_CNLINT_H

#include "bits/config.h"

#include <cstdint>

////////////////////////////////////////////////////////////////////////////////
// CNL_INTMAX_C and CNL_UINTMAX_C

#if defined(CNL_INT128_ENABLED)
#define CNL_INTMAX_C(N) (::cnl::_cnlint_impl::parse(CNL_STR(N)))
#define CNL_UINTMAX_C (::cnl::_cnlint_impl::parse(CNL_STR(N)))
#else
#define CNL_INTMAX_C INTMAX_C
#define CNL_UINTMAX_C UINTMAX_C
#endif

/// compositional numeric library
namespace cnl {

    using int8 = std::int8_t;
    using uint8 = std::uint8_t;
    using int16 = std::int16_t;
    using uint16 = std::uint16_t;
    using int32 = std::int32_t;
    using uint32 = std::uint32_t;
    using int64 = std::int64_t;
    using uint64 = std::uint64_t;

#if defined(CNL_INT128_ENABLED)
    using int128 = __int128;
    using uint128 = unsigned __int128;

    using intmax = int128;
    using uintmax = uint128;
#else
    using intmax = std::intmax_t;
    using uintmax = std::uintmax_t;
#endif  // defined(CNL_INT128_ENABLED)

    namespace _cnlint_impl {
        template<typename ParseDigit>
        constexpr intmax parse(char const* s, int base, ParseDigit parse_digit, intmax value = 0)
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
        constexpr intmax parse(const char (& s)[NumChars])
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
        constexpr intmax parse() {
            return parse<sizeof...(Chars) + 1>({Chars...,'\0'});
        }
    }
}

#endif //CNL_CNLINT_H
