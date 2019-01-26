
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_LITERALS_H)
#define CNL_IMPL_WIDE_INTEGER_LITERALS_H

#include "type.h"
#include "operators.h"
#include "../unreachable.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // known-base integer literal parser

        template<int Base, typename ParseDigit, typename Integer>
        constexpr auto wide_integer_parse(char const* s, ParseDigit parse_digit, Integer const& value) -> Integer
        {
            return *s
                    ? wide_integer_parse<Base>(
                            s+1,
                            parse_digit,
                            Integer{parse_digit(*s)}+value*Base)
                    : value;
        }

        // decimal
        constexpr int parse_dec_char(char c)
        {
            return (c>='0' && c<='9') ? c-'0' : unreachable<int>("invalid decimal digits");
        }

        template<int NumChars>
        constexpr auto decimal_wide_integer_parse(const char (& s)[NumChars])
        -> wide_integer<(NumChars-1)*3322/1000+1>
        {
            using result = wide_integer<(NumChars-1)*3322/1000+1>;
            return result(wide_integer_parse<10>(s, parse_dec_char, typename result::rep{}));
        }

        template<char ... Chars>
        struct wide_integer_parser {
            constexpr auto operator()() const
            -> decltype(decimal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'}))
            {
                return decimal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'});
            }
        };

        // octal
        constexpr int parse_oct_char(char c)
        {
            return (c>='0' && c<='7') ? c-'0' : unreachable<int>("invalid octal digits");
        }

        template<int NumChars>
        constexpr auto octal_wide_integer_parse(const char (& s)[NumChars])
        -> wide_integer<(NumChars-1)*3>
        {
            using result = wide_integer<(NumChars-1)*3>;
            return result(wide_integer_parse<8>(s, parse_oct_char, typename result::rep{}));
        }

        template<char ... Chars>
        struct wide_integer_parser<'0', Chars...> {
            constexpr auto operator()() const
            -> decltype(octal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'}))
            {
                return octal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'});
            }
        };

#if (__cpp_binary_literals >= 201304L)
        // binary
        constexpr int parse_bin_char(char c)
        {
            return (c=='0') ? 0 : (c=='1') ? 1 : unreachable<int>("invalid hexadecimal digits");
        }

        template<int NumChars>
        constexpr auto binary_wide_integer_parse(const char (& s)[NumChars])
        -> wide_integer<NumChars-1>
        {
            using result = wide_integer<NumChars-1>;
            return result(wide_integer_parse<2>(s, parse_bin_char, typename result::rep{}));
        }

        template<char ... Chars>
        struct wide_integer_parser<'0', 'B', Chars...> {
            constexpr auto operator()() const
            -> decltype(binary_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'}))
            {
                return binary_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'});
            }
        };

        template<char ... Chars>
        struct wide_integer_parser<'0', 'b', Chars...> {
            constexpr auto operator()() const
            -> decltype(binary_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'}))
            {
                return binary_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'});
            }
        };
#endif

        // hexadecimal
        constexpr int parse_hex_char(char c)
        {
            return (c>='0' && c<='9') ? c-'0' : (c>='a' && c<='z') ? c+10-'a' : (c>='A' && c<='Z') ? c
                    +10-'A' : unreachable<int>("invalid hexadecimal digits");
        }

        template<int NumChars>
        constexpr auto hexadecimal_wide_integer_parse(const char (& s)[NumChars])
        -> wide_integer<(NumChars-1)*4>
        {
            using result = wide_integer<(NumChars-1)*4>;
            return result(wide_integer_parse<16>(s, parse_hex_char, typename result::rep{}));
        }

        template<char ... Chars>
        struct wide_integer_parser<'0', 'X', Chars...> {
            constexpr auto operator()() const
            -> decltype(hexadecimal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'}))
            {
                return hexadecimal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'});
            }
        };

        template<char ... Chars>
        struct wide_integer_parser<'0', 'x', Chars...> {
            constexpr auto operator()() const
            -> decltype(hexadecimal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'}))
            {
                return hexadecimal_wide_integer_parse<sizeof...(Chars)+1>({Chars..., '\0'});
            }
        };

        // cnl::_impl::operator "" _wide()
        template<char ... Chars>
        constexpr auto operator "" _wide()
        -> decltype(_impl::wide_integer_parser<Chars...>{}())
        {
            return _impl::wide_integer_parser<Chars...>{}();
        }
    }
}

#endif  // CNL_IMPL_WIDE_INTEGER_LITERALS_H
