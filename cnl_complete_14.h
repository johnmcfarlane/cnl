
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// mechanically retrieved, single-header version of CNL library
// https://github.com/johnmcfarlane/cnl

#if ! defined(CNL_COMPLETE_H)
#define CNL_COMPLETE_H

#if (__cplusplus < 201402L)
#error This build of CNL requires C++14 or above.
#endif

#include <cmath>
#include <utility>
#include <istream>
#include <climits>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <type_traits>

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
        template<typename T>
        constexpr T deleted_fn() = delete;
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
        using value_type = decltype(Value);
        static constexpr value_type value = Value;
        constexpr operator value_type() const {
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
        template<
                typename S, int Exponent, int Radix,
                bool PositiveExponent = (0<Exponent),
                bool OddExponent = ((Exponent & 1)!=0),
                bool FloatingPointS = numeric_limits<S>::is_iec559>
        struct _power;
        template<typename S>
        struct _power<S, 0, 2, false, false, false> {
            constexpr int operator()() const
            {
                return 1;
            }
        };
        template<typename S, int Radix>
        struct _power<S, 0, Radix, false, false, false> {
            constexpr S operator()() const
            {
                return 1;
            }
        };
        template<typename S, int Exponent, bool OddExponent>
        struct _power<S, Exponent, 2, true, OddExponent, false> {
            constexpr auto operator()() const
            -> decltype(S{1} << constant<Exponent>{})
            {
                return S{1} << constant<Exponent>{};
            }
        };
        template<typename S, int Exponent, int Radix, bool OddExponent>
        struct _power<S, Exponent, Radix, true, OddExponent, false> {
            constexpr auto operator()() const
            -> decltype(_power<S, (Exponent-1), Radix>{}()*Radix)
            {
                return _power<S, (Exponent-1), Radix>{}()*Radix;
            }
        };
        template<typename S, int Exponent, int Radix, bool PositiveExponent, bool OddExponent>
        struct _power<S, Exponent, Radix, PositiveExponent, OddExponent, true> {
            constexpr S operator()() const
            {
                return Exponent
                       ? S(1.)/_power<S, -Exponent, Radix>{}()
                       : S{1.};
            }
        };
        template<typename S, int Exponent, int Radix>
        struct _power<S, Exponent, Radix, true, false, true> {
            constexpr static S square(S const& r)
            {
                return r*r;
            }
            constexpr S operator()() const
            {
                return square(_power<S, Exponent/2, Radix>{}());
            }
        };
        template<typename S, int Exponent, int Radix>
        struct _power<S, Exponent, Radix, true, true, true> {
            constexpr static S square(S const& r)
            {
                return r*r;
            }
            constexpr S operator()() const
            {
                return S(Radix)*_power<S, (Exponent-1), Radix>{}();
            }
        };
        template<typename S, int Exponent, int Radix>
        constexpr auto power()
        -> decltype(_power<S, Exponent, Radix>{}())
        {
            return _power<S, Exponent, Radix>{}();
        }
    }
}
namespace cnl {
    namespace _impl {
        template<class ... T>
        using common_type_t = typename std::common_type<T ...>::type;
        template<bool C, class ... T>
        using enable_if_t = typename std::enable_if<C, T ...>::type;
        template<class A, class B>
        constexpr bool identical(A const& a, B const& b)
        {
            static_assert(std::is_same<A, B>::value, "different types");
            return a==b;
        }
    }
}
namespace cnl {
    using _digits_type = int;
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
    template<class T, class Enable = void>
    struct is_composite : std::false_type {
        static_assert(!std::is_reference<T>::value, "T is a reference");
        static_assert(!std::is_const<T>::value, "T is const");
        static_assert(!std::is_volatile<T>::value, "T is volatile");
    };
    namespace _num_traits_impl {
        template<class ... Args>
        struct are_composite;
        template<>
        struct are_composite<> : std::false_type {
        };
        template<class ArgHead, class ... ArgTail>
        struct are_composite<ArgHead, ArgTail...>
                : std::integral_constant<bool, is_composite<typename std::decay<ArgHead>::type>::value || are_composite<ArgTail...>::value> {
        };
        template<typename T>
        constexpr bool narrower_than(_digits_type digits)
        {
            return std::is_same<T, void>::value ? true : numeric_limits<T>::digits<digits;
        }
        template<typename T>
        constexpr bool no_narrower_than(_digits_type digits)
        {
            return std::is_same<T, void>::value ? true : numeric_limits<T>::digits>=digits;
        }
        template<_digits_type MinNumDigits, class Smaller, class T>
        using enable_for_range_t = typename std::enable_if<
                no_narrower_than<T>(MinNumDigits) && narrower_than<Smaller>(MinNumDigits)>::type;
        template<_digits_type MinNumDigits, class Enable = void>
        struct set_digits_signed;
        template<_digits_type MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, void, int8>> {
            using type = int8;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, int8, int16>> {
            using type = int16;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, int16, int32>> {
            using type = int32;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, int32, int64>> {
            using type = int64;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, int64, int128>> {
            using type = int128;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, intmax, void>>
                : signed_integer_cannot_have<MinNumDigits>::template digits_because_maximum_is<numeric_limits<intmax>::digits> {
        };
        template<_digits_type MinNumDigits, class Enable = void>
        struct set_digits_unsigned;
        template<_digits_type MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, void, uint8>> {
            using type = uint8;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, uint8, uint16>> {
            using type = uint16;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, uint16, uint32>> {
            using type = uint32;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, uint32, uint64>> {
            using type = uint64;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, uint64, uint128>> {
            using type = uint128;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, uintmax, void>>
                : unsigned_integer_cannot_have<MinNumDigits>::template digits_because_maximum_is<numeric_limits<uintmax>::digits> {
        };
        template<class Integer, _digits_type MinNumDigits>
        using set_digits_integer = typename std::conditional<
                numeric_limits<Integer>::is_signed,
                set_digits_signed<MinNumDigits>,
                set_digits_unsigned<MinNumDigits>>::type;
    }
    template<typename T, int Radix = 2, class Enable = void>
    struct digits;
    template<typename T>
    struct digits<T, 2> : std::integral_constant<_digits_type, numeric_limits<T>::digits> {
        static_assert(numeric_limits<T>::is_specialized, "cnl::digits is not correctly specialized for T");
    };
    template<typename T>
    struct digits<T, 10> : std::integral_constant<_digits_type, numeric_limits<T>::digits10> {
        static_assert(numeric_limits<T>::is_specialized, "cnl::digits is not correctly specialized for T");
    };
    template<class T, _digits_type Digits, class Enable = void>
    struct set_digits;
    template<class T, _digits_type Digits>
    struct set_digits<T, Digits, _impl::enable_if_t<std::is_integral<T>::value>>
            : _num_traits_impl::set_digits_integer<T, Digits> {
    };
    template<_digits_type Digits>
    struct set_digits<int128, Digits>
            : _num_traits_impl::set_digits_integer<signed, Digits> {
    };
    template<_digits_type Digits>
    struct set_digits<uint128, Digits>
            : _num_traits_impl::set_digits_integer<unsigned, Digits> {
    };
    template<class T, _digits_type Digits>
    using set_digits_t = typename set_digits<T, Digits>::type;
    template<class T>
    struct is_integral : std::is_integral<T> {
    };
    template<>
    struct is_integral<int128> : std::integral_constant<bool, true> {
    };
    template<>
    struct is_integral<uint128> : std::integral_constant<bool, true> {
    };
    template<class T>
    struct is_signed : std::integral_constant<bool, numeric_limits<T>::is_signed> {
    };
    template<class, class = void>
    struct make_signed;
    template<class T>
    struct make_signed<T, _impl::enable_if_t<std::is_integral<T>::value>> : std::make_signed<T> {
    };
    template<class T>
    using make_signed_t = typename make_signed<T>::type;
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
    template<>
    struct make_signed<int128> {
        using type = int128;
    };
    template<>
    struct make_signed<uint128> {
        using type = int128;
    };
    template<class T>
    using make_unsigned_t = typename make_unsigned<T>::type;
    namespace _impl {
        template<class T, bool IsSigned = true>
        struct make_signed;
        template<class T>
        struct make_signed<T, true> : ::cnl::make_signed<T> {
        };
        template<class T>
        struct make_signed<T, false> : ::cnl::make_unsigned<T> {
        };
        template<class T, bool IsSigned>
        using make_signed_t = typename make_signed<T, IsSigned>::type;
        template<class T1, class T2>
        struct common_signedness {
            static constexpr bool _are_signed = numeric_limits<T1>::is_signed | numeric_limits<T2>::is_signed;
            using type = typename std::common_type<make_signed_t<T1, _are_signed>,
                    make_signed_t<T2, _are_signed>>::type;
        };
        template<class T1, class T2>
        using common_signedness_t = typename common_signedness<T1, T2>::type;
        template<class T>
        struct is_integer_or_float : std::integral_constant<
                bool,
                numeric_limits<T>::is_integer || numeric_limits<T>::is_iec559> {
        };
    }
    template<class Number>
    constexpr Number to_rep(Number const& number) {
        return number;
    }
    namespace _impl {
        using cnl::to_rep;
        template<class Number>
        using to_rep_t = decltype(to_rep(std::declval<Number>()));
    }
    template<class Number, class Enable = void>
    struct from_rep;
    template<class Number>
    struct from_rep<Number, _impl::enable_if_t<cnl::is_integral<Number>::value>> {
        template<class Rep>
        constexpr Number operator()(Rep const& rep) const {
            return static_cast<Number>(rep);
        }
    };
    namespace _impl {
        template<class Result, class F, class ... Args,
                _impl::enable_if_t<!_num_traits_impl::are_composite<Args ...>::value, int> dummy = 0>
        constexpr Result for_rep(F f, Args &&...args) {
            return f(std::forward<Args>(args)...);
        }
        template<class Result, class F, class ... Args,
                _impl::enable_if_t<_num_traits_impl::are_composite<Args ...>::value, int> dummy = 0>
        constexpr Result for_rep(F f, Args &&...args) {
            return for_rep<Result>(f, to_rep(std::forward<Args>(args))...);
        }
    }
    namespace _num_traits_impl {
        template<int Width, bool IsSigned>
        struct make_integer;
        template<int Width>
        struct make_integer<Width, true> {
            using type = set_digits_t<signed, Width-1>;
        };
        template<int Width>
        struct make_integer<Width, false> {
            using type = set_digits_t<unsigned, Width>;
        };
        template<int Width, bool IsSigned>
        using make_integer_t = typename make_integer<Width, IsSigned>::type;
    }
    template<class Number, class Value, class Enable = void>
    struct from_value {
        using type = void;
    };
    template<class Number, class Value>
    struct from_value<Number, Value, _impl::enable_if_t<cnl::is_integral<Number>::value>> {
        using type = typename std::conditional<
                cnl::is_integral<Value>::value,
                Value,
                _num_traits_impl::make_integer_t<cnl::digits<Value>::value, cnl::is_signed<Value>::value>>::type;
    };
    template<class Number, class Value>
    using from_value_t = typename from_value<Number, Value>::type;
    namespace _impl {
        template<class Number, class Value>
        constexpr auto from_value(Value const& value)
        -> cnl::from_value_t<Number, Value>
        {
            static_assert(
                    !std::is_same<void, cnl::from_value_t<Number, Value>>::value,
                    "cnl::from_value is missing a specialization");
            return value;
        }
    }
    template< ::cnl::intmax ConstantValue, class InputValue>
    struct from_value<constant<ConstantValue>, InputValue> {
        using type = constant<InputValue{ConstantValue}>;
    };
    template<int Digits, int Radix, class S, class Enable = void>
    struct shift;
    namespace _impl {
        template<int Digits, int Radix, typename S, class Enable = void>
        struct default_shift;
        template<int Digits, int Radix, typename S>
        struct default_shift<Digits, Radix, S, _impl::enable_if_t<0<=Digits>> {
            constexpr auto operator()(S const& s) const
            -> decltype(s*power<S, Digits, Radix>())
            {
                return s*power<S, Digits, Radix>();
            }
        };
        template<int Digits, int Radix, typename S>
        struct default_shift<Digits, Radix, S, _impl::enable_if_t<Digits<0>> {
            constexpr auto operator()(S const& s) const
            -> decltype(s/power<S, -Digits, Radix>())
            {
                return s/power<S, -Digits, Radix>();
            }
        };
    }
    template<int Digits, int Radix, class S>
    struct shift<Digits, Radix, S, _impl::enable_if_t<cnl::is_integral<S>::value>>
            : _impl::default_shift<Digits, Radix, S> {
    };
    namespace _impl {
        template<int Digits, int Radix=2, class S=void>
        constexpr auto shift(S const& s)
        -> decltype(cnl::shift<Digits, Radix, S>{}(s))
        {
            return cnl::shift<Digits, Radix, S>{}(s);
        }
    }
    template<int Digits, int Radix, class S>
    struct scale {
        constexpr S operator()(S const& s) const
        {
            return static_cast<S>(shift<Digits, Radix, S>()(s));
        }
    };
    namespace _impl {
        template<int Digits, int Radix=2, class S=void>
        constexpr S scale(S const& s)
        {
            return cnl::scale<Digits, Radix, S>()(s);
        }
    }
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
            return s%width==0 ? x : static_cast<T>((x << (s%width)) | (x >> (width-(s%width))));
        }
        template<typename T>
        constexpr T rotr(T x, unsigned int s, unsigned int width) noexcept
        {
            static_assert(is_integral_unsigned<T>(), "T must be unsigned integer");
            return s%width==0 ? x : static_cast<T>((x >> (s%width)) | (x << (width-(s%width))));
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
    template<typename T>
    constexpr T sqrt(T arg) {
        return std::sqrt(arg);
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
    namespace _numeric_impl {
        template<int Radix, typename Integer>
        constexpr int used_digits_positive(Integer const& value)
        {
            static_assert(cnl::numeric_limits<Integer>::is_integer,
                    "Integer parameter of used_digits_positive() must be a fundamental integer.");
            return (value>0) ? 1+used_digits_positive<Radix>(value/Radix) : 0;
        }
        template<int Radix, bool IsSigned>
        struct used_digits {
            template<class Integer>
            constexpr int operator()(Integer const& value) const
            {
                return value ? used_digits_positive<Radix>(value) : 0;
            }
        };
        template<int Radix>
        struct used_digits<Radix, true> {
            template<class Integer>
            constexpr int operator()(Integer const& value) const
            {
                static_assert(cnl::numeric_limits<Integer>::is_integer,
                        "Integer parameter of used_digits()() must be a fundamental integer.");
                return (value>0)
                       ? used_digits_positive<Radix>(value)
                       : (value==0)
                         ? 0
                         : used_digits()(Integer(-1)-value);
            }
        };
    }
    template<int Radix = 2, typename Integer=void>
    constexpr int used_digits(Integer const& value)
    {
        return _impl::for_rep<int>(_numeric_impl::used_digits<Radix, is_signed<Integer>::value>(), value);
    }
    template<class Integer>
    constexpr int leading_bits(Integer const& value)
    {
        return digits<Integer>::value-used_digits(value);
    }
    template< ::cnl::intmax Value>
    struct digits<constant<Value>> : std::integral_constant<_digits_type, used_digits<>((Value<0) ? -Value : Value)> {
    };
}
namespace cnl {
    namespace _impl {
        template<class T, class Enable = void>
        struct wants_generic_ops : std::false_type {
        };
        struct binary_op {
            using is_not_comparison = void;
        };
        struct comparison_op {
            using is_comparison = void;
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
        struct add_op : binary_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs+rhs)
            {
                return lhs+rhs;
            }
        };
        struct subtract_op : binary_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs-rhs)
            {
                return lhs-rhs;
            }
        };
        struct multiply_op : binary_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs*rhs)
            {
                return lhs*rhs;
            }
        };
        struct divide_op : binary_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs/rhs)
            {
                return lhs/rhs;
            }
        };
        struct modulo_op : binary_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs%rhs)
            {
                return lhs%rhs;
            }
        };
        struct bitwise_or_op : binary_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs | rhs)
            {
                return lhs | rhs;
            }
        };
        struct bitwise_and_op : binary_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs & rhs)
            {
                return lhs & rhs;
            }
        };
        struct bitwise_xor_op : binary_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs ^ rhs)
            {
                return lhs ^ rhs;
            }
        };
        struct shift_left_op : binary_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs << rhs)
            {
                return lhs << rhs;
            }
        };
        struct shift_right_op : binary_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs >> rhs)
            {
                return lhs >> rhs;
            }
        };
        struct equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs==rhs)
            {
                return lhs==rhs;
            }
        };
        struct not_equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs!=rhs)
            {
                return lhs!=rhs;
            }
        };
        struct less_than_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs<rhs)
            {
                return lhs<rhs;
            }
        };
        struct greater_than_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs>rhs)
            {
                return lhs>rhs;
            }
        };
        struct less_than_or_equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs<=rhs)
            {
                return lhs<=rhs;
            }
        };
        struct greater_than_or_equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> decltype(lhs>=rhs)
            {
                return lhs>=rhs;
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
        template<class Operator, class Lhs, class Rhs>
        using op_result = decltype(Operator()(std::declval<Lhs>(), std::declval<Rhs>()));
        template<class Operator, class Operand, class Enable = void>
        struct unary_operator;
        template<class Operator, class LhsOperand, class RhsOperand, class Enable = void>
        struct binary_operator;
        template<class Operator, class LhsOperand, class RhsOperand, class Enable = void>
        struct compound_assignment_operator {
            constexpr LhsOperand& operator()(LhsOperand& lhs, RhsOperand const& rhs) const
            {
                return lhs = static_cast<LhsOperand>(
                        binary_operator<typename Operator::binary, LhsOperand, RhsOperand>()(lhs, rhs));
            }
        };
    }
    namespace _operators_impl {
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
    }
    template<class Operand> constexpr auto operator + (Operand const& operand) -> decltype(cnl::_impl::unary_operator<cnl::_operators_impl::enable_unary_t< Operand, cnl::_impl::plus_op>, Operand>()(operand)) { return cnl::_impl::unary_operator<cnl::_impl::plus_op, Operand>()(operand); }
    template<class Operand> constexpr auto operator - (Operand const& operand) -> decltype(cnl::_impl::unary_operator<cnl::_operators_impl::enable_unary_t< Operand, cnl::_impl::minus_op>, Operand>()(operand)) { return cnl::_impl::unary_operator<cnl::_impl::minus_op, Operand>()(operand); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator + (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::add_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::add_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator - (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::subtract_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::subtract_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator * (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::multiply_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::multiply_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator / (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::divide_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::divide_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator % (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::modulo_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::modulo_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator | (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::bitwise_or_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::bitwise_or_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator & (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::bitwise_and_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::bitwise_and_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator ^ (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::bitwise_xor_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::bitwise_xor_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator << (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::shift_left_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::shift_left_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator >> (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::shift_right_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::shift_right_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator == (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::equal_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::equal_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator != (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::not_equal_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::not_equal_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator < (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::less_than_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::less_than_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator > (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::greater_than_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::greater_than_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator <= (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::less_than_or_equal_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::less_than_or_equal_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator >= (LhsOperand const& lhs, RhsOperand const& rhs) -> decltype(cnl::_impl::binary_operator<cnl::_operators_impl::enable_binary_t< LhsOperand, RhsOperand, cnl::_impl::greater_than_or_equal_op>, LhsOperand, RhsOperand>()(lhs, rhs)) { return cnl::_impl::binary_operator<cnl::_impl::greater_than_or_equal_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator += (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_operators_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_add_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_add_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator -= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_operators_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_subtract_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_subtract_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator *= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_operators_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_multiply_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_multiply_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator /= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_operators_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_divide_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_divide_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator %= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_operators_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_modulo_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_modulo_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator |= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_operators_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_bitwise_or_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_bitwise_or_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator &= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_operators_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_bitwise_and_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_bitwise_and_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator ^= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_operators_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_bitwise_xor_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_bitwise_xor_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator <<= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_operators_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_shift_left_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_shift_left_op, LhsOperand, RhsOperand>()(lhs, rhs); }
    template<class LhsOperand, class RhsOperand> constexpr auto operator >>= (LhsOperand& lhs, RhsOperand const& rhs) -> cnl::_operators_impl::enable_binary_t<LhsOperand, RhsOperand, decltype( cnl::_impl::compound_assignment_operator<cnl::_impl::assign_shift_right_op, LhsOperand, RhsOperand>()(lhs, rhs))> { return cnl::_impl::compound_assignment_operator<cnl::_impl::assign_shift_right_op, LhsOperand, RhsOperand>()(lhs, rhs); }
}
namespace cnl {
    namespace _impl {
        template<class Derived, class Rep>
        class number_base;
        template<class Derived, class Rep>
        constexpr Rep to_rep(number_base<Derived, Rep> const& number);
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
            constexpr number_base& operator=(T const& r) {
                _rep = r;
                return static_cast<Derived&>(*this);
            }
            friend constexpr rep to_rep<>(number_base const&);
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
        template<class Wrapper, class Rep = decltype(to_rep(std::declval<Wrapper>()))>
        struct depth;
        template<class Wrapper, class Rep>
        struct depth {
            static constexpr auto value = depth<Rep>::value + 1;
        };
        template<class T>
        struct depth<T, T> : std::integral_constant<int, 0> {};
        template<class Rep, class Wrapper>
        struct is_wrappable;
        template<class Rep, int RepN, class Wrapper>
        struct is_wrappable<Rep[RepN], Wrapper> : std::false_type {};
        template<class Rep, class Wrapper, int WrapperN>
        struct is_wrappable<Rep, Wrapper[WrapperN]> : std::false_type {};
        template<class Rep, class Wrapper>
        struct is_wrappable : std::integral_constant<bool, cnl::numeric_limits<Rep>::is_specialized
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
            -> decltype(Operator()(from_value<Rhs>(lhs), rhs)) {
                return Operator()(from_value<Rhs>(lhs), rhs);
            }
        };
        template<class Operator, class Lhs, class Rhs>
        struct binary_operator<
                Operator, Lhs, Rhs,
                enable_if_t<is_derived_from_number_base<Lhs>::value && is_wrappable<Rhs, Lhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(Operator()(lhs, from_value<Lhs>(rhs)))
            {
                return Operator()(lhs, from_value<Lhs>(rhs));
            }
        };
        template<class Number>
        struct wants_generic_ops<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> : std::true_type {
        };
    }
    template<class Number>
    struct is_composite<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> : std::true_type {
    };
    namespace _impl {
        template<class Number>
        struct get_rep;
        template<class Number>
        using get_rep_t = typename get_rep<Number>::type;
        template<class Number, class NewRep, class Enable = void>
        struct set_rep;
        template<class Number, class NewRep>
        using set_rep_t = typename set_rep<Number, NewRep>::type;
    }
    template<class Number>
    struct make_signed<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> {
        using type = _impl::set_rep_t<Number, make_signed_t<_impl::get_rep_t<Number>>>;
    };
    template<class Number>
    struct make_unsigned<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> {
        using type = _impl::set_rep_t<Number, make_unsigned_t<_impl::get_rep_t<Number>>>;
    };
    namespace _impl {
        template<class Derived, class Rep>
        constexpr Rep to_rep(number_base<Derived, Rep> const& number) {
            return number._rep;
        }
    }
    template<int Digits, int Radix, class Derived>
    struct shift<Digits, Radix, _impl::number_base<Derived, typename Derived::rep>> {
        using _scalar_type = _impl::number_base<Derived, typename Derived::rep>;
        constexpr auto operator()(_scalar_type const &s) const
        -> decltype(from_rep<Derived>{}(_impl::shift<Digits, Radix>(to_rep(s))))
        {
            return from_rep<Derived>{}(_impl::shift<Digits, Radix>(to_rep(s)));
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
            return from_rep<_value_type>{}(_rep_numeric_limits::min());
        }
        static constexpr _value_type max() noexcept
        {
            return from_rep<_value_type>{}(_rep_numeric_limits::max());
        }
        static constexpr _value_type lowest() noexcept
        {
            return from_rep<_value_type>{}(_rep_numeric_limits::lowest());
        }
        static constexpr _value_type epsilon() noexcept
        {
            return from_rep<_value_type>{}(_rep_numeric_limits::round_error());
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
    struct digits<elastic_integer<Digits, Narrowest>> : std::integral_constant<_digits_type, Digits> {
    };
    template<int Digits, class Narrowest, _digits_type MinNumBits>
    struct set_digits<elastic_integer<Digits, Narrowest>, MinNumBits> {
        using type = elastic_integer<MinNumBits, Narrowest>;
    };
    template<int Digits, class Narrowest>
    constexpr auto to_rep(elastic_integer<Digits, Narrowest> const& number)
    -> typename elastic_integer<Digits, Narrowest>::rep
    {
        using base_type = typename elastic_integer<Digits, Narrowest>::_base;
        return to_rep(static_cast<base_type const&>(number));
    }
    namespace _impl {
        template<int Digits, class Narrowest>
        struct get_rep<elastic_integer<Digits, Narrowest>> {
            using type = Narrowest;
        };
        template<int Digits, class OldNarrowest, class NewNarrowest>
        struct set_rep<elastic_integer<Digits, OldNarrowest>, NewNarrowest> {
            using type = elastic_integer<Digits, NewNarrowest>;
        };
    }
    template<int Digits, class Narrowest>
    struct from_rep<elastic_integer<Digits, Narrowest>> {
        template<typename Rep>
        constexpr auto operator()(Rep const& r) const
        -> elastic_integer<Digits, cnl::_impl::make_signed_t<Narrowest, cnl::is_signed<Rep>::value>>
        {
            return r;
        }
    };
    template<int Digits, class Narrowest, class Value>
    struct from_value<elastic_integer<Digits, Narrowest>, Value> {
        using type = elastic_integer<cnl::digits<Value>::value, cnl::_impl::make_signed_t<Narrowest, cnl::is_signed<Value>::value>>;
    };
    template<int Digits, class Narrowest, intmax Value>
    struct from_value<elastic_integer<Digits, Narrowest>, constant<Value>> {
        static constexpr auto _to_digits = digits<constant<Value>>::value;
        using type = elastic_integer<_to_digits, Narrowest>;
    };
    template<int ShiftDigits, int ScalarDigits, class ScalarNarrowest>
    struct scale<ShiftDigits, 2, elastic_integer<ScalarDigits, ScalarNarrowest>> {
        constexpr auto operator()(elastic_integer<ScalarDigits, ScalarNarrowest> const& s) const
        -> elastic_integer<ScalarDigits, ScalarNarrowest>
        {
            using result_type = elastic_integer<ScalarDigits, ScalarNarrowest>;
            using result_rep = typename result_type::rep;
            return to_rep(s) * (result_rep{1} << ShiftDigits);
        }
    };
    template<int ShiftDigits, int ScalarDigits, class ScalarNarrowest>
    struct shift<ShiftDigits, 2, elastic_integer<ScalarDigits, ScalarNarrowest>, _impl::enable_if_t<0 <= ShiftDigits>> {
        constexpr auto operator()(elastic_integer<ScalarDigits, ScalarNarrowest> const& s) const
        -> elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>
        {
            using result_type = elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>;
            using result_rep = typename result_type::rep;
            return to_rep(s) * (result_rep{1} << ShiftDigits);
        }
    };
    template<int ShiftDigits, int ScalarDigits, class ScalarNarrowest>
    struct shift<ShiftDigits, 2, elastic_integer<ScalarDigits, ScalarNarrowest>, _impl::enable_if_t<ShiftDigits < 0>> {
        constexpr auto operator()(elastic_integer<ScalarDigits, ScalarNarrowest> const& s) const
        -> elastic_integer<ShiftDigits+ScalarDigits, ScalarNarrowest>
        {
            using divisor_type = elastic_integer<1-ShiftDigits, ScalarNarrowest>;
            using divisor_rep = typename divisor_type::rep;
            return to_rep(s) / (divisor_rep{1} << -ShiftDigits);
        }
    };
    template<int Digits = digits<int>::value, class Narrowest = int>
    class elastic_integer : public _elastic_integer_impl::base_class_t<Digits, Narrowest> {
        static_assert(Digits > 0, "type requires positive number of digits");
    public:
        using _base = _elastic_integer_impl::base_class_t<Digits, Narrowest>;
        static_assert(!_elastic_integer_impl::is_elastic_integer<typename _base::rep>::value,
                "elastic_integer of elastic_integer is not a supported");
        using rep = typename _base::rep;
        constexpr elastic_integer() = default;
        template<class Number, _impl::enable_if_t<numeric_limits<Number>::is_specialized, int> Dummy = 0>
        constexpr elastic_integer(Number n)
                : _base(static_cast<rep>(n))
        {
        }
        template<int FromWidth, class FromNarrowest>
        explicit constexpr elastic_integer(elastic_integer<FromWidth, FromNarrowest> const& rhs)
                :_base(static_cast<rep>(to_rep(rhs)))
        {
        }
        template< ::cnl::intmax Value>
        constexpr elastic_integer(constant<Value>)
                : _base(static_cast<rep>(Value))
        {
        }
        template<class S, _impl::enable_if_t<std::is_floating_point<S>::value, int> Dummy = 0>
        elastic_integer& operator=(S s)
        {
            _base::operator=(floating_point_to_rep(s));
            return *this;
        }
        template<class S>
        explicit constexpr operator S() const
        {
            return static_cast<S>(to_rep(*this));
        }
    };
    namespace _elastic_integer_impl {
        template<bool Signed>
        struct machine_digits {
            static constexpr _digits_type value =
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
    template<class Narrowest = void, class Integral, _impl::enable_if_t<!_impl::is_constant<Integral>::value, int> Dummy = 0>
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
        return from_rep<elastic_integer>{}(
            static_cast<rep>(
                    to_rep(rhs)
                ^ ((static_cast<rep>(~0)) >> (numeric_limits<rep>::digits - RhsDigits))));
    }
    namespace _impl {
        template<int FromDigits, class FromNarrowest, int OtherDigits, class OtherNarrowest,
                _impl::enable_if_t<FromDigits!=OtherDigits || !std::is_same<FromNarrowest, OtherNarrowest>::value, std::nullptr_t> Dummy = nullptr>
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
        struct binary_operator<Operator,
                elastic_integer<LhsDigits, LhsNarrowest>, elastic_integer<RhsDigits, RhsNarrowest>,
                typename Operator::is_comparison> {
            constexpr auto operator()(
                    elastic_integer<LhsDigits, LhsNarrowest> const& lhs,
                    elastic_integer<RhsDigits, RhsNarrowest> const& rhs) const
            -> decltype(Operator()(cast_to_common_type(lhs, rhs), cast_to_common_type(rhs, lhs)))
            {
                return Operator()(cast_to_common_type(lhs, rhs), cast_to_common_type(rhs, lhs));
            }
        };
        template<class Operator, int Digits, class Narrowest>
        struct binary_operator<Operator, elastic_integer<Digits, Narrowest>, elastic_integer<Digits, Narrowest>,
                typename Operator::is_comparison> {
            constexpr auto operator()(
                    elastic_integer<Digits, Narrowest> const& lhs,
                    elastic_integer<Digits, Narrowest> const& rhs) const
            -> decltype(Operator()(to_rep(lhs), to_rep(rhs)))
            {
                return Operator()(to_rep(lhs), to_rep(rhs));
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
            static constexpr _digits_type narrowest_width = _impl::max(
                    digits<LhsNarrowest>::value + cnl::is_signed<LhsNarrowest>::value,
                    digits<RhsNarrowest>::value + cnl::is_signed<RhsNarrowest>::value);
            using narrowest = set_digits_t<_impl::make_signed_t<rep_result, policy::is_signed>, narrowest_width-policy::is_signed>;
            using result_type = elastic_integer<policy::digits, narrowest>;
        };
        template<class Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
        struct binary_operator<Operator,
                elastic_integer<LhsDigits, LhsNarrowest>, elastic_integer<RhsDigits, RhsNarrowest>,
                typename Operator::is_not_comparison> {
            constexpr auto operator()(
                    elastic_integer<LhsDigits, LhsNarrowest> const& lhs,
                    elastic_integer<RhsDigits, RhsNarrowest> const& rhs) const
            -> typename operate_params<Operator, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::result_type
            {
                using result_type = typename operate_params<Operator, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::result_type;
                return from_rep<result_type>{}(
                        static_cast<typename result_type::rep>(Operator()(
                                to_rep(static_cast<result_type>(lhs)),
                                to_rep(static_cast<result_type>(rhs)))));
            }
        };
    }
    template<int LhsDigits, class LhsNarrowest, ::cnl::intmax RhsValue>
    constexpr auto operator<<(elastic_integer<LhsDigits, LhsNarrowest> const& lhs, constant<RhsValue>)
    -> decltype(from_rep<elastic_integer<LhsDigits + static_cast<int>(RhsValue), LhsNarrowest>>{}(
            to_rep(static_cast<elastic_integer<LhsDigits + static_cast<int>(RhsValue), LhsNarrowest>>(lhs)) << RhsValue)) {
        using result_type = elastic_integer<LhsDigits + static_cast<int>(RhsValue), LhsNarrowest>;
        return from_rep<result_type>{}(to_rep(static_cast<result_type>(lhs)) << RhsValue);
    }
    template<int LhsDigits, class LhsNarrowest, ::cnl::intmax RhsValue>
    constexpr auto operator>>(elastic_integer<LhsDigits, LhsNarrowest> const& lhs, constant<RhsValue>)
    -> decltype (from_rep<elastic_integer<LhsDigits - static_cast<int>(RhsValue), LhsNarrowest>>{}(to_rep(lhs) >> RhsValue)) {
        return from_rep<elastic_integer<LhsDigits - static_cast<int>(RhsValue), LhsNarrowest>>{}(to_rep(lhs) >> RhsValue);
    }
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator-(elastic_integer<RhsDigits, RhsNarrowest> const& rhs)
    -> decltype(from_rep<elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>>{}(-to_rep(static_cast<elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>>(rhs))))
    {
        using result_type = elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>;
        return from_rep<result_type>{}(-to_rep(static_cast<result_type>(rhs)));
    }
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator+(elastic_integer<RhsDigits, RhsNarrowest> const& rhs)
    -> decltype(from_rep<elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>>{}(
            to_rep(static_cast<elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>>(rhs))))
    {
        using result_type = elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>;
        return from_rep<result_type>{}(to_rep(static_cast<result_type>(rhs)));
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
    namespace _elastic_integer_impl {
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
            return from_rep<_value_type>{}(1);
        }
        static constexpr _value_type max() noexcept
        {
            return from_rep<_value_type>{}(_rep_max());
        }
        static constexpr _value_type lowest() noexcept
        {
            return _elastic_integer_impl::lowest<_rep, _narrowest_numeric_limits::is_signed>()(_rep_max());
        }
    };
    template<int Digits, class Narrowest>
    struct numeric_limits<elastic_integer<Digits, Narrowest> const>
            : numeric_limits<elastic_integer<Digits, Narrowest>> {
    };
}
namespace cnl {
    template<typename Rep = int, int Exponent = 0, int Radix = 2>
    class fixed_point;
    template<typename Rep, int Exponent, int Radix>
    constexpr Rep to_rep(fixed_point<Rep, Exponent, Radix> const&);
    template<typename Numerator, typename Denominator>
    struct fractional;
    namespace _impl {
        template<class T>
        struct is_fixed_point
                : public std::false_type {
        };
        template<typename Rep, int Exponent, int Radix>
        struct is_fixed_point<fixed_point<Rep, Exponent, Radix>>
                : public std::true_type {
        };
        namespace fp {
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
            using float_of_same_size = typename float_of_size<digits<T>::value + is_signed<T>::value>::type;
        }
    }
    template<typename Rep, int Exponent, int Radix>
    class fixed_point
            : public _impl::number_base<fixed_point<Rep, Exponent, Radix>, Rep> {
        static_assert(Radix>=2, "Radix must be two or greater");
        static_assert(!_impl::is_fixed_point<Rep>::value,
                "fixed_point of fixed_point is not a supported");
    public:
        using rep = Rep;
        using _base = _impl::number_base<fixed_point<Rep, Exponent, Radix>, Rep>;
        constexpr static int exponent = Exponent;
        constexpr static int radix = Radix;
    private:
        constexpr fixed_point(rep r, int)
                :_base(r)
        {
        }
    public:
        constexpr fixed_point() = default;
        template<class FromRep, int FromExponent>
        constexpr fixed_point(fixed_point<FromRep, FromExponent, Radix> const& rhs)
                : _base(
                static_cast<Rep>(_impl::shift<FromExponent-exponent, Radix>(
                        _impl::from_value<Rep>(cnl::to_rep(rhs)))))
        {
        }
        template< ::cnl::intmax Value>
        constexpr fixed_point(constant<Value>)
                : fixed_point(from_rep<fixed_point<decltype(Value), 0>>{}(Value))
        {
        }
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_integer, int> Dummy = 0>
        constexpr fixed_point(S const& s)
                : _base(static_cast<Rep>(_impl::shift<-exponent, Radix>(_impl::from_value<Rep>(s))))
        {
        }
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
        constexpr fixed_point(S s)
                :_base(floating_point_to_rep(s))
        {
        }
        template<typename Numerator, typename Denominator>
        constexpr fixed_point(fractional<Numerator, Denominator> const& f);
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
        constexpr fixed_point& operator=(S s)
        {
            _base::operator=(floating_point_to_rep(s));
            return *this;
        }
        template<class FromRep, int FromExponent>
        constexpr fixed_point& operator=(fixed_point<FromRep, FromExponent, Radix> const& rhs)
        {
            _base::operator=(fixed_point_to_rep(rhs));
            return *this;
        }
        template<typename Numerator, typename Denominator>
        constexpr fixed_point& operator=(fractional<Numerator, Denominator> const& f);
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_integer, int> Dummy = 0>
        explicit constexpr operator S() const
        {
            return static_cast<S>(_impl::shift<exponent>(to_rep(*this)));
        }
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
        explicit constexpr operator S() const
        {
            static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
            return S(to_rep(*this))*inverse_one<S>();
        }
        template<class, class>
        friend struct from_rep;
    private:
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy = 0>
        static constexpr S one();
        template<class S, _impl::enable_if_t<numeric_limits<S>::is_integer, int> Dummy = 0>
        static constexpr S one();
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
    template<class S, _impl::enable_if_t<numeric_limits<S>::is_iec559, int> Dummy>
    constexpr S fixed_point<Rep, Exponent, Radix>::one()
    {
        return _impl::power<S, -exponent, Radix>();
    }
    template<typename Rep, int Exponent, int Radix>
    template<class S, _impl::enable_if_t<numeric_limits<S>::is_integer, int> Dummy>
    constexpr S fixed_point<Rep, Exponent, Radix>::one()
    {
        return from_rep<fixed_point<S, 0>>{}(1);
    }
    template<typename Rep, int Exponent, int Radix>
    template<class S>
    constexpr S fixed_point<Rep, Exponent, Radix>::inverse_one()
    {
        static_assert(numeric_limits<S>::is_iec559, "S must be floating-point type");
        return _impl::power<S, exponent, Radix>();
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
        return _impl::shift<FromExponent-exponent>(to_rep(rhs));
    }
}
namespace cnl {
    namespace _impl {
        template<typename Rep, int Exponent, int Radix>
        struct get_rep<fixed_point<Rep, Exponent, Radix>> {
            using type = Rep;
        };
        template <class OldRep, int Exponent, class NewRep>
        struct set_rep<fixed_point<OldRep, Exponent>, NewRep> {
            using type = fixed_point<NewRep, Exponent>;
        };
    }
    template<typename Rep, int Exponent, int Radix>
    struct digits<fixed_point<Rep, Exponent, Radix>> : digits<Rep> {
    };
    template<typename Rep, int Exponent, int Radix, _digits_type MinNumBits>
    struct set_digits<fixed_point<Rep, Exponent, Radix>, MinNumBits> {
        using type = fixed_point<set_digits_t<Rep, MinNumBits>, Exponent, Radix>;
    };
    template<typename ArchetypeRep, int Exponent, int Radix>
    struct from_rep<fixed_point<ArchetypeRep, Exponent, Radix>> {
        template<typename Rep>
        constexpr auto operator()(Rep const& r) const
        -> fixed_point<Rep, Exponent, Radix>
        {
            return fixed_point<Rep, Exponent, Radix>(r, 0);
        }
    };
    template<typename Rep, int Exponent, int Radix>
    constexpr Rep to_rep(fixed_point<Rep, Exponent, Radix> const& number)
    {
        using base_type = typename fixed_point<Rep, Exponent, Radix>::_base;
        return to_rep(static_cast<base_type const&>(number));
    }
    template<typename Rep, int Exponent, int Radix, typename Value>
    struct from_value<fixed_point<Rep, Exponent, Radix>, Value> {
        using type = fixed_point<Value, 0, Radix>;
    };
    template<typename Rep, int Exponent, int Radix, typename ValueRep, int ValueExponent>
    struct from_value<fixed_point<Rep, Exponent, Radix>, fixed_point<ValueRep, ValueExponent>> {
        using type = fixed_point<from_value_t<Rep, ValueRep>, ValueExponent>;
    };
    template<typename Rep, int Exponent, int Radix, ::cnl::intmax Value>
    struct from_value<fixed_point<Rep, Exponent, Radix>, constant<Value>> {
        using type = fixed_point<
        set_digits_t<int, _impl::max(digits<int>::value, used_digits(Value)-trailing_bits(Value))>,
        trailing_bits(Value)>;
    };
    namespace _impl {
        template <class T>
        struct fractional_digits : std::integral_constant<_digits_type, 0> {
        };
        template<typename Rep, int Exponent, int Radix>
        struct fractional_digits<fixed_point<Rep, Exponent, Radix>> : std::integral_constant<_digits_type, -Exponent> {
        };
        template <class T>
        struct integer_digits : std::integral_constant<_digits_type, digits<T>::value - fractional_digits<T>::value> {
        };
    }
}
namespace cnl {
    template<typename Value>
    constexpr auto make_fixed_point(Value const& value)
    -> cnl::from_value_t<fixed_point<Value, 0>, Value>
    {
        return _impl::from_value<fixed_point<Value, 0>>(value);
    }
    namespace _multiply_impl {
        template<class Operand>
        struct fixed_point_type {
            using type = fixed_point<Operand, 0>;
        };
        template<typename Rep, int Exponent, int Radix>
        struct fixed_point_type<fixed_point<Rep, Exponent, Radix>> {
            using type = fixed_point<Rep, Exponent, Radix>;
        };
        template<class Lhs, class Rhs>
        struct params {
            using lhs_type = typename fixed_point_type<Lhs>::type;
            using rhs_type = typename fixed_point_type<Rhs>::type;
            using lhs_rep = typename lhs_type::rep;
            using rhs_rep = typename rhs_type::rep;
            static constexpr int rep_exponent = lhs_type::exponent + rhs_type::exponent;
            using rep_op_result = _impl::op_result<_impl::multiply_op, lhs_rep, rhs_rep>;
            static constexpr int sum_digits = digits<lhs_type>::value + digits<rhs_type>::value;
            static constexpr bool is_signed =
                    numeric_limits<lhs_rep>::is_signed || numeric_limits<rhs_rep>::is_signed;
            using prewidened_result_rep = _impl::make_signed_t<rep_op_result, is_signed>;
            using result_rep = set_digits_t<prewidened_result_rep, sum_digits>;
            using rep_type = typename std::conditional<
                    digits<prewidened_result_rep>::value >= sum_digits,
                    lhs_rep, result_rep>::type;
            using result_type = fixed_point<result_rep, rep_exponent>;
            using intermediate_lhs = fixed_point<rep_type, lhs_type::exponent>;
            using intermediate_rhs = Rhs;
        };
    }
    template<class Lhs, class Rhs>
    constexpr auto multiply(Lhs const& lhs, Rhs const& rhs)
    -> typename _multiply_impl::params<Lhs, Rhs>::result_type
    {
        using params = _multiply_impl::params<Lhs, Rhs>;
        using intermediate_lhs = typename params::intermediate_lhs;
        using intermediate_rhs = typename params::intermediate_rhs;
        using result_type = typename params::result_type;
        using result_rep = typename result_type::rep;
        return from_rep<result_type>{}(
                static_cast<result_rep>(to_rep(static_cast<intermediate_lhs>(lhs))
                                        * to_rep(static_cast<intermediate_rhs>(rhs))));
    }
    namespace _divide_impl {
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
            return to_rep(f);
        }
        template<typename Number>
        struct exponent : constant<0> {};
        template<typename Rep, int Exponent, int Radix>
        struct exponent<fixed_point<Rep, Exponent, Radix>> : constant<Exponent> {
        };
        template<class Quotient, class Dividend, class Divisor>
        struct exponent_shift : std::integral_constant<
                int,
                _divide_impl::exponent<Dividend>::value
                    -_divide_impl::exponent<Divisor>::value
                    -_divide_impl::exponent<Quotient>::value> {
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
                    _impl::integer_digits<Dividend>::value + _impl::fractional_digits<Divisor>::value;
            static constexpr int fractional_digits =
                    _impl::fractional_digits<Dividend>::value + _impl::integer_digits<Divisor>::value;
            static constexpr auto necessary_digits = integer_digits + fractional_digits;
            static constexpr auto natural_digits = digits<natural_result>::value;
            static constexpr auto result_digits = _impl::max(necessary_digits, natural_digits);
            using rep_type = set_digits_t<natural_result, result_digits>;
            static constexpr int rep_exponent = -fractional_digits;
            using type = fixed_point<typename fixed_point_rep<rep_type>::type, rep_exponent>;
        };
    }
    template<
            class Quotient = _divide_impl::default_quotient_tag,
            class Dividend,
            class Divisor>
    constexpr auto divide(Dividend const& dividend, Divisor const& divisor)
    -> typename _divide_impl::result<Quotient, Dividend, Divisor>::type {
        using quotient = typename _divide_impl::result<Quotient, Dividend, Divisor>::type;
        using quotient_rep = typename quotient::rep;
        return from_rep<quotient>()(
                static_cast<quotient_rep>(_impl::scale<_divide_impl::exponent_shift<quotient, Dividend, Divisor>::value>(
                        static_cast<quotient_rep>(_divide_impl::not_fixed_point(dividend)))
                        /_divide_impl::not_fixed_point(divisor)));
    }
}
namespace cnl {
    template<typename Numerator, typename Denominator = int>
    struct fractional {
        using numerator_type = Numerator;
        using denominator_type = Denominator;
        explicit constexpr fractional(Numerator const& n, Denominator const& d)
                : numerator{n}, denominator{d} {}
        explicit constexpr fractional(Numerator const& n)
                : numerator{n}, denominator{1} {}
        template<typename Scalar, _impl::enable_if_t<std::is_floating_point<Scalar>::value, int> = 0>
        explicit constexpr operator Scalar() const
        {
            return static_cast<Scalar>(numerator)/static_cast<Scalar>(denominator);
        }
        numerator_type numerator;
        denominator_type denominator = 1;
    };
    template<typename Numerator, typename Denominator>
    constexpr fractional<Numerator, Denominator> make_fractional(Numerator const& n, Denominator const& d)
    {
        return fractional<Numerator, Denominator>{n, d};
    }
    template<typename Numerator>
    constexpr fractional<Numerator, Numerator> make_fractional(Numerator const& n)
    {
        return fractional<Numerator, Numerator>{n, 1};
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator+(
            fractional<LhsNumerator, LhsDenominator> const& lhs,
            fractional<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(make_fractional(
            lhs.numerator*rhs.denominator+rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator))
    {
        return make_fractional(
                lhs.numerator*rhs.denominator+rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator);
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator-(
            fractional<LhsNumerator, LhsDenominator> const& lhs,
            fractional<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(make_fractional(
            lhs.numerator*rhs.denominator-rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator))
    {
        return make_fractional(
                lhs.numerator*rhs.denominator-rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator);
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator*(
            fractional<LhsNumerator, LhsDenominator> const& lhs,
            fractional<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(make_fractional(lhs.numerator*rhs.numerator, lhs.denominator*rhs.denominator))
    {
        return make_fractional(lhs.numerator*rhs.numerator, lhs.denominator*rhs.denominator);
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator/(
            fractional<LhsNumerator, LhsDenominator> const& lhs,
            fractional<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(make_fractional(lhs.numerator*rhs.denominator, lhs.denominator*rhs.numerator))
    {
        return make_fractional(lhs.numerator*rhs.denominator, lhs.denominator*rhs.numerator);
    }
    namespace _fractional_impl {
        template<typename Numerator, typename Denominator>
        constexpr auto one(fractional<Numerator, Denominator> const& f)
        -> decltype(f.numerator==f.denominator)
        {
            return f.numerator==f.denominator;
        }
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator==(
            fractional<LhsNumerator, LhsDenominator> const& lhs,
            fractional<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(_fractional_impl::one(lhs/rhs))
    {
        return _fractional_impl::one(lhs/rhs);
    }
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator!=(
            fractional<LhsNumerator, LhsDenominator> const& lhs,
            fractional<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(!(lhs==rhs))
    {
        return !(lhs==rhs);
    }
}
namespace cnl {
    template<typename Numerator, typename Denominator>
    constexpr auto make_fixed_point(fractional<Numerator, Denominator> const& value)
    -> decltype(divide(value.numerator, value.denominator))
    {
        return divide(value.numerator, value.denominator);
    }
    template<typename Rep, int Exponent, int Radix>
    template<typename Numerator, typename Denominator>
    constexpr fixed_point<Rep, Exponent, Radix>::fixed_point(fractional<Numerator, Denominator> const& f)
            : fixed_point(divide<fixed_point>(f.numerator, f.denominator))
    {
    }
    template<typename Rep, int Exponent, int Radix>
    template<typename Numerator, typename Denominator>
    constexpr fixed_point<Rep, Exponent, Radix>&
    fixed_point<Rep, Exponent, Radix>::operator=(fractional<Numerator, Denominator> const& f)
    {
        return operator=(divide<fixed_point>(f.numerator, f.denominator));
    }
}
namespace cnl {
    namespace _impl {
        namespace fp {
            namespace ct {
                template<class Lhs, class Rhs, class _Enable = void>
                struct common_type_mixed;
                template<class LhsRep, int LhsExponent, int LhsRadix, class RhsInteger>
                struct common_type_mixed<
                        fixed_point<LhsRep, LhsExponent, LhsRadix>,
                        RhsInteger,
                        _impl::enable_if_t<numeric_limits<RhsInteger>::is_integer>>
                : std::common_type<fixed_point<LhsRep, LhsExponent, LhsRadix>, fixed_point<RhsInteger, 0, LhsRadix>> {
                };
                template<class LhsRep, int LhsExponent, int LhsRadix, class Float>
                struct common_type_mixed<
                        fixed_point<LhsRep, LhsExponent, LhsRadix>, Float,
                        _impl::enable_if_t<std::is_floating_point<Float>::value>>
                    : std::common_type<_impl::fp::float_of_same_size<LhsRep>, Float> {
                };
            }
        }
    }
}
namespace std {
    template<typename Rep, int Exponent, int Radix>
    struct common_type<cnl::fixed_point<Rep, Exponent, Radix>> {
        using type = cnl::fixed_point<
                typename std::common_type<Rep>::type,
                Exponent>;
    };
    template<class LhsRep, int LhsExponent, int LhsRadix, class Rhs>
    struct common_type<cnl::fixed_point<LhsRep, LhsExponent, LhsRadix>, Rhs> {
        static_assert(!cnl::_impl::is_fixed_point<Rhs>::value, "fixed-point Rhs type");
        using type = typename cnl::_impl::fp::ct::common_type_mixed<
                cnl::fixed_point<LhsRep, LhsExponent, LhsRadix>,
                Rhs>::type;
    };
    template<class Lhs, class RhsRep, int RhsExponent, int RhsRadix>
    struct common_type<Lhs, cnl::fixed_point<RhsRep, RhsExponent, RhsRadix>> {
        static_assert(!cnl::_impl::is_fixed_point<Lhs>::value, "fixed-point Lhs type");
        using type = typename cnl::_impl::fp::ct::common_type_mixed<cnl::fixed_point<RhsRep, RhsExponent, RhsRadix>, Lhs>::type;
    };
    template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent, int Radix>
    struct common_type<cnl::fixed_point<LhsRep, LhsExponent, Radix>, cnl::fixed_point<RhsRep, RhsExponent, Radix>> {
        using type = cnl::fixed_point<
                cnl::_impl::common_type_t<LhsRep, RhsRep>,
                cnl::_impl::min(LhsExponent, RhsExponent),
                Radix>;
    };
}
namespace cnl {
}
namespace cnl {
    namespace _fixed_point_operators_impl {
        template<class Lhs, class Rhs>
        constexpr bool is_heterogeneous() {
            return (!std::is_same<Lhs, Rhs>::value) &&
                   (_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value);
        }
    }
    namespace _impl {
        template<typename Operator, typename Rep, int Exponent, int Radix>
        struct unary_operator<Operator, fixed_point<Rep, Exponent, Radix>> {
            constexpr auto operator()(fixed_point<Rep, Exponent, Radix> const& rhs) const
            -> decltype(from_rep<fixed_point<decltype(Operator()(to_rep(rhs))), Exponent, Radix>>{}(Operator()(to_rep(rhs))))
            {
                return from_rep<fixed_point<decltype(Operator()(to_rep(rhs))), Exponent, Radix>>{}(Operator()(to_rep(rhs)));
            }
        };
        template<typename Operator, typename Rep, int Exponent, int Radix>
        struct binary_operator<Operator, fixed_point<Rep, Exponent, Radix>, fixed_point<Rep, Exponent, Radix>,
                typename Operator::is_comparison> {
            constexpr auto operator()(
                    fixed_point<Rep, Exponent, Radix> const& lhs,
                    fixed_point<Rep, Exponent, Radix> const& rhs) const
            -> decltype(Operator()(to_rep(lhs), to_rep(rhs)))
            {
                return Operator()(to_rep(lhs), to_rep(rhs));
            }
        };
        template<typename Operator, typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct binary_operator<Operator, fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>,
                typename Operator::is_comparison> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs,
                    fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(Operator()(static_cast<_impl::common_type_t<fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>>>(lhs), static_cast<_impl::common_type_t<fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>>>(rhs)))
            {
                using common_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>>;
                return Operator()(static_cast<common_type>(lhs), static_cast<common_type>(rhs));
            }
        };
        template<typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct binary_operator<multiply_op, fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs,
                    fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(from_rep<fixed_point<decltype(to_rep(lhs)*to_rep(rhs)), LhsExponent+RhsExponent, Radix>>{}(to_rep(lhs)*to_rep(rhs)))
            {
                return from_rep<fixed_point<decltype(to_rep(lhs)*to_rep(rhs)), LhsExponent+RhsExponent, Radix>>{}(to_rep(lhs)*to_rep(rhs));
            }
        };
        template<typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct binary_operator<divide_op, fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs,
                    fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(from_rep<fixed_point<decltype(to_rep(lhs)/to_rep(rhs)), LhsExponent-RhsExponent, Radix>>{}(to_rep(lhs)
                    /to_rep(rhs)))
            {
                return from_rep<fixed_point<decltype(to_rep(lhs)/to_rep(rhs)), LhsExponent-RhsExponent, Radix>>{}(to_rep(lhs)
                        /to_rep(rhs));
            }
        };
        template<typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct binary_operator<modulo_op, fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs,
                    fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(from_rep<fixed_point<decltype(to_rep(lhs)%to_rep(rhs)), LhsExponent, Radix>>{}(to_rep(lhs)%to_rep(rhs)))
            {
                return from_rep<fixed_point<decltype(to_rep(lhs)%to_rep(rhs)), LhsExponent, Radix>>{}(to_rep(lhs)%to_rep(rhs));
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
                enable_if_t<is_zero_degree<Operator>::value, typename Operator::is_not_comparison>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, Exponent, Radix> const& lhs, fixed_point<RhsRep, Exponent, Radix> const& rhs) const
            -> decltype(from_rep<fixed_point<decltype(Operator()(to_rep(lhs), to_rep(rhs))), Exponent, Radix>>{}(
                    Operator()(to_rep(lhs), to_rep(rhs))))
            {
                return from_rep<fixed_point<decltype(Operator()(to_rep(lhs), to_rep(rhs))), Exponent, Radix>>{}(
                        Operator()(to_rep(lhs), to_rep(rhs)));
            }
        };
        template<typename Operator, typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct binary_operator<Operator, fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>,
                        enable_if_t<is_zero_degree<Operator>::value, typename Operator::is_not_comparison>> {
        private:
            static constexpr int _common_exponent = min(LhsExponent, RhsExponent);
            static constexpr int _lhs_left_shift = LhsExponent-_common_exponent;
            static constexpr int _rhs_left_shift = RhsExponent-_common_exponent;
        public:
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs, fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(Operator{}(
                    from_rep<fixed_point<LhsRep, _common_exponent, Radix>>{}(
                            _impl::shift<_lhs_left_shift>(to_rep(lhs))),
                    from_rep<fixed_point<RhsRep, _common_exponent, Radix>>{}(
                            _impl::shift<_rhs_left_shift>(to_rep(rhs)))))
            {
                return Operator{}(
                        from_rep<fixed_point<LhsRep, _common_exponent, Radix>>{}(
                                _impl::shift<_lhs_left_shift>(to_rep(lhs))),
                        from_rep<fixed_point<RhsRep, _common_exponent, Radix>>{}(
                                _impl::shift<_rhs_left_shift>(to_rep(rhs))));
            }
        };
    }
    template<typename LhsRep, int LhsExponent, int LhsRadix, typename Rhs>
    constexpr auto operator<<(fixed_point<LhsRep, LhsExponent, LhsRadix> const& lhs, Rhs const& rhs)
    -> decltype(from_rep<fixed_point<decltype(to_rep(lhs) << int(rhs)), LhsExponent, LhsRadix>>{}(to_rep(lhs) << int(rhs)))
    {
        return from_rep<fixed_point<decltype(to_rep(lhs) << int(rhs)), LhsExponent, LhsRadix>>{}(to_rep(lhs) << int(rhs));
    }
    template<typename LhsRep, int LhsExponent, int LhsRadix, typename Rhs>
    constexpr auto operator>>(fixed_point<LhsRep, LhsExponent, LhsRadix> const& lhs, Rhs const& rhs)
    -> decltype(from_rep<fixed_point<decltype(to_rep(lhs) >> int(rhs)), LhsExponent, LhsRadix>>{}(to_rep(lhs) >> int(rhs)))
    {
        return from_rep<fixed_point<decltype(to_rep(lhs) >> int(rhs)), LhsExponent, LhsRadix>>{}(to_rep(lhs) >> int(rhs));
    }
    template<typename LhsRep, int LhsExponent, int LhsRadix, ::cnl::intmax RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent+static_cast<int>(RhsValue), LhsRadix>
    operator<<(fixed_point<LhsRep, LhsExponent, LhsRadix> const& lhs, constant<RhsValue>)
    {
        return from_rep<fixed_point<LhsRep, LhsExponent+static_cast<int>(RhsValue), LhsRadix>>{}(to_rep(lhs));
    }
    template<typename LhsRep, int LhsExponent, int LhsRadix, ::cnl::intmax RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent-static_cast<int>(RhsValue), LhsRadix>
    operator>>(fixed_point<LhsRep, LhsExponent, LhsRadix> const& lhs, constant<RhsValue>)
    {
        return from_rep<fixed_point<LhsRep, LhsExponent-static_cast<int>(RhsValue), LhsRadix>>{}(to_rep(lhs));
    }
}
namespace cnl {
    template<typename Rep, int Exponent, int Radix>
    constexpr auto abs(fixed_point<Rep, Exponent, Radix> const& x) noexcept
    -> decltype(-x)
    {
        return (x >= 0) ? static_cast<decltype(-x)>(x) : -x;
    }
    namespace _impl {
        namespace fp {
            namespace extras {
                template<class Rep>
                constexpr Rep sqrt_bit(Rep n, Rep bit)
                {
                    return (bit>n) ? sqrt_bit<Rep>(n, static_cast<Rep>(bit >> 2)) : bit;
                }
                template<class Rep>
                constexpr Rep sqrt_bit(Rep n)
                {
                    return sqrt_bit<Rep>(n, Rep(1) << (Rep((digits<Rep>::value + is_signed<Rep>::value) - 2)));
                }
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
                struct sqrt_solve1 {
                    template<class Rep>
                    constexpr Rep operator()(Rep n) const
                    {
                        return sqrt_solve3<Rep>(n, sqrt_bit<Rep>(n), Rep{0});
                    }
                };
            }
        }
    }
    template<typename Rep, int Exponent, int Radix>
    constexpr fixed_point <Rep, Exponent, Radix>
    sqrt(fixed_point<Rep, Exponent, Radix> const& x)
    {
        using widened_rep = set_digits_t<Rep, digits<Rep>::value*2>;
        return
                (x<from_rep<fixed_point<Rep, Exponent, Radix>>{}(0))
                ? throw std::invalid_argument("cannot represent square root of negative value") :
                from_rep<fixed_point<Rep, Exponent, Radix>>{}(_impl::for_rep<widened_rep>(
                        _impl::fp::extras::sqrt_solve1(),
                        _impl::scale<-Exponent>(static_cast<widened_rep>(to_rep(x)))));
    }
    namespace _impl {
        namespace fp {
            namespace extras {
                template<typename Rep, int Exponent, int Radix, _impl::fp::float_of_same_size<Rep>(* F)(
                        _impl::fp::float_of_same_size<Rep>)>
                constexpr fixed_point <Rep, Exponent, Radix>
                crib(fixed_point<Rep, Exponent, Radix> const& x) noexcept
                {
                    using floating_point = _impl::fp::float_of_same_size<Rep>;
                    return static_cast<fixed_point<Rep, Exponent, Radix>>(F(static_cast<floating_point>(x)));
                }
            }
        }
    }
    template<typename Rep, int Exponent, int Radix>
    constexpr fixed_point <Rep, Exponent, Radix>
    sin(fixed_point<Rep, Exponent, Radix> const& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, Radix, std::sin>(x);
    }
    template<typename Rep, int Exponent, int Radix>
    constexpr fixed_point <Rep, Exponent, Radix>
    cos(fixed_point<Rep, Exponent, Radix> const& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, Radix, std::cos>(x);
    }
    template<typename Rep, int Exponent, int Radix>
    constexpr fixed_point <Rep, Exponent, Radix>
    exp(fixed_point<Rep, Exponent, Radix> const& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, Radix, std::exp>(x);
    }
    template<typename Rep, int Exponent, int Radix>
    constexpr fixed_point <Rep, Exponent, Radix>
    pow(fixed_point<Rep, Exponent, Radix> const& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, Radix, std::pow>(x);
    }
    template<typename Rep, int Exponent, int Radix>
    ::std::ostream& operator<<(::std::ostream& out, fixed_point<Rep, Exponent, Radix> const& fp)
    {
        return out << static_cast<long double>(fp);
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
            return from_rep<_value_type>{}(_rep{1});
        }
        static constexpr _value_type max() noexcept
        {
            return from_rep<_value_type>{}(_rep_numeric_limits::max());
        }
        static constexpr _value_type lowest() noexcept
        {
            return from_rep<_value_type>{}(_rep_numeric_limits::lowest());
        }
        static constexpr bool is_integer = false;
        static constexpr _value_type epsilon() noexcept
        {
            return from_rep<_value_type>{}(_rep{1});
        }
        static constexpr _value_type round_error() noexcept
        {
            return from_rep<_value_type>{}(_rep{0});
        }
        static constexpr _value_type infinity() noexcept
        {
            return from_rep<_value_type>{}(_rep{0});
        }
        static constexpr _value_type quiet_NaN() noexcept
        {
            return from_rep<_value_type>{}(_rep{0});
        }
        static constexpr _value_type signaling_NaN() noexcept
        {
            return from_rep<_value_type>{}(_rep{0});
        }
        static constexpr _value_type denorm_min() noexcept
        {
            return from_rep<_value_type>{}(_rep{1});
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
    template<class Narrowest = int, class Integral = int>
    constexpr elastic_number<numeric_limits<Integral>::digits, 0, Narrowest>
    make_elastic_number(Integral value)
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
        template<class T, class Enable = void>
        struct unsigned_or_float;
        template<class T>
        struct unsigned_or_float<T, enable_if_t<numeric_limits<T>::is_iec559>> {
            using type = T;
        };
        template<class T>
        struct unsigned_or_float<T, enable_if_t<!numeric_limits<T>::is_iec559>> : make_unsigned<T> {
        };
        template<class T>
        using unsigned_or_float_t = typename unsigned_or_float<T>::type;
        template<class Encompasser, bool EncompasserSigned,
                class Encompassed, bool EncompassedSigned>
        struct encompasses_signedness
                : std::integral_constant<bool, ::cnl::digits<Encompasser>::value >= ::cnl::digits<Encompassed>::value> {
        };
        template<class Encompasser, class Encompassed>
        struct encompasses_signedness<Encompasser, false, Encompassed, true> : std::false_type {
        };
        template<class Encompasser, class Encompassed>
        struct encompasses
                : encompasses_signedness<
                        Encompasser, ::cnl::is_signed<Encompasser>::value,
                        Encompassed, ::cnl::is_signed<Encompassed>::value> {
        };
    }
}
namespace cnl {
    static constexpr struct native_overflow_tag {
    } native_overflow{};
    static constexpr struct throwing_overflow_tag {
    } throwing_overflow{};
    static constexpr struct saturated_overflow_tag {
    } saturated_overflow{};
    namespace _overflow_impl {
        template<class Result>
        constexpr Result return_if(bool condition, Result const& value, char const* )
        {
            return condition ? value : throw std::overflow_error("");
        }
        template<class T>
        struct positive_digits : public std::integral_constant<int, numeric_limits<T>::digits> {
        };
        template<class T>
        struct negative_digits
                : public std::integral_constant<int, std::is_signed<T>::value ? numeric_limits<T>::digits : 0> {
        };
        template<
                class Destination, class Source,
                _impl::enable_if_t<!(positive_digits<Destination>::value<positive_digits<Source>::value), int> dummy = 0>
        constexpr bool is_positive_overflow(Source const&)
        {
            return false;
        }
        template<
                class Destination, class Source,
                _impl::enable_if_t<(positive_digits<Destination>::value<positive_digits<Source>::value), int> dummy = 0>
        constexpr bool is_positive_overflow(Source const& source)
        {
            return source>static_cast<Source>(numeric_limits<Destination>::max());
        }
        template<
                class Destination, class Source,
                _impl::enable_if_t<!(negative_digits<Destination>::value<negative_digits<Source>::value), int> dummy = 0>
        constexpr bool is_negative_overflow(Source const&)
        {
            return false;
        }
        template<
                class Destination, class Source,
                _impl::enable_if_t<(negative_digits<Destination>::value<negative_digits<Source>::value), int> dummy = 0>
        constexpr bool is_negative_overflow(Source const& source)
        {
            return source<static_cast<Source>(numeric_limits<Destination>::lowest());
        }
        template<class OverflowTag, class Operator, class Enable = void>
        struct binary_operator;
        template<class OverflowTag, class Operator, class Enable = void>
        struct comparison_operator;
    }
    template<class Result, class Input>
    constexpr Result convert(native_overflow_tag, Input const& rhs)
    {
        return static_cast<Result>(rhs);
    }
    template<class Result, class Input>
    constexpr Result convert(throwing_overflow_tag, Input const& rhs)
    {
        return _impl::encompasses<Result, Input>::value
               ? static_cast<Result>(rhs)
               : _overflow_impl::return_if(
                        !_overflow_impl::is_positive_overflow<Result>(rhs),
                        _overflow_impl::return_if(
                                !_overflow_impl::is_negative_overflow<Result>(rhs),
                                static_cast<Result>(rhs),
                                "negative overflow in conversion"),
                        "positive overflow in conversion");
    }
    template<class Result, class Input>
    constexpr Result convert(saturated_overflow_tag, Input const& rhs)
    {
        using numeric_limits = numeric_limits<Result>;
        return !_impl::encompasses<Result, Input>::value
               ? _overflow_impl::is_positive_overflow<Result>(rhs)
                 ? numeric_limits::max()
                 : _overflow_impl::is_negative_overflow<Result>(rhs)
                   ? numeric_limits::lowest()
                   : static_cast<Result>(rhs)
               : static_cast<Result>(rhs);
    }
    namespace _overflow_impl {
        template<>
        struct binary_operator<native_overflow_tag, _impl::add_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs+rhs)
            {
                return lhs+rhs;
            }
        };
        template<>
        struct binary_operator<throwing_overflow_tag, _impl::add_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs+rhs)
            {
                using result_type = decltype(lhs+rhs);
                using numeric_limits = numeric_limits<result_type>;
                return _overflow_impl::return_if(
                        !((rhs>=from_rep<Rhs>{}(0))
                          ? (lhs>numeric_limits::max()-rhs)
                          : (lhs<numeric_limits::lowest()-rhs)),
                        lhs+rhs,
                        "overflow in addition");
            }
        };
        template<>
        struct binary_operator<saturated_overflow_tag, _impl::add_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> _impl::op_result<_impl::add_op, Lhs, Rhs>
            {
                using result_type = decltype(lhs+rhs);
                using numeric_limits = numeric_limits<result_type>;
                return (rhs>0)
                       ? (lhs>numeric_limits::max()-rhs) ? numeric_limits::max() : lhs+rhs
                       : (lhs<numeric_limits::lowest()-rhs) ? numeric_limits::lowest() : lhs+rhs;
            }
        };
    }
    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto add(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs+rhs)
    {
        return _impl::for_rep<decltype(lhs+rhs)>(_overflow_impl::binary_operator<OverflowTag, _impl::add_op>(), lhs, rhs);
    }
    namespace _overflow_impl {
        template<>
        struct binary_operator<native_overflow_tag, _impl::subtract_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs-rhs)
            {
                return lhs-rhs;
            }
        };
        template<>
        struct binary_operator<throwing_overflow_tag, _impl::subtract_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs-rhs)
            {
                using result_type = decltype(lhs-rhs);
                using numeric_limits = numeric_limits<result_type>;
                return _overflow_impl::return_if(
                        (rhs<from_rep<Rhs>{}(0))
                        ? (lhs<=numeric_limits::max()+rhs)
                        : (lhs>=numeric_limits::lowest()+rhs),
                        lhs-rhs,
                        "positive overflow in subtraction");
            }
        };
        template<>
        struct binary_operator<saturated_overflow_tag, _impl::subtract_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> _impl::op_result<_impl::subtract_op, Lhs, Rhs>
            {
                using result_type = decltype(lhs-rhs);
                using numeric_limits = numeric_limits<result_type>;
                return (rhs<0)
                       ? (lhs>numeric_limits::max()+rhs) ? numeric_limits::max() : lhs-rhs
                       : (lhs<numeric_limits::lowest()+rhs) ? numeric_limits::lowest() : lhs-rhs;
            }
        };
    }
    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto subtract(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs-rhs)
    {
        return _impl::for_rep<decltype(lhs-rhs)>(_overflow_impl::binary_operator<OverflowTag, _impl::subtract_op>(), lhs, rhs);
    }
    namespace _overflow_impl {
        template<>
        struct binary_operator<native_overflow_tag, _impl::multiply_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs*rhs)
            {
                return lhs*rhs;
            }
        };
        template<class Operand>
        constexpr bool is_multiply_overflow(Operand const& lhs, Operand const& rhs)
        {
            using result_nl = numeric_limits<decltype(lhs*rhs)>;
            return lhs && rhs && ((lhs>Operand{0})
                                  ? ((rhs>Operand{0}) ? (result_nl::max()/rhs) : (result_nl::lowest()/rhs))<lhs
                                  : ((rhs>Operand{0}) ? (result_nl::lowest()/rhs) : (result_nl::max()/rhs))>lhs);
        }
        template<>
        struct binary_operator<throwing_overflow_tag, _impl::multiply_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs*rhs)
            {
                return _overflow_impl::return_if(
                        !is_multiply_overflow(lhs, rhs),
                        lhs*rhs, "overflow in multiplication");
            }
        };
        template<>
        struct binary_operator<saturated_overflow_tag, _impl::multiply_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> _impl::op_result<_impl::multiply_op, Lhs, Rhs>
            {
                using result_type = decltype(lhs*rhs);
                return is_multiply_overflow(lhs, rhs)
                       ? ((lhs>0) ^ (rhs>0))
                         ? numeric_limits<result_type>::lowest()
                         : numeric_limits<result_type>::max()
                       : lhs*rhs;
            }
        };
    }
    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto multiply(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs*rhs)
    {
        return _impl::for_rep<decltype(lhs*rhs)>(_overflow_impl::binary_operator<OverflowTag, _impl::multiply_op>(), lhs, rhs);
    }
    namespace _overflow_impl {
        template<class OverflowTag>
        struct binary_operator<OverflowTag, _impl::divide_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs/rhs)
            {
                return lhs/rhs;
            }
        };
    }
    namespace _overflow_impl {
        template<class OverflowTag>
        struct binary_operator<OverflowTag, _impl::shift_right_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs>>rhs)
            {
                return lhs>>rhs;
            }
        };
    }
    namespace _overflow_impl {
        template<class Lhs, class Rhs>
        constexpr bool is_shift_left_overflow(Lhs const& lhs, Rhs const& rhs)
        {
            using result_type = decltype(lhs<<rhs);
            return rhs>0 && unsigned(cnl::leading_bits(static_cast<result_type>(lhs)))<unsigned(rhs);
        }
        template<>
        struct binary_operator<native_overflow_tag, _impl::shift_left_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs<<rhs)
            {
                return lhs<<rhs;
            }
        };
        template<>
        struct binary_operator<throwing_overflow_tag, _impl::shift_left_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs<<rhs)
            {
                return _overflow_impl::return_if(
                        !is_shift_left_overflow(lhs, rhs),
                        lhs<<rhs, "overflow in shift left");
            }
        };
        template<>
        struct binary_operator<saturated_overflow_tag, _impl::shift_left_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> _impl::op_result<_impl::shift_left_op, Lhs, Rhs>
            {
                using result_type = decltype(lhs<<rhs);
                return is_shift_left_overflow(lhs, rhs)
                       ? ((lhs>0) ^ (rhs>0))
                         ? numeric_limits<result_type>::lowest()
                         : numeric_limits<result_type>::max()
                       : lhs<<rhs;
            }
        };
    }
    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto shift_left(OverflowTag, Lhs const& lhs, Rhs const& rhs)
    -> decltype(lhs<<rhs)
    {
        return _impl::for_rep<decltype(lhs<<rhs)>(_overflow_impl::binary_operator<OverflowTag, _impl::shift_left_op>(), lhs, rhs);
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
        template<class, class, class = void>
        struct common_type;
        template<class LhsRep, class RhsRep, class OverflowTag>
        struct common_type<
                overflow_integer<LhsRep, OverflowTag>,
                overflow_integer<RhsRep, OverflowTag>> {
            using type = overflow_integer<
                    typename std::common_type<LhsRep, RhsRep>::type,
                    OverflowTag>;
        };
        template<class LhsRep, class LhsOverflowTag, class RhsInteger>
        struct common_type<
                overflow_integer<LhsRep, LhsOverflowTag>, RhsInteger,
                _impl::enable_if_t<
                        !_integer_impl::is_overflow_integer<RhsInteger>::value && std::is_integral<RhsInteger>::value>> {
            using type = typename cnl::overflow_integer<typename std::common_type<LhsRep, RhsInteger>::type, LhsOverflowTag>;
        };
        template<class LhsRep, class LhsOverflowTag, class Float>
        struct common_type<
                overflow_integer<LhsRep, LhsOverflowTag>, Float,
                _impl::enable_if_t<std::is_floating_point<Float>::value>> {
            using type = typename std::common_type<LhsRep, Float>::type;
        };
        template<class Lhs, class RhsRep, class RhsOverflowTag>
        struct common_type<Lhs, overflow_integer<RhsRep, RhsOverflowTag>>
                : common_type<overflow_integer<RhsRep, RhsOverflowTag>, Lhs> {
        };
    }
    template<class Rep = int, class OverflowTag = throwing_overflow_tag>
    class overflow_integer : public _impl::number_base<overflow_integer<Rep, OverflowTag>, Rep> {
        static_assert(!_integer_impl::is_overflow_integer<Rep>::value,
                "overflow_integer of overflow_integer is not a supported");
    public:
        using rep = Rep;
        using overflow_tag = OverflowTag;
        using _base = _impl::number_base<overflow_integer<Rep, OverflowTag>, Rep>;
        constexpr overflow_integer() = delete;
        template<class RhsRep, class RhsOverflowTag>
        constexpr overflow_integer(overflow_integer<RhsRep, RhsOverflowTag> const& rhs)
                :overflow_integer(to_rep(rhs))
        {
        }
        template<class Rhs, _impl::enable_if_t<!_integer_impl::is_overflow_integer<Rhs>::value, int> dummy = 0>
        constexpr overflow_integer(Rhs const& rhs)
                :_base(convert<rep>(overflow_tag{}, rhs))
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
            return static_cast<T>(to_rep(*this));
        }
    };
    namespace _impl {
        template<class Rep, class OverflowTag>
        struct get_rep<overflow_integer<Rep, OverflowTag>> {
            using type = Rep;
        };
        template<class OldRep, class OverflowTag, class NewRep>
        struct set_rep<overflow_integer<OldRep, OverflowTag>, NewRep> {
            using type = overflow_integer<NewRep, OverflowTag>;
        };
    }
    template<class Rep, class OverflowTag>
    struct digits<overflow_integer<Rep, OverflowTag>> : digits<Rep> {
    };
    template<class Rep, class OverflowTag, _digits_type MinNumBits>
    struct set_digits<overflow_integer<Rep, OverflowTag>, MinNumBits> {
        using type = overflow_integer<set_digits_t<Rep, MinNumBits>, OverflowTag>;
    };
    template<class Rep, class OverflowTag>
    constexpr Rep to_rep(overflow_integer<Rep, OverflowTag> const& number)
    {
        using base_type = typename overflow_integer<Rep, OverflowTag>::_base;
        return to_rep(static_cast<base_type const&>(number));
    }
    template<class ArchetypeRep, class OverflowTag>
    struct from_rep<overflow_integer<ArchetypeRep, OverflowTag>> {
        template<typename Rep>
        constexpr auto operator()(Rep const& r) const
        -> overflow_integer<Rep, OverflowTag>
        {
            return r;
        }
    };
    template<class Rep, class OverflowTag, class Value>
    struct from_value<overflow_integer<Rep, OverflowTag>, Value> {
        using type = overflow_integer<Value, OverflowTag>;
    };
    template<class Rep, class OverflowTag, class ValueRep, class ValueOverflowTag>
    struct from_value<overflow_integer<Rep, OverflowTag>, overflow_integer<ValueRep, ValueOverflowTag>> {
    private:
        using _overflow_tag = _impl::common_type_t<OverflowTag, ValueOverflowTag>;
        using _rep = from_value_t<Rep, ValueRep>;
    public:
        using type = overflow_integer<_rep, _overflow_tag>;
    };
    template<class Rep, class OverflowTag, ::cnl::intmax Value>
    struct from_value<overflow_integer<Rep, OverflowTag>, constant<Value>> {
        using _rep = typename std::conditional<digits<int>::value<used_digits(Value), decltype(Value), int>::type;
        using type = overflow_integer<_rep, OverflowTag>;
    };
    template<int Digits, int Radix, class Rep, class OverflowTag>
    struct shift<Digits, Radix, overflow_integer<Rep, OverflowTag>>
            : shift<Digits, Radix, _impl::number_base<overflow_integer<Rep, OverflowTag>, Rep>> {
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
            -> decltype(overflow_integer<decltype(Operator()(to_rep(operand))), OverflowTag>(Operator()(to_rep(operand))))
            {
                return overflow_integer<decltype(Operator()(to_rep(operand))), OverflowTag>(Operator()(to_rep(operand)));
            }
        };
        template<class Operator, class Rep, class OverflowTag>
        struct binary_operator<Operator,
                overflow_integer<Rep, OverflowTag>, overflow_integer<Rep, OverflowTag>,
                typename Operator::is_not_comparison> {
            constexpr auto operator()(
                    overflow_integer<Rep, OverflowTag> const& lhs,
                    overflow_integer<Rep, OverflowTag> const& rhs) const
            -> decltype(make_overflow_int<OverflowTag>(_overflow_impl::binary_operator<OverflowTag, Operator>()(to_rep(lhs), to_rep(rhs))))
            {
                return make_overflow_int<OverflowTag>(
                        _overflow_impl::binary_operator<OverflowTag, Operator>()(to_rep(lhs), to_rep(rhs)));
            }
        };
        template<class Operator, class LhsRep, class RhsRep, class OverflowTag>
        struct binary_operator<Operator,
                overflow_integer<LhsRep, OverflowTag>, overflow_integer<RhsRep, OverflowTag>,
                typename Operator::is_not_comparison> {
            constexpr auto operator()(
                    overflow_integer<LhsRep, OverflowTag> const& lhs,
                    overflow_integer<RhsRep, OverflowTag> const& rhs) const
            -> overflow_integer<decltype(Operator{}(std::declval<LhsRep>(), std::declval<RhsRep>())), OverflowTag>
            {
                using result_rep_type = decltype(Operator{}(std::declval<LhsRep>(), std::declval<RhsRep>()));
                using result_type = overflow_integer<result_rep_type, OverflowTag>;
                return _impl::from_value<result_type>(binary_operator<Operator, result_type, result_type>{}(lhs, rhs));
            }
        };
        template<class Operator, class Rep, class OverflowTag>
        struct binary_operator<Operator,
                overflow_integer<Rep, OverflowTag>, overflow_integer<Rep, OverflowTag>,
                typename Operator::is_comparison> {
            constexpr auto operator()(
                    overflow_integer<Rep, OverflowTag> const& lhs,
                    overflow_integer<Rep, OverflowTag> const& rhs) const
            -> decltype(Operator()(to_rep(lhs), to_rep(rhs)))
            {
                return Operator()(to_rep(lhs), to_rep(rhs));
            }
        };
        template<class Operator, class LhsRep, class LhsTag, class RhsRep, class RhsTag>
        struct binary_operator<Operator,
                overflow_integer<LhsRep, LhsTag>, overflow_integer<RhsRep, RhsTag>,
                typename Operator::is_comparison> {
            constexpr auto operator()(
                    const overflow_integer<LhsRep, LhsTag>& lhs,
                    const overflow_integer<RhsRep, RhsTag>& rhs) const
            -> decltype(binary_operator<
                    Operator,
                    overflow_integer<cnl::_impl::common_type_t<LhsRep, RhsRep>, cnl::_impl::common_type_t<LhsTag, RhsTag>>,
                    overflow_integer<cnl::_impl::common_type_t<LhsRep, RhsRep>, cnl::_impl::common_type_t<LhsTag, RhsTag>>>()(lhs, rhs))
            {
                using common_rep = cnl::_impl::common_type_t<LhsRep, RhsRep>;
                using common_tag = cnl::_impl::common_type_t<LhsTag, RhsTag>;
                return binary_operator<
                        Operator,
                        overflow_integer<common_rep, common_tag>,
                        overflow_integer<common_rep, common_tag>>()(lhs, rhs);
            }
        };
    }
}
namespace cnl {
    template<class Rep, class OverflowTag>
    struct numeric_limits<cnl::overflow_integer<Rep, OverflowTag>>
            : numeric_limits<cnl::_impl::number_base<cnl::overflow_integer<Rep, OverflowTag>, Rep>> {
        static constexpr bool is_integer = true;
    };
    template<class Rep, class OverflowTag>
    struct numeric_limits<cnl::overflow_integer<Rep, OverflowTag> const>
            : numeric_limits<cnl::_impl::number_base<cnl::overflow_integer<Rep, OverflowTag>, Rep>> {
        static constexpr bool is_integer = true;
    };
}
namespace std {
    template<
            class Lhs,
            class RhsRep, class RhsOverflowTag>
    struct common_type<
            Lhs,
            cnl::overflow_integer<RhsRep, RhsOverflowTag>>
            : cnl::_integer_impl::common_type<
                    Lhs,
                    cnl::overflow_integer<RhsRep, RhsOverflowTag>> {
    };
    template<
            class LhsRep, class LhsOverflowTag,
            class Rhs>
    struct common_type<
            cnl::overflow_integer<LhsRep, LhsOverflowTag>,
            Rhs>
            : cnl::_integer_impl::common_type<
                    cnl::overflow_integer<LhsRep, LhsOverflowTag>,
                    Rhs> {
    };
    template<
            class LhsRep, class LhsOverflowTag,
            class RhsRep, int RhsExponent>
    struct common_type<
            cnl::overflow_integer<LhsRep, LhsOverflowTag>,
            cnl::fixed_point<RhsRep, RhsExponent>>
            : std::common_type<
                    cnl::fixed_point<cnl::overflow_integer<LhsRep, LhsOverflowTag>, 0>,
                    cnl::fixed_point<RhsRep, RhsExponent>> {
    };
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, class RhsOverflowTag>
    struct common_type<
            cnl::fixed_point<LhsRep, LhsExponent>,
            cnl::overflow_integer<RhsRep, RhsOverflowTag>>
            : std::common_type<
                    cnl::fixed_point<LhsRep, LhsExponent>,
                    cnl::fixed_point<cnl::overflow_integer<RhsRep, RhsOverflowTag>, 0>> {
    };
    template<
            class LhsRep, class LhsOverflowTag,
            class RhsRep, class RhsOverflowTag>
    struct common_type<
            cnl::overflow_integer<LhsRep, LhsOverflowTag>,
            cnl::overflow_integer<RhsRep, RhsOverflowTag>>
            : cnl::_integer_impl::common_type<
                    cnl::overflow_integer<LhsRep, LhsOverflowTag>,
                    cnl::overflow_integer<RhsRep, RhsOverflowTag>> {
    };
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
    struct closest_rounding_tag {
        template<class To, class From>
        static constexpr To convert(From const& from)
        {
            return static_cast<To>(intmax(from+((from>=0) ? .5 : -.5)));
        }
    };
    template<class Rep = int, class RoundingTag = closest_rounding_tag>
    class rounding_integer;
    namespace _rounding_integer_impl {
        template<class T>
        struct is_rounding_integer : std::false_type {
        };
        template<class Rep, class RoundingTag>
        struct is_rounding_integer<rounding_integer<Rep, RoundingTag>> : std::true_type {
        };
    }
    template<class Rep, class RoundingTag>
    class rounding_integer : public _impl::number_base<rounding_integer<Rep, RoundingTag>, Rep> {
        static_assert(!_rounding_integer_impl::is_rounding_integer<Rep>::value,
                "rounding_integer of rounding_integer is not a supported");
        using super = _impl::number_base<rounding_integer<Rep, RoundingTag>, Rep>;
    public:
        using rounding = RoundingTag;
        using _base = _impl::number_base<rounding_integer<Rep, RoundingTag>, Rep>;
        constexpr rounding_integer() = default;
        template<class T, _impl::enable_if_t<numeric_limits<T>::is_integer, int> Dummy = 0>
        constexpr rounding_integer(T const& v)
                : super(static_cast<Rep>(v)) { }
        template<class T, _impl::enable_if_t<!numeric_limits<T>::is_integer, int> Dummy = 0>
        constexpr rounding_integer(T const& v)
                : super(rounding::template convert<Rep>(v)) { }
        template<class T>
        constexpr explicit operator T() const
        {
            return static_cast<T>(to_rep(*this));
        }
    };
    template<class Rep, class RoundingTag>
    struct digits<rounding_integer<Rep, RoundingTag>> : digits<Rep> {
    };
    template<class Rep, class RoundingTag, _digits_type MinNumBits>
    struct set_digits<rounding_integer<Rep, RoundingTag>, MinNumBits> {
        using type = rounding_integer<set_digits_t<Rep, MinNumBits>, RoundingTag>;
    };
    template<class ArchetypeRep, class RoundingTag>
    struct from_rep<rounding_integer<ArchetypeRep, RoundingTag>> {
        template<typename Rep>
        constexpr auto operator()(Rep const& r) const
        -> rounding_integer<Rep, RoundingTag>
        {
            return r;
        }
    };
    template<class Rep, class RoundingTag>
    constexpr Rep to_rep(rounding_integer<Rep, RoundingTag> const& number)
    {
        using base_type = typename rounding_integer<Rep, RoundingTag>::_base;
        return to_rep(static_cast<base_type const&>(number));
    }
    namespace _impl {
        template<class Rep, class RoundingTag>
        struct get_rep<rounding_integer<Rep, RoundingTag>> {
            using type = Rep;
        };
        template<class OldRep, class RoundingTag, class NewRep>
        struct set_rep<rounding_integer<OldRep, RoundingTag>, NewRep> {
            using type = rounding_integer<NewRep, RoundingTag>;
        };
    }
    template<class Rep, class RoundingTag, class Value>
    struct from_value<rounding_integer<Rep, RoundingTag>, Value> {
        using type = rounding_integer<Value, RoundingTag>;
    };
    template<class Rep, class RoundingTag, class ValueRep, class ValueRoundingTag>
    struct from_value<rounding_integer<Rep, RoundingTag>, rounding_integer<ValueRep, ValueRoundingTag>> {
    private:
        using _overflow_tag = _impl::common_type_t<RoundingTag, ValueRoundingTag>;
        using _rep = from_value_t<Rep, ValueRep>;
    public:
        using type = rounding_integer<_rep, _overflow_tag>;
    };
    template<class Rep, class RoundingTag, ::cnl::intmax Value>
    struct from_value<rounding_integer<Rep, RoundingTag>, constant<Value>> {
        using _rep = typename std::conditional<digits<int>::value<used_digits(Value),
                decltype(Value),
                int>::type;
        using type = rounding_integer<_rep, RoundingTag>;
    };
    template<int Digits, int Radix, class Rep, class RoundingTag>
    struct shift<Digits, Radix, rounding_integer<Rep, RoundingTag>>
            : shift<Digits, Radix, _impl::number_base<rounding_integer<Rep, RoundingTag>, Rep>> {
    };
    namespace _impl {
        template<class Operator, class Rep, class RoundingTag>
        struct unary_operator<Operator, rounding_integer<Rep, RoundingTag>> {
            constexpr auto operator()(rounding_integer<Rep, RoundingTag> const& operand) const
            -> decltype(from_rep<rounding_integer<decltype(Operator()(to_rep(operand))), RoundingTag>>{}(
                    Operator()(to_rep(operand))))
            {
                using result_type = rounding_integer<decltype(Operator()(to_rep(operand))), RoundingTag>;
                return from_rep<result_type>{}(Operator()(to_rep(operand)));
            }
        };
    }
    namespace _impl {
        template<class Operator, class LhsRep, class RhsRep, class RoundingTag>
        struct binary_operator<Operator, rounding_integer<LhsRep, RoundingTag>, rounding_integer<RhsRep, RoundingTag>,
                typename Operator::is_not_comparison> {
            constexpr auto operator()(
                    rounding_integer<LhsRep, RoundingTag> const& lhs,
                    rounding_integer<RhsRep, RoundingTag> const& rhs) const
            -> decltype(from_rep<rounding_integer<op_result<Operator, LhsRep, RhsRep>, RoundingTag>>{}(
                    Operator()(to_rep(lhs), to_rep(rhs))))
            {
                using result_type = rounding_integer<op_result<Operator, LhsRep, RhsRep>, RoundingTag>;
                return from_rep<result_type>{}(Operator()(to_rep(lhs), to_rep(rhs)));
            }
        };
        template<class Operator, class LhsRep, class RhsRep, class RoundingTag>
        struct binary_operator<Operator, rounding_integer<LhsRep, RoundingTag>, rounding_integer<RhsRep, RoundingTag>,
                typename Operator::is_comparison> {
        constexpr auto operator()(
                rounding_integer<LhsRep, RoundingTag> const& lhs,
                rounding_integer<RhsRep, RoundingTag> const& rhs) const
            -> decltype(Operator()(to_rep(lhs), to_rep(rhs)))
            {
                return Operator()(to_rep(lhs), to_rep(rhs));
            }
        };
        template<class Operator, class LhsRep, class LhsRoundingTag, class RhsRep, class RhsRoundingTag>
        struct binary_operator<Operator,
                rounding_integer<LhsRep, LhsRoundingTag>, rounding_integer<RhsRep, RhsRoundingTag>,
                typename Operator::is_comparison> {
            constexpr auto operator()(
                    rounding_integer<LhsRep, LhsRoundingTag> const& lhs,
                    rounding_integer<RhsRep, RhsRoundingTag> const& rhs) const
            -> decltype(binary_operator<Operator, rounding_integer<LhsRep, common_type_t<LhsRoundingTag, RhsRoundingTag>>, rounding_integer<LhsRep, common_type_t<LhsRoundingTag, RhsRoundingTag>>>()(lhs, rhs))
            {
                using common_tag = common_type_t<LhsRoundingTag, RhsRoundingTag>;
                return binary_operator<Operator, rounding_integer<LhsRep, common_tag>, rounding_integer<LhsRep, common_tag>>()(lhs, rhs);
            }
        };
    }
    template<class LhsRep, class LhsRoundingTag, class RhsInteger>
    constexpr auto operator<<(
            rounding_integer<LhsRep, LhsRoundingTag> const& lhs,
            RhsInteger const& rhs)
    -> decltype(from_rep<rounding_integer<decltype(to_rep(lhs) << rhs), LhsRoundingTag>>{}(to_rep(lhs) << rhs))
    {
        return from_rep<rounding_integer<
                decltype(to_rep(lhs) << rhs),
                LhsRoundingTag>>{}(to_rep(lhs) << rhs);
    }
    template<class Rep, class RoundingTag>
    struct numeric_limits<cnl::rounding_integer<Rep, RoundingTag>>
            : numeric_limits<cnl::_impl::number_base<cnl::rounding_integer<Rep, RoundingTag>, Rep>> {
        static constexpr bool is_integer = true;
    };
    template<class Rep, class RoundingTag>
    struct numeric_limits<cnl::rounding_integer<Rep, RoundingTag> const>
            : numeric_limits<cnl::_impl::number_base<cnl::rounding_integer<Rep, RoundingTag>, Rep>> {
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

#endif // CNL_COMPLETE_H
