
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_LITERALS_H)
#define CNL_IMPL_WIDE_INTEGER_LITERALS_H

#include "../num_traits/rep.h"
#include "../unreachable.h"
#include "definition.h"
#include "generic.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // known-base integer literal parser

        template<int Base, typename ParseDigit, typename Integer>
        CNL_NODISCARD constexpr auto
        wide_integer_parse(char const* s, ParseDigit parse_digit, Integer const& value) -> Integer
        {
            return *s
                    ? wide_integer_parse<Base>(
                            s+1,
                            parse_digit,
                            Integer{parse_digit(*s)}+value*Base)
                    : value;
        }

        // decimal
        CNL_NODISCARD constexpr int parse_dec_char(char c)
        {
            return (c>='0' && c<='9') ? c-'0' : unreachable<int>("invalid decimal digits");
        }

        template<int NumChars>
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays)
        CNL_NODISCARD constexpr auto decimal_wide_integer_parse(const char (& s)[NumChars])
        -> wide_integer<(NumChars-1)*3322/1000+1>
        {
            using result = wide_integer<(NumChars-1)*3322/1000+1>;
            return result(wide_integer_parse<10>(s, parse_dec_char, rep_t<result>{}));
        }

        template<char ... Chars>
        struct wide_integer_parser {
            CNL_NODISCARD constexpr auto operator()() const
            -> decltype(decimal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'}))
            {
                return decimal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'});
            }
        };

        // octal
        CNL_NODISCARD constexpr int parse_oct_char(char c)
        {
            return (c>='0' && c<='7') ? c-'0' : unreachable<int>("invalid octal digits");
        }

        template<int NumChars>
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays)
        CNL_NODISCARD constexpr auto octal_wide_integer_parse(const char (& s)[NumChars])
        -> wide_integer<(NumChars-1)*3>
        {
            using result = wide_integer<(NumChars-1)*3>;
            return result(wide_integer_parse<8>(s, parse_oct_char, rep_t<result>{}));
        }

        template<char ... Chars>
        struct wide_integer_parser<'0', Chars...> {
            CNL_NODISCARD constexpr auto operator()() const
            -> decltype(octal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'}))
            {
                return octal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'});
            }
        };

        // binary
        CNL_NODISCARD constexpr int parse_bin_char(char c)
        {
            return (c=='0') ? 0 : (c=='1') ? 1 : unreachable<int>("invalid hexadecimal digits");
        }

        template<int NumChars>
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays)
        CNL_NODISCARD constexpr auto binary_wide_integer_parse(const char (& s)[NumChars])
        -> wide_integer<NumChars-1>
        {
            using result = wide_integer<NumChars-1>;
            return result(wide_integer_parse<2>(s, parse_bin_char, rep_t<result>{}));
        }

        template<char ... Chars>
        struct wide_integer_parser<'0', 'B', Chars...> {
            CNL_NODISCARD constexpr auto operator()() const
            -> decltype(binary_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'}))
            {
                return binary_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'});
            }
        };

        template<char ... Chars>
        struct wide_integer_parser<'0', 'b', Chars...> {
            CNL_NODISCARD constexpr auto operator()() const
            -> decltype(binary_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'}))
            {
                return binary_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'});
            }
        };

        // hexadecimal
        CNL_NODISCARD constexpr int parse_hex_char(char c)
        {
            return (c>='0' && c<='9') ? c-'0' : (c>='a' && c<='z') ? c+10-'a' : (c>='A' && c<='Z') ? c
                    +10-'A' : unreachable<int>("invalid hexadecimal digits");
        }

        template<int NumChars>
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays)
        CNL_NODISCARD constexpr auto hexadecimal_wide_integer_parse(const char (& s)[NumChars])
        -> wide_integer<(NumChars-1)*4>
        {
            using result = wide_integer<(NumChars-1)*4>;
            return result(wide_integer_parse<16>(s, parse_hex_char, rep_t<result>{}));
        }

        template<char ... Chars>
        struct wide_integer_parser<'0', 'X', Chars...> {
            CNL_NODISCARD constexpr auto operator()() const
            -> decltype(hexadecimal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'}))
            {
                return hexadecimal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'});
            }
        };

        template<char ... Chars>
        struct wide_integer_parser<'0', 'x', Chars...> {
            CNL_NODISCARD constexpr auto operator()() const
            -> decltype(hexadecimal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'}))
            {
                return hexadecimal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'});
            }
        };
    }

    namespace literals {
        // cnl::_impl::operator "" _wide()
        template<char ... Chars>
        CNL_NODISCARD constexpr auto operator "" _wide()
        -> decltype(_impl::wide_integer_parser<Chars...>{}())
        {
            return _impl::wide_integer_parser<Chars...>{}();
        }
    }
}

#endif  // CNL_IMPL_WIDE_INTEGER_LITERALS_H
