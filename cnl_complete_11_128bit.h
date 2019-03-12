
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// mechanically retrieved, single-header version of CNL library
// https://github.com/johnmcfarlane/cnl

#if ! defined(CNL_COMPLETE_H)
#define CNL_COMPLETE_H

#if (__cplusplus == 199711L) && defined(_MSC_VER)
#error Required Visual C++ compiler flags: /std:c++11 /Zc:__cplusplus /EHsc
#endif

#if (__cplusplus < 201103L)
#error This build of CNL requires C++11 or above.
#endif
#include <array>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <functional>
#include <istream>
#include <iterator>
#include <limits>
#include <numeric>
#include <ostream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <utility>
namespace cnl {
    namespace _impl {
        template<class T>
        constexpr T max(T a, T b)
        {
            return (a<b) ? b : a;
        }
        template<class T>
        constexpr T min(T a, T b)
        {
            return (a<b) ? a : b;
        }
    }
}
namespace cnl {
    using int8 = std::int8_t;
    using uint8 = std::uint8_t;
    using int16 = std::int16_t;
    using uint16 = std::uint16_t;
    using int32 = std::int32_t;
    using uint32 = std::uint32_t;
    using int64 = std::int64_t;
    using uint64 = std::uint64_t;
    using int128 = __int128;
    using uint128 = unsigned __int128;
    using intmax = int128;
    using uintmax = uint128;
    namespace _cnlint_impl {
        template<typename ParseDigit>
        constexpr intmax parse(char const* s, int base, ParseDigit parse_digit, intmax value = 0)
        {
            return (*s) ? parse(s+1, base, parse_digit, parse_digit(*s)+value*base) : value;
        }
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
        constexpr intmax parse_positive(char const* s)
        {
            return (s[0]!='0')
                   ? parse(s, 10, parse_dec_char)
                   : (s[1]=='x' || s[1]=='X')
                     ? parse(s+2, 16, parse_hex_char)
                     : (s[1]=='b' || s[1]=='B')
                       ? parse(s+2, 2, parse_bin_char)
                       : parse(s+1, 8, parse_oct_char);
        }
        template<int NumChars>
        constexpr intmax parse(const char (& s)[NumChars])
        {
            return (s[0]=='-')
                   ? -parse_positive(s+1)
                   : s[0]=='+'
                     ? parse_positive(s+1)
                     : parse_positive(s);
        }
        template<char... Chars>
        constexpr intmax parse() {
            return parse<sizeof...(Chars) + 1>({Chars...,'\0'});
        }
    }
}
namespace cnl {
    template<class T>
    struct numeric_limits : std::numeric_limits<T> {};
    template<>
    struct numeric_limits<int128> : numeric_limits<long long> {
        static int const digits = 8*sizeof(int128)-1;
        static int const digits10 = 38;
        struct _s {
            constexpr _s(uint64 upper, uint64 lower) : value(lower + (int128{upper} << 64)) {}
            constexpr operator int128() const { return value; }
            int128 value;
        };
        static constexpr int128 min()
        {
            return _s(0x8000000000000000, 0x0000000000000000);
        }
        static constexpr int128 max()
        {
            return _s(0x7fffffffffffffff, 0xffffffffffffffff);
        }
        static constexpr int128 lowest()
        {
            return min();
        }
    };
    template<>
    struct numeric_limits<uint128> : numeric_limits<unsigned long long> {
        static int const digits = 8*sizeof(int128);
        static int const digits10 = 38;
        struct _s {
            constexpr _s(uint64 upper, uint64 lower) : value(lower + (uint128{upper} << 64)) {}
            constexpr operator uint128() const { return value; }
            uint128 value;
        };
        static constexpr int128 min()
        {
            return 0;
        }
        static constexpr uint128 max()
        {
            return _s(0xffffffffffffffff, 0xffffffffffffffff);
        }
        static constexpr int128 lowest()
        {
            return min();
        }
    };
}
namespace cnl {
    template< ::cnl::intmax Value>
    struct constant {
        using value_type = ::cnl::intmax;
        static constexpr value_type value = Value;
        constexpr operator value_type() const
        {
            return value;
        }
    };
    template< ::cnl::intmax Value> constexpr auto operator +(constant<Value>) noexcept -> constant<+ Value> { return constant<+ Value>{}; }
    template< ::cnl::intmax Value> constexpr auto operator -(constant<Value>) noexcept -> constant<- Value> { return constant<- Value>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator +(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue + RhsValue)> { return constant<(LhsValue + RhsValue)>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator -(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue - RhsValue)> { return constant<(LhsValue - RhsValue)>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator *(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue * RhsValue)> { return constant<(LhsValue * RhsValue)>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator /(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue / RhsValue)> { return constant<(LhsValue / RhsValue)>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator %(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue % RhsValue)> { return constant<(LhsValue % RhsValue)>{}; }
    template< ::cnl::intmax Value> constexpr auto operator ~(constant<Value>) noexcept -> constant<~ Value> { return constant<~ Value>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator &(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue & RhsValue)> { return constant<(LhsValue & RhsValue)>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator |(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue | RhsValue)> { return constant<(LhsValue | RhsValue)>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator ^(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue ^ RhsValue)> { return constant<(LhsValue ^ RhsValue)>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator <<(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue << RhsValue)> { return constant<(LhsValue << RhsValue)>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator >>(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue >> RhsValue)> { return constant<(LhsValue >> RhsValue)>{}; }
    template< ::cnl::intmax Value> constexpr auto operator !(constant<Value>) noexcept -> constant<! Value> { return constant<! Value>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator &&(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue && RhsValue)> { return constant<(LhsValue && RhsValue)>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator ||(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue || RhsValue)> { return constant<(LhsValue || RhsValue)>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator ==(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue == RhsValue)> { return constant<(LhsValue == RhsValue)>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator !=(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue != RhsValue)> { return constant<(LhsValue != RhsValue)>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator <(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue < RhsValue)> { return constant<(LhsValue < RhsValue)>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator >(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue > RhsValue)> { return constant<(LhsValue > RhsValue)>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator <=(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue <= RhsValue)> { return constant<(LhsValue <= RhsValue)>{}; }
    template< ::cnl::intmax LhsValue, ::cnl::intmax RhsValue> constexpr auto operator >=(constant<LhsValue>, constant<RhsValue>) noexcept -> constant<(LhsValue >= RhsValue)> { return constant<(LhsValue >= RhsValue)>{}; }
    namespace _impl {
        template<class T>
        struct is_constant : std::false_type {
        };
        template< ::cnl::intmax Value>
        struct is_constant<::cnl::constant<Value>> : std::true_type {
        };
    }
    namespace literals {
        template<char... Chars>
        constexpr auto operator "" _c()
        -> constant<_cnlint_impl::parse<Chars...,'\0'>()>
        {
            return {};
        }
    }
    template< ::cnl::intmax Value>
    struct numeric_limits<constant<Value>> : cnl::numeric_limits<typename constant<Value>::value_type> {
        using _value_type = typename constant<Value>::value_type;
        static constexpr _value_type min()
        {
            return {};
        }
        static constexpr _value_type max()
        {
            return {};
        }
        static constexpr _value_type lowest()
        {
            return {};
        }
    };
}
namespace cnl {
    namespace _impl {
        template<bool IsSigned>
        struct used_digits_signed;
        template<>
        struct used_digits_signed<false> {
            template<class Integer>
            constexpr int operator()(Integer const& value, int radix) const
            {
                static_assert(cnl::numeric_limits<Integer>::is_integer,
                        "Integer parameter of used_digits_positive() must be a fundamental integer.");
                return (value>0) ? 1+used_digits_signed<false>{}(value/radix, radix) : 0;
            }
        };
        template<>
        struct used_digits_signed<true> {
            template<class Integer>
            constexpr int operator()(Integer const& value, int radix) const
            {
                static_assert(cnl::numeric_limits<Integer>::is_integer,
                        "Integer parameter of used_digits_signed()() must be a fundamental integer.");
                return (value<0)
                       ? used_digits_signed<false>{}(Integer(-1)-value, radix)
                       : used_digits_signed<false>{}(value, radix);
            }
        };
        template<typename Integer>
        constexpr int used_digits(Integer const& value, int radix = numeric_limits<Integer>::radix)
        {
            static_assert(std::is_integral<Integer>::value
                    || std::is_same<Integer, intmax>::value
                    || std::is_same<Integer, uintmax>::value, "Integer must be a fundamental integral");
            return used_digits_signed<std::is_signed<Integer>::value>{}(value, radix);
        }
    }
}
namespace cnl {
    namespace _impl {
        template<bool C, class ... T>
        using enable_if_t = typename std::enable_if<C, T ...>::type;
    }
}
namespace cnl {
    template<class T, class Enable = void>
    struct is_composite : std::false_type {
        static_assert(!std::is_const<T>::value, "T is const");
        static_assert(!std::is_volatile<T>::value, "T is volatile");
    };
    namespace _impl {
        template<class ... Args>
        struct are_composite;
        template<>
        struct are_composite<> : std::false_type {
        };
        template<class ArgHead, class ... ArgTail>
        struct are_composite<ArgHead, ArgTail...>
                : std::integral_constant<bool, is_composite<typename std::decay<ArgHead>::type>::value || are_composite<ArgTail...>::value> {
        };
    }
}
namespace cnl {
    namespace _impl {
        template<typename T>
        using remove_cvref_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
    }
}
namespace cnl {
    namespace _impl {
        template<class T>
        struct is_integral : std::is_integral<T> {
        };
        template<>
        struct is_integral<int128> : std::integral_constant<bool, true> {
        };
        template<>
        struct is_integral<uint128> : std::integral_constant<bool, true> {
        };
    }
}
namespace cnl {
    namespace _impl {
        template<typename Number>
        struct default_to_rep {
            constexpr Number& operator()(Number& number) const {
                return number;
            };
            constexpr Number const& operator()(Number const& number) const {
                return number;
            };
            constexpr Number&& operator()(Number&& number) const {
                return std::forward<Number>(number);
            };
        };
    }
    template<typename Number, class Enable = void>
    struct to_rep;
    template<typename Number>
    struct to_rep<Number, _impl::enable_if_t<
            _impl::is_integral<Number>::value
            ||std::is_floating_point<Number>::value
            ||_impl::is_constant<Number>::value>>
            : _impl::default_to_rep<Number> {
    };
    namespace _impl {
        template<class Number>
        constexpr auto to_rep(Number&& number)
        -> decltype(cnl::to_rep<remove_cvref_t<Number>>()(std::forward<Number>(number))) {
            return cnl::to_rep<remove_cvref_t<Number>>()(std::forward<Number>(number));
        }
        template<class Number>
        using to_rep_t = typename std::remove_reference<decltype(to_rep(std::declval<Number>()))>::type;
    }
}
namespace cnl {
    template<typename T, class Enable = void>
    struct is_signed;
    template<>
    struct is_signed<char> : std::is_signed<char> {
    };
    template<>
    struct is_signed<signed char> : std::true_type {
    };
    template<>
    struct is_signed<unsigned char> : std::false_type {
    };
    template<>
    struct is_signed<wchar_t> : std::is_signed<char> {
    };
    template<>
    struct is_signed<char16_t> : std::is_signed<char16_t> {
    };
    template<>
    struct is_signed<char32_t> : std::is_signed<char32_t> {
    };
    template<>
    struct is_signed<signed short> : std::true_type {
    };
    template<>
    struct is_signed<unsigned short> : std::false_type {
    };
    template<>
    struct is_signed<signed int> : std::true_type {
    };
    template<>
    struct is_signed<unsigned int> : std::false_type {
    };
    template<>
    struct is_signed<signed long> : std::true_type {
    };
    template<>
    struct is_signed<unsigned long> : std::false_type {
    };
    template<>
    struct is_signed<signed long long> : std::true_type {
    };
    template<>
    struct is_signed<unsigned long long> : std::false_type {
    };
    template<>
    struct is_signed<int128> : std::true_type {
    };
    template<>
    struct is_signed<uint128> : std::false_type {
    };
    template<>
    struct is_signed<float> : std::true_type {
    };
    template<>
    struct is_signed<double> : std::true_type {
    };
    template<>
    struct is_signed<long double> : std::true_type {
    };
    template< ::cnl::intmax Value>
    struct is_signed<constant<Value>> : is_signed<decltype(Value)> {
    };
    template<typename T>
    struct is_signed<T, _impl::enable_if_t<is_composite<T>::value>>
            : is_signed<_impl::to_rep_t<T>> {
    };
}
namespace cnl {
    template<typename T, class Enable = void>
    struct digits;
    namespace _impl {
        template<typename Integer>
        struct fundamental_digits
                : std::integral_constant<int, 8*sizeof(Integer)-is_signed<Integer>::value> {
        };
    }
    template<>
    struct digits<char> : _impl::fundamental_digits<char> {
    };
    template<>
    struct digits<signed char> : _impl::fundamental_digits<signed char> {
    };
    template<>
    struct digits<unsigned char> : _impl::fundamental_digits<unsigned char> {
    };
    template<>
    struct digits<wchar_t> : _impl::fundamental_digits<char> {
    };
    template<>
    struct digits<char16_t> : _impl::fundamental_digits<char16_t> {
    };
    template<>
    struct digits<char32_t> : _impl::fundamental_digits<char32_t> {
    };
    template<>
    struct digits<signed short> : _impl::fundamental_digits<signed short> {
    };
    template<>
    struct digits<unsigned short> : _impl::fundamental_digits<unsigned short> {
    };
    template<>
    struct digits<signed int> : _impl::fundamental_digits<signed int> {
    };
    template<>
    struct digits<unsigned int> : _impl::fundamental_digits<unsigned int> {
    };
    template<>
    struct digits<signed long> : _impl::fundamental_digits<signed long> {
    };
    template<>
    struct digits<unsigned long> : _impl::fundamental_digits<unsigned long> {
    };
    template<>
    struct digits<signed long long> : _impl::fundamental_digits<signed long long> {
    };
    template<>
    struct digits<unsigned long long> : _impl::fundamental_digits<unsigned long long> {
    };
    template<>
    struct digits<int128> : _impl::fundamental_digits<int128> {
    };
    template<>
    struct digits<uint128> : _impl::fundamental_digits<uint128> {
    };
    template< ::cnl::intmax Value>
    struct digits<constant<Value>> : std::integral_constant<
            int,
            _impl::used_digits((Value<0) ? -Value : Value)> {
    };
}
namespace cnl {
    template<typename Number, typename Rep, class Enable = void>
    struct from_rep;
    template<typename Number, typename Rep>
    struct from_rep<Number, Rep, _impl::enable_if_t<cnl::_impl::is_integral<Number>::value>> {
        constexpr Number operator()(Rep const& rep) const {
            return static_cast<Number>(rep);
        }
    };
    namespace _impl {
        template<class Number, class Rep>
        constexpr auto from_rep(Rep const& rep)
        -> decltype(cnl::from_rep<Number, Rep>{}(rep))
        {
            return cnl::from_rep<Number, Rep>{}(rep);
        }
        template<class Number, class Rep>
        using from_rep_t = decltype(from_rep<Number>(std::declval<Rep>()));
    }
}
namespace cnl {
    namespace _impl {
        template<typename T>
        struct type_identity {
            using type = T;
        };
        template<typename T>
        using type_identity_t = typename type_identity<T>::type;
    }
}
namespace cnl {
    template<class, class = void>
    struct make_unsigned;
    template<class T>
    struct make_unsigned<T, _impl::enable_if_t<std::is_integral<T>::value>> : std::make_unsigned<T> {
    };
    template<>
    struct make_unsigned<int128> {
        using type = uint128;
    };
    template<>
    struct make_unsigned<uint128> {
        using type = uint128;
    };
    template<class T>
    using make_unsigned_t = typename make_unsigned<T>::type;
    template<typename T>
    struct make_unsigned<T, _impl::enable_if_t<is_composite<T>::value>>
            : _impl::type_identity<_impl::from_rep_t<T, make_unsigned_t<_impl::to_rep_t<T>>>> {
    };
}
namespace cnl {
    namespace _bit_impl {
        template<typename T>
        constexpr bool is_integral_unsigned()
        {
            return numeric_limits<T>::is_integer && !is_signed<T>::value;
        }
        template<typename T>
        constexpr bool is_integral_signed()
        {
            return numeric_limits<T>::is_integer && is_signed<T>::value;
        }
        template<typename T>
        constexpr T rotl(T x, unsigned int s, unsigned int width) noexcept
        {
            static_assert(is_integral_unsigned<T>(), "T must be unsigned integer");
            return static_cast<T>((x << (s%width)) | (x >> (width-(s%width))));
        }
        template<typename T>
        constexpr T rotr(T x, unsigned int s, unsigned int width) noexcept
        {
            static_assert(is_integral_unsigned<T>(), "T must be unsigned integer");
            return static_cast<T>((x >> (s%width)) | (x << (width-(s%width))));
        }
        template<typename T>
        constexpr int countr_zero(T x) noexcept
        {
            static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");
            return (x & 1) ? 0 : countr_zero<T>(static_cast<T>(x >> 1))+1;
        }
    }
    template<typename T>
    constexpr T rotl(T x, unsigned int s) noexcept
    {
        return _bit_impl::rotl(x, s, cnl::digits<T>::value);
    }
    template<typename T>
    constexpr T rotr(T x, unsigned int s) noexcept
    {
        return _bit_impl::rotr(x, s, cnl::digits<T>::value);
    }
    template<typename T>
    constexpr int countl_zero(T x) noexcept;
    template<>
    constexpr int countl_zero(unsigned int x) noexcept
    {
        return x ? __builtin_clz(x) : cnl::digits<unsigned int>::value;
    }
    template<>
    constexpr int countl_zero(unsigned long x) noexcept
    {
        return x ? __builtin_clzl(x) : cnl::digits<unsigned long>::value;
    }
    template<>
    constexpr int countl_zero(unsigned long long x) noexcept
    {
        return x ? __builtin_clzll(x) : cnl::digits<unsigned long long>::value;
    }
    template<typename T>
    constexpr int countl_zero(T x) noexcept
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");
        return x ? countl_zero<T>(static_cast<T>(x >> 1))-1 : cnl::digits<T>::value;
    }
    template<typename T>
    constexpr int countl_one(T x) noexcept;
    template<>
    constexpr int countl_one(unsigned int x) noexcept
    {
        return ~x ? __builtin_clz(~x) : cnl::digits<unsigned int>::value;
    }
    template<>
    constexpr int countl_one(unsigned long x) noexcept
    {
        return ~x ? __builtin_clzl(~x) : cnl::digits<unsigned long>::value;
    }
    template<>
    constexpr int countl_one(unsigned long long x) noexcept
    {
        return ~x ? __builtin_clzll(~x) : cnl::digits<unsigned long long>::value;
    }
    template<typename T>
    constexpr int countl_one(T x) noexcept
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");
        return (x & (T{1} << (cnl::digits<T>::value-1))) ? countl_one<T>(static_cast<T>(x << 1))+1 : 0;
    }
    template<typename T>
    constexpr int countr_zero(T x) noexcept;
    template<typename T>
    constexpr int countr_zero(T x) noexcept
    {
        return x ? _bit_impl::countr_zero(x) : cnl::digits<T>::value;
    }
    template<typename T>
    constexpr int countr_one(T x) noexcept;
    template<>
    constexpr int countr_one(unsigned int x) noexcept
    {
        return countr_zero(~x);
    }
    template<typename T>
    constexpr int countr_one(T x) noexcept
    {
        return (x & T{1}) ? countr_one(x >> 1)+1 : 0;
    }
    template<typename T>
    constexpr int popcount(T x) noexcept;
    template<>
    constexpr int popcount(unsigned int x) noexcept
    {
        return __builtin_popcount(x);
    }
    template<>
    constexpr int popcount(unsigned long x) noexcept
    {
        return __builtin_popcountl(x);
    }
    template<>
    constexpr int popcount(unsigned long long x) noexcept
    {
        return __builtin_popcountll(x);
    }
    template<typename T>
    constexpr int popcount(T x) noexcept
    {
        return x ? popcount(x & (x-1))+1 : 0;
    }
    template<class T>
    constexpr bool ispow2(T x) noexcept
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");
        return x && !(x & (x-1));
    }
    template<class T>
    constexpr T ceil2(T x) noexcept
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");
        return x ? static_cast<T>(T{1} << (digits<T>::value-countl_zero(T(x-T(1))))) : T{0};
    }
    template<class T>
    constexpr T floor2(T x) noexcept
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");
        return x ? static_cast<T>(T{1} << (digits<T>::value-1-countl_zero(x))) : T{0};
    }
    template<class T>
    constexpr int log2p1(T x) noexcept
    {
        static_assert(_bit_impl::is_integral_unsigned<T>(), "T must be unsigned integer");
        return digits<T>::value-countl_zero(x);
    }
    template<typename T>
    constexpr int countl_rsb(T x) noexcept;
    template<typename T>
    constexpr int countl_rsb(T x) noexcept
    {
        static_assert(_bit_impl::is_integral_signed<T>(), "T must be signed integer");
        using unsigned_type = typename make_unsigned<T>::type;
        return ((x<0)
                ? countl_one(static_cast<unsigned_type>(x))
                : countl_zero(static_cast<unsigned_type>(x))) - 1;
    }
    namespace _bit_impl {
        template<bool IsSigned>
        struct countl_rb {
            template<class Integer>
            constexpr int operator()(Integer const& value) const
            {
                static_assert(_bit_impl::is_integral_unsigned<Integer>(), "T must be unsigned integer");
                return countl_zero(value);
            }
        };
        template<>
        struct countl_rb<true> {
            template<class Integer>
            constexpr int operator()(Integer const& value) const
            {
                static_assert(_bit_impl::is_integral_signed<Integer>(), "T must be signed integer");
                return countl_rsb(value);
            }
        };
    }
    template<typename T>
    constexpr int countl_rb(T x) noexcept
    {
        return _bit_impl::countl_rb<is_signed<T>::value>()(x);
    }
    template<typename T>
    constexpr int countr_used(T x) noexcept
    {
        return digits<T>::value - countl_rb(x);
    }
}
namespace cnl {
    namespace _impl {
        template<typename T>
        constexpr auto abs(T const& value)
        -> enable_if_t<is_signed<T>::value, T>
        {
            static_assert(
                    std::is_same<decltype(+value), decltype(-value)>::value,
                    "cnl::abs only supports types with symetrically-typed unary operators");
            return static_cast<T>((value<0) ? -value : +value);
        }
        template<typename T>
        constexpr auto abs(T const& value)
        -> enable_if_t<!is_signed<T>::value, T>
        {
            return value;
        }
    }
}
namespace cnl {
    using _impl::abs;
    template<typename T>
    constexpr T sqrt(T arg) {
        return std::sqrt(arg);
    }
}
namespace cnl {
    namespace _impl {
        template<class Rep, bool IsSigned>
        struct lowest;
        template<class Rep>
        struct lowest<Rep, true> {
            constexpr Rep operator()(Rep const& max) const noexcept
            {
                return static_cast<Rep>(-max);
            }
        };
        template<class Rep>
        struct lowest<Rep, false> {
            constexpr Rep operator()(Rep const&) const noexcept
            {
                return 0;
            }
        };
    }
}
namespace cnl {
    template<int NumDigits>
    struct signed_integer_cannot_have {
        template<int MaxNumDigits>
        struct digits_because_maximum_is;
    };
    template<int NumDigits>
    struct unsigned_integer_cannot_have {
        template<int MaxNumDigits>
        struct digits_because_maximum_is;
    };
    namespace _impl {
        template<typename T, int digits>
        struct narrower_than
                : std::integral_constant<
                        bool,
                        std::is_same<T, void>::value ? true : numeric_limits<T>::digits<digits> {
        };
        template<typename T, int digits>
        struct no_narrower_than
                : std::integral_constant<
                        bool,
                        std::is_same<T, void>::value ? true : numeric_limits<T>::digits>=digits> {
        };
        template<int MinNumDigits, class Smaller, class T>
        using enable_for_range_t = typename std::enable_if<
                no_narrower_than<T, MinNumDigits>::value && narrower_than<Smaller, MinNumDigits>::value>::type;
        template<int MinNumDigits, class Enable = void>
        struct set_digits_signed;
        template<int MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, void, int8>> {
            using type = int8;
        };
        template<int MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, int8, int16>> {
            using type = int16;
        };
        template<int MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, int16, int32>> {
            using type = int32;
        };
        template<int MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, int32, int64>> {
            using type = int64;
        };
        template<int MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, int64, int128>> {
            using type = int128;
        };
        template<int MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, intmax, void>>
                : signed_integer_cannot_have<MinNumDigits>::template digits_because_maximum_is<numeric_limits<intmax>::digits> {
        };
        template<int MinNumDigits, class Enable = void>
        struct set_digits_unsigned;
        template<int MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, void, uint8>> {
            using type = uint8;
        };
        template<int MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, uint8, uint16>> {
            using type = uint16;
        };
        template<int MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, uint16, uint32>> {
            using type = uint32;
        };
        template<int MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, uint32, uint64>> {
            using type = uint64;
        };
        template<int MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, uint64, uint128>> {
            using type = uint128;
        };
        template<int MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, uintmax, void>>
                : unsigned_integer_cannot_have<MinNumDigits>::template digits_because_maximum_is<numeric_limits<uintmax>::digits> {
        };
        template<class Integer, int MinNumDigits>
        using set_digits_integer = typename std::conditional<
                numeric_limits<Integer>::is_signed,
                set_digits_signed<MinNumDigits>,
                set_digits_unsigned<MinNumDigits>>::type;
    }
    template<class T, int Digits, class _Enable = void>
    struct set_digits;
    template<class T, int Digits>
    struct set_digits<T, Digits, _impl::enable_if_t<_impl::is_integral<T>::value>>
            : _impl::set_digits_integer<T, Digits> {
    };
    template<int Digits>
    struct set_digits<int128, Digits>
            : _impl::set_digits_integer<signed, Digits> {
    };
    template<int Digits>
    struct set_digits<uint128, Digits>
            : _impl::set_digits_integer<unsigned, Digits> {
    };
    template<class T, int Digits>
    using set_digits_t = typename set_digits<T, Digits>::type;
}
namespace cnl {
    namespace _impl {
        template<typename Mimic, typename Source>
        struct adopt_digits : set_digits<Mimic, digits<Source>::value> {
        };
        template<typename Mimic, typename Source>
        using adopt_digits_t = typename adopt_digits<Mimic, Source>::type;
    }
}
namespace cnl {
    template<class, class = void>
    struct make_signed;
    template<class T>
    struct make_signed<T, _impl::enable_if_t<std::is_integral<T>::value>> : std::make_signed<T> {
    };
    template<>
    struct make_signed<int128> {
        using type = int128;
    };
    template<>
    struct make_signed<uint128> {
        using type = int128;
    };
    template<class T>
    using make_signed_t = typename make_signed<T>::type;
    template<typename T>
    struct make_signed<T, _impl::enable_if_t<is_composite<T>::value>>
            : _impl::type_identity<_impl::from_rep_t<T, make_signed_t<_impl::to_rep_t<T>>>> {
    };
}
namespace cnl {
    namespace _impl {
        template<class T, bool IsSigned>
        struct set_signedness;
        template<class T>
        struct set_signedness<T, true> : make_signed<T> {
        };
        template<class T>
        struct set_signedness<T, false> : make_unsigned<T> {
        };
        template<class T, bool IsSigned>
        using set_signedness_t = typename set_signedness<T, IsSigned>::type;
    }
}
namespace cnl {
    namespace _impl {
        template<typename Mimic, typename Source>
        struct adopt_signedness : set_signedness<Mimic, is_signed<Source>::value> {
        };
        template<typename Mimic, typename Source>
        using adopt_signedness_t = typename adopt_signedness<Mimic, Source>::type;
    }
}
namespace cnl {
    namespace _impl {
        template<typename Mimic, typename Source>
        struct adopt : adopt_digits<adopt_signedness_t<Mimic, Source>, Source> {
        };
        template<typename Mimic, typename Source>
        using adopt_t = typename adopt<Mimic, Source>::type;
    }
}
namespace cnl {
    namespace _impl {
        template<
                typename S, int Exponent, int Radix,
                bool PositiveExponent = (0<Exponent),
                bool OddExponent = ((Exponent & 1)!=0),
                bool FloatingPointS = numeric_limits<S>::is_iec559>
        struct default_power;
        template<typename S, int Radix>
        struct default_power<S, 0, Radix, false, false, false> {
            constexpr S operator()() const
            {
                return S{1};
            }
        };
        template<typename S, int Exponent, bool OddExponent>
        struct default_power<S, Exponent, 2, true, OddExponent, false> {
            constexpr auto operator()() const
            -> decltype(S{1} << constant<Exponent>{})
            {
                using result_numeric_limits = numeric_limits<decltype(S{1} << constant<Exponent>{})>;
                static_assert(!std::is_integral<S>::value
                        || !std::is_signed<S>::value
                        || Exponent<result_numeric_limits::digits, "attempted operation will result in overflow");
                return S{1} << constant<Exponent>{};
            }
        };
        template<typename S, int Exponent, int Radix, bool OddExponent>
        struct default_power<S, Exponent, Radix, true, OddExponent, false> {
            constexpr auto operator()() const
            -> decltype(default_power<S, (Exponent-1), Radix>{}()*Radix)
            {
                return default_power<S, (Exponent-1), Radix>{}()*Radix;
            }
        };
        template<typename S, int Exponent, int Radix, bool PositiveExponent, bool OddExponent>
        struct default_power<S, Exponent, Radix, PositiveExponent, OddExponent, true> {
            constexpr S operator()() const
            {
                return Exponent
                       ? S(1.)/default_power<S, -Exponent, Radix>{}()
                       : S{1.};
            }
        };
        template<typename S, int Exponent, int Radix>
        struct default_power<S, Exponent, Radix, true, false, true> {
            constexpr static S square(S const& r)
            {
                return r*r;
            }
            constexpr S operator()() const
            {
                return square(default_power<S, Exponent/2, Radix>{}());
            }
        };
        template<typename S, int Exponent, int Radix>
        struct default_power<S, Exponent, Radix, true, true, true> {
            constexpr static S square(S const& r)
            {
                return r*r;
            }
            constexpr S operator()() const
            {
                return S(Radix)*default_power<S, (Exponent-1), Radix>{}();
            }
        };
        template<typename S, int Exponent, int Radix, class Enable = void>
        struct power {
            constexpr auto operator()() const
            -> decltype(default_power<S, Exponent, Radix>{}()) {
                return default_power<S, Exponent, Radix>{}();
            }
        };
    }
    template<typename S, int Exponent, int Radix>
    constexpr auto power()
    -> decltype(_impl::power<S, Exponent, Radix>{}())
    {
        return _impl::power<S, Exponent, Radix>{}();
    }
}
namespace cnl {
    template<int Digits, int Radix, class S, class Enable = void>
    struct scale;
    namespace _impl {
        template<int Digits, int Radix, typename S, class Enable = void>
        struct default_scale;
        template<int Digits, int Radix, typename S>
        struct default_scale<Digits, Radix, S, _impl::enable_if_t<0<=Digits>> {
            constexpr auto operator()(S const& s) const
            -> decltype(s*cnl::power<S, Digits, Radix>())
            {
                return s*cnl::power<S, Digits, Radix>();
            }
        };
        template<int Digits, int Radix, typename S>
        struct default_scale<Digits, Radix, S, _impl::enable_if_t<Digits<0>> {
            constexpr auto operator()(S const& s) const
            -> decltype(s/cnl::power<S, -Digits, Radix>())
            {
                return s/cnl::power<S, -Digits, Radix>();
            }
        };
    }
    template<int Digits, int Radix, class S>
    struct scale<Digits, Radix, S, _impl::enable_if_t<cnl::_impl::is_integral<S>::value>>
            : _impl::default_scale<Digits, Radix, S> {
    };
    namespace _impl {
        template<int Digits, int Radix=2, class S>
        constexpr auto scale(S const& s)
        -> decltype(cnl::scale<Digits, Radix, S>{}(s))
        {
            return cnl::scale<Digits, Radix, S>{}(s);
        }
    }
}
namespace cnl {
    template<int Digits, int Radix, class S, class Enable=void>
    struct fixed_width_scale {
        constexpr S operator()(S const& s) const
        {
            static_assert(
                    Radix!=2||digits<S>::value>-Digits,
                    "this operation will flush the given value");
            return static_cast<S>(scale<Digits, Radix, S>()(s));
        }
    };
    namespace _impl {
        template<int Digits, class S=void>
        constexpr S fixed_width_scale(S const& s)
        {
            return cnl::fixed_width_scale<Digits, numeric_limits<S>::radix, S>()(s);
        }
    }
}
namespace cnl {
    namespace _impl {
        template<typename T>
        struct width : std::integral_constant<int, digits<T>::value+is_signed<T>::value> {
        };
        template<typename T, int Bits>
        struct set_width : set_digits<T, Bits - is_signed<T>::value> {
        };
        template<typename T, int Bits>
        using set_width_t = typename set_width<T, Bits>::type;
    }
}
namespace cnl {
    namespace _impl {
        template<typename Result, typename Value>
        struct from_value_simple {
            constexpr Result operator()(Value const& value) const {
                return value;
            }
        };
    }
    template<typename Number, typename Value, class Enable = void>
    struct from_value : _impl::from_value_simple<void, Value> {
        void operator()(Value const &) const;
    };
    template<class Number, class Value>
    struct from_value<
            Number, Value, _impl::enable_if_t<_impl::is_integral<Number>::value && _impl::is_integral<Value>::value>>
            : _impl::from_value_simple<Value, Value> {
    };
    template<class Number, ::cnl::intmax Value>
    struct from_value<Number, constant<Value>, _impl::enable_if_t<_impl::is_integral<Number>::value>> {
    private:
        using _result_type = set_digits_t<
                make_signed_t<Number>,
                _impl::max(digits<int>::value, _impl::used_digits(Value))>;
    public:
        constexpr _result_type operator()(constant<Value> const &value) const {
            return _result_type(value);
        }
    };
    namespace _impl {
        template<typename Number, typename Value>
        constexpr auto make_number(Value const& value)
        -> decltype(cnl::from_value<Number, Value>{}(value))
        {
            return cnl::from_value<Number, Value>{}(value);
        }
    }
    template<typename Number, typename Value>
    using from_value_t = decltype(_impl::make_number<Number>(std::declval<Value>()));
}
namespace cnl {
    namespace _impl {
        template<class T, class Enable = void>
        struct wants_generic_ops : std::false_type {
        };
        struct minus_op {
            template<class Rhs>
            constexpr auto operator()(Rhs const& rhs) const -> decltype(-rhs)
            {
                return -rhs;
            }
        };
        struct plus_op {
            template<class Rhs>
            constexpr auto operator()(Rhs const& rhs) const -> decltype(+rhs)
            {
                return +rhs;
            }
        };
        struct bitwise_not_op {
            template<class Rhs>
            constexpr auto operator()(Rhs const& rhs) const -> decltype(~std::declval<Rhs>())
            {
                return ~rhs;
            }
        };
        struct add_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs+rhs)
            {
                return lhs+rhs;
            }
        };
        struct subtract_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs-rhs)
            {
                return lhs-rhs;
            }
        };
        struct multiply_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs*rhs)
            {
                return lhs*rhs;
            }
        };
        struct divide_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs/rhs)
            {
                return lhs/rhs;
            }
        };
        struct modulo_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs%rhs)
            {
                return lhs%rhs;
            }
        };
        struct bitwise_or_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs | rhs)
            {
                return lhs | rhs;
            }
        };
        struct bitwise_and_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs & rhs)
            {
                return lhs & rhs;
            }
        };
        struct bitwise_xor_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs ^ rhs)
            {
                return lhs ^ rhs;
            }
        };
        struct shift_left_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs << rhs)
            {
                return lhs << rhs;
            }
        };
        struct shift_right_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs >> rhs)
            {
                return lhs >> rhs;
            }
        };
        struct equal_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs==rhs)
            {
                return lhs==rhs;
            }
        };
        struct not_equal_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs!=rhs)
            {
                return lhs!=rhs;
            }
        };
        struct less_than_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs<rhs)
            {
                return lhs<rhs;
            }
        };
        struct greater_than_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs>rhs)
            {
                return lhs>rhs;
            }
        };
        struct less_than_or_equal_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs<=rhs)
            {
                return lhs<=rhs;
            }
        };
        struct greater_than_or_equal_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs>=rhs)
            {
                return lhs>=rhs;
            }
        };
        struct pre_increment_op {
            template<class Rhs>
            constexpr auto operator()(Rhs& rhs) const -> decltype(++rhs)
            {
                return ++rhs;
            }
        };
        struct pre_decrement_op {
            template<class Rhs>
            constexpr auto operator()(Rhs& rhs) const -> decltype(--rhs)
            {
                return --rhs;
            }
        };
        struct post_increment_op {
            template<class Lhs>
            constexpr auto operator()(Lhs& lhs) const -> decltype(lhs++)
            {
                return lhs++;
            }
        };
        struct post_decrement_op {
            template<class Lhs>
            constexpr auto operator()(Lhs& lhs) const -> decltype(lhs--)
            {
                return lhs--;
            }
        };
        struct assign_add_op {
            using binary = add_op;
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const -> decltype(lhs += rhs)
            {
                return lhs += rhs;
            }
        };
        struct assign_subtract_op {
            using binary = subtract_op;
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const -> decltype(lhs -= rhs)
            {
                return lhs -= rhs;
            }
        };
        struct assign_multiply_op {
            using binary = multiply_op;
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const -> decltype(lhs *= rhs)
            {
                return lhs *= rhs;
            }
        };
        struct assign_divide_op {
            using binary = divide_op;
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const -> decltype(lhs /= rhs)
            {
                return lhs /= rhs;
            }
        };
        struct assign_modulo_op {
            using binary = modulo_op;
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const -> decltype(lhs %= rhs)
            {
                return lhs %= rhs;
            }
        };
        struct assign_bitwise_or_op {
            using binary = bitwise_or_op;
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const -> decltype(lhs |= rhs)
            {
                return lhs |= rhs;
            }
        };
        struct assign_bitwise_and_op {
            using binary = bitwise_and_op;
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const -> decltype(lhs &= rhs)
            {
                return lhs &= rhs;
            }
        };
        struct assign_bitwise_xor_op {
            using binary = bitwise_xor_op;
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const -> decltype(lhs ^= rhs)
            {
                return lhs ^= rhs;
            }
        };
        struct assign_shift_left_op {
            using binary = shift_left_op;
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const -> decltype(lhs <<= rhs)
            {
                return lhs <<= rhs;
            }
        };
        struct assign_shift_right_op {
            using binary = shift_right_op;
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs& lhs, Rhs const& rhs) const -> decltype(lhs >>= rhs)
            {
                return lhs >>= rhs;
            }
        };
        template<class Operator, class ... Operands>
        using op_result = decltype(Operator()(std::declval<Operands>() ...));
        template<class Operator>
        struct pre_to_assign;
        template<>
        struct pre_to_assign<pre_increment_op> : type_identity<assign_add_op> {
        };
        template<>
        struct pre_to_assign<pre_decrement_op> : type_identity<assign_subtract_op> {
        };
        template<class Operator>
        struct post_to_assign;
        template<>
        struct post_to_assign<post_increment_op> : type_identity<assign_add_op> {
        };
        template<>
        struct post_to_assign<post_decrement_op> : type_identity<assign_subtract_op> {
        };
        template<class Operator, class Operand, class Enable = void>
        struct unary_operator;
        template<class Operator, class LhsOperand, class RhsOperand, class Enable = void>
        struct binary_operator;
        template<class Operator, class LhsOperand, class RhsOperand, class Enable = void>
        struct comparison_operator;
        template<class Operator, class RhsOperand, class Enable = void>
        struct pre_operator;
        template<class Operator, class LhsOperand, class Enable = void>
        struct post_operator;
        template<class Operator, class LhsOperand, class RhsOperand, class Enable = void>
        struct compound_assignment_operator {
            constexpr LhsOperand& operator()(LhsOperand& lhs, RhsOperand const& rhs) const
            {
                return lhs = static_cast<LhsOperand>(
                        binary_operator<typename Operator::binary, LhsOperand, RhsOperand>()(lhs, rhs));
            }
        };
        template<class Operand, class T>
        using enable_unary_t = ::cnl::_impl::enable_if_t<_impl::wants_generic_ops<Operand>::value, T>;
        template<class LhsOperand, class RhsOperand>
        struct enable_binary;
        template<class LhsOperand, int LhsSize, class RhsOperand>
        struct enable_binary<LhsOperand[LhsSize], RhsOperand> : std::false_type {
        };
        template<class LhsOperand, class RhsOperand, int RhsSize>
        struct enable_binary<LhsOperand, RhsOperand[RhsSize]> : std::false_type {
        };
        template<class LhsOperand, class RhsOperand>
        struct enable_binary
                : std::integral_constant<
                        bool,
                        (numeric_limits<LhsOperand>::is_specialized && numeric_limits<RhsOperand>::is_specialized)
                                && (_impl::wants_generic_ops<LhsOperand>::value
                                        || _impl::wants_generic_ops<RhsOperand>::value)> {
        };
        template<class LhsOperand, class RhsOperand, class T>
        using enable_binary_t = _impl::enable_if_t<enable_binary<LhsOperand, RhsOperand>::value, T>;
        template<class Operand> constexpr auto operator + (Operand const& operand) -> decltype(cnl::_impl::unary_operator<cnl::_impl::enable_unary_t< Operand, cnl::_impl::plus_op>, Operand>()(operand)) { return cnl::_impl::unary_operator<cnl::_impl::plus_op, Operand>()(operand); }
        template<class Operand> constexpr auto operator - (Operand const& operand) -> decltype(cnl::_impl::unary_operator<cnl::_impl::enable_unary_t< Operand, cnl::_impl::minus_op>, Operand>()(operand)) { return cnl::_impl::unary_operator<cnl::_impl::minus_op, Operand>()(operand); }
        template<class Operand> constexpr auto operator ~ (Operand const& operand) -> decltype(cnl::_impl::unary_operator<cnl::_impl::enable_unary_t< Operand, cnl::_impl::bitwise_not_op>, Operand>()(operand)) { return cnl::_impl::unary_operator<cnl::_impl::bitwise_not_op, Operand>()(operand); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator + (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::add_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::add_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator - (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::subtract_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::subtract_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator * (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::multiply_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::multiply_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator / (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::divide_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::divide_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator % (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::modulo_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::modulo_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator | (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::bitwise_or_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::bitwise_or_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator & (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::bitwise_and_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::bitwise_and_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator ^ (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::bitwise_xor_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::bitwise_xor_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator << (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::shift_left_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::shift_left_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator >> (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::shift_right_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::shift_right_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator == (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::comparison_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::equal_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::comparison_operator<cnl::_impl::equal_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator != (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::comparison_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::not_equal_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::comparison_operator<cnl::_impl::not_equal_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator < (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::comparison_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::less_than_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::comparison_operator<cnl::_impl::less_than_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator > (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::comparison_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::greater_than_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::comparison_operator<cnl::_impl::greater_than_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator <= (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::comparison_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::less_than_or_equal_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::comparison_operator<cnl::_impl::less_than_or_equal_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator >= (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::comparison_operator<cnl::_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::greater_than_or_equal_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::comparison_operator<cnl::_impl::greater_than_or_equal_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class RhsOperand> constexpr auto operator ++ (RhsOperand& rhs) -> decltype(cnl::_impl::pre_operator<cnl::_impl::pre_increment_op, RhsOperand>()(rhs)) { return cnl::_impl::pre_operator<cnl::_impl::pre_increment_op, RhsOperand>()(rhs); }
        template<class RhsOperand> constexpr auto operator -- (RhsOperand& rhs) -> decltype(cnl::_impl::pre_operator<cnl::_impl::pre_decrement_op, RhsOperand>()(rhs)) { return cnl::_impl::pre_operator<cnl::_impl::pre_decrement_op, RhsOperand>()(rhs); }
        template<class LhsOperand> constexpr auto operator ++ (LhsOperand& lhs, int) -> decltype(cnl::_impl::post_operator<cnl::_impl::post_increment_op, LhsOperand>()(lhs)) { return cnl::_impl::post_operator<cnl::_impl::post_increment_op, LhsOperand>()(lhs); }
        template<class LhsOperand> constexpr auto operator -- (LhsOperand& lhs, int) -> decltype(cnl::_impl::post_operator<cnl::_impl::post_decrement_op, LhsOperand>()(lhs)) { return cnl::_impl::post_operator<cnl::_impl::post_decrement_op, LhsOperand>()(lhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator += (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_add_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_add_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator -= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_subtract_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_subtract_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator *= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_multiply_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_multiply_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator /= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_divide_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_divide_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator %= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_modulo_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_modulo_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator |= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_bitwise_or_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_bitwise_or_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator &= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_bitwise_and_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_bitwise_and_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator ^= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_bitwise_xor_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_bitwise_xor_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator <<= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_shift_left_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_shift_left_op, LhsOperand, RhsOperand>()(lhs, rhs); }
        template<class LhsOperand, class RhsOperand> constexpr auto operator >>= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_shift_right_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_shift_right_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    }
}
namespace cnl {
    namespace _impl {
        template<class Derived, class Rep>
        class number_base;
        template<class Derived, class Rep>
        class number_base {
        public:
            using rep = Rep;
            explicit constexpr operator bool() const
            {
                return static_cast<bool>(_rep);
            }
        protected:
            static_assert(numeric_limits<Rep>::is_integer, "number_base must be specialized with integer Rep type template parameter");
            number_base() = default;
            explicit constexpr number_base(rep const& r)
                : _rep(r) { }
            template<class T>
                                  number_base& operator=(T const& r) {
                _rep = r;
                return static_cast<Derived&>(*this);
            }
            friend struct cnl::to_rep<number_base>;
        private:
            rep _rep;
        };
        template<class Derived, class Enable = void>
        struct is_class_derived_from_number_base : std::false_type {};
        template<class Derived>
        struct is_class_derived_from_number_base<const Derived> : is_class_derived_from_number_base<Derived> {};
        template<class Derived>
        struct is_class_derived_from_number_base<
                Derived,
                enable_if_t<std::is_base_of<number_base<Derived, typename Derived::rep>, Derived>::value>>
                : std::true_type {};
        template<class T, class Enable = void>
        struct is_derived_from_number_base : std::false_type {};
        template<class Derived>
        struct is_derived_from_number_base<Derived, enable_if_t<std::is_class<Derived>::value>>
        : is_class_derived_from_number_base<Derived> { };
        template<class Wrapper, bool IsComposite = is_composite<Wrapper>::value>
        struct depth;
        template<class Wrapper>
        struct depth<Wrapper, true> {
            using _rep = _impl::to_rep_t<Wrapper>;
            static constexpr auto value = depth<_rep>::value + 1;
        };
        template<class T>
        struct depth<T, false> : std::integral_constant<int, 0> {};
        template<class Rep, class Wrapper>
        struct is_wrappable;
        template<class Rep, int RepN, class Wrapper>
        struct is_wrappable<Rep[RepN], Wrapper> : std::false_type {};
        template<class Rep, class Wrapper, int WrapperN>
        struct is_wrappable<Rep, Wrapper[WrapperN]> : std::false_type {};
        template<class Rep, class Wrapper>
        struct is_wrappable
                : std::integral_constant<bool, cnl::numeric_limits<Rep>::is_specialized
                        && !std::is_floating_point<Rep>::value
                        && !std::is_same<from_value_t<Rep, int>, from_value_t<Wrapper, int>>::value
                        && (depth<Rep>::value < depth<Wrapper>::value)> {};
        template<class Operator, class Lhs, class Rhs>
        struct binary_operator<
                Operator, Lhs, Rhs,
                enable_if_t<std::is_floating_point<Lhs>::value && is_derived_from_number_base<Rhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(Operator()(lhs, static_cast<Lhs>(rhs)))
            {
                return Operator()(lhs, static_cast<Lhs>(rhs));
            }
        };
        template<class Operator, class Lhs, class Rhs>
        struct binary_operator<
                Operator, Lhs, Rhs,
                enable_if_t<is_derived_from_number_base<Lhs>::value && std::is_floating_point<Rhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(Operator()(static_cast<Rhs>(lhs), rhs))
            {
                return Operator()(static_cast<Rhs>(lhs), rhs);
            }
        };
        template<class Operator, class Lhs, class Rhs>
        struct binary_operator<
                Operator, Lhs, Rhs,
                enable_if_t<is_wrappable<Lhs, Rhs>::value && is_derived_from_number_base<Rhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(Operator()(make_number<Rhs>(lhs), rhs)) {
                return Operator()(make_number<Rhs>(lhs), rhs);
            }
        };
        template<class Operator, class Lhs, class Rhs>
        struct binary_operator<
                Operator, Lhs, Rhs,
                enable_if_t<is_derived_from_number_base<Lhs>::value && is_wrappable<Rhs, Lhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(Operator()(lhs, make_number<Lhs>(rhs)))
            {
                return Operator()(lhs, make_number<Lhs>(rhs));
            }
        };
        template<class Operator, class Lhs, class Rhs>
        struct comparison_operator<
                Operator, Lhs, Rhs,
                enable_if_t<std::is_floating_point<Lhs>::value && is_derived_from_number_base<Rhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(Operator()(lhs, static_cast<Lhs>(rhs)))
            {
                return Operator()(lhs, static_cast<Lhs>(rhs));
            }
        };
        template<class Operator, class Lhs, class Rhs>
        struct comparison_operator<
                Operator, Lhs, Rhs,
                enable_if_t<is_derived_from_number_base<Lhs>::value && std::is_floating_point<Rhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(Operator()(static_cast<Rhs>(lhs), rhs))
            {
                return Operator()(static_cast<Rhs>(lhs), rhs);
            }
        };
        template<class Operator, class Lhs, class Rhs>
        struct comparison_operator<
                Operator, Lhs, Rhs,
                enable_if_t<is_wrappable<Lhs, Rhs>::value && is_derived_from_number_base<Rhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(Operator()(make_number<Rhs>(lhs), rhs)) {
                return Operator()(make_number<Rhs>(lhs), rhs);
            }
        };
        template<class Operator, class Lhs, class Rhs>
        struct comparison_operator<
                Operator, Lhs, Rhs,
                enable_if_t<is_derived_from_number_base<Lhs>::value && is_wrappable<Rhs, Lhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(Operator()(lhs, make_number<Lhs>(rhs)))
            {
                return Operator()(lhs, make_number<Lhs>(rhs));
            }
        };
        template<class Operator, class Derived, typename Rep>
        struct pre_operator<Operator, number_base<Derived, Rep>> {
                                  Derived& operator()(Derived& rhs) const
            {
                Operator()(_impl::to_rep(rhs));
                return rhs;
            }
        };
        template<class Operator, class Derived, typename Rep>
        struct post_operator<Operator, number_base<Derived, Rep>> {
                                  Derived operator()(Derived& lhs) const
            {
                auto copy = lhs;
                Operator()(_impl::to_rep(lhs));
                return copy;
            }
        };
        template<class Number>
        struct wants_generic_ops<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> : std::true_type {
        };
    }
    template<class Number>
    struct is_composite<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> : std::true_type {
    };
    template<class Derived, class Rep>
    struct to_rep<_impl::number_base<Derived, Rep>> {
        constexpr Rep& operator()(Derived& number) const {
            return number._rep;
        }
        constexpr Rep const& operator()(Derived const& number) const {
            return number._rep;
        }
        constexpr Rep&& operator()(Derived&& number) const {
            return std::forward<Rep>(number._rep);
        }
    };
    template<class Derived>
    struct to_rep<Derived, _impl::enable_if_t<_impl::is_derived_from_number_base<Derived>::value>>
            : to_rep<_impl::number_base<Derived, typename Derived::rep>> {
    };
    template<int Digits, int Radix, class Derived>
    struct scale<Digits, Radix, _impl::number_base<Derived, typename Derived::rep>> {
        using _scalar_type = _impl::number_base<Derived, typename Derived::rep>;
        constexpr auto operator()(Derived const &s) const
        -> decltype(_impl::from_rep<Derived>(_impl::scale<Digits, Radix>(_impl::to_rep(s))))
        {
            return _impl::from_rep<Derived>(_impl::scale<Digits, Radix>(_impl::to_rep(s)));
        }
    };
    template<class Derived, class Rep>
    struct numeric_limits<cnl::_impl::number_base<Derived, Rep>>
    : numeric_limits<Rep> {
        using _value_type = Derived;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;
        static constexpr _value_type min() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::min());
        }
        static constexpr _value_type max() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::max());
        }
        static constexpr _value_type lowest() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::lowest());
        }
        static constexpr _value_type epsilon() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::round_error());
        }
        static constexpr _value_type round_error() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::round_error());
        }
        static constexpr _value_type infinity() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::infinity());
        }
        static constexpr _value_type quiet_NaN() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::quiet_NaN());
        }
        static constexpr _value_type signaling_NaN() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::signaling_NaN());
        }
        static constexpr _value_type denorm_min() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::denorm_min());
        }
    };
}
namespace cnl {
    namespace _impl {
        template<class ... T>
        using common_type_t = typename std::common_type<T ...>::type;
    }
}
namespace cnl {
    template<int Digits, class Narrowest>
    class elastic_integer;
    namespace _elastic_integer_impl {
        template<class ElasticInteger>
        struct is_elastic_integer : std::false_type {
        };
        template<int Digits, class Narrowest>
        struct is_elastic_integer<elastic_integer<Digits, Narrowest>> : std::true_type {
        };
        template<int Digits, class Narrowest>
        using rep_t = typename set_digits<Narrowest, _impl::max(cnl::digits<Narrowest>::value, Digits)>::type;
        template<int Digits, class Narrowest>
        using base_class_t = _impl::number_base<
                elastic_integer<Digits, Narrowest>,
                _elastic_integer_impl::rep_t<Digits, Narrowest>>;
    }
    template<int Digits, class Narrowest>
    struct digits<elastic_integer<Digits, Narrowest>> : std::integral_constant<int, Digits> {
    };
    template<int Digits, class Narrowest, int MinNumBits>
    struct set_digits<elastic_integer<Digits, Narrowest>, MinNumBits> {
        using type = elastic_integer<MinNumBits, Narrowest>;
    };
    namespace _impl {
        template<class T1, class T2>
        struct common_signedness {
            static constexpr bool _are_signed = numeric_limits<T1>::is_signed | numeric_limits<T2>::is_signed;
            using type = typename std::common_type<set_signedness_t<T1, _are_signed>,
                    set_signedness_t<T2, _are_signed>>::type;
        };
        template<class T1, class T2>
        using common_signedness_t = typename common_signedness<T1, T2>::type;
    }
    template<int Digits, typename Narrowest, typename Rep>
    struct from_rep<elastic_integer<Digits, Narrowest>, Rep> {
        constexpr auto operator()(Rep const& r) const
        -> elastic_integer<Digits, cnl::_impl::adopt_signedness_t<Narrowest, Rep>>
        {
            return r;
        }
    };
    template<int Digits, class Narrowest, class Value>
    struct from_value<elastic_integer<Digits, Narrowest>, Value>
            : _impl::from_value_simple<
                    elastic_integer<
                            cnl::digits<Value>::value,
                            _impl::set_width_t<Value, _impl::width<Narrowest>::value>>,
                    Value> {
    };
    template<int Digits, typename Narrowest, int ValueDigits, typename ValueNarrowest>
    struct from_value<elastic_integer<Digits, Narrowest>, elastic_integer<ValueDigits, ValueNarrowest>>
            : _impl::from_value_simple<
                    elastic_integer<_impl::max(Digits, ValueDigits), Narrowest>,
                    elastic_integer<ValueDigits, Narrowest>> {
    };
    template<int Digits, class Narrowest, ::cnl::intmax Value>
    struct from_value<elastic_integer<Digits, Narrowest>, constant<Value>> : _impl::from_value_simple<
            elastic_integer<digits<constant<Value>>::value, int>,
            constant<Value>> {
    };
    template<int ScalePower, int ScaleRadix, int ScalarDigits, class ScalarNarrowest>
    struct fixed_width_scale<
            ScalePower, ScaleRadix, elastic_integer<ScalarDigits, ScalarNarrowest>> {
        constexpr auto operator()(elastic_integer<ScalarDigits, ScalarNarrowest> const& s) const
        -> elastic_integer<ScalarDigits, ScalarNarrowest>
        {
            using result_type = elastic_integer<ScalarDigits, ScalarNarrowest>;
            using result_rep = typename result_type::rep;
            return fixed_width_scale<ScalePower, ScaleRadix, result_rep>()(_impl::to_rep(s));
        }
    };
    template<int ShiftDigits, int ScaleRadix, int ScalarDigits, class ScalarNarrowest>
    struct scale<ShiftDigits, ScaleRadix, elastic_integer<ScalarDigits, ScalarNarrowest>,
            _impl::enable_if_t<(0<=ShiftDigits)>> {
        constexpr auto operator()(elastic_integer<ScalarDigits, ScalarNarrowest> const& s) const
        -> elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>
        {
            using result_type = elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>;
            using result_rep = typename result_type::rep;
            return _impl::from_rep<result_type>(
                    scale<ShiftDigits, ScaleRadix, result_rep>()(_impl::to_rep(s)));
        }
    };
    template<int ShiftDigits, int ScalarDigits, class ScalarNarrowest>
    struct scale<ShiftDigits, 2, elastic_integer<ScalarDigits, ScalarNarrowest>,
            _impl::enable_if_t<(ShiftDigits<0)>> {
        constexpr auto operator()(elastic_integer<ScalarDigits, ScalarNarrowest> const& s) const
        -> elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>
        {
            using divisor_type = elastic_integer<1-ShiftDigits, ScalarNarrowest>;
            using divisor_rep = typename divisor_type::rep;
            return _impl::to_rep(s) / (divisor_rep{1} << -ShiftDigits);
        }
    };
    template<int Digits = digits<int>::value, class Narrowest = int>
    class elastic_integer : public _elastic_integer_impl::base_class_t<Digits, Narrowest> {
    public:
        using _base = _elastic_integer_impl::base_class_t<Digits, Narrowest>;
        static_assert(!_elastic_integer_impl::is_elastic_integer<typename _base::rep>::value,
                "elastic_integer of elastic_integer is not a supported");
        using rep = typename _base::rep;
        elastic_integer() = default;
        template<class Number, _impl::enable_if_t<numeric_limits<Number>::is_specialized, int> = 0>
        constexpr elastic_integer(Number n)
                : _base(static_cast<rep>(n))
        {
        }
        template<int FromWidth, class FromNarrowest>
        explicit constexpr elastic_integer(elastic_integer<FromWidth, FromNarrowest> const& rhs)
                :_base(static_cast<rep>(_impl::to_rep(rhs)))
        {
        }
        template< ::cnl::intmax Value>
        constexpr elastic_integer(constant<Value>)
                : _base(static_cast<rep>(Value))
        {
        }
        template<class S, _impl::enable_if_t<std::is_floating_point<S>::value, int> = 0>
        elastic_integer& operator=(S s)
        {
            _base::operator=(floating_point_to_rep(s));
            return *this;
        }
        template<class S>
        explicit constexpr operator S() const
        {
            return static_cast<S>(_impl::to_rep(*this));
        }
    };
    namespace _elastic_integer_impl {
        template<bool Signed>
        struct machine_digits {
            static constexpr int value =
                    cnl::digits<typename std::conditional<Signed, signed, unsigned>::type>::value;
        };
        template<typename S>
        using narrowest = set_digits_t<S, machine_digits<is_signed<S>::value>::value>;
    }
    template< ::cnl::intmax Value>
    constexpr auto make_elastic_integer(constant<Value>)
    -> elastic_integer<digits<constant<Value>>::value>
    {
        return elastic_integer<digits<constant<Value>>::value>{Value};
    }
    namespace _elastic_integer_impl {
        template<class Narrowest, class Integral>
        struct make_narrowest {
            using type = Narrowest;
        };
        template<class Integral>
        struct make_narrowest<void, Integral> {
            using type = narrowest<Integral>;
        };
        template<class Narrowest, class Integral>
        using make_narrowest_t = typename make_narrowest<Narrowest, Integral>::type;
        template<class Narrowest, class Integral>
        using make_type = elastic_integer<cnl::digits<Integral>::value, make_narrowest_t<Narrowest, Integral>>;
    }
    template<class Narrowest = void, class Integral, _impl::enable_if_t<!_impl::is_constant<Integral>::value, int> = 0>
    constexpr auto make_elastic_integer(Integral const& value)
    -> _elastic_integer_impl::make_type<Narrowest, Integral>
    {
        return _elastic_integer_impl::make_type<Narrowest, Integral>{value};
    }
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator~(elastic_integer<RhsDigits, RhsNarrowest> const& rhs)
    -> elastic_integer<RhsDigits, RhsNarrowest>
    {
        using elastic_integer = elastic_integer<RhsDigits, RhsNarrowest>;
        using rep = typename elastic_integer::rep;
        return _impl::from_rep<elastic_integer>(
            static_cast<rep>(
                    _impl::to_rep(rhs)
                ^ ((static_cast<rep>(~0)) >> (numeric_limits<rep>::digits - RhsDigits))));
    }
    namespace _impl {
        template<int FromDigits, class FromNarrowest, int OtherDigits, class OtherNarrowest,
                _impl::enable_if_t<FromDigits!=OtherDigits || !std::is_same<FromNarrowest, OtherNarrowest>::value, std::nullptr_t> = nullptr>
        constexpr auto cast_to_common_type(
                elastic_integer<FromDigits, FromNarrowest> const& from,
                elastic_integer<OtherDigits, OtherNarrowest> const&)
        -> decltype(static_cast<_impl::common_type_t<
                elastic_integer<FromDigits, FromNarrowest>,
                elastic_integer<OtherDigits, OtherNarrowest>>>(from)) {
            return static_cast<_impl::common_type_t<
                    elastic_integer<FromDigits, FromNarrowest>,
                    elastic_integer<OtherDigits, OtherNarrowest>>>(from);
        }
        template<class Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
        struct comparison_operator<Operator,
                elastic_integer<LhsDigits, LhsNarrowest>, elastic_integer<RhsDigits, RhsNarrowest>> {
            constexpr auto operator()(
                    elastic_integer<LhsDigits, LhsNarrowest> const& lhs,
                    elastic_integer<RhsDigits, RhsNarrowest> const& rhs) const
            -> decltype(Operator()(cast_to_common_type(lhs, rhs), cast_to_common_type(rhs, lhs)))
            {
                return Operator()(cast_to_common_type(lhs, rhs), cast_to_common_type(rhs, lhs));
            }
        };
        template<class Operator, int Digits, class Narrowest>
        struct comparison_operator<Operator, elastic_integer<Digits, Narrowest>, elastic_integer<Digits, Narrowest>> {
            constexpr auto operator()(
                    elastic_integer<Digits, Narrowest> const& lhs,
                    elastic_integer<Digits, Narrowest> const& rhs) const
            -> decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)))
            {
                return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
            }
        };
        template<class Operation, class LhsTraits, class RhsTraits>
        struct policy;
        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::add_op, LhsTraits, RhsTraits> {
            static constexpr int digits = _impl::max(LhsTraits::digits, RhsTraits::digits)+1;
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed;
        };
        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::subtract_op, LhsTraits, RhsTraits> {
            static constexpr int digits = _impl::max(LhsTraits::digits, RhsTraits::digits) + (LhsTraits::is_signed | RhsTraits::is_signed);
            static constexpr bool is_signed = true;
        };
        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::multiply_op, LhsTraits, RhsTraits> {
            static constexpr int contribution(int operand_digits) { return operand_digits == 1 ? 0 : operand_digits; }
            static constexpr int digits = max(1, contribution(LhsTraits::digits)+contribution(RhsTraits::digits));
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed;
        };
        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::divide_op, LhsTraits, RhsTraits> {
            static constexpr int digits = LhsTraits::digits;
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed;
        };
        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::modulo_op, LhsTraits, RhsTraits> {
            static constexpr int digits = LhsTraits::digits;
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed;
        };
        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::bitwise_or_op, LhsTraits, RhsTraits> {
            static constexpr int digits = _impl::max(LhsTraits::digits, RhsTraits::digits);
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed;
        };
        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::bitwise_and_op, LhsTraits, RhsTraits> {
            static constexpr int digits = _impl::min(LhsTraits::digits, RhsTraits::digits);
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed;
        };
        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::bitwise_xor_op, LhsTraits, RhsTraits> {
            static constexpr int digits = _impl::max(LhsTraits::digits, RhsTraits::digits);
            static constexpr bool is_signed = LhsTraits::is_signed || RhsTraits::is_signed;
        };
        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::shift_left_op, LhsTraits, RhsTraits> {
            static constexpr int digits = LhsTraits::digits;
            static constexpr bool is_signed = LhsTraits::is_signed;
        };
        template<class LhsTraits, class RhsTraits>
        struct policy<_impl::shift_right_op, LhsTraits, RhsTraits> {
            static constexpr int digits = LhsTraits::digits;
            static constexpr bool is_signed = LhsTraits::is_signed;
        };
        template<class OperationTag, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
        struct operate_params {
            using lhs = elastic_integer<LhsDigits, LhsNarrowest>;
            using rhs = elastic_integer<RhsDigits, RhsNarrowest>;
            using lhs_traits = numeric_limits<lhs>;
            using rhs_traits = numeric_limits<rhs>;
            using policy = typename _impl::policy<OperationTag, lhs_traits, rhs_traits>;
            using lhs_rep = typename lhs::rep;
            using rhs_rep = typename rhs::rep;
            using rep_result = typename _impl::op_result<OperationTag, lhs_rep, rhs_rep>;
            static constexpr int narrowest_width = _impl::max(
                    width<LhsNarrowest>::value,
                    width<RhsNarrowest>::value);
            using narrowest = set_digits_t<
                    _impl::set_signedness_t<rep_result, policy::is_signed>,
                    narrowest_width-policy::is_signed>;
            using result_type = elastic_integer<policy::digits, narrowest>;
        };
        template<class Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
        struct binary_operator<Operator,
                elastic_integer<LhsDigits, LhsNarrowest>, elastic_integer<RhsDigits, RhsNarrowest>> {
            constexpr auto operator()(
                    elastic_integer<LhsDigits, LhsNarrowest> const& lhs,
                    elastic_integer<RhsDigits, RhsNarrowest> const& rhs) const
            -> typename operate_params<Operator, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::result_type
            {
                using result_type = typename operate_params<Operator, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::result_type;
                return from_rep<result_type>(
                        static_cast<typename result_type::rep>(Operator()(
                                to_rep(static_cast<result_type>(lhs)),
                                to_rep(static_cast<result_type>(rhs)))));
            }
        };
        template<class Operator, int Digits, typename Narrowest>
        struct pre_operator<Operator, elastic_integer<Digits, Narrowest>>
                : pre_operator<Operator, typename elastic_integer<Digits, Narrowest>::_base> {
        };
        template<class Operator, int Digits, typename Narrowest>
        struct post_operator<Operator, elastic_integer<Digits, Narrowest>>
                : post_operator<Operator, typename elastic_integer<Digits, Narrowest>::_base> {
        };
    }
    template<int LhsDigits, class LhsNarrowest, ::cnl::intmax RhsValue>
    constexpr auto operator<<(elastic_integer<LhsDigits, LhsNarrowest> const& lhs, constant<RhsValue>)
    -> decltype(_impl::from_rep<elastic_integer<LhsDigits + static_cast<int>(RhsValue), LhsNarrowest>>(
            _impl::to_rep(static_cast<elastic_integer<LhsDigits + static_cast<int>(RhsValue), LhsNarrowest>>(lhs)) << RhsValue)) {
        using result_type = elastic_integer<LhsDigits + static_cast<int>(RhsValue), LhsNarrowest>;
        return _impl::from_rep<result_type>(_impl::to_rep(static_cast<result_type>(lhs)) << RhsValue);
    }
    template<int LhsDigits, class LhsNarrowest, ::cnl::intmax RhsValue>
    constexpr auto operator>>(elastic_integer<LhsDigits, LhsNarrowest> const& lhs, constant<RhsValue>)
    -> decltype (_impl::from_rep<elastic_integer<LhsDigits - static_cast<int>(RhsValue), LhsNarrowest>>(
            _impl::to_rep(lhs) >> RhsValue)) {
        return _impl::from_rep<elastic_integer<LhsDigits - static_cast<int>(RhsValue), LhsNarrowest>>(
                _impl::to_rep(lhs) >> RhsValue);
    }
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator-(elastic_integer<RhsDigits, RhsNarrowest> const& rhs)
    -> decltype(_impl::from_rep<elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>>(
            -_impl::to_rep(static_cast<elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>>(rhs))))
    {
        using result_type = elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>;
        return _impl::from_rep<result_type>(-_impl::to_rep(static_cast<result_type>(rhs)));
    }
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator+(elastic_integer<RhsDigits, RhsNarrowest> const& rhs)
    -> decltype(_impl::from_rep<elastic_integer<RhsDigits, RhsNarrowest>>(
            +_impl::to_rep(static_cast<elastic_integer<RhsDigits, RhsNarrowest>>(rhs))))
    {
        return _impl::from_rep<elastic_integer<RhsDigits, RhsNarrowest>>(
                +_impl::to_rep(static_cast<elastic_integer<RhsDigits, RhsNarrowest>>(rhs)));
    }
}
namespace std {
    template<int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
    struct common_type<cnl::elastic_integer<LhsDigits, LhsNarrowest>, cnl::elastic_integer<RhsDigits, RhsNarrowest>> {
        using type = cnl::elastic_integer<
                cnl::_impl::max(LhsDigits, RhsDigits),
                cnl::_impl::common_signedness_t<LhsNarrowest, RhsNarrowest>>;
    };
    template<int LhsDigits, class LhsNarrowest, class Rhs>
    struct common_type<cnl::elastic_integer<LhsDigits, LhsNarrowest>, Rhs>
            : common_type<cnl::elastic_integer<LhsDigits, LhsNarrowest>, cnl::elastic_integer<numeric_limits<Rhs>::digits, Rhs>> {
    };
    template<class Lhs, int RhsDigits, class RhsNarrowest>
    struct common_type<Lhs, cnl::elastic_integer<RhsDigits, RhsNarrowest>>
            : common_type<cnl::elastic_integer<numeric_limits<Lhs>::digits, Lhs>, cnl::elastic_integer<RhsDigits, RhsNarrowest>> {
    };
}
namespace cnl {
    template<int Digits, class Narrowest>
    struct numeric_limits<elastic_integer<Digits, Narrowest>>
            : numeric_limits<Narrowest> {
        using _narrowest_numeric_limits = numeric_limits<Narrowest>;
        using _value_type = elastic_integer<Digits, Narrowest>;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;
        static constexpr _rep _rep_max() noexcept
        {
            return static_cast<_rep>(_rep_numeric_limits::max() >> (_rep_numeric_limits::digits-digits));
        }
        static constexpr int digits = Digits;
        static constexpr _value_type min() noexcept
        {
            return _impl::from_rep<_value_type>(1);
        }
        static constexpr _value_type max() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_max());
        }
        static constexpr _value_type lowest() noexcept
        {
            return _impl::lowest<_rep, _narrowest_numeric_limits::is_signed>()(_rep_max());
        }
    };
    template<int Digits, class Narrowest>
    struct numeric_limits<elastic_integer<Digits, Narrowest> const>
            : numeric_limits<elastic_integer<Digits, Narrowest>> {
    };
    template<int Digits, class Narrowest>
    std::ostream &operator<<(std::ostream &o, elastic_integer<Digits, Narrowest> const &i) {
        return o << _impl::to_rep(i);
    }
}
namespace cnl {
    template<typename Rep = int, int Exponent = 0, int Radix = cnl::numeric_limits<Rep>::radix>
    class fixed_point;
}
namespace cnl {
    namespace _impl {
        template<typename Number, typename Enable = void>
        struct unwrap;
        template<typename Number>
        struct unwrap<Number, enable_if_t<!is_composite<Number>::value>> {
            constexpr Number operator()(Number const& number) const
            {
                return number;
            }
        };
        template<typename Number>
        struct unwrap<Number, enable_if_t<is_composite<Number>::value>> {
            constexpr auto operator()(Number const& number) const
            -> decltype(unwrap<to_rep_t<Number>>{}(to_rep(number)))
            {
                return unwrap<to_rep_t<Number>>{}(to_rep(number));
            }
        };
    }
    template<typename Number>
    constexpr auto unwrap(Number const& number)
    -> decltype(_impl::unwrap<Number>{}(number))
    {
        return _impl::unwrap<Number>{}(number);
    }
}
namespace cnl {
    namespace _numeric_impl {
        template<class Integer, bool IsSigned>
        struct trailing_bits {
            constexpr int operator()(Integer const& integer) const noexcept
            {
                return countr_zero(integer);
            }
        };
        template<class Integer>
        struct trailing_bits<Integer, true> {
            constexpr int operator()(Integer const& integer) const noexcept
            {
                using unsigned_type = make_unsigned_t<Integer>;
                return countr_zero(static_cast<unsigned_type>(integer));
            }
        };
    }
    template<class Integer>
    constexpr int trailing_bits(Integer const& value)
    {
        return value ? _numeric_impl::trailing_bits<Integer, is_signed<Integer>::value>()(value) : 0;
    }
    template<typename Integer>
    constexpr int used_digits(Integer const& value, int radix = numeric_limits<Integer>::radix)
    {
        return _impl::used_digits_signed<is_signed<Integer>::value>{}(unwrap(value), radix);
    }
    template<class Integer>
    constexpr int leading_bits(Integer const& value)
    {
        return digits<Integer>::value-cnl::used_digits(value);
    }
}
namespace cnl {
    namespace _impl {
        template<class T>
        struct is_fixed_point
                : public std::false_type {
        };
        template<typename Rep, int Exponent, int Radix>
        struct is_fixed_point<fixed_point<Rep, Exponent, Radix>>
                : public std::true_type {
        };
    }
}
namespace cnl {
    template<typename Numerator, typename Denominator>
    struct fraction;
    template<typename Rep, int Exponent, int Radix>
    class fixed_point
            : public _impl::number_base<fixed_point<Rep, Exponent, Radix>, Rep> {
        static_assert(Radix>=2, "Radix must be two or greater");
        static_assert(!_impl::is_fixed_point<Rep>::value,
                "fixed_point of fixed_point is not a supported");
        using _base = _impl::number_base<fixed_point<Rep, Exponent, Radix>, Rep>;
    public:
        using rep = Rep;
        constexpr static int exponent = Exponent;
        constexpr static int radix = Radix;
    private:
        constexpr fixed_point(rep r, int)
                :_base(r)
        {
        }
    public:
        fixed_point() = default;
        template<class FromRep, int FromExponent>
        constexpr fixed_point(fixed_point<FromRep, FromExponent, Radix> const& rhs)
                : _base(
                static_cast<Rep>(_impl::scale<FromExponent-exponent, Radix>(
                        _impl::make_number<Rep>(cnl::_impl::to_rep(rhs)))))
        {
        }
        template< ::cnl::intmax Value>
        constexpr fixed_point(constant<Value> rhs)
                : fixed_point(_impl::from_rep<fixed_point<typename decltype(rhs)::value_type, 0>>(Value))
        {
        }
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_integer, int> Dummy = 0>
        constexpr fixed_point(S const& s)
                : _base(static_cast<Rep>(_impl::scale<-exponent, Radix>(_impl::make_number<Rep>(s))))
        {
        }
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
        constexpr fixed_point(S s)
                :_base(floating_point_to_rep(s))
        {
        }
        template<typename Numerator, typename Denominator>
        constexpr fixed_point(fraction<Numerator, Denominator> const& f);
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
                              fixed_point& operator=(S s)
        {
            _base::operator=(floating_point_to_rep(s));
            return *this;
        }
        template<class FromRep, int FromExponent>
                              fixed_point& operator=(fixed_point<FromRep, FromExponent, Radix> const& rhs)
        {
            _base::operator=(fixed_point_to_rep(rhs));
            return *this;
        }
        template<typename Numerator, typename Denominator>
                              fixed_point& operator=(fraction<Numerator, Denominator> const& f);
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_integer, int> Dummy = 0>
        explicit constexpr operator S() const
        {
            return static_cast<S>(_impl::scale<exponent>(_impl::to_rep(*this)));
        }
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
        explicit constexpr operator S() const
        {
            static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
            return S(_impl::to_rep(*this))*inverse_one<S>();
        }
        template<typename, typename, typename>
        friend struct from_rep;
    private:
        template<class S>
        static constexpr _impl::enable_if_t<numeric_limits<S>::is_iec559, S> one();
        template<class S>
        static constexpr _impl::enable_if_t<numeric_limits<S>::is_integer, S> one();
        template<class S>
        static constexpr S inverse_one();
        template<class S>
        static constexpr S rep_to_integral(rep r);
        template<class S>
        static constexpr rep floating_point_to_rep(S s);
        template<class S>
        static constexpr S rep_to_floating_point(rep r);
        template<class FromRep, int FromExponent>
        static constexpr rep fixed_point_to_rep(fixed_point<FromRep, FromExponent, Radix> const& rhs);
    };
    template<typename Rep, int Exponent, int Radix>
    constexpr int fixed_point<Rep, Exponent, Radix>::exponent;
    template<typename Rep, int Exponent, int Radix>
    template<class S>
    constexpr auto fixed_point<Rep, Exponent, Radix>::one()
    -> _impl::enable_if_t<numeric_limits<S>::is_iec559, S>
    {
        return power<S, -exponent, Radix>();
    }
    template<typename Rep, int Exponent, int Radix>
    template<class S>
    constexpr auto fixed_point<Rep, Exponent, Radix>::one()
    -> _impl::enable_if_t<numeric_limits<S>::is_integer, S>
    {
        return _impl::from_rep<fixed_point<S, 0>>(1);
    }
    template<typename Rep, int Exponent, int Radix>
    template<class S>
    constexpr S fixed_point<Rep, Exponent, Radix>::inverse_one()
    {
        static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
        return power<S, exponent, Radix>();
    }
    template<typename Rep, int Exponent, int Radix>
    template<class S>
    constexpr typename fixed_point<Rep, Exponent, Radix>::rep
    fixed_point<Rep, Exponent, Radix>::floating_point_to_rep(S s)
    {
        static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
        return static_cast<rep>(s*one<S>());
    }
    template<typename Rep, int Exponent, int Radix>
    template<class FromRep, int FromExponent>
    constexpr typename fixed_point<Rep, Exponent, Radix>::rep
    fixed_point<Rep, Exponent, Radix>::fixed_point_to_rep(fixed_point<FromRep, FromExponent, Radix> const& rhs)
    {
        return _impl::scale<FromExponent-exponent>(_impl::to_rep(rhs));
    }
}
namespace cnl {
    template<typename ArchetypeRep, int Exponent, int Radix, typename Rep>
    struct from_rep<fixed_point<ArchetypeRep, Exponent, Radix>, Rep> {
        constexpr auto operator()(Rep const& r) const
        -> fixed_point<Rep, Exponent, Radix>
        {
            return fixed_point<Rep, Exponent, Radix>(r, 0);
        }
    };
}
namespace cnl {
    template<typename Rep, int Exponent, int Radix>
    struct digits<fixed_point<Rep, Exponent, Radix>> : digits<Rep> {
    };
    template<typename Rep, int Exponent, int Radix, int MinNumBits>
    struct set_digits<fixed_point<Rep, Exponent, Radix>, MinNumBits> {
        using type = fixed_point<set_digits_t<Rep, MinNumBits>, Exponent, Radix>;
    };
    template<typename Rep, int Exponent, int Radix, typename Value>
    struct from_value<fixed_point<Rep, Exponent, Radix>, Value>
            : _impl::from_value_simple<fixed_point<Value, 0, Radix>, Value> {
    };
    template<typename Rep, int Exponent, int Radix, typename ValueRep, int ValueExponent>
    struct from_value<fixed_point<Rep, Exponent, Radix>, fixed_point<ValueRep, ValueExponent>> : _impl::from_value_simple<
            fixed_point<from_value_t<Rep, ValueRep>, ValueExponent>,
            fixed_point<ValueRep, ValueExponent>> {
    };
    template<typename Rep, int Exponent, int Radix, typename Numerator, typename Denominator>
    struct from_value<fixed_point<Rep, Exponent, Radix>, fraction<Numerator, Denominator>> {
        constexpr auto operator()(fraction<Numerator, Denominator> const& value) const
        -> decltype(quotient(value.numerator, value.denominator)) {
            return quotient(value.numerator, value.denominator);
        }
    };
    template<typename Rep, int Exponent, int Radix, ::cnl::intmax Value>
    struct from_value<fixed_point<Rep, Exponent, Radix>, constant<Value>> : _impl::from_value_simple<
            fixed_point<
                    set_digits_t<int, _impl::max(digits<int>::value, _impl::used_digits(Value)-trailing_bits(Value))>,
                    trailing_bits(Value)>,
            constant<Value>> {
    };
    namespace _impl {
        template <class T>
        struct fractional_digits : std::integral_constant<int, 0> {
        };
        template<typename Rep, int Exponent, int Radix>
        struct fractional_digits<fixed_point<Rep, Exponent, Radix>> : std::integral_constant<int, -Exponent> {
        };
        template <class T>
        struct integer_digits : std::integral_constant<int, digits<T>::value - fractional_digits<T>::value> {
        };
    }
}
namespace cnl {
    template<typename Value>
    constexpr auto make_fixed_point(Value const& value)
    -> cnl::from_value_t<fixed_point<Value, 0>, Value>
    {
        return _impl::make_number<fixed_point<Value, 0>>(value);
    }
    namespace _impl {
        template<typename Number>
        struct fixed_point_rep {
            using type = Number;
        };
        template<typename Rep, int Exponent, int Radix>
        struct fixed_point_rep<fixed_point<Rep, Exponent, Radix>> : fixed_point_rep<Rep> {
        };
        template<typename Number>
        constexpr Number not_fixed_point(Number const& number)
        {
            return number;
        }
        template<typename Rep, int Exponent, int Radix>
        constexpr Rep not_fixed_point(fixed_point<Rep, Exponent, Radix> const& f)
        {
            return _impl::to_rep(f);
        }
        template<typename Number>
        struct exponent : constant<0> {};
        template<typename Rep, int Exponent, int Radix>
        struct exponent<fixed_point<Rep, Exponent, Radix>> : constant<Exponent> {
        };
        template<class Quotient, class Dividend, class Divisor>
        struct exponent_shift : std::integral_constant<
                int,
                _impl::exponent<Dividend>::value
                    -_impl::exponent<Divisor>::value
                    -_impl::exponent<Quotient>::value> {
        };
        struct default_quotient_tag {};
        template<class Quotient, class Dividend, class Divisor>
        struct result;
        template<typename Rep, int Exponent, int Radix, typename Dividend, typename Divisor>
        struct result<fixed_point<Rep, Exponent, Radix>, Dividend, Divisor> {
            using type = fixed_point<Rep, Exponent, Radix>;
        };
        template<class Dividend, class Divisor>
        struct result<default_quotient_tag, Dividend, Divisor> {
            using natural_result = _impl::op_result<_impl::divide_op, Dividend, Divisor>;
            static constexpr int integer_digits =
                    _impl::integer_digits<Dividend>::value+_impl::fractional_digits<Divisor>::value;
            static constexpr int fractional_digits =
                    _impl::fractional_digits<Dividend>::value+_impl::integer_digits<Divisor>::value;
            static constexpr auto necessary_digits = integer_digits+fractional_digits;
            static constexpr auto natural_digits = digits<natural_result>::value;
            static constexpr auto result_digits = _impl::max(necessary_digits, natural_digits);
            using rep_type = set_digits_t<natural_result, result_digits>;
            static constexpr int rep_exponent = -fractional_digits;
            using type = fixed_point<typename fixed_point_rep<rep_type>::type, rep_exponent>;
        };
    }
    template<
            class Quotient = _impl::default_quotient_tag,
            class Dividend,
            class Divisor>
    constexpr auto quotient(Dividend const& dividend, Divisor const& divisor)
    -> typename _impl::result<Quotient, Dividend, Divisor>::type {
        using result_type = typename _impl::result<Quotient, Dividend, Divisor>::type;
        using result_rep = typename result_type::rep;
        return _impl::from_rep<result_type>(
                static_cast<result_rep>(_impl::fixed_width_scale<_impl::exponent_shift<result_type, Dividend, Divisor>::value>(
                        static_cast<result_rep>(_impl::not_fixed_point(dividend)))
                        /_impl::not_fixed_point(divisor)));
    }
}
namespace cnl {
    template<typename Numerator = int, typename Denominator = Numerator>
    struct fraction {
        static_assert(
                _impl::is_integral<Numerator>::value==_impl::is_integral<Denominator>::value,
                "ill-formed if only one template parameter is floating-point");
        using numerator_type = Numerator;
        using denominator_type = Denominator;
        explicit constexpr fraction(Numerator const& n, Denominator const& d)
                : numerator{n}, denominator{d} {}
        explicit constexpr fraction(Numerator const& n)
                : numerator{n}, denominator{1} {}
        template<typename RhsNumerator, typename RhsDenominator>
        constexpr fraction(fraction<RhsNumerator, RhsDenominator> const& f)
                : numerator(f.numerator), denominator(f.denominator) { }
        template<typename Scalar, _impl::enable_if_t<std::is_floating_point<Scalar>::value, int> = 0>
        explicit constexpr operator Scalar() const
        {
            return static_cast<Scalar>(numerator)/static_cast<Scalar>(denominator);
        }
        numerator_type numerator;
        denominator_type denominator = 1;
    };
}
namespace cnl {
    template<typename Rep, int Exponent, int Radix>
    template<typename Numerator, typename Denominator>
    constexpr fixed_point<Rep, Exponent, Radix>::fixed_point(fraction<Numerator, Denominator> const& f)
            : fixed_point(quotient<fixed_point>(f.numerator, f.denominator))
    {
    }
    template<typename Rep, int Exponent, int Radix>
    template<typename Numerator, typename Denominator>
                          fixed_point<Rep, Exponent, Radix>&
    fixed_point<Rep, Exponent, Radix>::operator=(fraction<Numerator, Denominator> const& f)
    {
        return operator=(quotient<fixed_point>(f.numerator, f.denominator));
    }
}
namespace cnl {
}
template<typename Type>
struct CNL_ERROR___cannot_use {
    struct as_a_tag;
};
namespace cnl {
    namespace _impl {
        struct native_tag {};
    }
    template<class Tag, typename Result, typename Input, typename Enable = void>
    struct convert : public CNL_ERROR___cannot_use<Tag>::as_a_tag {
    };
    template<typename Result, typename Input>
    struct convert<_impl::native_tag, Result, Input> {
        constexpr Result operator()(Input const& rhs) const
        {
            return static_cast<Result>(rhs);
        }
    };
    namespace _impl {
        template<class Tag, typename Result, typename Input>
        constexpr auto convert(Input const& from)
        -> decltype(cnl::convert<Tag, Result, Input>{}(from))
        {
            return cnl::convert<Tag, Result, Input>{}(from);
        }
        template<class Tag, class Operator>
        struct tagged_binary_operator : public CNL_ERROR___cannot_use<Tag>::as_a_tag {
        };
        template<class Operator>
        struct tagged_binary_operator<native_tag, Operator> : Operator {};
        template<>
        struct tagged_binary_operator<native_tag, shift_left_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs<<rhs)
            {
                using result_type = decltype(lhs<<rhs);
                return static_cast<result_type>(static_cast<cnl::make_unsigned_t<result_type>>(lhs)<<rhs);
            }
        };
    }
}
namespace cnl {
    namespace _impl {
        struct native_rounding_tag : public native_tag {
        };
        struct nearest_rounding_tag {
        };
    }
}
namespace cnl {
    template<typename Result, typename Input>
    struct convert<_impl::native_rounding_tag, Result, Input>
            : convert<_impl::native_tag, Result, Input> {
    };
    template<typename Result, typename Input>
    struct convert<
            _impl::nearest_rounding_tag, Result, Input,
            _impl::enable_if_t<
                    cnl::numeric_limits<Result>::is_integer && std::is_floating_point<Input>::value>> {
        constexpr Result operator()(Input const& from) const
        {
            return static_cast<Result>(from+((from>=0) ? .5 : -.5));
        }
    };
    template<typename Result, typename Input>
    struct convert<
           _impl::nearest_rounding_tag, Result, Input,
           _impl::enable_if_t<
                   std::is_floating_point<Result>::value && std::is_floating_point<Input>::value >> {
        constexpr Result operator()(Input const& from) const
        {
            return static_cast<Result>(from);
        }
    };
    template<typename Result, typename Input>
    struct convert<
           _impl::nearest_rounding_tag, Result, Input,
           _impl::enable_if_t<
                   cnl::numeric_limits<Result>::is_integer && cnl::numeric_limits<Input>::is_integer >> {
        constexpr Result operator()(Input const& from) const
        {
            return static_cast<Result>(from);
        }
    };
    template<typename Result, typename Input>
    struct convert<
           _impl::nearest_rounding_tag, Result, Input,
           _impl::enable_if_t<
                   std::is_floating_point<Result>::value && cnl::numeric_limits<Input>::is_integer >> {
        constexpr Result operator()(Input const& from) const
        {
            return static_cast<Result>(from);
        }
    };
}
namespace cnl {
    template<
            typename ResultRep, int ResultExponent,
            typename InputRep, int InputExponent,
            int Radix>
    struct convert<
            _impl::nearest_rounding_tag,
            fixed_point<ResultRep, ResultExponent, Radix>,
            fixed_point<InputRep, InputExponent, Radix>,
            _impl::enable_if_t<ResultExponent<=InputExponent>>
            : convert<
                    _impl::native_rounding_tag,
                    fixed_point<ResultRep, ResultExponent, Radix>,
                    fixed_point<InputRep, InputExponent, Radix>> {
    };
    template<
            typename ResultRep, int ResultExponent,
            typename InputRep, int InputExponent,
            int Radix>
    struct convert<
            _impl::nearest_rounding_tag,
            fixed_point<ResultRep, ResultExponent, Radix>,
            fixed_point<InputRep, InputExponent, Radix>,
            _impl::enable_if_t<(ResultExponent>InputExponent)>> {
    private:
        using _result = fixed_point<ResultRep, ResultExponent, Radix>;
        using _input = fixed_point<InputRep, InputExponent, Radix>;
        static constexpr _input half()
        {
            return static_cast<_input>(_impl::from_rep<_result>(1))/2;
        }
    public:
        constexpr _result operator()(_input const& from) const
        {
            return static_cast<_result>(from+((from>=0) ? half() : -half()));
        }
    };
    template<
            typename ResultRep, int ResultExponent, int ResultRadix,
            typename Input>
    struct convert<
            _impl::nearest_rounding_tag,
            fixed_point<ResultRep, ResultExponent, ResultRadix>,
            Input,
            _impl::enable_if_t<std::is_floating_point<Input>::value>> {
    private:
        using _result = fixed_point<ResultRep, ResultExponent, ResultRadix>;
        static constexpr Input half()
        {
            return power<Input, ResultExponent-1, ResultRadix>();
        }
    public:
        constexpr _result operator()(Input const& from) const
        {
            return static_cast<_result>(from+((from>=0) ? half() : -half()));
        }
    };
    template<
            typename ResultRep, int ResultExponent, int ResultRadix,
            typename Input>
    struct convert<
            _impl::nearest_rounding_tag,
            fixed_point<ResultRep, ResultExponent, ResultRadix>,
            Input,
            _impl::enable_if_t<cnl::numeric_limits<Input>::is_integer>>
            : convert<
                    _impl::nearest_rounding_tag,
                    fixed_point<ResultRep, ResultExponent, ResultRadix>,
                    fixed_point<Input>> {
    };
    template<
            typename Result,
            typename InputRep, int InputExponent, int InputRadix>
    struct convert<
            _impl::nearest_rounding_tag,
            Result,
            fixed_point<InputRep, InputExponent, InputRadix>,
            _impl::enable_if_t<cnl::numeric_limits<Result>::is_integer>> {
        using _input = fixed_point<InputRep, InputExponent, InputRadix>;
        constexpr Result operator()(_input const& from) const
        {
            return _impl::to_rep(_impl::convert<_impl::nearest_rounding_tag, fixed_point<Result>>(from));
        }
    };
}
namespace cnl {
    namespace _impl {
        template<typename Operator, typename Rep, int Exponent, int Radix>
        struct unary_operator<Operator, fixed_point<Rep, Exponent, Radix>> {
            constexpr auto operator()(fixed_point<Rep, Exponent, Radix> const& rhs) const
            -> decltype(from_rep<fixed_point<decltype(Operator()(_impl::to_rep(rhs))), Exponent, Radix>>(Operator()(_impl::to_rep(rhs))))
            {
                return from_rep<fixed_point<decltype(Operator()(_impl::to_rep(rhs))), Exponent, Radix>>(Operator()(_impl::to_rep(rhs)));
            }
        };
        template<typename Operator, typename LhsRep, typename RhsRep, int Exponent, int Radix>
        struct comparison_operator<Operator,
                fixed_point<LhsRep, Exponent, Radix>,
                fixed_point<RhsRep, Exponent, Radix>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, Exponent, Radix> const& lhs,
                    fixed_point<RhsRep, Exponent, Radix> const& rhs) const
            -> decltype(Operator{}(_impl::to_rep(lhs), _impl::to_rep(rhs)))
            {
                return Operator{}(_impl::to_rep(lhs), _impl::to_rep(rhs));
            }
        };
        template<typename Operator, typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct comparison_operator<Operator,
                fixed_point<LhsRep, LhsExponent, Radix>,
                fixed_point<RhsRep, RhsExponent, Radix>,
                enable_if_t<(LhsExponent<RhsExponent)>> {
            static constexpr int shiftage = RhsExponent - LhsExponent;
            using lhs_type = fixed_point<LhsRep, LhsExponent, Radix>;
            using rhs_type = fixed_point<decltype(std::declval<RhsRep>()<<constant<shiftage>()), LhsExponent, Radix>;
            using operator_type = comparison_operator<Operator, lhs_type, rhs_type>;
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs,
                    fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(operator_type{}(lhs, rhs))
            {
                return operator_type{}(lhs, rhs);
            }
        };
        template<typename Operator, typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct comparison_operator<Operator,
                fixed_point<LhsRep, LhsExponent, Radix>,
                fixed_point<RhsRep, RhsExponent, Radix>,
                enable_if_t<(RhsExponent<LhsExponent)>> {
            static constexpr int shiftage = LhsExponent - RhsExponent;
            using lhs_type = fixed_point<decltype(std::declval<LhsRep>()<<constant<shiftage>()), RhsExponent, Radix>;
            using rhs_type = fixed_point<RhsRep, RhsExponent, Radix>;
            using operator_type = comparison_operator<Operator, lhs_type, rhs_type>;
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs,
                    fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(operator_type{}(lhs, rhs))
            {
                return operator_type{}(lhs, rhs);
            }
        };
        template<typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct binary_operator<multiply_op, fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs,
                    fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(from_rep<fixed_point<
                    decltype(_impl::to_rep(lhs)*_impl::to_rep(rhs)),
                    LhsExponent+RhsExponent,
                    Radix>>(_impl::to_rep(lhs)*_impl::to_rep(rhs)))
            {
                return from_rep<fixed_point<
                        decltype(_impl::to_rep(lhs)*_impl::to_rep(rhs)),
                        LhsExponent+RhsExponent,
                        Radix>>(_impl::to_rep(lhs)*_impl::to_rep(rhs));
            }
        };
        template<typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct binary_operator<divide_op, fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs,
                    fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(from_rep<fixed_point<
                    decltype(_impl::to_rep(lhs)/_impl::to_rep(rhs)),
                    LhsExponent-RhsExponent,
                    Radix>>(_impl::to_rep(lhs)/_impl::to_rep(rhs)))
            {
                return from_rep<fixed_point<
                        decltype(_impl::to_rep(lhs)/_impl::to_rep(rhs)),
                        LhsExponent-RhsExponent,
                        Radix>>(_impl::to_rep(lhs)/_impl::to_rep(rhs));
            }
        };
        template<typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct binary_operator<modulo_op, fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs,
                    fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(from_rep<fixed_point<
                    decltype(_impl::to_rep(lhs)%_impl::to_rep(rhs)),
                    LhsExponent,
                    Radix>>(_impl::to_rep(lhs)%_impl::to_rep(rhs)))
            {
                return from_rep<fixed_point<
                        decltype(_impl::to_rep(lhs)%_impl::to_rep(rhs)),
                        LhsExponent,
                        Radix>>(_impl::to_rep(lhs)%_impl::to_rep(rhs));
            }
        };
        template<class Operator> struct is_zero_degree : std::true_type {};
        template<> struct is_zero_degree<multiply_op> : std::false_type {};
        template<> struct is_zero_degree<divide_op> : std::false_type {};
        template<> struct is_zero_degree<modulo_op> : std::false_type {};
        template<> struct is_zero_degree<shift_left_op> : std::false_type {};
        template<> struct is_zero_degree<shift_right_op> : std::false_type {};
        template<class Operator, class LhsRep, class RhsRep, int Exponent, int Radix>
        struct binary_operator<Operator, fixed_point<LhsRep, Exponent, Radix>, fixed_point<RhsRep, Exponent, Radix>,
                enable_if_t<is_zero_degree<Operator>::value>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, Exponent, Radix> const& lhs, fixed_point<RhsRep, Exponent, Radix> const& rhs) const
            -> decltype(from_rep<fixed_point<
                    decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs))),
                    Exponent,
                    Radix>>(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs))))
            {
                return from_rep<fixed_point<
                        decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs))),
                        Exponent,
                        Radix>>(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)));
            }
        };
        template<typename Operator, typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct binary_operator<Operator, fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>,
                        enable_if_t<is_zero_degree<Operator>::value>> {
        private:
            static constexpr int _common_exponent = min(LhsExponent, RhsExponent);
            static constexpr int _lhs_left_shift = LhsExponent-_common_exponent;
            static constexpr int _rhs_left_shift = RhsExponent-_common_exponent;
        public:
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs, fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(Operator{}(
                    from_rep<fixed_point<LhsRep, _common_exponent, Radix>>(
                            _impl::scale<_lhs_left_shift, Radix>(_impl::to_rep(lhs))),
                    from_rep<fixed_point<RhsRep, _common_exponent, Radix>>(
                            _impl::scale<_rhs_left_shift, Radix>(_impl::to_rep(rhs)))))
            {
                return Operator{}(
                        from_rep<fixed_point<LhsRep, _common_exponent, Radix>>(
                                _impl::scale<_lhs_left_shift, Radix>(_impl::to_rep(lhs))),
                        from_rep<fixed_point<RhsRep, _common_exponent, Radix>>(
                                _impl::scale<_rhs_left_shift, Radix>(_impl::to_rep(rhs))));
            }
        };
        template<typename Operator, typename Rep, int Exponent, int Radix>
        struct pre_operator<Operator, fixed_point<Rep, Exponent, Radix>> {
            constexpr auto operator()(fixed_point<Rep, Exponent, Radix>& rhs) const
            -> decltype(typename pre_to_assign<Operator>::type{}(rhs, 1))
            {
                return typename pre_to_assign<Operator>::type{}(rhs, 1);
            }
        };
        template<typename Operator, typename Rep, int Exponent, int Radix>
        struct post_operator<Operator, fixed_point<Rep, Exponent, Radix>> {
                                  fixed_point<Rep, Exponent, Radix> operator()(fixed_point<Rep, Exponent, Radix>& rhs) const
            {
                auto copy = rhs;
                typename post_to_assign<Operator>::type{}(rhs, 1);
                return copy;
            }
        };
    }
    template<typename LhsRep, int LhsExponent, int LhsRadix, typename Rhs>
    constexpr auto operator<<(fixed_point<LhsRep, LhsExponent, LhsRadix> const& lhs, Rhs const& rhs)
    -> decltype(_impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) << int(rhs)), LhsExponent, LhsRadix>>(
            _impl::to_rep(lhs) << int(rhs)))
    {
        return _impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) << int(rhs)), LhsExponent, LhsRadix>>(
                _impl::to_rep(lhs) << int(rhs));
    }
    template<typename LhsRep, int LhsExponent, int LhsRadix, typename Rhs>
    constexpr auto operator>>(fixed_point<LhsRep, LhsExponent, LhsRadix> const& lhs, Rhs const& rhs)
    -> decltype(_impl::from_rep<fixed_point<
            decltype(_impl::to_rep(lhs) >> int(rhs)),
            LhsExponent,
            LhsRadix>>(_impl::to_rep(lhs) >> int(rhs)))
    {
        return _impl::from_rep<fixed_point<
                decltype(_impl::to_rep(lhs) >> int(rhs)),
                LhsExponent,
                LhsRadix>>(_impl::to_rep(lhs) >> int(rhs));
    }
    template<typename LhsRep, int LhsExponent, int LhsRadix, ::cnl::intmax RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent+static_cast<int>(RhsValue), LhsRadix>
    operator<<(fixed_point<LhsRep, LhsExponent, LhsRadix> const& lhs, constant<RhsValue>)
    {
        return _impl::from_rep<fixed_point<LhsRep, LhsExponent+static_cast<int>(RhsValue), LhsRadix>>(
                _impl::to_rep(lhs));
    }
    template<typename LhsRep, int LhsExponent, int LhsRadix, ::cnl::intmax RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent-static_cast<int>(RhsValue), LhsRadix>
    operator>>(fixed_point<LhsRep, LhsExponent, LhsRadix> const& lhs, constant<RhsValue>)
    {
        return _impl::from_rep<fixed_point<LhsRep, LhsExponent-static_cast<int>(RhsValue), LhsRadix>>(
                _impl::to_rep(lhs));
    }
}
namespace cnl {
    using _impl::native_rounding_tag;
    using _impl::nearest_rounding_tag;
    template<typename Number, class Enable = void>
    struct rounding;
    template<typename Number>
    struct rounding<Number&&> : rounding<Number> {};
    template<typename Number>
    struct rounding<Number, _impl::enable_if_t<_impl::is_integral<Number>::value>>
            : _impl::type_identity<native_rounding_tag> {
    };
    template<typename Number>
    using rounding_t = typename rounding<Number>::type;
}
namespace cnl {
    template<typename Number, class RoundingTag, class Enable = void>
    struct set_rounding;
    template<typename Number>
    struct set_rounding<Number, rounding_t<Number>, _impl::enable_if_t<!is_composite<Number>::value>>
            : _impl::type_identity<Number> {
    };
    template<typename Number, class RoundingTag>
    struct set_rounding<Number const&, RoundingTag>
            : set_rounding<_impl::remove_cvref_t<Number>, RoundingTag> {
    };
    template<typename Number, class RoundingTag>
    struct set_rounding<Number&, RoundingTag>
            : set_rounding<Number, RoundingTag> {
    };
    template<typename Number, class RoundingTag>
    struct set_rounding<Number&&, RoundingTag>
            : set_rounding<Number, RoundingTag> {
    };
    template<typename Number, class RoundingTag>
    using set_rounding_t = typename set_rounding<Number, RoundingTag>::type;
}
namespace cnl {
    namespace _impl {
        template<class Operator>
        struct tagged_binary_operator<native_rounding_tag, Operator>
                : tagged_binary_operator<native_tag, Operator> {
        };
        template<class Operator>
        struct tagged_binary_operator<nearest_rounding_tag, Operator> : Operator {
        };
        template<>
        struct tagged_binary_operator<nearest_rounding_tag, divide_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs/rhs)
            {
                return (((lhs<0) ^ (rhs<0))
                        ? lhs-(rhs/2)
                        : lhs+(rhs/2))/rhs;
            }
        };
        template<>
        struct tagged_binary_operator<nearest_rounding_tag, shift_right_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs >> rhs)
            {
                return (lhs + ((static_cast<from_value_t<Lhs, constant<1>>>(constant<1>{}) << rhs) >> constant<1>{}))
                        >> rhs;
            }
        };
        template<>
        struct tagged_binary_operator<nearest_rounding_tag, shift_left_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs << rhs)
            {
                return lhs << rhs;
            }
        };
        template<class RoundingTag, class Lhs, class Rhs>
        struct divide {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs/rhs)
            {
                return tagged_binary_operator<RoundingTag, divide_op>{}(cnl::unwrap(lhs), cnl::unwrap(rhs));
            }
        };
        template<class RoundingTag, class Lhs, class Rhs>
        struct shift_right {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs >> rhs)
            {
                return tagged_binary_operator<RoundingTag, shift_right_op>{}(lhs, rhs);
            }
        };
    }
}
namespace cnl {
    template<class Rep = int, class RoundingTag = _impl::nearest_rounding_tag>
    class rounding_integer;
    namespace _impl {
        template<class T>
        struct is_rounding_integer : std::false_type {
        };
        template<class Rep, class RoundingTag>
        struct is_rounding_integer<rounding_integer<Rep, RoundingTag>> : std::true_type {
        };
    }
    template<typename Number>
    struct rounding<
            Number,
            _impl::enable_if_t<
                    is_composite<Number>::value
                            &&!_impl::is_rounding_integer<Number>::value>>
            : rounding<_impl::to_rep_t<Number>> {
    };
    template<typename Rep, class RoundingTag>
    struct rounding<rounding_integer<Rep, RoundingTag>>
        : _impl::type_identity<RoundingTag> {
    };
    template<typename Number, class RoundingTag>
    struct set_rounding<
            Number,
            RoundingTag,
            _impl::enable_if_t<
                    is_composite<Number>::value
                            && !_impl::is_rounding_integer<Number>::value>>
            : _impl::type_identity<_impl::from_rep_t<
                    Number,
                    typename set_rounding<_impl::to_rep_t<Number>, RoundingTag>::type>> {
    };
    template<typename InputRep, class InputRoundingTag, class OutputRoundingTag>
    struct set_rounding<rounding_integer<InputRep, InputRoundingTag>, OutputRoundingTag>
            : _impl::type_identity<rounding_integer<InputRep, OutputRoundingTag>> {
    };
    template<class Rep, class RoundingTag>
    class rounding_integer : public _impl::number_base<rounding_integer<Rep, RoundingTag>, Rep> {
        static_assert(!_impl::is_rounding_integer<Rep>::value,
                "rounding_integer of rounding_integer is not a supported");
        static_assert(std::is_same<native_rounding_tag, rounding_t<Rep>>::value,
                "rounding_integer requires a Rep type that rounds natively");
    public:
        using rounding = RoundingTag;
        using _base = _impl::number_base<rounding_integer<Rep, RoundingTag>, Rep>;
        rounding_integer() = default;
        template<class T, _impl::enable_if_t<numeric_limits<T>::is_integer, int> Dummy = 0>
        constexpr rounding_integer(T const& v)
                : _base(static_cast<Rep>(v)) { }
        template<class T, _impl::enable_if_t<!numeric_limits<T>::is_integer, int> Dummy = 0>
        constexpr rounding_integer(T const& v)
                : _base(_impl::convert<rounding, Rep>(v)) { }
        template<class T>
        constexpr explicit operator T() const
        {
            return static_cast<T>(_impl::to_rep(*this));
        }
    };
    template<class Rep, class RoundingTag>
    struct digits<rounding_integer<Rep, RoundingTag>> : digits<Rep> {
    };
    template<class Rep, class RoundingTag, int MinNumBits>
    struct set_digits<rounding_integer<Rep, RoundingTag>, MinNumBits> {
        using type = rounding_integer<set_digits_t<Rep, MinNumBits>, RoundingTag>;
    };
    template<typename ArchetypeRep, class RoundingTag, typename Rep>
    struct from_rep<rounding_integer<ArchetypeRep, RoundingTag>, Rep> {
        constexpr auto operator()(Rep const& r) const
        -> rounding_integer<Rep, RoundingTag>
        {
            return r;
        }
    };
    template<class Rep, class RoundingTag, class Value>
    struct from_value<rounding_integer<Rep, RoundingTag>, Value> : _impl::from_value_simple<
            rounding_integer<Value, RoundingTag>, Value> {
    };
    template<class Rep, class RoundingTag, class ValueRep, class ValueRoundingTag>
    struct from_value<rounding_integer<Rep, RoundingTag>, rounding_integer<ValueRep, ValueRoundingTag>>
            : _impl::from_value_simple<
            rounding_integer<ValueRep, RoundingTag>, rounding_integer<ValueRep, ValueRoundingTag>> {
    };
    template<class Rep, class RoundingTag, ::cnl::intmax Value>
    struct from_value<rounding_integer<Rep, RoundingTag>, constant<Value>> : _impl::from_value_simple<
            rounding_integer<typename std::conditional<
                    digits<int>::value<_impl::used_digits(Value),
                            decltype(Value),
                            int>::type, RoundingTag>,
                    constant<Value>> {
    };
    template<int Digits, class Rep, class RoundingTag>
    struct scale<Digits, 2, rounding_integer<Rep, RoundingTag>,
            _impl::enable_if_t<Digits<0>>
            : _impl::default_scale<Digits, 2, rounding_integer<Rep, RoundingTag>> {
    };
    template<int Digits, int Radix, class Rep, class RoundingTag>
    struct scale<Digits, Radix, rounding_integer<Rep, RoundingTag>,
            _impl::enable_if_t<0<=Digits>> {
        constexpr auto operator()(rounding_integer<Rep, RoundingTag> const& s) const
        -> decltype(_impl::from_rep<rounding_integer<Rep, RoundingTag>>(
                scale<Digits, Radix, Rep>{}(_impl::to_rep(s))))
        {
            return _impl::from_rep<rounding_integer<Rep, RoundingTag>>(
                    scale<Digits, Radix, Rep>{}(_impl::to_rep(s)));
        }
    };
    template<int Digits, class Rep, class RoundingTag>
    struct fixed_width_scale<Digits, 2, rounding_integer<Rep, RoundingTag>, _impl::enable_if_t<0<=Digits>> {
        constexpr auto operator()(rounding_integer<Rep, RoundingTag> const& s) const
        -> decltype(_impl::from_rep<rounding_integer<Rep, RoundingTag>>(
                fixed_width_scale<Digits, 2, Rep>{}(_impl::to_rep(s))))
        {
            return _impl::from_rep<rounding_integer<Rep, RoundingTag>>(
                    fixed_width_scale<Digits, 2, Rep>{}(_impl::to_rep(s)));
        }
    };
    template<class RoundingTag, class Rep>
    constexpr auto make_rounding_integer(Rep const& value)
    -> rounding_integer<Rep, RoundingTag>
    {
        return value;
    }
    namespace _impl {
        template<class Operator, class Rep, class RoundingTag>
        struct unary_operator<Operator, rounding_integer<Rep, RoundingTag>> {
            constexpr auto operator()(rounding_integer<Rep, RoundingTag> const& operand) const
            -> decltype(from_rep<rounding_integer<decltype(Operator()(_impl::to_rep(operand))), RoundingTag>>(
                    Operator()(_impl::to_rep(operand))))
            {
                using result_type = rounding_integer<decltype(Operator()(_impl::to_rep(operand))), RoundingTag>;
                return from_rep<result_type>(Operator()(_impl::to_rep(operand)));
            }
        };
        template<class Operator, class LhsRep, class RhsRep, class RoundingTag>
        struct binary_operator<Operator,
                rounding_integer<LhsRep, RoundingTag>, rounding_integer<RhsRep, RoundingTag>> {
            constexpr auto operator()(
                    rounding_integer<LhsRep, RoundingTag> const& lhs,
                    rounding_integer<RhsRep, RoundingTag> const& rhs) const
            -> decltype(make_rounding_integer<RoundingTag>(tagged_binary_operator<RoundingTag, Operator>()(
                    _impl::to_rep(lhs), _impl::to_rep(rhs))))
            {
                return make_rounding_integer<RoundingTag>(
                        tagged_binary_operator<RoundingTag, Operator>()(_impl::to_rep(lhs), _impl::to_rep(rhs)));
            }
        };
        template<class LhsRep, class RhsRep, class RoundingTag>
        struct binary_operator<shift_right_op,
                rounding_integer<LhsRep, RoundingTag>, rounding_integer<RhsRep, RoundingTag>> {
            constexpr auto operator()(
                    rounding_integer<LhsRep, RoundingTag> const& lhs,
                    rounding_integer<RhsRep, RoundingTag> const& rhs) const
            -> decltype(from_rep<rounding_integer<decltype(_impl::to_rep(lhs)>>_impl::to_rep(rhs)), RoundingTag>>(
                    _impl::to_rep(lhs)>>_impl::to_rep(rhs)))
            {
                return from_rep<rounding_integer<decltype(_impl::to_rep(lhs)>>_impl::to_rep(rhs)), RoundingTag>>(
                        _impl::to_rep(lhs)>>_impl::to_rep(rhs));
            }
        };
        template<class Operator, class LhsRep, class RhsRep, class RoundingTag>
        struct comparison_operator<Operator, rounding_integer<LhsRep, RoundingTag>, rounding_integer<RhsRep, RoundingTag>> {
        constexpr auto operator()(
                rounding_integer<LhsRep, RoundingTag> const& lhs,
                rounding_integer<RhsRep, RoundingTag> const& rhs) const
            -> decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)))
            {
                return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
            }
        };
        template<class Operator, class LhsRep, class LhsRoundingTag, class RhsRep, class RhsRoundingTag>
        struct comparison_operator<Operator,
                rounding_integer<LhsRep, LhsRoundingTag>, rounding_integer<RhsRep, RhsRoundingTag>> {
            constexpr auto operator()(
                    rounding_integer<LhsRep, LhsRoundingTag> const& lhs,
                    rounding_integer<RhsRep, RhsRoundingTag> const& rhs) const
            -> decltype(comparison_operator<Operator, rounding_integer<LhsRep, common_type_t<LhsRoundingTag, RhsRoundingTag>>, rounding_integer<LhsRep, common_type_t<LhsRoundingTag, RhsRoundingTag>>>()(lhs, rhs))
            {
                using common_tag = common_type_t<LhsRoundingTag, RhsRoundingTag>;
                return comparison_operator<Operator, rounding_integer<LhsRep, common_tag>, rounding_integer<LhsRep, common_tag>>()(lhs, rhs);
            }
        };
        template<class Operator, typename Rep, class RoundingTag>
        struct pre_operator<Operator, rounding_integer<Rep, RoundingTag>>
                : pre_operator<Operator, typename rounding_integer<Rep, RoundingTag>::_base> {
        };
        template<class Operator, typename Rep, class RoundingTag>
        struct post_operator<Operator, rounding_integer<Rep, RoundingTag>>
                : post_operator<Operator, typename rounding_integer<Rep, RoundingTag>::_base> {
        };
    }
    template<class Rep, class RoundingTag>
    struct numeric_limits<rounding_integer<Rep, RoundingTag>>
            : numeric_limits<_impl::number_base<rounding_integer<Rep, RoundingTag>, Rep>> {
        static constexpr bool is_integer = true;
    };
    template<class Rep, class RoundingTag>
    struct numeric_limits<rounding_integer<Rep, RoundingTag> const>
            : numeric_limits<_impl::number_base<rounding_integer<Rep, RoundingTag>, Rep>> {
        static constexpr bool is_integer = true;
    };
}
namespace std {
    template<class Rep, class RoundingTag>
    struct numeric_limits<cnl::rounding_integer<Rep, RoundingTag>>
            : cnl::numeric_limits<cnl::rounding_integer<Rep, RoundingTag>> {
    };
    template<class Rep, class RoundingTag>
    struct numeric_limits<cnl::rounding_integer<Rep, RoundingTag> const>
            : cnl::numeric_limits<cnl::rounding_integer<Rep, RoundingTag>> {
    };
}
namespace cnl {
    namespace _impl {
        template<class Result>
        [[noreturn]] Result terminate(char const* message) noexcept
        {
            std::fputs(message, stderr);
            std::fputc('\n', stderr);
            std::terminate();
        }
    }
}
namespace cnl {
    struct to_chars_result {
        char* ptr;
        std::errc ec;
    };
    namespace _impl {
        template<typename Scalar, int Base=10>
        struct max_to_chars_chars;
        template<typename Scalar>
        char itoc(Scalar value) noexcept
        {
            static_assert(
                    std::is_same<
                            typename rounding<Scalar>::type,
                            native_rounding_tag>::value,
                    "wrong rounding type");
            auto c = '0'+static_cast<int>(value);
            return static_cast<char>(c);
        }
        template<class Integer>
        char* to_chars_natural(char* const ptr, char* const last, Integer const& value)
        {
            auto const quotient = value/10;
            auto const next_ptr = quotient
                    ? to_chars_natural(ptr, last, quotient)
                    : ptr;
            if (next_ptr==last || next_ptr==nullptr) {
                return nullptr;
            }
            auto const remainder = value-(quotient*10);
            *next_ptr = itoc(remainder);
            return next_ptr+1;
        }
        template<typename Number, bool is_signed = is_signed<Number>::value>
        struct to_chars_non_zero;
        template<typename Number>
        struct to_chars_non_zero<Number, false> {
            to_chars_result operator()(char* const first, char* const last, Number const& value) const
            {
                return to_chars_positive(first, last, value);
            }
        };
        template<typename Number>
        struct to_chars_non_zero<Number, true> {
            to_chars_result operator()(char* const first, char* const last, Number const& value) const
            {
                if (value>0.) {
                    return to_chars_positive(first, last, value);
                }
                else {
                    auto const destination_length = std::distance(first, last);
                    if (destination_length<2) {
                        return to_chars_result{last, std::errc::value_too_large};
                    }
                    *first = '-';
                    return to_chars_positive(first+1, last, -value);
                }
            }
        };
    }
    template<typename Number>
    std::array<
            char,
            _impl::max_to_chars_chars<Number>::value+1>
    to_chars(Number const& value)
    {
        constexpr auto max_num_chars = _impl::max_to_chars_chars<Number>::value;
        std::array<char, max_num_chars+1> chars;
        auto result = to_chars(chars.data(), chars.data()+max_num_chars, value);
        ((result.ptr>chars.data()) ? static_cast<void>(0) : __builtin_unreachable());
        ((result.ptr<=chars.data()+max_num_chars) ? static_cast<void>(0) : __builtin_unreachable());
        ((result.ec==std::errc{}) ? static_cast<void>(0) : __builtin_unreachable());
        *result.ptr = '\0';
        return chars;
    }
}
namespace cnl {
    namespace _impl {
        template<typename Rep, int Exponent>
        struct max_to_chars_chars<fixed_point<Rep, Exponent>> {
            using _scalar = cnl::fixed_point<Rep, Exponent>;
            static constexpr auto _fractional_digits = cnl::_impl::fractional_digits<_scalar>::value;
            static constexpr auto _sign_chars = static_cast<int>(cnl::is_signed<_scalar>::value);
            static constexpr auto _integer_chars = ((cnl::_impl::integer_digits<_scalar>::value + 2) / 3);
            static constexpr auto _radix_chars = static_cast<int>(_fractional_digits > 0);
            static constexpr auto _fractional_chars = max(0, _fractional_digits);
            static constexpr auto value = _sign_chars + _integer_chars + _radix_chars + _fractional_chars;
        };
        template<typename Rep, int Exponent, int Radix, bool Flushed = (Radix==2 && Exponent<=-digits<Rep>::value)>
        struct split;
        template<typename Rep, int Exponent, int Radix>
        struct split<Rep, Exponent, Radix, false> {
        private:
            using value_type = fixed_point<Rep, Exponent, Radix>;
            constexpr auto integral(value_type const& scalar) const
            -> decltype(scale<Exponent, Radix>(to_rep(scalar)))
            {
                return scale<Exponent, Radix>(to_rep(scalar));
            }
            template<typename Integral>
            static auto from_integral_and_value(Integral const& integral, value_type const& value)
            -> decltype(std::make_pair(integral, value-integral))
            {
                return std::make_pair(integral, value-integral);
            }
        public:
            constexpr auto operator()(value_type const& value) const
            -> decltype(from_integral_and_value(integral(value), value))
            {
                return from_integral_and_value(integral(value), value);
            }
        };
        template<typename Rep, int Exponent, int Radix>
        struct split<Rep, Exponent, Radix, true> {
            using value_type = fixed_point<Rep, Exponent, Radix>;
            constexpr auto operator()(value_type const& value) const
            -> decltype(std::make_pair(Rep{}, value))
            {
                return std::make_pair(Rep{}, value);
            }
        };
        template<typename Rep, int Exponent, int Radix>
        auto to_chars_fractional_specialized(
                char* first,
                char const* const last,
                fixed_point<Rep, Exponent, Radix> value) noexcept
        -> enable_if_t<integer_digits<fixed_point<Rep, Exponent, Radix>>::value>=4, char*>
        {
            do {
                using fixed_point = fixed_point<Rep, Exponent, Radix>;
                ((value<=numeric_limits<fixed_point>::max()/Rep{10}) ? static_cast<void>(0) : __builtin_unreachable());
                value = from_rep<fixed_point>(
                        cnl::fixed_width_scale<1, 10, Rep>{}(to_rep(value)));
                auto const split = _impl::split<Rep, Exponent, Radix>{}(value);
                *first = itoc(split.first);
                ++first;
                value = split.second;
                if (!value) {
                    break;
                }
            }
            while (first!=last);
            return first;
        }
        template<typename Rep, int Exponent, int Radix>
        auto to_chars_fractional_specialized(
                char* const first,
                char* last,
                fixed_point<Rep, Exponent, Radix> const& value) noexcept
        -> enable_if_t<integer_digits<fixed_point<Rep, Exponent, Radix>>::value<4, char*>
        {
            std::fill<char*>(first, last, '0');
            auto const digits = std::distance(first, last);
            std::array<char, (Exponent*-302LL)/100> bit{ };
            ((std::ptrdiff_t(bit.size())>=digits) ? static_cast<void>(0) : __builtin_unreachable());
            bit[0] = 5;
            for (auto mask = fixed_point<Rep, Exponent, Radix>{ .5 };;) {
                if (value & mask) {
                    auto carry = 0;
                    for (auto pos = digits-1; pos>=0; --pos) {
                        *(first+pos) = char(*(first+pos)+bit[pos]+carry);
                        if (*(first+pos)>'9') {
                            *(first+pos) = char(*(first+pos)-10);
                            carry = 1;
                        }
                        else {
                            carry = 0;
                        }
                    }
                    ((carry==0) ? static_cast<void>(0) : __builtin_unreachable());
                }
                mask >>= 1;
                if (!mask) {
                    break;
                }
                for (auto digit = std::end(bit)-1; digit>std::begin(bit); --digit) {
                    auto const before = digit[-1];
                    digit[-1] = char(before >> 1);
                    if (before & 1) {
                        digit[0] = char(digit[0]+5);
                        ((digit[0]<10) ? static_cast<void>(0) : __builtin_unreachable());
                    }
                }
            }
            return last;
        }
        template<typename Rep, int Exponent, int Radix>
        auto to_chars_fractional(
                char* first,
                char* last,
                fixed_point<Rep, Exponent, Radix> const& value) noexcept
        -> to_chars_result
        {
            auto const destination_length = std::distance(first, last);
            if (destination_length<2) {
                return to_chars_result{first, std::errc{}};
            }
            *first = '.';
            first++;
            last = to_chars_fractional_specialized(first, last, value);
            for (;;) {
                auto const prev = last[-1];
                if (prev!='0') {
                    if (prev=='.') {
                        --last;
                    }
                    break;
                }
                --last;
            }
            return to_chars_result{last, std::errc{}};
        }
        template<typename Rep, int Exponent, int Radix>
        to_chars_result to_chars_positive(
                char* const first,
                char* const last,
                fixed_point<Rep, Exponent, Radix> const& value) noexcept
        {
            auto const split = _impl::split<Rep, Exponent, Radix>{}(value);
            auto const natural_last = to_chars_natural(first, last, split.first);
            if (!natural_last) {
                return to_chars_result{last, std::errc::value_too_large};
            }
            if (!split.second) {
                return to_chars_result{natural_last, std::errc{}};
            }
            return to_chars_fractional(natural_last, last, split.second);
        }
    }
    template<typename Rep, int Exponent, int Radix>
    to_chars_result to_chars(
            char* const first,
            char* const last,
            cnl::fixed_point<Rep, Exponent, Radix> const& value)
    {
        if (!value) {
            if (first==last) {
                return to_chars_result{last, std::errc::value_too_large};
            }
            *first = '0';
            return to_chars_result{first+1, std::errc{}};
        }
        using native_rounding_type = set_rounding_t<decltype(value), _impl::native_rounding_tag>;
        auto const& native_rounding_value = static_cast<native_rounding_type>(value);
        return _impl::to_chars_non_zero<native_rounding_type>{}(
                first, last, native_rounding_value);
    }
}
namespace cnl {
    namespace _impl {
        template<class Result>
                              Result unreachable(char const* ) noexcept
        {
            __builtin_unreachable();
        }
    }
}
namespace cnl {
    template<typename Rep, int Exponent, int Radix>
    constexpr auto abs(fixed_point<Rep, Exponent, Radix> const& x) noexcept
    -> decltype(-x)
    {
        return (x>=fixed_point<Rep, Exponent, Radix>{}) ? static_cast<decltype(-x)>(x) : -x;
    }
    namespace _impl {
        template<class Rep>
        constexpr Rep sqrt_solve3(
                Rep n,
                Rep bit,
                Rep result)
        {
            return (bit!=Rep{0})
                   ? (n>=result+bit)
                     ? sqrt_solve3<Rep>(
                                    static_cast<Rep>(n-(result+bit)),
                                    static_cast<Rep>(bit >> 2),
                                    static_cast<Rep>((result >> 1)+bit))
                     : sqrt_solve3<Rep>(
                                    n,
                                    static_cast<Rep>(bit >> 2),
                                    static_cast<Rep>(result >> 1))
                   : result;
        }
        template<int Exponent>
        struct sqrt_solve1 {
            template<class Rep>
            constexpr Rep operator()(Rep n) const
            {
                using widened_rep = _impl::set_width_t<Rep, _impl::width<Rep>::value*2>;
                return static_cast<Rep>(sqrt_solve3<widened_rep>(
                        _impl::fixed_width_scale<-Exponent>(static_cast<widened_rep>(n)),
                        widened_rep((widened_rep{1}<<((countr_used(n)+1-Exponent)&~1))>>2),
                        widened_rep{0}));
            }
        };
    }
    template<typename Rep, int Exponent, int Radix>
    constexpr fixed_point <Rep, Exponent, Radix>
    sqrt(fixed_point<Rep, Exponent, Radix> const& x)
    {
        using type = fixed_point<Rep, Exponent, Radix>;
        return _impl::to_rep(x)<0
               ? _impl::unreachable<type>("negative value passed to cnl::sqrt")
               : type{_impl::from_rep<type>(_impl::sqrt_solve1<Exponent>{}(unwrap(x)))};
    }
    template<class Rep, int Exponent, int Radix,
            _impl::enable_if_t<(Exponent<0), int> dummy = 0>
    constexpr auto floor(fixed_point<Rep, Exponent, Radix> const& x)
    -> decltype(_impl::from_rep<fixed_point<Rep, 0, Radix>>(_impl::to_rep(x)>>constant<-Exponent>())) {
        static_assert(
                Radix==2,
                "cnl::floor(fixed_point<Rep, Exponent, Radix>) not implemented for Exponent<0 && Radix!=2");
        return _impl::from_rep<fixed_point<Rep, 0, Radix>>(_impl::to_rep(x)>>constant<-Exponent>());
    }
    template<class Rep, int Exponent, int Radix>
    constexpr auto floor(fixed_point<Rep, Exponent, Radix> const& x)
    -> _impl::enable_if_t<Exponent>=0, fixed_point<Rep, Exponent, Radix>> {
        return x;
    }
    namespace _impl {
        template<int NumBits, class Enable = void>
        struct float_of_size;
        template<int NumBits>
        struct float_of_size<NumBits, enable_if_t<NumBits <= sizeof(float)*8>> {
            using type = float;
        };
        template<int NumBits>
        struct float_of_size<NumBits, enable_if_t<sizeof(float)*8 < NumBits && NumBits <= sizeof(double)*8>> {
            using type = double;
        };
        template<int NumBits>
        struct float_of_size<NumBits, enable_if_t<sizeof(double)*8 < NumBits && NumBits <= sizeof(long double)*8>> {
            using type = long double;
        };
        template<class T>
        using float_of_same_size = typename float_of_size<_impl::width<T>::value>::type;
        template<typename Rep, int Exponent, int Radix, _impl::float_of_same_size<Rep>(* F)(
                _impl::float_of_same_size<Rep>)>
        constexpr fixed_point <Rep, Exponent, Radix>
        crib(fixed_point<Rep, Exponent, Radix> const& x) noexcept
        {
            using floating_point = _impl::float_of_same_size<Rep>;
            return static_cast<fixed_point<Rep, Exponent, Radix>>(F(static_cast<floating_point>(x)));
        }
    }
    template<typename Rep, int Exponent, int Radix>
    constexpr fixed_point <Rep, Exponent, Radix>
    sin(fixed_point<Rep, Exponent, Radix> const& x) noexcept
    {
        return _impl::crib<Rep, Exponent, Radix, std::sin>(x);
    }
    template<typename Rep, int Exponent, int Radix>
    constexpr fixed_point <Rep, Exponent, Radix>
    cos(fixed_point<Rep, Exponent, Radix> const& x) noexcept
    {
        return _impl::crib<Rep, Exponent, Radix, std::cos>(x);
    }
    template<typename Rep, int Exponent, int Radix>
    constexpr fixed_point <Rep, Exponent, Radix>
    exp(fixed_point<Rep, Exponent, Radix> const& x) noexcept
    {
        return _impl::crib<Rep, Exponent, Radix, std::exp>(x);
    }
    template<typename Rep, int Exponent, int Radix>
    constexpr fixed_point <Rep, Exponent, Radix>
    pow(fixed_point<Rep, Exponent, Radix> const& x) noexcept
    {
        return _impl::crib<Rep, Exponent, Radix, std::pow>(x);
    }
    template<typename Rep, int Exponent, int Radix>
    ::std::ostream& operator<<(::std::ostream& out, fixed_point<Rep, Exponent, Radix> const& fp)
    {
        return out << to_chars(fp).data();
    }
    template<typename Rep, int Exponent, int Radix>
    ::std::istream& operator>>(::std::istream& in, fixed_point <Rep, Exponent, Radix>& fp)
    {
        long double ld;
        in >> ld;
        fp = ld;
        return in;
    }
}
namespace cnl {
    template<typename Rep, int Exponent, int Radix>
    struct numeric_limits<cnl::fixed_point<Rep, Exponent, Radix>>
            : numeric_limits<cnl::_impl::number_base<cnl::fixed_point<Rep, Exponent, Radix>, Rep>> {
        using _value_type = cnl::fixed_point<Rep, Exponent, Radix>;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;
        static constexpr _value_type min() noexcept
        {
            return _impl::from_rep<_value_type>(_rep{1});
        }
        static constexpr _value_type max() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::max());
        }
        static constexpr _value_type lowest() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::lowest());
        }
        static constexpr bool is_integer = false;
        static constexpr _value_type epsilon() noexcept
        {
            return _impl::from_rep<_value_type>(_rep{1});
        }
        static constexpr _value_type round_error() noexcept
        {
            return _impl::from_rep<_value_type>(_rep{0});
        }
        static constexpr _value_type infinity() noexcept
        {
            return _impl::from_rep<_value_type>(_rep{0});
        }
        static constexpr _value_type quiet_NaN() noexcept
        {
            return _impl::from_rep<_value_type>(_rep{0});
        }
        static constexpr _value_type signaling_NaN() noexcept
        {
            return _impl::from_rep<_value_type>(_rep{0});
        }
        static constexpr _value_type denorm_min() noexcept
        {
            return _impl::from_rep<_value_type>(_rep{1});
        }
    };
}
namespace std {
    template<typename Rep, int Exponent, int Radix>
    struct numeric_limits<cnl::fixed_point<Rep, Exponent, Radix>>
            : cnl::numeric_limits<cnl::fixed_point<Rep, Exponent, Radix>> {
    };
    template<typename Rep, int Exponent, int Radix>
    struct numeric_limits<cnl::fixed_point<Rep, Exponent, Radix> const>
            : cnl::numeric_limits<cnl::fixed_point<Rep, Exponent, Radix>> {
    };
}
namespace cnl {
    using std::to_string;
    template<typename Rep, int Exponent>
    std::string to_string(cnl::fixed_point<Rep, Exponent> const& value)
    {
        auto chars = to_chars(value);
        return chars.data();
    }
}
namespace cnl {
    template<int Digits, int Exponent = 0, class Narrowest = signed>
    using elastic_number = fixed_point<elastic_integer<Digits, Narrowest>, Exponent>;
    template<
            typename Narrowest = int,
            ::cnl::intmax Value = 0>
    constexpr elastic_number<
            _impl::max(digits<constant<Value>>::value-trailing_bits(Value), 1),
            trailing_bits(Value),
            Narrowest>
    make_elastic_number(constant<Value>)
    {
        return Value;
    }
    template<typename Narrowest = void, typename Integral = int>
    constexpr elastic_number<
            numeric_limits<Integral>::digits,
            0,
            typename std::conditional<
                    std::is_same<void, Narrowest>::value,
                    _impl::adopt_signedness_t<int, Integral>,
                    Narrowest>::type>
    make_elastic_number(Integral const& value)
    {
        return {value};
    }
    template<typename Narrowest = void, typename Rep = int, int Exponent = 0, int Radix = 2>
    constexpr elastic_number<
            numeric_limits<Rep>::digits,
            Exponent,
            typename std::conditional<
                    std::is_same<void, Narrowest>::value,
                    _impl::adopt_signedness_t<int, Rep>,
                    Narrowest>::type>
    make_elastic_number(fixed_point<Rep, Exponent, Radix> const& value)
    {
        return {value};
    }
    namespace literals {
        template<char... Chars>
        constexpr auto operator "" _elastic()
        -> decltype(make_elastic_number<int>(
                constant<_cnlint_impl::parse<sizeof...(Chars)+1>({Chars..., '\0'})>{}))
        {
            return make_elastic_number<int>(
                    constant<_cnlint_impl::parse<sizeof...(Chars)+1>({Chars..., '\0'})>{});
        }
    }
}
namespace cnl {
    namespace _impl {
        template<typename Numerator, typename Denominator>
        constexpr fraction<Numerator, Denominator> make_fraction(Numerator const& n, Denominator const& d)
        {
            return fraction<Numerator, Denominator>{n, d};
        }
        template<typename Numerator>
        constexpr fraction<Numerator, Numerator> make_fraction(Numerator const& n)
        {
            return fraction<Numerator, Numerator>{n, 1};
        }
    }
}
namespace cnl {
    namespace _impl {
        template<typename N, typename D>
        constexpr auto abs(fraction<N, D> const& f)
        -> decltype(make_fraction(abs(f.numerator), abs(f.denominator)))
        {
            return make_fraction(abs(f.numerator), abs(f.denominator));
        }
    }
}
namespace cnl {
    namespace _impl {
    }
}
namespace cnl {
    namespace _impl {
    }
}
namespace cnl {
    namespace _impl {
    }
}
namespace cnl {
    template<typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator+(fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(_impl::make_fraction(+rhs.numerator, +rhs.denominator))
    {
        return _impl::make_fraction(+rhs.numerator, +rhs.denominator);
    }
    template<typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator-(fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(_impl::make_fraction(-rhs.numerator, -rhs.denominator))
    {
        return _impl::make_fraction(-rhs.numerator, -rhs.denominator);
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator+(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(_impl::make_fraction(
            lhs.numerator*rhs.denominator+rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator))
    {
        return _impl::make_fraction(
                lhs.numerator*rhs.denominator+rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator);
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator-(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(_impl::make_fraction(
            lhs.numerator*rhs.denominator-rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator))
    {
        return _impl::make_fraction(
                lhs.numerator*rhs.denominator-rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator);
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator*(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(_impl::make_fraction(lhs.numerator*rhs.numerator, lhs.denominator*rhs.denominator))
    {
        return _impl::make_fraction(lhs.numerator*rhs.numerator, lhs.denominator*rhs.denominator);
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator/(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(_impl::make_fraction(lhs.numerator*rhs.denominator, lhs.denominator*rhs.numerator))
    {
        return _impl::make_fraction(lhs.numerator*rhs.denominator, lhs.denominator*rhs.numerator);
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator==(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(lhs.numerator*rhs.denominator==rhs.numerator*lhs.denominator)
    {
        return lhs.numerator*rhs.denominator==rhs.numerator*lhs.denominator;
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator!=(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(lhs.numerator*rhs.denominator!=rhs.numerator*lhs.denominator)
    {
        return lhs.numerator*rhs.denominator!=rhs.numerator*lhs.denominator;
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator<(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(lhs.numerator*rhs.denominator<rhs.numerator*lhs.denominator)
    {
        return lhs.numerator*rhs.denominator<rhs.numerator*lhs.denominator;
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator>(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(lhs.numerator*rhs.denominator>rhs.numerator*lhs.denominator)
    {
        return lhs.numerator*rhs.denominator>rhs.numerator*lhs.denominator;
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator<=(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(lhs.numerator*rhs.denominator<=rhs.numerator*lhs.denominator)
    {
        return lhs.numerator*rhs.denominator<=rhs.numerator*lhs.denominator;
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator>=(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(lhs.numerator*rhs.denominator>=rhs.numerator*lhs.denominator)
    {
        return lhs.numerator*rhs.denominator>=rhs.numerator*lhs.denominator;
    }
}
namespace cnl {
    namespace _impl {
        using std::to_string;
        template<typename N, typename D>
        std::string to_string(fraction<N, D> const& f)
        {
            auto const numerator_string = to_string(f.numerator);
            auto const denominator_string = to_string(f.denominator);
            auto const total_length = numerator_string.length()+1+denominator_string.length();
            std::string fraction_string;
            fraction_string.reserve(total_length);
            fraction_string = numerator_string;
            fraction_string.push_back('/');
            fraction_string += denominator_string;
            return fraction_string;
        }
    }
}
namespace cnl {
    using _impl::abs;
    using _impl::make_fraction;
    using _impl::to_string;
}
namespace cnl {
    namespace _impl {
        template<typename Number, typename Rep, typename Enable = void>
        struct wrap;
        template<typename Number, typename Rep>
        struct wrap<Number, Rep, enable_if_t<!is_composite<Number>::value>> {
            constexpr Number operator()(Rep const& number) const
            {
                return number;
            }
        };
        template<typename Number, typename Rep>
        struct wrap<Number, Rep, enable_if_t<is_composite<Number>::value>> {
            constexpr auto operator()(Rep const& rep) const
            -> decltype(from_rep<Number>(wrap<to_rep_t<Number>, Rep>{}(rep)))
            {
                return from_rep<Number>(wrap<to_rep_t<Number>, Rep>{}(rep));
            }
        };
    }
    template<typename Number, typename Rep>
    constexpr auto wrap(Rep const& rep)
    -> decltype(_impl::wrap<Number, Rep>{}(rep))
    {
        return _impl::wrap<Number, Rep>{}(rep);
    }
}
namespace cnl {
    using _impl::is_integral;
}
namespace cnl {
    namespace _impl {
        template<class T>
        struct positive_digits : public std::integral_constant<int, numeric_limits<T>::digits> {
        };
        template<class T>
        struct negative_digits
                : public std::integral_constant<int, cnl::is_signed<T>::value ? digits<T>::value : 0> {
        };
        template<typename Destination, typename Source, typename Enable = void>
        struct convert_test;
        template<typename Destination, typename Source>
        struct convert_test<Destination, Source, enable_if_t<!std::is_floating_point<Source>::value>> {
            static constexpr bool positive(Source const &rhs) {
                return positive_digits<Destination>::value<positive_digits<Source>::value
                                                           && rhs>
                static_cast<Source>(numeric_limits<Destination>::max());
            }
            static constexpr bool negative(Source const &rhs) {
                return negative_digits<Destination>::value<negative_digits<Source>::value
                                                           && rhs <
                                                              static_cast<Source>(numeric_limits<Destination>::lowest());
            }
        };
        template<typename Destination, typename Source>
        struct convert_test<Destination, Source, enable_if_t<std::is_floating_point<Source>::value>> {
            static constexpr bool positive(Source const& rhs)
            {
                return rhs > static_cast<Source>(numeric_limits<Destination>::max());
            }
            static constexpr bool negative(Source const& rhs)
            {
                return rhs < static_cast<Source>(numeric_limits<Destination>::lowest());
            }
        };
        template<class Operator, typename ... Operands>
        struct operator_overflow_traits {
            using result = op_result<Operator, Operands...>;
            using numeric_limits = cnl::numeric_limits<result>;
            static constexpr int positive_digits = _impl::positive_digits<result>::value;
            static constexpr int negative_digits = _impl::negative_digits<result>::value;
            static constexpr result lowest()
            {
                return numeric_limits::lowest();
            }
            static constexpr result max()
            {
                return numeric_limits::max();
            }
            template<typename Operand>
            static constexpr int leading_bits(Operand const& operand)
            {
                return cnl::leading_bits(static_cast<result>(operand));
            }
        };
        template<typename Operand, bool IsSigned=is_signed<Operand>::value>
        struct has_most_negative_number : std::false_type {};
        template<typename Operand>
        struct has_most_negative_number<Operand, true> : std::integral_constant<bool,
                numeric_limits<Operand>::lowest()<-numeric_limits<Operand>::max()> {
        };
        template<class Operator, typename ... Operands>
        struct overflow_test_base {
            static constexpr bool positive(Operands const &...)
            {
                return false;
            }
            static constexpr bool negative(Operands const &...)
            {
                return false;
            }
        };
        template<class Operator, typename ... Operands>
        struct overflow_test : overflow_test_base<Operator, Operands...> {
        };
        template<typename Rhs>
        struct overflow_test<minus_op, Rhs> : overflow_test_base<minus_op, Rhs> {
            using traits = operator_overflow_traits<minus_op, Rhs>;
            static constexpr bool positive(Rhs const &rhs) {
                return has_most_negative_number<Rhs>::value
                        ? (-traits::max()) > rhs
                        : 0;
            }
            static constexpr bool negative(Rhs const &rhs) {
                return !is_signed<Rhs>::value && rhs;
            }
        };
        template<typename Lhs, typename Rhs>
        struct overflow_test<add_op, Lhs, Rhs> : overflow_test_base<add_op, Lhs, Rhs> {
            using traits = operator_overflow_traits<add_op, Lhs, Rhs>;
            static constexpr bool positive(Lhs const& lhs, Rhs const& rhs)
            {
                return (max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >traits::positive_digits)
                        && lhs>Lhs{0}
                        && rhs>Rhs{0}
                        && typename traits::result(lhs)>traits::max()-rhs;
            }
            static constexpr bool negative(Lhs const& lhs, Rhs const& rhs)
            {
                return (max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >traits::positive_digits)
                        && lhs<Lhs{0}
                        && rhs<Rhs{0}
                        && typename traits::result(lhs)<traits::lowest()-rhs;
            }
        };
        template<typename Lhs, typename Rhs>
        struct overflow_test<subtract_op, Lhs, Rhs> : overflow_test_base<subtract_op, Lhs, Rhs> {
            using traits = operator_overflow_traits<subtract_op, Lhs, Rhs>;
 static constexpr bool positive(Lhs const& lhs, Rhs const& rhs)
            {
                return (max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >traits::positive_digits)
                        && lhs>Lhs{0}
                        && rhs<Rhs{0}
                        && lhs>traits::max()+rhs;
            }
            static constexpr bool negative(Lhs const& lhs, Rhs const& rhs)
            {
                return (max(positive_digits<Lhs>::value, positive_digits<Rhs>::value)+1
                        >traits::positive_digits)
                        && (rhs>=0)
                        && lhs<traits::lowest()+rhs;
            }
 };
        template<typename Lhs, typename Rhs>
        struct overflow_test<multiply_op, Lhs, Rhs> : overflow_test_base<multiply_op, Lhs, Rhs> {
            using traits = operator_overflow_traits<multiply_op, Lhs, Rhs>;
            static constexpr bool positive(Lhs const& lhs, Rhs const& rhs)
            {
                return (positive_digits<Lhs>::value+positive_digits<Rhs>::value>traits::positive_digits)
                        && ((lhs>Lhs{0})
                                ? (rhs>Rhs{0}) && (traits::max()/rhs)<lhs
                                : (rhs<Rhs{0}) && (traits::max()/rhs)>lhs);
            }
            static constexpr bool negative(Lhs const& lhs, Rhs const& rhs)
            {
                return (positive_digits<Lhs>::value+positive_digits<Rhs>::value>traits::positive_digits)
                        && ((lhs<Lhs{0})
                                ? (rhs>Rhs{0}) && (traits::lowest()/rhs)>lhs
                                : (rhs<Rhs{0}) && (traits::lowest()/rhs)<lhs);
            }
        };
        template<typename Lhs, typename Rhs>
        struct overflow_test<divide_op, Lhs, Rhs>
                : overflow_test_base<divide_op, Lhs, Rhs> {
            using traits = operator_overflow_traits<divide_op, Lhs, Rhs>;
            static constexpr bool positive(Lhs const &lhs, Rhs const &rhs) {
                return (has_most_negative_number<Lhs>::value && has_most_negative_number<Rhs>::value)
                        ? rhs == -1 && lhs == traits::lowest()
                        : false;
            }
        };
        template<typename Lhs, typename Rhs, bool IsSigned>
        struct shift_left_overflow_test;
        template<typename Lhs, typename Rhs>
        struct shift_left_overflow_test<Lhs, Rhs, false>
                : overflow_test_base<Lhs, Rhs> {
            using traits = operator_overflow_traits<shift_left_op, Lhs, Rhs>;
            static constexpr bool negative(Lhs const&, Rhs const&)
            {
                return false;
            }
        };
        template<typename Lhs, typename Rhs>
        struct shift_left_overflow_test<Lhs, Rhs, true>
                : overflow_test_base<Lhs, Rhs> {
            using traits = operator_overflow_traits<shift_left_op, Lhs, Rhs>;
            static constexpr bool negative(Lhs const& lhs, Rhs const& rhs)
            {
                return lhs<0
                        ? rhs>0
                                ? rhs<traits::positive_digits
                                        ? (lhs >> (traits::positive_digits-rhs))!=-1
                                        : true
                                : false
                        : false;
            }
        };
        template<typename Lhs, typename Rhs>
        struct overflow_test<shift_left_op, Lhs, Rhs>
                : shift_left_overflow_test<Lhs, Rhs, is_signed<Lhs>::value> {
            using traits = operator_overflow_traits<shift_left_op, Lhs, Rhs>;
            static constexpr bool positive(Lhs const& lhs, Rhs const& rhs)
            {
                return lhs>0
                        ? rhs>0
                                ? rhs<traits::positive_digits
                                        ? (lhs >> (traits::positive_digits-rhs))!=0
                                        : true
                                : false
                        : false;
            }
        };
        template<typename Result, class OverflowTag>
        struct positive_overflow_result;
        template<typename Result, class OverflowTag>
        struct negative_overflow_result;
        template<class OverflowTag, typename Result, typename Input>
        struct overflow_convert {
            constexpr Result operator()(Input const& rhs) const
            {
                using test = convert_test<Result, Input>;
                return test::positive(rhs)
                        ? positive_overflow_result<Result, OverflowTag>{}()
                        : test::negative(rhs)
                                ? negative_overflow_result<Result, OverflowTag>{}()
                                : static_cast<Result>(rhs);
            }
        };
        template<class OverflowTag, class Operator>
        struct tagged_binary_overflow_operator {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> op_result<Operator, Lhs, Rhs>
            {
                using overflow_test = overflow_test<Operator, Lhs, Rhs>;
                using result_type = op_result<Operator, Lhs, Rhs>;
                return overflow_test::positive(lhs, rhs)
                        ? positive_overflow_result<result_type, OverflowTag>{}()
                        : overflow_test::negative(lhs, rhs)
                                ? negative_overflow_result<result_type, OverflowTag>{}()
                                : tagged_binary_operator<native_tag, Operator>{}(lhs, rhs);
            }
        };
    }
}
namespace cnl {
    struct native_overflow_tag : _impl::native_tag {
    };
    static constexpr native_overflow_tag native_overflow{};
    template<typename Result, typename Input>
    struct convert<native_overflow_tag, Result, Input>
            : convert<_impl::native_tag, Result, Input> {
    };
    namespace _impl {
        template<class Operator>
        struct tagged_binary_operator<native_overflow_tag, Operator>
                : tagged_binary_operator<_impl::native_tag, Operator> {
        };
    }
}
namespace cnl {
    static constexpr struct saturated_overflow_tag {
    } saturated_overflow{};
    namespace _impl {
        template<typename Result>
        struct positive_overflow_result<Result, saturated_overflow_tag> {
            constexpr Result operator()() const
            {
                return numeric_limits<Result>::max();
            }
        };
        template<typename Result>
        struct negative_overflow_result<Result, saturated_overflow_tag> {
            constexpr Result operator()() const
            {
                return numeric_limits<Result>::lowest();
            }
        };
    }
    template<typename Result, typename Input>
    struct convert<saturated_overflow_tag, Result, Input>
            : _impl::overflow_convert<saturated_overflow_tag, Result, Input> {
    };
    namespace _impl {
        template<class Operator>
        struct tagged_binary_operator<saturated_overflow_tag, Operator>
                : tagged_binary_overflow_operator<saturated_overflow_tag, Operator> {
        };
    }
}
namespace cnl {
    namespace _impl {
        template<typename Result, class Exception>
        Result throw_exception(char const* message)
        {
            return true ? throw Exception(message) : Result{};
        }
    }
}
namespace cnl {
    static constexpr struct throwing_overflow_tag {
    } throwing_overflow{};
    namespace _impl {
        template<typename Result>
        struct positive_overflow_result<Result, throwing_overflow_tag> {
            constexpr Result operator()() const
            {
                return throw_exception<Result, std::overflow_error>("positive overflow");
            }
        };
        template<typename Result>
        struct negative_overflow_result<Result, throwing_overflow_tag> {
            constexpr Result operator()() const
            {
                return throw_exception<Result, std::overflow_error>("negative overflow");
            }
        };
    }
    template<typename Result, typename Input>
    struct convert<throwing_overflow_tag, Result, Input>
            : _impl::overflow_convert<throwing_overflow_tag, Result, Input> {
    };
    namespace _impl {
        template<class Operator>
        struct tagged_binary_operator<throwing_overflow_tag, Operator>
                : tagged_binary_overflow_operator<throwing_overflow_tag, Operator> {
        };
    }
}
namespace cnl {
    static constexpr struct trapping_overflow_tag {
    } trapping_overflow{};
    namespace _impl {
        template<typename Result>
        struct positive_overflow_result<Result, trapping_overflow_tag> {
            constexpr Result operator()() const
            {
                return terminate<Result>("positive overflow");
            }
        };
        template<typename Result>
        struct negative_overflow_result<Result, trapping_overflow_tag> {
            constexpr Result operator()() const
            {
                return terminate<Result>("negative overflow");
            }
        };
    }
    template<typename Result, typename Input>
    struct convert<trapping_overflow_tag, Result, Input>
            : _impl::overflow_convert<trapping_overflow_tag, Result, Input> {
    };
    namespace _impl {
        template<class Operator>
        struct tagged_binary_operator<trapping_overflow_tag, Operator>
                : tagged_binary_overflow_operator<trapping_overflow_tag, Operator> {
        };
    }
}
namespace cnl {
    static constexpr struct undefined_overflow_tag {
    } undefined_overflow{};
    namespace _impl {
        template<typename Result>
        struct positive_overflow_result<Result, undefined_overflow_tag> {
            constexpr Result operator()() const
            {
                return unreachable<Result>("positive overflow");
            }
        };
        template<typename Result>
        struct negative_overflow_result<Result, undefined_overflow_tag> {
            constexpr Result operator()() const
            {
                return unreachable<Result>("negative overflow");
            }
        };
    }
    template<typename Result, typename Input>
    struct convert<undefined_overflow_tag, Result, Input>
            : _impl::overflow_convert<undefined_overflow_tag, Result, Input> {
    };
    namespace _impl {
        template<class Operator>
        struct tagged_binary_operator<undefined_overflow_tag, Operator>
                : tagged_binary_overflow_operator<undefined_overflow_tag, Operator> {
        };
    }
}
namespace cnl {
    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto add(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs+rhs)
    {
        return _impl::tagged_binary_operator<OverflowTag, _impl::add_op>{}(lhs, rhs);
    }
    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto subtract(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs-rhs)
    {
        return _impl::tagged_binary_operator<OverflowTag, _impl::subtract_op>{}(lhs, rhs);
    }
    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto multiply(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs*rhs)
    {
        return _impl::tagged_binary_operator<OverflowTag, _impl::multiply_op>{}(lhs, rhs);
    }
    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto shift_left(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs<<rhs)
    {
        return _impl::tagged_binary_operator<OverflowTag, _impl::shift_left_op>{}(
                unwrap(lhs), unwrap(rhs));
    }
}
namespace cnl {
    namespace _impl {
        template<class A, class B>
        constexpr bool identical(A const& a, B const& b)
        {
            static_assert(std::is_same<A, B>::value, "different types");
            return a==b;
        }
    }
}
namespace cnl {
    template<class Rep, class OverflowTag>
    class overflow_integer;
    namespace _integer_impl {
        template<class T>
        struct is_overflow_integer
                : std::false_type {
        };
        template<class Rep, class OverflowTag>
        struct is_overflow_integer<overflow_integer<Rep, OverflowTag>>
                : std::true_type {
        };
    }
    template<class Rep = int, class OverflowTag = undefined_overflow_tag>
    class overflow_integer : public _impl::number_base<overflow_integer<Rep, OverflowTag>, Rep> {
        static_assert(!_integer_impl::is_overflow_integer<Rep>::value,
                "overflow_integer of overflow_integer is not a supported");
    public:
        using rep = Rep;
        using overflow_tag = OverflowTag;
        using _base = _impl::number_base<overflow_integer<Rep, OverflowTag>, Rep>;
        overflow_integer() = default;
        template<class RhsRep, class RhsOverflowTag>
        constexpr overflow_integer(overflow_integer<RhsRep, RhsOverflowTag> const& rhs)
                :overflow_integer(_impl::to_rep(rhs))
        {
        }
        template<class Rhs, _impl::enable_if_t<!_integer_impl::is_overflow_integer<Rhs>::value, int> dummy = 0>
        constexpr overflow_integer(Rhs const& rhs)
                :_base(_impl::convert<overflow_tag, rep>(rhs))
        {
        }
        template< ::cnl::intmax Value>
        constexpr overflow_integer(constant<Value>)
                : _base(static_cast<rep>(Value))
        {
            static_assert(Value <= numeric_limits<rep>::max(), "initialization by out-of-range value");
            static_assert(!numeric_limits<decltype(Value)>::is_signed || Value >= numeric_limits<rep>::lowest(), "initialization by out-of-range value");
        }
        template<class T>
        constexpr explicit operator T() const
        {
            return static_cast<T>(_impl::to_rep(*this));
        }
    };
    template<class Rep, class OverflowTag>
    struct digits<overflow_integer<Rep, OverflowTag>> : digits<Rep> {
    };
    template<class Rep, class OverflowTag, int MinNumBits>
    struct set_digits<overflow_integer<Rep, OverflowTag>, MinNumBits> {
        using type = overflow_integer<set_digits_t<Rep, MinNumBits>, OverflowTag>;
    };
    template<typename ArchetypeRep, class OverflowTag, typename Rep>
    struct from_rep<overflow_integer<ArchetypeRep, OverflowTag>, Rep> {
        constexpr auto operator()(Rep const& r) const
        -> overflow_integer<Rep, OverflowTag>
        {
            return r;
        }
    };
    template<class Rep, class OverflowTag, class Value>
    struct from_value<overflow_integer<Rep, OverflowTag>, Value>
            : _impl::from_value_simple<overflow_integer<Value, OverflowTag>, Value> {
    };
    template<class Rep, class OverflowTag, class ValueRep, class ValueOverflowTag>
    struct from_value<overflow_integer<Rep, OverflowTag>, overflow_integer<ValueRep, ValueOverflowTag>>
            : _impl::from_value_simple<
                    overflow_integer<
                            from_value_t<Rep, ValueRep>,
                            _impl::common_type_t<OverflowTag, ValueOverflowTag>>,
                    overflow_integer<ValueRep, ValueOverflowTag>> {
    };
    template<class Rep, class OverflowTag, ::cnl::intmax Value>
    struct from_value<overflow_integer<Rep, OverflowTag>, constant<Value>>
            : _impl::from_value_simple<overflow_integer<
                    typename std::conditional<
                            digits<int>::value<_impl::used_digits(Value), decltype(Value), int>::type, OverflowTag>,
                    constant<Value>>{
    };
    template<int Digits, class Rep, class OverflowTag>
    struct scale<Digits, 2, overflow_integer<Rep, OverflowTag>,
            _impl::enable_if_t<(Digits>=0)>> {
        using _value_type = overflow_integer<Rep, OverflowTag>;
        constexpr auto operator()(_value_type const& s) const
        -> decltype(_impl::from_rep<_value_type>(shift_left(OverflowTag{}, _impl::to_rep(s), constant<Digits>{})))
        {
            return _impl::from_rep<_value_type>(shift_left(OverflowTag{}, _impl::to_rep(s), constant<Digits>{}));
        }
    };
    template<int Digits, int Radix, class Rep, class OverflowTag>
    struct scale<Digits, Radix, overflow_integer<Rep, OverflowTag>,
            _impl::enable_if_t<(Digits<0||Radix!=2)>>
            : scale<Digits, Radix, _impl::number_base<overflow_integer<Rep, OverflowTag>, Rep>> {
    };
    template<class OverflowTag, class Rep>
    constexpr auto make_overflow_int(Rep const& value)
    -> overflow_integer<Rep, OverflowTag>
    {
        return value;
    }
    namespace _impl {
        template<class Operator, class Rep, class OverflowTag>
        struct unary_operator<Operator, overflow_integer<Rep, OverflowTag>> {
            constexpr auto operator()(overflow_integer<Rep, OverflowTag> const& operand) const
            -> decltype(overflow_integer<decltype(Operator()(_impl::to_rep(operand))), OverflowTag>(Operator()(_impl::to_rep(operand))))
            {
                return overflow_integer<decltype(Operator()(_impl::to_rep(operand))), OverflowTag>(Operator()(_impl::to_rep(operand)));
            }
        };
        template<class Operator, class LhsRep, class RhsRep, class OverflowTag>
        struct binary_operator<Operator,
                overflow_integer<LhsRep, OverflowTag>, overflow_integer<RhsRep, OverflowTag>> {
            constexpr auto operator()(
                    overflow_integer<LhsRep, OverflowTag> const& lhs,
                    overflow_integer<RhsRep, OverflowTag> const& rhs) const
            -> overflow_integer<op_result<Operator, LhsRep, RhsRep>, OverflowTag>
            {
                return from_rep<overflow_integer<op_result<Operator, LhsRep, RhsRep>, OverflowTag>>(
                        _impl::tagged_binary_operator<OverflowTag, Operator>{}(_impl::to_rep(lhs), _impl::to_rep(rhs)));
            }
        };
        template<class Operator, class LhsRep, class RhsRep, class OverflowTag>
        struct comparison_operator<Operator,
                overflow_integer<LhsRep, OverflowTag>, overflow_integer<RhsRep, OverflowTag>> {
            constexpr auto operator()(
                    overflow_integer<LhsRep, OverflowTag> const& lhs,
                    overflow_integer<RhsRep, OverflowTag> const& rhs) const
            -> decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)))
            {
                return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
            }
        };
        template<class Operator, class LhsRep, class LhsTag, class RhsRep, class RhsTag>
        struct comparison_operator<Operator,
                overflow_integer<LhsRep, LhsTag>, overflow_integer<RhsRep, RhsTag>> {
            using common_tag = cnl::_impl::common_type_t<LhsTag, RhsTag>;
            using operator_type = comparison_operator<
                    Operator,
                    overflow_integer<LhsRep, common_tag>,
                    overflow_integer<RhsRep, common_tag>>;
            constexpr auto operator()(
                    const overflow_integer<LhsRep, LhsTag>& lhs,
                    const overflow_integer<RhsRep, RhsTag>& rhs) const
            -> decltype(operator_type{}(lhs, rhs))
            {
                return operator_type{}(lhs, rhs);
            }
        };
        template<class Operator, typename Rep, class OverflowTag>
        struct pre_operator<Operator, overflow_integer<Rep, OverflowTag>> {
            constexpr auto operator()(overflow_integer<Rep, OverflowTag>& rhs) const
            -> decltype(typename pre_to_assign<Operator>::type{}(rhs, 1))
            {
                return typename pre_to_assign<Operator>::type{}(rhs, 1);
            }
        };
        template<class Operator, typename Rep, class OverflowTag>
        struct post_operator<Operator, overflow_integer<Rep, OverflowTag>> {
                                  auto operator()(overflow_integer<Rep, OverflowTag>& rhs) const
            -> overflow_integer<Rep, OverflowTag>
            {
                auto copy = rhs;
                typename post_to_assign<Operator>::type{}(rhs, 1);
                return copy;
            }
        };
    }
    template<class Rep, class OverflowTag>
    struct numeric_limits<overflow_integer<Rep, OverflowTag>>
            : numeric_limits<_impl::number_base<overflow_integer<Rep, OverflowTag>, Rep>> {
        static constexpr bool is_integer = true;
    };
    template<class Rep, class OverflowTag>
    struct numeric_limits<overflow_integer<Rep, OverflowTag> const>
            : numeric_limits<_impl::number_base<overflow_integer<Rep, OverflowTag>, Rep>> {
        static constexpr bool is_integer = true;
    };
    template<class Rep, class OverflowTag>
    std::ostream &operator<<(std::ostream &o, overflow_integer<Rep, OverflowTag> const &i) {
        return o << _impl::to_rep(i);
    }
}
namespace std {
    template<class Rep, class OverflowTag>
    struct numeric_limits<cnl::overflow_integer<Rep, OverflowTag>>
            : cnl::numeric_limits<cnl::overflow_integer<Rep, OverflowTag>> {
    };
    template<class Rep, class OverflowTag>
    struct numeric_limits<cnl::overflow_integer<Rep, OverflowTag> const>
            : cnl::numeric_limits<cnl::overflow_integer<Rep, OverflowTag>> {
    };
}
namespace cnl {
    using _impl::nearest_rounding_tag;
    using _impl::native_rounding_tag;
    using _impl::divide;
    using _impl::shift_right;
}
namespace cnl {
    namespace _impl {
        template<int Digits = digits<int>::value, typename Narrowest = int>
        class wide_integer;
    }
}
namespace cnl {
    namespace _impl {
        template<bool Signed>
        struct max_digits_fundamental;
        template<>
        struct max_digits_fundamental<true> : digits<intmax> {
        };
        template<>
        struct max_digits_fundamental<false> : digits<uintmax> {
        };
        template<typename T, class Enable = void>
        struct max_digits;
        template<typename T>
        struct max_digits<T, enable_if_t<is_integral<T>::value>>
                : max_digits_fundamental<is_signed<T>::value> {
        };
        template<typename T>
        struct max_digits<T, enable_if_t<is_composite<T>::value>>
                : max_digits<to_rep_t<T>> {
        };
    }
}
namespace cnl {
    namespace _impl {
        template<typename Upper, typename Lower>
        class duplex_integer;
    }
}
namespace cnl {
    namespace _impl {
        template<typename Word, int NumWords>
        struct multiword_integer;
        template<typename Word>
        struct multiword_integer<Word, 1> {
            using type = Word;
        };
        template<typename Word>
        struct multiword_integer<Word, 2> {
            using upper = Word;
            using lower = set_signedness_t<upper, false>;
            using type = duplex_integer<upper, lower>;
        };
        template<typename Word, int NumWords>
        struct multiword_integer {
        private:
            static_assert(NumWords>2, "");
            static constexpr auto num_words = NumWords;
            static constexpr auto num_words_rounded_up = (1 << used_digits(num_words-1));
            static constexpr auto upper_num_words = num_words_rounded_up/2;
            static constexpr auto lower_num_words = num_words-upper_num_words;
            using upper = typename multiword_integer<Word, upper_num_words>::type;
            using lower = typename multiword_integer<set_signedness_t<Word, false>, lower_num_words>::type;
        public:
            using type = duplex_integer<upper, lower>;
        };
        template<typename Word, int NumWords>
        using multiword_integer_t = typename multiword_integer<Word, NumWords>::type;
        template<int N, class Enable = void>
        struct is_power_of_two;
        template<int N>
        struct is_power_of_two<N, enable_if_t<(N>0)>>
                : std::integral_constant<bool, !(N & (N-1))> {
        };
        template<typename Word, typename Signedness = set_signedness_t<int, is_signed<Word>::value>>
        struct optimal_duplex;
        template<typename Narrowest>
        struct optimal_duplex<Narrowest, unsigned> {
            static constexpr auto double_word_digits = max_digits<Narrowest>::value;
            static_assert(double_word_digits>=2 && is_power_of_two<double_word_digits>::value,
                    "invalid integer type, Narrowest");
            static constexpr auto word_digits = double_word_digits/2;
            using word = set_digits_t<Narrowest, word_digits>;
            static_assert(digits<word>::value==word_digits, "failed to half a double-width word");
            using type = word;
        };
        template<typename Narrowest>
        struct optimal_duplex<Narrowest, signed> {
            using unsiged_narrowest = make_unsigned_t<Narrowest>;
            using unsigned_multiword_integer = optimal_duplex<unsiged_narrowest, unsigned>;
            using type = make_signed_t<typename unsigned_multiword_integer::type>;
        };
        template<int Digits, typename Narrowest>
        struct instantiate_duplex_integer {
            using word = typename optimal_duplex<Narrowest>::type;
            static constexpr auto num_sign_bits = is_signed<word>::value;
            static constexpr auto word_digits = digits<word>::value+num_sign_bits;
            static constexpr auto required_num_words = (Digits+num_sign_bits+word_digits-1)/word_digits;
            static constexpr auto plural_num_words = max(2, required_num_words);
            using type = multiword_integer_t<word, plural_num_words>;
        };
        template<int Digits, typename Narrowest>
        using instantiate_duplex_integer_t = typename instantiate_duplex_integer<Digits, Narrowest>::type;
    }
}
namespace cnl {
    template<typename Upper, typename Lower>
    struct make_unsigned<_impl::duplex_integer<Upper, Lower>>
            : _impl::type_identity<_impl::duplex_integer<make_unsigned_t<Upper>, Lower>> {
    };
}
namespace cnl {
    namespace _impl {
        template<typename Result, typename Lhs>
        constexpr auto sensible_right_shift(Lhs const& lhs, int rhs)
        -> enable_if_t<digits<Result>::value <= digits<decltype(lhs>>rhs)>::value, Result>
        {
            using promoted_type = decltype(lhs >> rhs);
            return static_cast<Result>((rhs>=digits<promoted_type>::value)
                   ? lhs >> (digits<Lhs>::value-1) >> 1
                   : (lhs >> rhs) & static_cast<promoted_type>(~Result{}));
        }
        template<typename Result, typename Lhs>
        constexpr auto sensible_right_shift(Lhs const& lhs, int rhs)
        -> enable_if_t<(digits<Result>::value > digits<decltype(lhs>>rhs)>::value), Result>
        {
            using promoted_type = decltype(lhs >> rhs);
            return (rhs>=digits<promoted_type>::value)
                   ? Result{}
                   : static_cast<Result>(lhs >> rhs);
        }
        template<typename Result, typename Lhs>
        constexpr auto sensible_left_shift(Lhs const& lhs, int rhs)
        -> enable_if_t<digits<Result>::value <= digits<decltype(lhs<<rhs)>::value, Result>
        {
            using promoted_type = decltype(lhs << rhs);
            using unsigned_type = make_unsigned_t<decltype(lhs & lhs)>;
            return (rhs>=digits<promoted_type>::value)
                   ? Result{}
                   : static_cast<Result>(
                           static_cast<unsigned_type>(lhs & sensible_right_shift<Lhs>(~Result{}, rhs)) << rhs);
        }
        template<typename Result, typename Lhs>
        constexpr auto sensible_left_shift(Lhs const& lhs, int rhs)
        -> enable_if_t<(digits<Result>::value > digits<decltype(lhs<<rhs)>::value), Result>
        {
            return sensible_left_shift<Result>(static_cast<Result>(lhs), rhs);
        }
        template<typename Result, typename Lhs>
        constexpr auto extra_sensible_right_shift(Lhs const& lhs, int rhs) -> Result
        {
            return (rhs<0)
                    ? sensible_left_shift<Result>(lhs, -rhs)
                    : sensible_right_shift<Result>(lhs, rhs);
        }
    }
    template<typename Upper, typename Lower>
    struct digits<_impl::duplex_integer<Upper, Lower>>
            : std::integral_constant<int, digits<Upper>::value+digits<Lower>::value> {
    };
}
namespace cnl {
    template<typename Upper, typename Lower>
    struct is_signed<_impl::duplex_integer<Upper, Lower>>
            : std::integral_constant<bool, is_signed<Upper>::value> {
    };
}
namespace cnl {
    namespace _impl {
        template<typename Integer>
        constexpr bool is_flushed(Integer const& value)
        {
            return value==0 || value==static_cast<Integer>(~Integer{});
        }
        template<typename Result, typename Upper, typename Lower>
        constexpr auto upper_value(Upper const& upper) -> Result
        {
            return (digits<Result>::value<=digits<Lower>::value)
                   ? !is_flushed(upper)
                     ? unreachable<Result>("overflow in narrowing conversion")
                     : Result{}
                   : Result(sensible_left_shift<Result>(upper, digits<Lower>::value));
        }
        template<typename Upper, typename Lower>
        class duplex_integer {
            static_assert(!is_signed<Lower>::value, "Lower component must be unsigned.");
            using upper_type = Upper;
            using lower_type = Lower;
            static constexpr int lower_width = width<lower_type>::value;
        public:
            duplex_integer() = default;
            constexpr duplex_integer(upper_type const& u, lower_type const& l);
            template<typename Integer, _impl::enable_if_t<(numeric_limits<Integer>::is_integer), int> Dummy = 0>
            constexpr duplex_integer(Integer const& i);
            template<typename Number, _impl::enable_if_t<(numeric_limits<Number>::is_iec559), int> Dummy = 0>
            constexpr duplex_integer(Number const& i);
            constexpr auto upper() const -> upper_type const&
            {
                return _upper;
            }
                                  auto upper() -> upper_type&
            {
                return _upper;
            }
            constexpr auto lower() const -> lower_type const&
            {
                return _lower;
            }
                                  auto lower() -> lower_type&
            {
                return _lower;
            }
            explicit constexpr operator bool() const { return _lower || _upper; }
            template<typename Integer, _impl::enable_if_t<numeric_limits<Integer>::is_integer, int> = 0>
            explicit constexpr operator Integer() const
            {
                return upper_value<Integer, Upper, Lower>(_upper) | static_cast<Integer>(_lower);
            }
            template<typename Number, _impl::enable_if_t<numeric_limits<Number>::is_iec559, int> = 0>
            explicit constexpr operator Number() const
            {
                return static_cast<Number>(_upper)*cnl::power<Number, lower_width, 2>()
                        +static_cast<Number>(_lower);
            }
        private:
            upper_type _upper;
            lower_type _lower;
        };
    }
}
namespace cnl {
    namespace _impl {
        template<typename T>
        struct is_duplex_integer : std::false_type {};
        template<typename Upper, typename Lower>
        struct is_duplex_integer<duplex_integer<Upper, Lower>> : std::true_type {};
    }
}
namespace cnl {
    namespace _impl {
        template<typename Upper, typename Lower>
        struct wants_generic_ops<duplex_integer<Upper, Lower>> : std::true_type {
        };
    }
}
namespace cnl {
    namespace _impl {
        template<typename Operator, typename Upper, typename Lower>
        struct comparison_operator<Operator, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>> {
            constexpr auto operator()(
                    duplex_integer<Upper, Lower> const& lhs,
                    duplex_integer<Upper, Lower> const& rhs) const -> bool
            {
                using tuple = std::tuple<Upper const&, Lower const&>;
                return Operator{}(tuple(lhs.upper(), lhs.lower()), tuple(rhs.upper(), rhs.lower()));
            }
        };
        template<class Operator, typename Lhs, typename Rhs>
        struct comparison_operator<Operator, Lhs, Rhs,
                enable_if_t<is_duplex_integer<Lhs>::value!=is_duplex_integer<Rhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> bool
            {
                using common_type = common_type_t<Lhs, Rhs>;
                return comparison_operator<Operator, common_type, common_type>{}(lhs, rhs);
            }
        };
    }
}
namespace cnl {
    namespace _impl {
        template<int Digits, typename Narrowest, typename Enable = void>
        struct wide_integer_rep;
        template<int Digits, typename Narrowest>
        struct wide_integer_rep<Digits, Narrowest, enable_if_t<(max_digits<Narrowest>::value>=Digits)>>
                : set_digits<Narrowest, max(Digits, digits<Narrowest>::value)> {
        };
        template<int Digits, typename Narrowest>
        struct wide_integer_rep<Digits, Narrowest, enable_if_t<(max_digits<Narrowest>::value<Digits)>>
                : instantiate_duplex_integer<Digits, Narrowest> {
        };
        template<int Digits, typename Narrowest>
        using wide_integer_rep_t = typename wide_integer_rep<Digits, Narrowest>::type;
    }
}
namespace cnl {
    namespace _impl {
        template<typename Lhs, typename Rhs>
        struct heterogeneous_duplex_divide_operator {
            using common_type = wide_integer_rep_t<
                    max(digits<Lhs>::value, digits<Rhs>::value),
                    set_signedness_t<int, is_signed<Lhs>::value|is_signed<Rhs>::value>>;
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> Lhs
            {
                return static_cast<Lhs>(static_cast<common_type>(lhs) / static_cast<common_type>(rhs));
            }
        };
        template<typename Upper, typename Lower>
        struct binary_operator<divide_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>> {
            using _duplex_integer = duplex_integer<Upper, Lower>;
            using _unsigned_duplex_integer = make_unsigned_t<_duplex_integer>;
            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const
            -> _duplex_integer
            {
                return (lhs<_duplex_integer{0})
                       ? (rhs<_duplex_integer{0})
                         ? non_negative_division(-lhs, -rhs)
                         : -non_negative_division(-lhs, rhs)
                       : (rhs<_duplex_integer{0})
                         ? -non_negative_division(lhs, -rhs)
                         : non_negative_division(lhs, rhs);
            }
            static auto non_negative_division(
                    _unsigned_duplex_integer const& dividend,
                    _unsigned_duplex_integer const& divisor)
            -> _unsigned_duplex_integer
            {
                auto const high = divisor.upper();
                if (!high) {
                    return div_by_lower(dividend, divisor.lower());
                }
                int n = fls(high);
                auto quot = div_by_lower(dividend >> n, (divisor >> n).lower());
                if (quot) {
                    --quot;
                }
                if ((dividend-quot*divisor)>=divisor) {
                    ++quot;
                }
                return quot;
            }
            static auto fls(Upper n) -> int
            {
                auto half_digits = numeric_limits<_duplex_integer>::digits/2;
                if (!n) {
                    return 0;
                }
                auto const msd = Upper{1} << (half_digits-1);
                for (int r = half_digits;; n <<= 1, r--) {
                    if (n & msd) {
                        return r;
                    }
                }
            };
            static auto
            div_by_lower(_unsigned_duplex_integer const& dividend, Lower const& divisor)
            -> _unsigned_duplex_integer
            {
                _unsigned_duplex_integer rem = dividend;
                _unsigned_duplex_integer b = divisor;
                _unsigned_duplex_integer d = 1;
                using unsigned_upper = set_signedness_t<Upper, false>;
                auto high = rem.upper();
                _unsigned_duplex_integer quot = 0;
                if (static_cast<unsigned_upper>(high)>=divisor) {
                    high /= divisor;
                    quot = _unsigned_duplex_integer{high, 0};
                    rem -= _unsigned_duplex_integer(high*divisor, 0);
                }
                while (b<rem) {
                    b <<= 1;
                    d <<= 1;
                }
                do {
                    if (rem>=b) {
                        rem -= b;
                        quot += d;
                    }
                    b >>= 1;
                    d >>= 1;
                }
                while (d);
                return quot;
            };
        };
        template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
        struct binary_operator<divide_op, duplex_integer<LhsUpper, LhsLower>, duplex_integer<RhsUpper, RhsLower>>
                : heterogeneous_duplex_divide_operator<
                        duplex_integer<LhsUpper, LhsLower>, duplex_integer<RhsUpper, RhsLower>> {
        };
        template<typename Lhs, typename RhsUpper, typename RhsLower>
        struct binary_operator<divide_op, Lhs, duplex_integer<RhsUpper, RhsLower>>
                : heterogeneous_duplex_divide_operator<Lhs, duplex_integer<RhsUpper, RhsLower>> {
        };
        template<typename LhsUpper, typename LhsLower, typename Rhs>
        struct binary_operator<divide_op, duplex_integer<LhsUpper, LhsLower>, Rhs>
                : heterogeneous_duplex_divide_operator<duplex_integer<LhsUpper, LhsLower>, Rhs> {
        };
    }
}
namespace cnl {
    template<typename Upper, typename Lower>
    struct make_signed<_impl::duplex_integer<Upper, Lower>>
            : _impl::type_identity<_impl::duplex_integer<make_signed_t<Upper>, Lower>> {
    };
}
namespace cnl {
    namespace _impl {
        template<typename Lhs, typename Rhs>
        struct heterogeneous_duplex_modulo_operator {
            using common_type = wide_integer_rep_t<
                    max(digits<Lhs>::value, digits<Rhs>::value),
                    set_signedness_t<int, is_signed<Lhs>::value|is_signed<Rhs>::value>>;
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> Lhs
            {
                return static_cast<Lhs>(static_cast<common_type>(lhs) / static_cast<common_type>(rhs));
            }
        };
        template<typename Upper, typename Lower>
        struct binary_operator<modulo_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>> {
            using _duplex_integer = duplex_integer<Upper, Lower>;
            using _unsigned_duplex_integer = make_unsigned_t<_duplex_integer>;
            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const
            -> _duplex_integer
            {
                return lhs - rhs*(lhs/rhs);
            }
        };
        template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
        struct binary_operator<modulo_op, duplex_integer<LhsUpper, LhsLower>, duplex_integer<RhsUpper, RhsLower>>
                : heterogeneous_duplex_modulo_operator<
                        duplex_integer<LhsUpper, LhsLower>, duplex_integer<RhsUpper, RhsLower>> {
        };
        template<typename Lhs, typename RhsUpper, typename RhsLower>
        struct binary_operator<modulo_op, Lhs, duplex_integer<RhsUpper, RhsLower>>
                : heterogeneous_duplex_modulo_operator<Lhs, duplex_integer<RhsUpper, RhsLower>> {
        };
        template<typename LhsUpper, typename LhsLower, typename Rhs>
        struct binary_operator<modulo_op, duplex_integer<LhsUpper, LhsLower>, Rhs>
                : heterogeneous_duplex_modulo_operator<duplex_integer<LhsUpper, LhsLower>, Rhs> {
        };
    }
}
namespace cnl {
    namespace _impl {
        template<typename Upper, typename Lower, int Digits>
        struct set_width<_impl::duplex_integer<Upper, Lower>, Digits> {
            using word = _impl::to_rep_t<Upper>;
            using type = typename _impl::instantiate_duplex_integer<Digits+!is_signed<word>::value, word>::type;
        };
    }
}
namespace cnl {
    namespace _impl {
        template<
                int Sign,
                typename Positive, typename Zero, typename Negative,
                bool IsNegative = (Sign<0), bool IsPositive = (Sign>0)>
        struct conditional3;
        template<int Sign, typename Positive, typename Zero, typename Negative>
        struct conditional3<Sign, Positive, Zero, Negative, false, false> : type_identity<Zero> {
        };
        template<int Sign, typename Positive, typename Zero, typename Negative>
        struct conditional3<Sign, Positive, Zero, Negative, true, false> : type_identity<Negative> {
        };
        template<int Sign, typename Positive, typename Zero, typename Negative>
        struct conditional3<Sign, Positive, Zero, Negative, false, true> : type_identity<Positive> {
        };
        template<int Sign, typename Positive, typename Zero, typename Negative>
        using conditional3_t = typename conditional3<Sign, Positive, Zero, Negative>::type;
    }
}
namespace cnl {
    namespace _impl {
        template<typename Lhs, typename Rhs>
        struct heterogeneous_duplex_multiply_operator {
            using common_type = conditional3_t<
                    width<Lhs>::value-width<Rhs>::value,
                    Lhs,
                    conditional3_t<
                            (is_signed<Lhs>::value-is_signed<Rhs>::value),
                            Lhs,
                            void,
                            Rhs>,
                    Rhs>;
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(std::declval<common_type>()*std::declval<common_type>())
            {
                return static_cast<common_type>(lhs)*static_cast<common_type>(rhs);
            }
        };
        template<typename T>
        struct long_multiply;
        template<typename Word>
        struct long_multiply {
            template<typename Lhs, typename Rhs>
            using result_type = set_width_t<Word, width<Lhs>::value+width<Rhs>::value>;
            template<typename Lhs, typename Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> result_type<Lhs, Rhs>
            {
                using result_type = result_type<Lhs, Rhs>;
                return static_cast<result_type>(lhs)*static_cast<result_type>(rhs);
            }
        };
        template<typename Upper, typename Lower>
        struct long_multiply<duplex_integer<Upper, Lower>> {
            using result_type = duplex_integer<duplex_integer<Upper, Lower>, duplex_integer<Lower, Lower>>;
            template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
            constexpr auto operator()(
                    duplex_integer<LhsUpper, LhsLower> const& lhs,
                    duplex_integer<RhsUpper, RhsLower> const& rhs) const
            -> result_type
            {
                return multiply_components(lhs.upper(), lhs.lower(), rhs.upper(), rhs.lower());
            }
            template<typename Lhs, typename RhsUpper, typename RhsLower>
            constexpr auto operator()(
                    Lhs const& lhs,
                    duplex_integer<RhsUpper, RhsLower> const& rhs) const
            -> result_type
            {
                return multiply_components(0, lhs, rhs.upper(), rhs.lower());
            }
            template<typename LhsUpper, typename LhsLower, typename Rhs>
            constexpr auto operator()(
                    duplex_integer<LhsUpper, LhsLower> const& lhs,
                    Rhs const& rhs) const
            -> result_type
            {
                return multiply_components(lhs.upper(), lhs.lower(), 0, rhs);
            }
            template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
            static constexpr auto multiply_components(
                    LhsUpper const& lhs_upper, LhsLower const& lhs_lower,
                    RhsUpper const& rhs_upper, RhsLower const& rhs_lower)
            -> result_type
            {
                return ((result_type{_impl::long_multiply<Upper>{}(lhs_upper, rhs_upper)})
                        << (digits<Lower>::value+digits<Upper>::value))
                        +((result_type{_impl::long_multiply<Upper>{}(lhs_lower, rhs_upper)}
                                +result_type{_impl::long_multiply<Upper>{}(lhs_upper, rhs_lower)})
                                << digits<Lower>::value)
                        +((result_type{_impl::long_multiply<Lower>{}(lhs_lower, rhs_lower)}));
            }
        };
        template<typename Upper, typename Lower>
        struct binary_operator<multiply_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>> {
            using _duplex_integer = duplex_integer<Upper, Lower>;
            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const
            -> _duplex_integer
            {
                return multiply_components(lhs.upper(), lhs.lower(), rhs.upper(), rhs.lower());
            }
            static constexpr auto multiply_components(
                    Upper const& lhs_upper,
                    Lower const& lhs_lower,
                    Upper const& rhs_upper,
                    Lower const& rhs_lower)
            -> _duplex_integer
            {
                using common_result_type = decltype(long_multiply<Upper>{}(lhs_upper, rhs_upper));
                return (long_multiply<Upper>{}(lhs_upper, rhs_upper) << digits<Upper>::value)
                        +((long_multiply<Upper>{}(lhs_upper, rhs_lower)+long_multiply<Upper>{}(lhs_lower, rhs_upper))
                                << digits<Lower>::value)
                        +static_cast<common_result_type>(long_multiply<Lower>{}(lhs_lower, rhs_lower));
            }
        };
        template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
        struct binary_operator<multiply_op, duplex_integer<LhsUpper, LhsLower>, duplex_integer<RhsUpper, RhsLower>>
                : heterogeneous_duplex_multiply_operator<
                        duplex_integer<LhsUpper, LhsLower>, duplex_integer<RhsUpper, RhsLower>> {
        };
        template<typename LhsUpper, typename LhsLower, typename Rhs>
        struct binary_operator<multiply_op, duplex_integer<LhsUpper, LhsLower>, Rhs>
                : heterogeneous_duplex_multiply_operator<
                        duplex_integer<LhsUpper, LhsLower>, Rhs> {
        };
        template<typename Lhs, typename RhsUpper, typename RhsLower>
        struct binary_operator<multiply_op, Lhs, duplex_integer<RhsUpper, RhsLower>>
                : heterogeneous_duplex_multiply_operator<
                        Lhs, duplex_integer<RhsUpper, RhsLower>> {
        };
    }
}
namespace cnl {
    template<typename Upper, typename Lower>
    struct to_rep<_impl::duplex_integer<Upper, Lower>> {
        constexpr auto operator()(_impl::duplex_integer<Upper, Lower> const& number) const
        -> _impl::to_rep_t<Upper>
        {
            return _impl::to_rep(Upper(number));
        }
    };
}
namespace cnl {
    template<typename Upper, typename Lower, int Digits>
    struct set_digits<_impl::duplex_integer<Upper, Lower>, Digits>
            : _impl::instantiate_duplex_integer<Digits, _impl::to_rep_t<Upper>> {
    };
}
namespace cnl {
    namespace _impl {
        template<typename Upper, typename Lower, typename Rhs>
        struct binary_operator<shift_left_op, duplex_integer<Upper, Lower>, Rhs> {
            using _duplex_integer = duplex_integer<Upper, Lower>;
            constexpr auto operator()(_duplex_integer const& lhs, Rhs const& rhs) const
            -> _duplex_integer
            {
                return with_int(lhs, static_cast<int>(rhs));
            }
        private:
            constexpr auto with_int(_duplex_integer const& lhs, int const& rhs) const
            -> _duplex_integer
            {
                return _duplex_integer(
                        sensible_left_shift<Upper>(lhs.upper(), rhs)
                                | extra_sensible_right_shift<Upper>(lhs.lower(), width<Lower>::value-rhs),
                        sensible_left_shift<Lower>(lhs.lower(), rhs));
            }
        };
        template<typename Upper, typename Lower, typename Rhs>
        struct binary_operator<shift_right_op, duplex_integer<Upper, Lower>, Rhs> {
            using _duplex_integer = duplex_integer<Upper, Lower>;
            constexpr auto operator()(_duplex_integer const& lhs, Rhs const& rhs) const
            -> _duplex_integer
            {
                return with_int(lhs, static_cast<int>(rhs));
            }
        private:
            constexpr auto with_int(_duplex_integer const& lhs, int rhs) const
            -> _duplex_integer
            {
                return _duplex_integer(calculate_upper(lhs, rhs), calculate_lower(lhs, rhs));
            }
            constexpr auto calculate_upper(_duplex_integer const& lhs, int rhs) const
            -> Upper
            {
                return sensible_right_shift<Upper>(lhs.upper(), rhs);
            }
            constexpr auto calculate_lower(_duplex_integer const& lhs, int rhs) const
            -> Lower
            {
                return static_cast<Lower>(
                        sensible_right_shift<Lower>(lhs.lower(), rhs)
                                | extra_sensible_right_shift<Lower>(lhs.upper(), rhs-width<Lower>::value));
            }
        };
    }
}
namespace cnl {
    namespace _impl {
        template<typename Upper, typename Lower>
        struct max_to_chars_chars<duplex_integer<Upper, Lower>> {
            using _scalar = duplex_integer<Upper, Lower>;
            static constexpr auto _sign_chars = static_cast<int>(cnl::is_signed<_scalar>::value);
            static constexpr auto _integer_chars = ((cnl::digits<_scalar>::value+2)/3);
            static constexpr auto value = _sign_chars+_integer_chars;
        };
        template<typename Upper, typename Lower>
        to_chars_result to_chars_positive(
                char* const first, char* const last, duplex_integer<Upper, Lower> const& value) noexcept
        {
            auto const natural_last = to_chars_natural(first, last, value);
            return to_chars_result{natural_last, natural_last?std::errc{}:std::errc::value_too_large};
        }
        template<typename Upper, typename Lower>
        to_chars_result to_chars(
                char* const first,
                char* const last,
                _impl::duplex_integer<Upper, Lower> const& value)
        {
            if (!value) {
                if (first==last) {
                    return to_chars_result{last, std::errc::value_too_large};
                }
                *first = '0';
                return to_chars_result{first+1, std::errc{}};
            }
            using native_rounding_type = set_rounding_t<decltype(value), _impl::native_rounding_tag>;
            auto const& native_rounding_value = static_cast<native_rounding_type>(value);
            return _impl::to_chars_non_zero<native_rounding_type>{}(
                    first, last, native_rounding_value);
        }
    }
    using _impl::to_chars;
}
namespace cnl {
    template<int Digits, typename Narrowest, typename Rep>
    struct from_rep<_impl::wide_integer<Digits, Narrowest>, Rep> {
        constexpr auto operator()(Rep const& rep) const
        -> _impl::wide_integer<
                Digits,
                _impl::adopt_signedness_t<Narrowest, Rep>>
        {
            return rep;
        }
    };
}
namespace cnl {
    template<int Digits, class Narrowest, class Value>
    struct from_value<_impl::wide_integer<Digits, Narrowest>, Value>
            : _impl::from_value_simple<
                    _impl::wide_integer<
                            cnl::digits<Value>::value,
                            _impl::adopt_signedness_t<Narrowest, Value>>,
                    Value> {
    };
}
namespace cnl {
    namespace _impl {
        template<typename Rep>
        constexpr auto make_wide_integer(Rep const& rep)
        -> wide_integer<
                digits<Rep>::value,
                adopt_signedness_t<int, Rep>> {
            return rep;
        }
    }
}
namespace cnl {
    template<int Digits, typename Narrowest>
    struct numeric_limits<_impl::wide_integer<Digits, Narrowest>>
            : numeric_limits<typename _impl::wide_integer<Digits, Narrowest>::rep> {
        static constexpr bool is_integer = true;
        using _narrowest_numeric_limits = numeric_limits<Narrowest>;
        using _value_type = _impl::wide_integer<Digits, Narrowest>;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;
        static constexpr int digits = Digits;
        static constexpr _value_type min() noexcept
        {
            return _impl::from_rep<_value_type>(1);
        }
        static constexpr _value_type max() noexcept
        {
            return static_cast<_rep>(_rep_numeric_limits::max() >> (_rep_numeric_limits::digits-digits));
        }
        static constexpr _value_type lowest() noexcept
        {
            return static_cast<_rep>(_rep_numeric_limits::lowest() >> (_rep_numeric_limits::digits-digits));
        }
    };
    template<int Digits, typename Narrowest>
    struct numeric_limits<_impl::wide_integer<Digits, Narrowest> const>
            : numeric_limits<_impl::wide_integer<Digits, Narrowest>> {
        static constexpr bool is_integer = true;
    };
}
namespace cnl {
    namespace _impl {
        template<int LhsDigits, class LhsNarrowest, class Rhs>
        constexpr auto operator<<(wide_integer<LhsDigits, LhsNarrowest> const& lhs, Rhs const& rhs)
        -> wide_integer<LhsDigits, LhsNarrowest>
        {
            return wide_integer<LhsDigits, LhsNarrowest>(
                    to_rep(lhs) << set_signedness_t<int, is_signed<Rhs>::value>(rhs));
        }
        template<int LhsDigits, class LhsNarrowest, class Rhs>
        constexpr auto operator>>(wide_integer<LhsDigits, LhsNarrowest> const& lhs, Rhs const& rhs)
        -> wide_integer<LhsDigits, LhsNarrowest>
        {
            return wide_integer<LhsDigits, LhsNarrowest>(
                    to_rep(lhs) >> set_signedness_t<int, is_signed<Rhs>::value>(rhs));
        }
        template<typename Operator, int Digits, typename Narrowest>
        struct unary_operator<Operator, wide_integer<Digits, Narrowest>> {
            constexpr auto operator()(wide_integer<Digits, Narrowest> const& rhs) const -> wide_integer<Digits, Narrowest>
            {
                return Operator()(to_rep(rhs));
            }
        };
        template<class Operator, int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
        struct binary_operator<
                Operator,
                wide_integer<LhsDigits, LhsNarrowest>, wide_integer<RhsDigits, RhsNarrowest>> {
            using _lhs = wide_integer<LhsDigits, LhsNarrowest>;
            using _rhs = wide_integer<RhsDigits, RhsNarrowest>;
            using _result = typename std::common_type<_lhs, _rhs>::type;
            constexpr auto operator()(_lhs const& lhs, _rhs const& rhs) const -> _result
            {
                return Operator{}(to_rep(lhs), to_rep(rhs));
            }
        };
        template<class Operator, int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
        struct comparison_operator<
                Operator,
                wide_integer<LhsDigits, LhsNarrowest>, wide_integer<RhsDigits, RhsNarrowest>> {
            constexpr auto operator()(
                    wide_integer<LhsDigits, LhsNarrowest> const& lhs,
                    wide_integer<RhsDigits, RhsNarrowest> const& rhs) const
            -> decltype(Operator()(to_rep(lhs), to_rep(rhs)))
            {
                return Operator()(to_rep(lhs), to_rep(rhs));
            }
        };
        template<class Operator, int Digits, typename Narrowest>
        struct pre_operator<Operator, wide_integer<Digits, Narrowest>> {
                                  auto operator()(wide_integer<Digits, Narrowest>& rhs) const
            -> wide_integer<Digits, Narrowest>&
            {
                Operator()(_impl::to_rep(rhs));
                return rhs;
            }
        };
        template<class Operator, int Digits, typename Narrowest>
        struct post_operator<Operator, wide_integer<Digits, Narrowest>> {
                                  auto operator()(wide_integer<Digits, Narrowest>& lhs) const
            -> wide_integer<Digits, Narrowest>
            {
                auto copy = lhs;
                Operator()(_impl::to_rep(lhs));
                return copy;
            }
        };
        template<int Digits, typename Narrowest>
        ::std::ostream& operator<<(::std::ostream& out, wide_integer<Digits, Narrowest> const& value)
        {
            return out << to_rep(value);
        }
    }
}
namespace cnl {
    namespace _impl {
        template<class Operator, typename Upper, typename Lower>
        struct default_binary_operator {
            using _duplex_integer = duplex_integer<Upper, Lower>;
            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const
            -> _duplex_integer
            {
                return _duplex_integer(
                        static_cast<Upper>(Operator{}(lhs.upper(), rhs.upper())),
                        static_cast<Lower>(Operator{}(lhs.lower(), rhs.lower())));
            }
        };
        template<class Operator, typename Upper, typename Lower>
        struct first_degree_binary_operator {
            using _duplex_integer = duplex_integer<Upper, Lower>;
            static constexpr auto lower_digits = digits<Lower>::value;
            using wide_lower = set_digits_t<set_signedness_t<Lower, true>, lower_digits+1>;
            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const
            -> _duplex_integer
            {
                return from_sums(
                        static_cast<Upper>(Operator{}(lhs.upper(), rhs.upper())),
                        wide_lower(Operator{}(wide_lower{lhs.lower()}, wide_lower{rhs.lower()})));
            }
            static constexpr auto from_sums(Upper const& upper_sum, wide_lower const& lower_sum)
            -> _duplex_integer
            {
                return _duplex_integer{
                        static_cast<Upper>(upper_sum+static_cast<Upper>(lower_sum >> constant<lower_digits>{})),
                        static_cast<Lower>(lower_sum)};
            }
        };
        template<typename Upper, typename Lower>
        struct unary_operator<bitwise_not_op, duplex_integer<Upper, Lower>> {
            constexpr auto operator()(duplex_integer<Upper, Lower> const& rhs) const
            -> duplex_integer<Upper, Lower>
            {
                return duplex_integer<Upper, Lower>(~rhs.upper(), ~rhs.lower());
            }
        };
        template<typename Upper, typename Lower>
        struct unary_operator<minus_op, duplex_integer<Upper, Lower>> {
            constexpr auto operator()(duplex_integer<Upper, Lower> const& rhs) const
            -> duplex_integer<Upper, Lower>
            {
                return unary_operator<bitwise_not_op, duplex_integer<Upper, Lower>>{}(
                        rhs-duplex_integer<Upper, Lower>{1});
            }
        };
        template<typename Upper, typename Lower>
        struct unary_operator<plus_op, duplex_integer<Upper, Lower>> {
            constexpr auto operator()(duplex_integer<Upper, Lower> const& rhs) const
            -> duplex_integer<Upper, Lower>
            {
                return duplex_integer<Upper, Lower>(+rhs.upper(), +rhs.lower());
            }
        };
        template<typename Upper, typename Lower>
        struct binary_operator<add_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>>
                : first_degree_binary_operator<add_op, Upper, Lower> {
        };
        template<typename Upper, typename Lower>
        struct binary_operator<subtract_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>>
                : first_degree_binary_operator<subtract_op, Upper, Lower> {
        };
        template<typename Upper, typename Lower>
        struct binary_operator<bitwise_or_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>>
                : default_binary_operator<bitwise_or_op, Upper, Lower> {
        };
        template<typename Upper, typename Lower>
        struct binary_operator<bitwise_and_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>>
                : default_binary_operator<bitwise_and_op, Upper, Lower> {
        };
        template<typename Upper, typename Lower>
        struct binary_operator<bitwise_xor_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>>
                : default_binary_operator<bitwise_xor_op, Upper, Lower> {
        };
        template<typename Operator, typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
        struct comparison_operator<Operator, duplex_integer<LhsUpper, LhsLower>, duplex_integer<RhsUpper, RhsLower>> {
            constexpr auto operator()(
                    duplex_integer<LhsUpper, LhsLower> const& lhs,
                    duplex_integer<RhsUpper, RhsLower> const& rhs) const -> bool
            {
                using common_type = duplex_integer<
                        common_type_t<LhsUpper, RhsUpper>,
                        common_type_t<LhsLower, RhsLower>>;
                return comparison_operator<Operator, common_type, common_type>{}(lhs, rhs);
            }
        };
        template<typename Upper, typename Lower>
        struct pre_operator<pre_increment_op, duplex_integer<Upper, Lower>> {
            constexpr auto operator()(duplex_integer<Upper, Lower>& rhs) const
            -> duplex_integer<Upper, Lower>
            {
                return __builtin_expect(!!(rhs.lower()==numeric_limits<Lower>::max()), 0)
                       ? duplex_integer<Upper, Lower>{++rhs.upper(), numeric_limits<Lower>::lowest()}
                       : duplex_integer<Upper, Lower>{rhs.upper(), ++rhs.lower()};
            }
        };
        template<typename Upper, typename Lower>
        struct pre_operator<pre_decrement_op, duplex_integer<Upper, Lower>> {
            constexpr auto operator()(duplex_integer<Upper, Lower>& rhs) const
            -> duplex_integer<Upper, Lower>
            {
                return __builtin_expect(!!(rhs.lower()==numeric_limits<Lower>::lowest()), 0)
                       ? duplex_integer<Upper, Lower>{static_cast<Upper>(--rhs.upper()), numeric_limits<Lower>::max()}
                       : duplex_integer<Upper, Lower>{rhs.upper(), --rhs.lower()};
            }
        };
        template<typename Upper, typename Lower>
        ::std::ostream& operator<<(::std::ostream& out, duplex_integer<Upper, Lower> const& value)
        {
            return out << cnl::to_chars(value).data();
        }
    }
}
namespace cnl {
    namespace _impl {
        template<typename Lower, typename Integer>
        constexpr auto calculate_lower(Integer const& input)
        -> enable_if_t<digits<Lower>::value>=digits<Integer>::value, Lower>
        {
            return Lower(input) & numeric_limits<Lower>::max();
        }
        template<typename Lower, typename Integer>
        constexpr auto calculate_lower(Integer const& input)
        -> enable_if_t<digits<Lower>::value<digits<Integer>::value, Lower>
        {
            return static_cast<Lower>(input & static_cast<Integer>(numeric_limits<Lower>::max()));
        }
        template<typename Upper, typename Lower, typename Integer>
        constexpr auto calculate_upper(Integer const& input)
        -> enable_if_t<digits<Lower>::value>=digits<Integer>::value, Upper>
        {
            return static_cast<Upper>((input >> (digits<Integer>::value-1)) >> 1);
        }
        template<typename Upper, typename Lower, typename Integer>
        constexpr auto calculate_upper(Integer const& input)
        -> enable_if_t<digits<Lower>::value<digits<Integer>::value, Upper>
        {
            return sensible_right_shift<Upper>(input, digits<Lower>::value);
        }
        template<typename Upper, typename Lower>
        constexpr int duplex_integer<Upper, Lower>::lower_width;
        template<typename Upper, typename Lower>
        constexpr duplex_integer<Upper, Lower>::duplex_integer(upper_type const& u, lower_type const& l)
                :_upper(u), _lower(l) { }
        template<typename Upper, typename Lower>
        template<typename Integer, _impl::enable_if_t<(numeric_limits<Integer>::is_integer), int> Dummy>
        constexpr duplex_integer<Upper, Lower>::duplex_integer(Integer const& i)
                : _upper(calculate_upper<Upper, Lower>(i)),
                  _lower(calculate_lower<Lower>(i))
        {
        }
        template<typename Upper, typename Lower>
        template<typename Number, _impl::enable_if_t<(numeric_limits<Number>::is_iec559), int> Dummy>
        constexpr duplex_integer<Upper, Lower>::duplex_integer(Number const& n)
                : _upper(Upper(n / cnl::power<Number, lower_width, 2>())),
                  _lower(Lower(std::fmod(n, cnl::power<Number, lower_width, 2>())))
        {
        }
    }
}
namespace cnl {
    template<typename Upper, typename Lower>
    struct numeric_limits<_impl::duplex_integer<Upper, Lower>>
            : numeric_limits<Upper> {
        static constexpr bool is_integer = true;
        using _lower_numeric_limits = numeric_limits<Lower>;
        using _upper_numeric_limits = numeric_limits<Upper>;
        using _value_type = _impl::duplex_integer<Upper, Lower>;
        static constexpr int digits = _lower_numeric_limits::digits+_upper_numeric_limits::digits;
        static constexpr _value_type lowest() noexcept
        {
            return _value_type{
                    numeric_limits<Upper>::lowest(),
                    numeric_limits<Lower>::lowest()};
        }
        static constexpr _value_type min() noexcept
        {
            return _value_type{
                    numeric_limits<Upper>::min(),
                    numeric_limits<Lower>::min()};
        }
        static constexpr _value_type max() noexcept
        {
            return _value_type{
                    numeric_limits<Upper>::max(),
                    numeric_limits<Lower>::max()};
        }
    };
}
namespace cnl {
    template<typename Upper, typename Lower>
    struct rounding<_impl::duplex_integer<Upper, Lower>>
            : _impl::type_identity<native_rounding_tag> {
        static_assert(
                std::is_same<rounding_t<Upper>, native_rounding_tag>::value,
                "This type can only be specialized with integers that have int-like rounding behavior.");
        static_assert(
                std::is_same<rounding_t<Lower>, native_rounding_tag>::value,
                "This type can only be specialized with integers that have int-like rounding behavior.");
    };
}
namespace cnl {
    namespace _impl {
        template<typename Rep=int>
        class integer : public number_base<integer<Rep>, Rep> {
            using _base = number_base<integer<Rep>, Rep>;
        public:
            integer() = default;
            template<typename T>
            constexpr integer(T const& value)
                    : _base(static_cast<Rep>(value)) { }
            template<class S>
            explicit constexpr operator S() const
            {
                return static_cast<S>(_impl::to_rep(*this));
            }
        };
    }
}
namespace cnl {
    namespace _impl {
        template<int Digits, typename Narrowest>
        class wide_integer
                : public number_base<
                        wide_integer<Digits, Narrowest>,
                        wide_integer_rep_t<Digits, Narrowest>> {
            using _base = number_base<
                    wide_integer<Digits, Narrowest>,
                    wide_integer_rep_t<Digits, Narrowest>>;
        public:
            using rep = typename _base::rep;
            wide_integer() = default;
            template<typename T>
            constexpr wide_integer(T const& value)
                    : _base(static_cast<rep>(value)) { }
            template<class S>
            explicit constexpr operator S() const
            {
                return static_cast<S>(_impl::to_rep(*this));
            }
        };
    }
}
namespace std {
    template<int Digits1, typename Narrowest1, int Digits2, typename Narrowest2>
    struct common_type<cnl::_impl::wide_integer<Digits1, Narrowest1>, cnl::_impl::wide_integer<Digits2, Narrowest2>> {
        static constexpr auto _max_digits = cnl::_impl::max(Digits1, Digits2);
        static constexpr auto _are_signed = cnl::is_signed<Narrowest1>::value || cnl::is_signed<Narrowest2>::value;
        using _common_type = typename std::common_type<Narrowest1, Narrowest2>::type;
        using _narrowest = cnl::_impl::set_signedness_t<_common_type, _are_signed>;
        using type = cnl::_impl::wide_integer<cnl::_impl::max(Digits1, Digits2), _narrowest>;
    };
}
namespace cnl {
    template<int Digits, typename Narrowest>
    struct digits<_impl::wide_integer<Digits, Narrowest>>
            : std::integral_constant<int, Digits> {
    };
}
namespace cnl {
    namespace _impl {
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
        template<char ... Chars>
        constexpr auto operator "" _wide()
        -> decltype(_impl::wide_integer_parser<Chars...>{}())
        {
            return _impl::wide_integer_parser<Chars...>{}();
        }
    }
}
namespace cnl {
    template<int Digits, int Radix, int WideIntegerDigits, typename WideIntegerNarrowest>
    struct scale<Digits, Radix, _impl::wide_integer<WideIntegerDigits, WideIntegerNarrowest>>
            : _impl::default_scale<Digits, Radix, _impl::wide_integer<WideIntegerDigits, WideIntegerNarrowest>> {
    };
}
namespace cnl {
    template<int FromDigits, class Rep, int ToDigits>
    struct set_digits<_impl::wide_integer<FromDigits, Rep>, ToDigits>
            : _impl::type_identity<_impl::wide_integer<ToDigits, Rep>> {
    };
}
namespace cnl {
    template<int Digits = digits<int>::value, typename Narrowest = int>
    using wide_integer = _impl::wide_integer<Digits, Narrowest>;
    namespace literals {
        using _impl::operator "" _wide;
    }
}
namespace cnl {
    namespace _impl {
        template<
                int Digits = digits<int>::value,
                class RoundingTag = nearest_rounding_tag,
                class OverflowTag = undefined_overflow_tag,
                class Narrowest = int>
        using static_integer = rounding_integer<
                overflow_integer<
                        elastic_integer<
                                Digits,
                                wide_integer<
                                        digits<Narrowest>::value,
                                        Narrowest>>,
                        OverflowTag>,
                RoundingTag>;
        template<
                class RoundingTag = nearest_rounding_tag,
                class OverflowTag = undefined_overflow_tag,
                class Narrowest = int,
                class Input = int>
        _impl::enable_if_t<!_impl::is_constant<Input>::value,
                static_integer<
                        numeric_limits<Input>::digits,
                        RoundingTag, OverflowTag,
                        Narrowest>>
        constexpr make_static_integer(Input const& input)
        {
            return input;
        }
        template<
                class RoundingTag = nearest_rounding_tag,
                class OverflowTag = undefined_overflow_tag,
                class Narrowest = int,
                ::cnl::intmax InputValue = 0>
        static_integer<
                used_digits(InputValue),
                RoundingTag, OverflowTag,
                Narrowest>
        constexpr make_static_integer(constant<InputValue>)
        {
            return InputValue;
        }
    }
}
namespace cnl {
    template<
            int Digits = digits<int>::value,
            class RoundingTag = nearest_rounding_tag,
            class OverflowTag = undefined_overflow_tag,
            class Narrowest = int>
    using static_integer = _impl::static_integer<Digits, RoundingTag, OverflowTag, Narrowest>;
    using _impl::make_static_integer;
}
namespace cnl {
    template<
            int Digits,
            int Exponent = 0,
            class RoundingTag = nearest_rounding_tag,
            class OverflowTag = undefined_overflow_tag,
            class Narrowest = signed>
    using static_number = fixed_point<
            _impl::static_integer<Digits, RoundingTag, OverflowTag, Narrowest>,
            Exponent>;
    template<
            class RoundingTag = nearest_rounding_tag,
            class OverflowTag = undefined_overflow_tag,
            class Narrowest = signed,
            class Input = int>
    static_number<
            numeric_limits<Input>::digits, 0,
            RoundingTag, OverflowTag,
            Narrowest>
    constexpr make_static_number(Input const& input)
    {
        return input;
    }
    template<
            class RoundingTag = rounding_integer<>::rounding,
            class OverflowTag = overflow_integer<>::overflow_tag,
            class Narrowest = int,
            class Input = int,
            ::cnl::intmax Value>
    static_number<
            _impl::used_digits(Value)-trailing_bits(Value), trailing_bits(Value),
            RoundingTag, OverflowTag,
            Narrowest>
    constexpr make_static_number(constant<Value> const&)
    {
        return constant<Value>{};
    }
}
namespace cnl {
    using _impl::common_type_t;
    using _impl::enable_if_t;
    using _impl::is_integral;
    using _impl::remove_cvref_t;
    using _impl::type_identity;
    using _impl::type_identity_t;
}

#endif // CNL_COMPLETE_H
