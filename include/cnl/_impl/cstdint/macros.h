
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief integer creation macros equivalent to those in \verbatim<cstdint>\endverbatim

#if !defined(CNL_IMPL_CSTDINT_MACROS_H)
#define CNL_IMPL_CSTDINT_MACROS_H

#include "../common.h"
#include "../config.h"
#include "types.h"

#include <cstdint>

////////////////////////////////////////////////////////////////////////////////
// CNL_INTMAX_C and CNL_UINTMAX_C

#if defined(CNL_INT128_ENABLED)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_INTMAX_C(N) (::cnl::_cnlint_impl::parse(CNL_STR(N)))

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_UINTMAX_C(N) (static_cast<::cnl::uint128>(::cnl::_cnlint_impl::parse(CNL_STR(N))))

#else

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_INTMAX_C INTMAX_C

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_UINTMAX_C UINTMAX_C

#endif

/// compositional numeric library
namespace cnl {

    namespace _cnlint_impl {
        template<typename ParseDigit>
        CNL_NODISCARD constexpr intmax parse(
                char const* s, int base, ParseDigit parse_digit, intmax value = 0)
        {
            return (*s) ? parse(s + 1, base, parse_digit, parse_digit(*s) + value * base) : value;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // digit parsers

        CNL_NODISCARD constexpr int parse_bin_char(char c)
        {
            return (c == '0') ? 0 : (c == '1') ? 1
                                               : int{};
        }

        CNL_NODISCARD constexpr int parse_dec_char(char c)
        {
            return (c >= '0' && c <= '9') ? c - '0' : int{};
        }

        CNL_NODISCARD constexpr int parse_oct_char(char c)
        {
            return (c >= '0' && c <= '7') ? c - '0' : int{};
        }

        CNL_NODISCARD constexpr int parse_hex_char(char c)
        {
            return (c >= '0' && c <= '9') ? c - '0'
                 : (c >= 'a' && c <= 'z') ? c + 10 - 'a'
                 : (c >= 'A' && c <= 'Z') ? c + 10 - 'A'
                                          : int{};
        }

        CNL_NODISCARD constexpr intmax parse_positive(char const* s)
        {
            return (s[0] != '0')                ? parse(s, 10, parse_dec_char)
                 : (s[1] == 'x' || s[1] == 'X') ? parse(s + 2, 16, parse_hex_char)
                 : (s[1] == 'b' || s[1] == 'B') ? parse(s + 2, 2, parse_bin_char)
                                                : parse(s + 1, 8, parse_oct_char);
        }

        template<int NumChars>
        CNL_NODISCARD constexpr intmax parse(
                const char (&s)[NumChars])  // NOLINT(cppcoreguidelines-avoid-c-arrays)
        {
            return (s[0] == '-') ? -parse_positive(s + 1)
                 : s[0] == '+'   ? parse_positive(s + 1)
                                 : parse_positive(s);
        }

        template<char... Chars>
        CNL_NODISCARD constexpr intmax parse()
        {
            return parse<sizeof...(Chars) + 1>({Chars..., '\0'});
        }
    }
}

#endif  // CNL_IMPL_CSTDINT_MACROS_H
