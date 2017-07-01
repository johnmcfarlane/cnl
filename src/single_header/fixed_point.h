
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// mechanically retrieved, single-header version of fixed_point library
// https://github.com/johnmcfarlane/fixed_point

#if ! defined(SG14_FIXED_POINT_SINGLE_HEADER)
#define SG14_FIXED_POINT_SINGLE_HEADER

#include <cmath>
#include <utility>
#include <istream>
#include <climits>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <type_traits>

namespace sg14 {
    namespace _impl {
        template<class ... T>
        using common_type_t = typename std::common_type<T ...>::type;
        template<bool C, class ... T>
        using enable_if_t = typename std::enable_if<C, T ...>::type;
        template<class A, class B>
        constexpr bool identical(const A& a, const B& b)
        {
            static_assert(std::is_same<A, B>::value, "different types");
            return a==b;
        }
    }
}
namespace sg14 {
    using _digits_type = int;
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
        template<_digits_type MinNumDigits, class Smaller, class T>
        struct enable_for_range
                : std::enable_if<MinNumDigits <= std::numeric_limits<T>::digits &&
                                 std::numeric_limits<Smaller>::digits < MinNumDigits> {
        };
        template<_digits_type MinNumDigits, class Smallest>
        struct enable_for_range<MinNumDigits, void, Smallest>
                : std::enable_if<MinNumDigits <= std::numeric_limits<Smallest>::digits> {
        };
        template<_digits_type MinNumDigits, class Smaller, class T>
        using enable_for_range_t = typename enable_for_range<MinNumDigits, Smaller, T>::type;
        template<_digits_type MinNumDigits, class Enable = void>
        struct set_digits_signed;
        template<_digits_type MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, void, std::int8_t>> {
            using type = std::int8_t;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, std::int8_t, std::int16_t>> {
            using type = std::int16_t;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, std::int16_t, std::int32_t>> {
            using type = std::int32_t;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, std::int32_t, std::int64_t>> {
            using type = std::int64_t;
        };
        template<_digits_type MinNumDigits, class Enable = void>
        struct set_digits_unsigned;
        template<_digits_type MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, void, std::uint8_t>> {
            using type = std::uint8_t;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, std::uint8_t, std::uint16_t>> {
            using type = std::uint16_t;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, std::uint16_t, std::uint32_t>> {
            using type = std::uint32_t;
        };
        template<_digits_type MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, std::uint32_t, std::uint64_t>> {
            using type = std::uint64_t;
        };
        template<class Integer, _digits_type MinNumDigits>
        using set_digits_integer = typename std::conditional<
                std::numeric_limits<Integer>::is_signed,
                set_digits_signed<MinNumDigits>,
                set_digits_unsigned<MinNumDigits>>::type;
    }
    template<class T, class Enable = void>
    struct digits : std::integral_constant<_digits_type, std::numeric_limits<T>::digits> {
    };
    template<class T, _digits_type Digits, class Enable = void>
    struct set_digits;
    template<class T, _digits_type Digits>
    struct set_digits<T, Digits, _impl::enable_if_t<std::is_integral<T>::value>>
            : _num_traits_impl::set_digits_integer<T, Digits> {
    };
    template<class T, _digits_type Digits>
    using set_digits_t = typename set_digits<T, Digits>::type;
    template<class T>
    struct is_signed : std::integral_constant<bool, std::numeric_limits<T>::is_signed> {
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
    template<class T>
    using make_unsigned_t = typename make_unsigned<T>::type;
    namespace _impl {
        template<class T, bool IsSigned = true>
        struct make_signed;
        template<class T>
        struct make_signed<T, true> : ::sg14::make_signed<T> {
        };
        template<class T>
        struct make_signed<T, false> : ::sg14::make_unsigned<T> {
        };
        template<class T, bool IsSigned>
        using make_signed_t = typename make_signed<T, IsSigned>::type;
        template<class T1, class T2>
        struct common_signedness {
            static constexpr bool _are_signed = std::numeric_limits<T1>::is_signed | std::numeric_limits<T2>::is_signed;
            using type = typename std::common_type<make_signed_t<T1, _are_signed>,
                    make_signed_t<T2, _are_signed>>::type;
        };
        template<class T1, class T2>
        using common_signedness_t = typename common_signedness<T1, T2>::type;
        template<class T, class Enable = void>
        struct unsigned_or_float;
        template<class T>
        struct unsigned_or_float<T, enable_if_t<std::numeric_limits<T>::is_iec559>> {
            using type = T;
        };
        template<class T>
        struct unsigned_or_float<T, enable_if_t<!std::numeric_limits<T>::is_iec559>> : make_unsigned<T> {
        };
        template<class T>
        using unsigned_or_float_t = typename unsigned_or_float<T>::type;
        template<class Encompasser, class Encompassed, class Enable = void>
        struct encompasses_lower;
        template<class Encompasser, class Encompassed>
        struct encompasses_lower<Encompasser, Encompassed,
                enable_if_t<std::numeric_limits<Encompasser>::is_signed
                            && std::numeric_limits<Encompassed>::is_signed>> {
            static constexpr bool value = std::numeric_limits<Encompasser>::lowest()
                                          <= std::numeric_limits<Encompassed>::lowest();
        };
        template<class Encompasser, class Encompassed>
        struct encompasses_lower<Encompasser, Encompassed,
                enable_if_t<!std::numeric_limits<Encompassed>::is_signed>> : std::true_type {
        };
        template<class Encompasser, class Encompassed>
        struct encompasses_lower<Encompasser, Encompassed,
                enable_if_t<!std::numeric_limits<Encompasser>::is_signed
                            && std::numeric_limits<Encompassed>::is_signed>> : std::false_type {
        };
        template<class Encompasser, class Encompassed>
        struct encompasses {
            static constexpr bool _lower = encompasses_lower<Encompasser, Encompassed>::value;
            static constexpr bool _upper =
                    static_cast<unsigned_or_float_t<Encompasser>>(std::numeric_limits<Encompasser>::max())
                    >= static_cast<unsigned_or_float_t<Encompassed>>(std::numeric_limits<Encompassed>::max());
            static constexpr bool value = _lower && _upper;
        };
        template<class T>
        struct is_integer_or_float : std::integral_constant<
                bool,
                std::numeric_limits<T>::is_integer || std::numeric_limits<T>::is_iec559> {
        };
    }
    template<class Number, class Enable = void>
    struct to_rep {
        constexpr Number operator()(const Number &number) const {
            return number;
        }
    };
    namespace _impl {
        template<class Number, class Enable = void>
        constexpr auto to_rep(const Number &number)
        -> decltype(sg14::to_rep<Number>()(number)) {
            return sg14::to_rep<Number>()(number);
        }
    }
    template<class Number, class Enable = void>
    struct from_rep {
        template<class Rep>
        constexpr Number operator()(const Rep &rep) const {
            return static_cast<Number>(rep);
        }
    };
    namespace _impl {
        template<class Number, class Rep>
        constexpr auto from_rep(const Rep &rep)
        -> decltype(sg14::from_rep<Number>()(rep)) {
            return sg14::from_rep<Number>()(rep);
        }
    }
    template<class Result, class F, class ... Args,
            _impl::enable_if_t<!_num_traits_impl::are_composite<Args ...>::value, int> dummy = 0>
    constexpr Result for_rep(F f, Args &&...args) {
        return f(std::forward<Args>(args)...);
    }
    template<class Result, class F, class ... Args,
            _impl::enable_if_t<_num_traits_impl::are_composite<Args ...>::value, int> dummy = 0>
    constexpr Result for_rep(F f, Args &&...args) {
        return for_rep<Result>(f, _impl::to_rep<typename std::decay<Args>::type>(std::forward<Args>(args))...);
    }
    template<class Number, class Value, class Enable = void>
    struct from_value;
    template<class Number, class Value>
    struct from_value<Number, Value, _impl::enable_if_t<std::is_integral<Number>::value>> {
        using type = Number;
    };
    template<class Number, class Value>
    using from_value_t = typename from_value<Number, Value>::type;
    namespace _impl {
        template<class Number, class Value>
        constexpr auto from_value(const Value &value)
        -> sg14::from_value_t<Number, Value> {
            return value;
        }
    }
    namespace _num_traits_impl {
        template<class T>
        using scale_result_type = decltype(std::declval<T>() * std::declval<T>());
        template<class T>
        constexpr scale_result_type<T> pown(int base, int exp) {
            return exp
                   ? pown<T>(base, exp - 1) * static_cast<scale_result_type<T>>(base)
                   : static_cast<scale_result_type<T>>(1);
        }
        template<class T>
        constexpr scale_result_type<T> pow2(int exp) {
            return scale_result_type<T>{1} << exp;
        }
        template<class T>
        constexpr scale_result_type<T> pow(int base, int exp) {
            return (base == 2) ? pow2<T>(exp) : pown<T>(base, exp);
        }
    }
    template<class T>
    struct scale {
        constexpr auto operator()(const T &i, int base, int exp) const
        -> _num_traits_impl::scale_result_type<T> {
            return _impl::from_rep<_num_traits_impl::scale_result_type<T>>(
                    (exp < 0)
                    ? _impl::to_rep<T>(i) / _num_traits_impl::pow<T>(base, -exp)
                    : _impl::to_rep<T>(i) * _num_traits_impl::pow<T>(base, exp));
        }
    };
    namespace _impl {
        template<class T>
        constexpr auto scale(const T &i, int base, int exp)
        -> decltype(sg14::scale<T>()(i, base, exp)) {
            return sg14::scale<T>()(i, base, exp);
        }
    }
}
namespace sg14 {
    namespace _numeric_impl {
        template<class Integer>
        constexpr int trailing_bits_positive(Integer value, int mask_bits = sizeof(Integer)*8/2)
        {
            return ((value & ((Integer{1} << mask_bits)-1))==0)
                   ? mask_bits+trailing_bits_positive(value/(Integer{1} << mask_bits), mask_bits)
                   : (mask_bits>1)
                     ? trailing_bits_positive(value, mask_bits/2)
                     : 0;
        }
        template<class Integer, class Enable = void>
        struct trailing_bits {
            static constexpr int f(Integer value)
            {
                return value ? trailing_bits_positive(value) : 0;
            }
        };
        template<class Integer>
        struct trailing_bits<Integer, _impl::enable_if_t<std::numeric_limits<Integer>::is_signed>> {
            static constexpr int f(Integer value)
            {
                return (value>0)
                       ? trailing_bits_positive(value)
                       : (value<0)
                         ? trailing_bits_positive(-value)
                         : 0;
            }
        };
    }
    template<class Integer>
    constexpr int trailing_bits(Integer value)
    {
        return _numeric_impl::trailing_bits<Integer>::f(value);
    }
    namespace _numeric_impl {
        template<class Integer>
        constexpr int used_bits_positive(Integer value, int mask_bits = sizeof(Integer)*8/2)
        {
            return (value>=(Integer{1} << mask_bits))
                   ? mask_bits+used_bits_positive(value/(Integer{1} << mask_bits), mask_bits)
                   : (mask_bits>1)
                     ? used_bits_positive(value, mask_bits/2)
                     : 1;
        }
    }
    namespace _impl {
        template<class Integer>
        constexpr int used_bits_symmetric(Integer value)
        {
            return (value>0)
                   ? _numeric_impl::used_bits_positive<Integer>(value)
                   : (value<0)
                     ? _numeric_impl::used_bits_positive<Integer>(-value)
                     : 0;
        }
    }
    namespace _numeric_impl {
        struct used_bits {
            template<class Integer>
            constexpr _impl::enable_if_t<!std::numeric_limits<Integer>::is_signed, int> operator()(Integer value) const
            {
                return value ? used_bits_positive(value) : 0;
            }
            template<class Integer, class = _impl::enable_if_t<std::numeric_limits<Integer>::is_signed, int>>
            constexpr int operator()(Integer value) const
            {
                return (value>0)
                       ? used_bits_positive(value)
                       : (value==0)
                         ? 0
                         : used_bits()(Integer(-1)-value);
            }
        };
    }
    template<class Integer>
    constexpr int used_bits(Integer value)
    {
        return for_rep<int>(_numeric_impl::used_bits(), value);
    }
    template<class Integer>
    constexpr int leading_bits(const Integer& value)
    {
        return digits<Integer>::value-used_bits(value);
    }
}
namespace sg14 {
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
        struct arithmetic_op {
            static constexpr bool is_arithmetic = true;
        };
        struct comparison_op {
            static constexpr bool is_comparison = true;
        };
        struct minus_op : arithmetic_op {
            template<class Rhs>
            constexpr auto operator()(const Rhs& rhs) const -> decltype(-rhs)
            {
                return -rhs;
            }
        };
        struct plus_op : arithmetic_op {
            template<class Rhs>
            constexpr auto operator()(const Rhs& rhs) const -> decltype(+rhs)
            {
                return +rhs;
            }
        };
        struct add_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs+rhs)
            {
                return lhs+rhs;
            }
        };
        struct subtract_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs-rhs)
            {
                return lhs-rhs;
            }
        };
        struct multiply_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs*rhs)
            {
                return lhs*rhs;
            }
        };
        struct divide_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs/rhs)
            {
                return lhs/rhs;
            }
        };
        struct bitwise_or_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs|rhs)
            {
                return lhs|rhs;
            }
        };
        struct bitwise_and_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs&rhs)
            {
                return lhs&rhs;
            }
        };
        struct bitwise_xor_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs^rhs)
            {
                return lhs^rhs;
            }
        };
        struct equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs==rhs)
            {
                return lhs==rhs;
            }
        };
        struct not_equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs!=rhs)
            {
                return lhs!=rhs;
            }
        };
        struct less_than_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs<rhs)
            {
                return lhs<rhs;
            }
        };
        struct greater_than_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs>rhs)
            {
                return lhs>rhs;
            }
        };
        struct less_than_or_equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs<=rhs)
            {
                return lhs<=rhs;
            }
        };
        struct greater_than_or_equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs>=rhs)
            {
                return lhs>=rhs;
            }
        };
        static constexpr plus_op plus_tag {};
        static constexpr minus_op minus_tag {};
        static constexpr add_op add_tag {};
        static constexpr subtract_op subtract_tag {};
        static constexpr multiply_op multiply_tag {};
        static constexpr divide_op divide_tag {};
        static constexpr bitwise_or_op bitwise_or_tag {};
        static constexpr bitwise_and_op bitwise_and_tag {};
        static constexpr bitwise_xor_op bitwise_xor_tag {};
        static constexpr equal_op equal_tag {};
        static constexpr not_equal_op not_equal_tag {};
        static constexpr less_than_op less_than_tag {};
        static constexpr greater_than_op greater_than_tag {};
        static constexpr less_than_or_equal_op less_than_or_equal_tag {};
        static constexpr greater_than_or_equal_op greater_than_or_equal_tag {};
        template<class Operator, class Lhs, class Rhs>
        using op_result = decltype(Operator()(std::declval<Lhs>(), std::declval<Rhs>()));
    }
}
namespace sg14 {
    namespace _const_integer_impl {
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
            return (C>='0' && C<='9')
                   ? C-'0'
                   : throw std::out_of_range("only decimal digits are allowed");
        }
        constexpr int parse_hex(char C) {
            return (C >= '0' && C <= '9')
                   ? C - '0'
                   : (C >= 'a' && C <= 'f')
                     ? C - 'a'
                     : (C >= 'A' && C <= 'F')
                       ? C - 'A'
                       : throw std::out_of_range("only decimal digits are allowed")
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
    namespace _const_integer_impl {
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
    template<class RhsIntegral, RhsIntegral RhsValue>
    constexpr const_integer<decltype(-RhsValue), -RhsValue>
    operator-(const_integer<RhsIntegral, RhsValue>) noexcept
    {
        return const_integer<decltype(-RhsValue), -RhsValue>{};
    }
    template<class T>
    struct is_const_integer : std::false_type {};
    template<class Integral, Integral Value>
    struct is_const_integer<const_integer<Integral, Value>> : std::true_type {};
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
    template<class Integral, Integral Value, int Digits, int Zeros, class Rhs>
    struct common_type<sg14::const_integer<Integral, Value, Digits, Zeros>, Rhs>
        : common_type<Integral, Rhs> {
    };
    template<class Lhs, class Integral, Integral Value, int Digits, int Zeros>
    struct common_type<Lhs, sg14::const_integer<Integral, Value, Digits, Zeros>>
        : common_type<Lhs, Integral> {
    };
    template<class Integral, Integral Value, int Digits, int Zeros>
    struct numeric_limits<sg14::const_integer<Integral, Value, Digits, Zeros>>
    : numeric_limits<Integral> {
        static constexpr int digits = Digits;
    };
}
namespace sg14 {
    namespace _impl {
        template<class Derived, class Rep>
        class number_base {
        public:
            using rep = Rep;
            using _derived = Derived;
            number_base() = default;
            constexpr number_base(const rep& r)
                : _rep(r) { }
            template<class T>
            number_base& operator=(const T& r) {
                _rep = r;
                return static_cast<Derived&>(*this);
            }
            explicit constexpr operator bool() const
            {
                return static_cast<bool>(_rep);
            }
            constexpr const rep& data() const
            {
                return _rep;
            }
            static constexpr Derived from_data(const rep& r)
            {
                return Derived(r);
            }
        private:
            rep _rep;
        };
        template<class Derived, class Enable = void>
        struct is_class_derived_from_number_base : std::false_type {};
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
        template<class Former, class Latter>
        struct precedes {
            static constexpr bool value =
                    (std::is_floating_point<Former>::value && !std::is_floating_point<Latter>::value)
                            || (is_derived_from_number_base<Former>::value &&
                                    !(is_derived_from_number_base<Latter>::value
                                            || std::is_floating_point<Latter>::value));
        };
        template<
                class Operator, class Lhs, class RhsDerived, class RhsRep,
                enable_if_t <precedes<Lhs, RhsDerived>::value, std::nullptr_t> = nullptr>
        constexpr auto operate(const Lhs& lhs, const number_base<RhsDerived, RhsRep>& rhs, Operator op)
        -> decltype(op(lhs, static_cast<Lhs>(static_cast<const RhsDerived&>(rhs))))
        {
            return op(lhs, static_cast<Lhs>(static_cast<const RhsDerived&>(rhs)));
        }
        template<
                class Operator, class LhsDerived, class LhsRep, class Rhs,
                enable_if_t <precedes<Rhs, LhsDerived>::value, std::nullptr_t> = nullptr>
        constexpr auto operate(const number_base<LhsDerived, LhsRep>& lhs, const Rhs& rhs, Operator op)
        -> decltype(op(static_cast<Rhs>(static_cast<const LhsDerived&>(lhs)), rhs))
        {
            return op(static_cast<Rhs>(static_cast<const LhsDerived&>(lhs)), rhs);
        }
        template<
                class Operator, class Lhs, class RhsDerived, class RhsRep,
                enable_if_t <precedes<RhsDerived, Lhs>::value, std::nullptr_t> = nullptr>
        constexpr auto operate(const Lhs& lhs, const number_base<RhsDerived, RhsRep>& rhs, Operator op)
        -> decltype(op(_impl::from_value<RhsDerived>(lhs), static_cast<const RhsDerived&>(rhs))) {
            return op(from_value<RhsDerived>(lhs), static_cast<const RhsDerived&>(rhs));
        }
        template<
                class Operator, class LhsDerived, class LhsRep, class Rhs,
                enable_if_t <precedes<LhsDerived, Rhs>::value, std::nullptr_t> = nullptr>
        constexpr auto operate(const number_base<LhsDerived, LhsRep>& lhs, const Rhs& rhs, Operator op)
        -> decltype(op(static_cast<const LhsDerived &>(lhs), from_value<LhsDerived>(rhs)))
        {
            return op(static_cast<const LhsDerived &>(lhs), from_value<LhsDerived>(rhs));
        }
        template<class Operator, class RhsDerived, class RhsRep>
        constexpr auto operate(const number_base<RhsDerived, RhsRep>& rhs, Operator op)
        -> decltype(op(rhs.data()))
        {
            return op(rhs.data());
        }
        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator+=(Lhs& lhs, const Rhs& rhs)
        -> decltype(lhs = lhs + rhs)
        {
            return lhs = lhs + rhs;
        }
        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator-=(Lhs& lhs, const Rhs& rhs)
        -> decltype(lhs = lhs - rhs)
        {
            return lhs = lhs - rhs;
        }
        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator*=(Lhs& lhs, const Rhs& rhs)
        -> decltype(lhs = lhs * rhs)
        {
            return lhs = lhs * rhs;
        }
        template<class Lhs, class Rhs, class = enable_if_t <is_derived_from_number_base<Lhs>::value>>
        auto operator/=(Lhs& lhs, const Rhs& rhs)
        -> decltype(lhs = lhs / rhs)
        {
            return lhs = lhs / rhs;
        }
        template<class RhsDerived, class RhsRep>
        constexpr auto operator+(const number_base<RhsDerived, RhsRep>& rhs)
        -> decltype(operate(rhs, plus_tag))
        {
            return operate(rhs, plus_tag);
        }
        template<class RhsDerived, class RhsRep>
        constexpr auto operator-(const number_base<RhsDerived, RhsRep>& rhs)
        -> decltype(operate(rhs, minus_tag))
        {
            return operate(rhs, minus_tag);
        }
        template<class Lhs, class Rhs>
        constexpr auto operator+(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, add_tag))
        {
            return operate(lhs, rhs, add_tag);
        }
        template<class Lhs, class Rhs>
        constexpr auto operator-(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, subtract_tag))
        {
            return operate(lhs, rhs, subtract_tag);
        }
        template<class Lhs, class Rhs>
        constexpr auto operator*(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, multiply_tag))
        {
            return operate(lhs, rhs, multiply_tag);
        }
        template<class Lhs, class Rhs>
        constexpr auto operator/(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, divide_tag))
        {
            return operate(lhs, rhs, divide_tag);
        }
        template<class Lhs, class Rhs>
        constexpr auto operator|(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, bitwise_or_tag))
        {
            return operate(lhs, rhs, bitwise_or_tag);
        }
        template<class Lhs, class Rhs>
        constexpr auto operator&(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, bitwise_and_tag))
        {
            return operate(lhs, rhs, bitwise_and_tag);
        }
        template<class Lhs, class Rhs>
        constexpr auto operator^(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, bitwise_xor_tag))
        {
            return operate(lhs, rhs, bitwise_xor_tag);
        }
        template<class Lhs, class Rhs>
        constexpr auto operator==(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, equal_tag))
        {
            return operate(lhs, rhs, equal_tag);
        }
        template<class Lhs, class Rhs>
        constexpr auto operator!=(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, not_equal_tag))
        {
            return operate(lhs, rhs, not_equal_tag);
        }
        template<class Lhs, class Rhs>
        constexpr auto operator<(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, less_than_tag))
        {
            return operate(lhs, rhs, less_than_tag);
        }
        template<class Lhs, class Rhs>
        constexpr auto operator>(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, greater_than_tag))
        {
            return operate(lhs, rhs, greater_than_tag);
        }
        template<class Lhs, class Rhs>
        constexpr auto operator<=(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, less_than_or_equal_tag))
        {
            return operate(lhs, rhs, less_than_or_equal_tag);
        }
        template<class Lhs, class Rhs>
        constexpr auto operator>=(const Lhs& lhs, const Rhs& rhs)
        -> decltype(operate(lhs, rhs, greater_than_or_equal_tag))
        {
            return operate(lhs, rhs, greater_than_or_equal_tag);
        }
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
    template<class Number>
    struct from_rep<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> {
        template<class Rep>
        constexpr auto operator()(const Rep &rep) const -> Number {
            return Number::from_data(static_cast<typename Number::rep>(rep));
        }
    };
    template<class Number>
    struct to_rep<Number, _impl::enable_if_t<_impl::is_derived_from_number_base<Number>::value>> {
        constexpr auto operator()(const typename Number::_derived& number) const
        -> decltype(number.data()){
            return number.data();
        }
    };
    template<class Derived, class Rep>
    struct scale<_impl::number_base<Derived, Rep>> {
        template<class Input>
        constexpr Rep operator()(const Input &i, int base, int exp) const {
            return (exp < 0)
                   ? _impl::to_rep(i) / _num_traits_impl::pow<Rep>(base, -exp)
                   : _impl::to_rep(i) * _num_traits_impl::pow<Rep>(base, exp);
        }
    };
}
namespace std {
    template<class Derived, class Rep>
    struct numeric_limits<sg14::_impl::number_base<Derived, Rep>>
    : numeric_limits<Rep> {
        using _value_type = Derived;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;
        static constexpr _value_type min() noexcept
        {
            return _value_type::from_data(_rep_numeric_limits::min());
        }
        static constexpr _value_type max() noexcept
        {
            return _value_type::from_data(_rep_numeric_limits::max());
        }
        static constexpr _value_type lowest() noexcept
        {
            return _value_type::from_data(_rep_numeric_limits::lowest());
        }
        static constexpr _value_type epsilon() noexcept
        {
            return _value_type::from_data(_rep_numeric_limits::round_error());
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
namespace sg14 {
    template<int Digits, class Narrowest>
    class elastic_integer;
    namespace _elastic_integer_impl {
        template<int Digits, class Narrowest>
        struct base_class {
            static constexpr _digits_type digits = Digits;
            static constexpr _digits_type rep_digits = _impl::max(sg14::digits<Narrowest>::value, digits);
            using rep = typename set_digits<Narrowest, rep_digits>::type;
            using type = _impl::number_base<elastic_integer<Digits, Narrowest>, rep>;
        };
        template<int Digits, class Narrowest>
        using base_class_t = typename base_class<Digits, Narrowest>::type;
    }
    template<int Digits, class Narrowest>
    struct digits<elastic_integer<Digits, Narrowest>> : std::integral_constant<_digits_type, Digits> {
        static constexpr _digits_type value = Digits;
    };
    template<int Digits, class Narrowest, _digits_type MinNumBits>
    struct set_digits<elastic_integer<Digits, Narrowest>, MinNumBits> {
        using type = elastic_integer<MinNumBits, Narrowest>;
    };
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
    template<int Digits, class Narrowest, class Value>
    struct from_value<elastic_integer<Digits, Narrowest>, Value> {
        using type = elastic_integer<sg14::digits<Value>::value, sg14::_impl::make_signed_t<Narrowest, sg14::is_signed<Value>::value>>;
    };
    template<int Digits, class Narrowest>
    struct scale<elastic_integer<Digits, Narrowest>> {
        using _value_type = elastic_integer<Digits, Narrowest>;
        constexpr _value_type operator()(const _value_type& i, int base, int exp) const {
            using _rep = typename _value_type::rep;
            return _value_type{ _impl::scale(i.data(), base, exp) };
        }
    };
    template<int Digits, class Narrowest = int>
    class elastic_integer : public _elastic_integer_impl::base_class_t<Digits, Narrowest> {
        static_assert(Digits > 0, "type requires positive number of digits");
        using _base = _elastic_integer_impl::base_class_t<Digits, Narrowest>;
    public:
        static constexpr int digits = Digits;
        using narrowest = Narrowest;
        using rep = typename _base::rep;
        constexpr elastic_integer() = default;
        constexpr elastic_integer(const elastic_integer& rhs)
                :_base(rhs)
        {
        }
        template<class Number, _impl::enable_if_t<std::numeric_limits<Number>::is_specialized, int> Dummy = 0>
        constexpr elastic_integer(Number n)
                : _base(static_cast<rep>(n))
        {
        }
        template<int FromWidth, class FromNarrowest>
        explicit constexpr elastic_integer(const elastic_integer<FromWidth, FromNarrowest>& rhs)
                :_base(static_cast<rep>(rhs.data()))
        {
        }
        template<class Integral, Integral Value, int Exponent>
        constexpr elastic_integer(const_integer<Integral, Value, Digits, Exponent>)
                : _base(static_cast<rep>(Value))
        {
            static_assert(!sg14::is_signed<Integral>::value || sg14::is_signed<rep>::value, "initialization by out-of-range value");
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
            return static_cast<S>(_base::data());
        }
    };
    template<
            class Integral, Integral Value>
    constexpr auto make_elastic_integer(const_integer<Integral, Value>)
    -> elastic_integer<used_bits(Value)>
    {
        return elastic_integer<used_bits(Value)>{Value};
    }
    template<class Narrowest = int, class Integral, _impl::enable_if_t<!is_const_integer<Integral>::value, int> Dummy = 0>
    constexpr auto make_elastic_integer(const Integral& value)
    -> decltype(elastic_integer<std::numeric_limits<Integral>::digits, Narrowest>{value})
    {
        return elastic_integer<std::numeric_limits<Integral>::digits, Narrowest>{value};
    }
    namespace _elastic_integer_impl {
        template<class ElasticInteger>
        struct is_elastic_integer : std::false_type {
        };
        template<int Digits, class Narrowest>
        struct is_elastic_integer<elastic_integer<Digits, Narrowest>> : std::true_type {
        };
        template<class Lhs, class Rhs>
        struct are_integer_class_operands {
            static constexpr int integer_class = is_elastic_integer<Lhs>::value+is_elastic_integer<Rhs>::value;
            static constexpr int integer_or_float =
                    _impl::is_integer_or_float<Lhs>::value+_impl::is_integer_or_float<Rhs>::value;
            static constexpr bool value = (integer_class>=1) && (integer_or_float==2);
        };
    }
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator~(const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> elastic_integer<RhsDigits, RhsNarrowest>
    {
        using elastic_integer = elastic_integer<RhsDigits, RhsNarrowest>;
        using rep = typename elastic_integer::rep;
        return elastic_integer::from_data(
            static_cast<rep>(
                rhs.data()
                ^ ((static_cast<rep>(~0)) >> (std::numeric_limits<rep>::digits - RhsDigits))));
    }
    template<int LhsDigits, class LhsNarrowest, class Rhs>
    constexpr auto operator<<(const elastic_integer<LhsDigits, LhsNarrowest>& lhs, const Rhs& rhs)
    -> elastic_integer<LhsDigits, LhsNarrowest>
    {
        return elastic_integer<LhsDigits, LhsNarrowest>::from_data(lhs.data() << rhs);
    }
    template<class Lhs, int RhsDigits, class RhsNarrowest>
    constexpr auto operator<<(const Lhs& lhs, const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> decltype(lhs << 0)
    {
        return lhs << rhs.data();
    }
    template<int LhsDigits, class LhsNarrowest, class Rhs>
    constexpr auto operator>>(const elastic_integer<LhsDigits, LhsNarrowest>& lhs, const Rhs& rhs)
    -> elastic_integer<LhsDigits, LhsNarrowest>
    {
        return elastic_integer<LhsDigits, LhsNarrowest>::from_data(lhs.data() >> rhs);
    }
    template<class Lhs, int RhsDigits, class RhsNarrowest>
    constexpr auto operator>>(const Lhs& lhs, const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> decltype(lhs >> 0)
    {
        return lhs >> rhs.data();
    }
    namespace _impl {
        template<int FromDigits, class FromNarrowest, int OtherDigits, class OtherNarrowest,
                _impl::enable_if_t<FromDigits!=OtherDigits || !std::is_same<FromNarrowest, OtherNarrowest>::value, std::nullptr_t> Dummy = nullptr>
        constexpr auto cast_to_common_type(
                const elastic_integer<FromDigits, FromNarrowest>& from,
                const elastic_integer<OtherDigits, OtherNarrowest>&)
        -> decltype(static_cast<_impl::common_type_t<
                elastic_integer<FromDigits, FromNarrowest>,
                elastic_integer<OtherDigits, OtherNarrowest>>>(from)) {
            return static_cast<_impl::common_type_t<
                    elastic_integer<FromDigits, FromNarrowest>,
                    elastic_integer<OtherDigits, OtherNarrowest>>>(from);
        };
        template<class Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest,
        bool Enable = Operator::is_comparison>
        constexpr auto operate(
                const elastic_integer<LhsDigits, LhsNarrowest>& lhs,
                const elastic_integer<RhsDigits, RhsNarrowest>& rhs,
                Operator op)
        -> decltype(op(cast_to_common_type(lhs, rhs), cast_to_common_type(rhs, lhs)))
        {
            return op(cast_to_common_type(lhs, rhs), cast_to_common_type(rhs, lhs));
        }
        template<class Operator, int Digits, class Narrowest,
                class = enable_if_t<Operator::is_comparison>>
        constexpr auto
        operate(const elastic_integer<Digits, Narrowest>& lhs, const elastic_integer<Digits, Narrowest>& rhs, Operator op)
        -> decltype(op(lhs.data(), rhs.data()))
        {
            return op(lhs.data(), rhs.data());
        }
    }
    namespace _impl {
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
        template<class OperationTag, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest,
                class = enable_if_t<OperationTag::is_arithmetic>>
        struct operate_params {
            using lhs = elastic_integer<LhsDigits, LhsNarrowest>;
            using rhs = elastic_integer<RhsDigits, RhsNarrowest>;
            using lhs_traits = std::numeric_limits<lhs>;
            using rhs_traits = std::numeric_limits<rhs>;
            using policy = typename _impl::policy<OperationTag, lhs_traits, rhs_traits>;
            using lhs_rep = typename lhs::rep;
            using rhs_rep = typename rhs::rep;
            using rep_result = typename _impl::op_result<OperationTag, lhs_rep, rhs_rep>;
            static constexpr _digits_type narrowest_width = _impl::max(
                    digits<LhsNarrowest>::value + sg14::is_signed<LhsNarrowest>::value,
                    digits<RhsNarrowest>::value + sg14::is_signed<RhsNarrowest>::value);
            using narrowest = set_digits_t<_impl::make_signed_t<rep_result, policy::is_signed>, narrowest_width-policy::is_signed>;
            using result_type = elastic_integer<policy::digits, narrowest>;
        };
        template<class Operator, int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest,
                bool Enable = Operator::is_arithmetic>
        constexpr auto operate(
                const elastic_integer<LhsDigits, LhsNarrowest>& lhs,
                const elastic_integer<RhsDigits, RhsNarrowest>& rhs,
                Operator op)
        -> typename operate_params<Operator, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::result_type
        {
            using result_type = typename operate_params<Operator, LhsDigits, LhsNarrowest, RhsDigits, RhsNarrowest>::result_type;
            return result_type::from_data(
                    static_cast<typename result_type::rep>(op(
                            static_cast<result_type>(lhs).data(),
                            static_cast<result_type>(rhs).data())));
        }
    }
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator-(const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>
    {
        using result_type = elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>;
        return result_type::from_data(-static_cast<result_type>(rhs).data());
    }
    template<int RhsDigits, class RhsNarrowest>
    constexpr auto operator+(const elastic_integer<RhsDigits, RhsNarrowest>& rhs)
    -> elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>
    {
        using result_type = elastic_integer<RhsDigits, typename make_signed<RhsNarrowest>::type>;
        return result_type::from_data(static_cast<result_type>(rhs).data());
    }
}
namespace std {
    template<int LhsDigits, class LhsNarrowest, int RhsDigits, class RhsNarrowest>
    struct common_type<sg14::elastic_integer<LhsDigits, LhsNarrowest>, sg14::elastic_integer<RhsDigits, RhsNarrowest>> {
        using type = sg14::elastic_integer<
                sg14::_impl::max(LhsDigits, RhsDigits),
                sg14::_impl::common_signedness_t<LhsNarrowest, RhsNarrowest>>;
    };
    template<int LhsDigits, class LhsNarrowest, class Rhs>
    struct common_type<sg14::elastic_integer<LhsDigits, LhsNarrowest>, Rhs>
            : common_type<sg14::elastic_integer<LhsDigits, LhsNarrowest>, sg14::elastic_integer<std::numeric_limits<Rhs>::digits, Rhs>> {
    };
    template<class Lhs, int RhsDigits, class RhsNarrowest>
    struct common_type<Lhs, sg14::elastic_integer<RhsDigits, RhsNarrowest>>
            : common_type<sg14::elastic_integer<std::numeric_limits<Lhs>::digits, Lhs>, sg14::elastic_integer<RhsDigits, RhsNarrowest>> {
    };
    namespace _elastic_integer_impl {
        template<class Rep, bool IsSigned>
        struct lowest;
        template<class Rep>
        struct lowest<Rep, true> {
            constexpr Rep operator()(const Rep& max) const noexcept
            {
                return -max;
            }
        };
        template<class Rep>
        struct lowest<Rep, false> {
            constexpr Rep operator()(const Rep&) const noexcept
            {
                return 0;
            }
        };
    };
    template<int Digits, class Narrowest>
    struct numeric_limits<sg14::elastic_integer<Digits, Narrowest>>
            : numeric_limits<Narrowest> {
        using _narrowest_numeric_limits = numeric_limits<Narrowest>;
        using _value_type = sg14::elastic_integer<Digits, Narrowest>;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;
        static constexpr _rep _rep_max() noexcept
        {
            return _rep_numeric_limits::max() >> (_rep_numeric_limits::digits-digits);
        }
        static constexpr int digits = Digits;
        static constexpr _value_type min() noexcept
        {
            return _value_type::from_data(1);
        }
        static constexpr _value_type max() noexcept
        {
            return _value_type::from_data(_rep_max());
        }
        static constexpr _value_type lowest() noexcept
        {
            return _elastic_integer_impl::lowest<_rep, _narrowest_numeric_limits::is_signed>()(_rep_max());
        }
    };
}
namespace sg14 {
    template<class Rep = int, int Exponent = 0>
    class fixed_point;
    namespace _impl {
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
    template<class Rep, int Exponent>
    class fixed_point
            : public _impl::number_base<fixed_point<Rep, Exponent>, Rep> {
        using _base = _impl::number_base<fixed_point<Rep, Exponent>, Rep>;
    public:
        using rep = Rep;
        constexpr static int exponent = Exponent;
        constexpr static int digits = std::numeric_limits<Rep>::digits;
        constexpr static int integer_digits = digits+exponent;
        constexpr static int fractional_digits = -exponent;
    private:
        constexpr fixed_point(rep r, int)
                :_base(r)
        {
        }
    public:
        constexpr fixed_point() : _base() { }
        template<class FromRep, int FromExponent>
        constexpr fixed_point(const fixed_point<FromRep, FromExponent>& rhs)
                : _base(fixed_point_to_rep(rhs))
        {
        }
        template<class Integral, Integral Constant>
        constexpr fixed_point(const std::integral_constant<Integral, Constant>&)
                : fixed_point(fixed_point<Integral, 0>::from_data(Constant))
        {
        }
        template<class S, _impl::enable_if_t<std::numeric_limits<S>::is_integer, int> Dummy = 0>
        constexpr fixed_point(const S& s)
            : fixed_point(fixed_point<S, 0>::from_data(s))
        {
        }
        template<class Integral, Integral Value, int Digits>
        constexpr fixed_point(const_integer<Integral, Value, Digits, Exponent> ci)
            : _base(ci << Exponent)
        {
        }
        template<class S, _impl::enable_if_t<std::is_floating_point<S>::value, int> Dummy = 0>
        constexpr fixed_point(S s)
                :_base(floating_point_to_rep(s))
        {
        }
        template<class S, _impl::enable_if_t<std::numeric_limits<S>::is_integer, int> Dummy = 0>
        fixed_point& operator=(S s)
        {
            return operator=(fixed_point<S, 0>::from_data(s));
        }
        template<class S, _impl::enable_if_t<std::is_floating_point<S>::value, int> Dummy = 0>
        fixed_point& operator=(S s)
        {
            _base::operator=(floating_point_to_rep(s));
            return *this;
        }
        template<class FromRep, int FromExponent>
        fixed_point& operator=(const fixed_point<FromRep, FromExponent>& rhs)
        {
            _base::operator=(fixed_point_to_rep(rhs));
            return *this;
        }
        constexpr operator bool() const
        {
            return static_cast<bool>(_base::data());
        }
        template<class S, _impl::enable_if_t<std::numeric_limits<S>::is_integer, int> Dummy = 0>
        constexpr operator S() const
        {
            return rep_to_integral<S>(_base::data());
        }
        template<class S, _impl::enable_if_t<std::is_floating_point<S>::value, int> Dummy = 0>
        explicit constexpr operator S() const
        {
            return rep_to_floating_point<S>(_base::data());
        }
        static constexpr fixed_point from_data(rep const & r)
        {
            return fixed_point(r, 0);
        }
    private:
        template<class S, _impl::enable_if_t<std::is_floating_point<S>::value, int> Dummy = 0>
        static constexpr S one();
        template<class S, _impl::enable_if_t<std::numeric_limits<S>::is_integer, int> Dummy = 0>
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
        static constexpr rep fixed_point_to_rep(const fixed_point<FromRep, FromExponent>& rhs);
    };
    template<class Rep, int Exponent>
    constexpr int fixed_point<Rep, Exponent>::exponent;
    template<class Rep, int Exponent>
    constexpr int fixed_point<Rep, Exponent>::digits;
    template<class Rep, int Exponent>
    constexpr int fixed_point<Rep, Exponent>::integer_digits;
    template<class Rep, int Exponent>
    constexpr int fixed_point<Rep, Exponent>::fractional_digits;
    namespace _impl {
        template<class T>
        struct is_fixed_point
                : public std::false_type {
        };
        template<class Rep, int Exponent>
        struct is_fixed_point<fixed_point<Rep, Exponent>>
                : public std::true_type {
        };
        template<int exp, class Output, class Input>
        constexpr Output shift_left(Input i)
        {
            using larger = typename std::conditional<
                    digits<Input>::value<=digits<Output>::value,
                    Output, Input>::type;
            return (exp>-std::numeric_limits<larger>::digits)
                   ? static_cast<Output>(_impl::scale<larger>(i, 2, exp))
                   : Output{0};
        }
        namespace fp {
            namespace type {
                template<class S, int Exponent, enable_if_t<Exponent==0, int> Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
                    return 1;
                }
                template<class S, int Exponent,
                        enable_if_t<!(Exponent<=0) && (Exponent<8), int> Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
                    return pow2<S, Exponent-1>()*S(2);
                }
                template<class S, int Exponent, enable_if_t<(Exponent>=8), int> Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
                    return pow2<S, Exponent-8>()*S(256);
                }
                template<class S, int Exponent,
                        enable_if_t<!(Exponent>=0) && (Exponent>-8), int> Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
                    return pow2<S, Exponent+1>()*S(.5);
                }
                template<class S, int Exponent, enable_if_t<(Exponent<=-8), int> Dummy = 0>
                constexpr S pow2()
                {
                    static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
                    return pow2<S, Exponent+8>()*S(.003906250);
                }
            }
        }
    }
    template<class Rep, int Exponent>
    template<class S, _impl::enable_if_t<std::is_floating_point<S>::value, int> Dummy>
    constexpr S fixed_point<Rep, Exponent>::one()
    {
        return _impl::fp::type::pow2<S, -exponent>();
    }
    template<class Rep, int Exponent>
    template<class S, _impl::enable_if_t<std::numeric_limits<S>::is_integer, int> Dummy>
    constexpr S fixed_point<Rep, Exponent>::one()
    {
        return fixed_point<S, 0>::from_data(1);
    }
    template<class Rep, int Exponent>
    template<class S>
    constexpr S fixed_point<Rep, Exponent>::inverse_one()
    {
        static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
        return _impl::fp::type::pow2<S, exponent>();
    }
    template<class Rep, int Exponent>
    template<class S>
    constexpr S fixed_point<Rep, Exponent>::rep_to_integral(rep r)
    {
        static_assert(std::numeric_limits<S>::is_integer, "S must be integral type");
        return _impl::shift_left<exponent, S>(r);
    }
    template<class Rep, int Exponent>
    template<class S>
    constexpr typename fixed_point<Rep, Exponent>::rep fixed_point<Rep, Exponent>::floating_point_to_rep(S s)
    {
        static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
        return static_cast<rep>(s*one<S>());
    }
    template<class Rep, int Exponent>
    template<class S>
    constexpr S fixed_point<Rep, Exponent>::rep_to_floating_point(rep r)
    {
        static_assert(std::is_floating_point<S>::value, "S must be floating-point type");
        return S(r)*inverse_one<S>();
    }
    template<class Rep, int Exponent>
    template<class FromRep, int FromExponent>
    constexpr typename fixed_point<Rep, Exponent>::rep fixed_point<Rep, Exponent>::fixed_point_to_rep(const fixed_point<FromRep, FromExponent>& rhs)
    {
        return _impl::shift_left<FromExponent-exponent, rep>(rhs.data());
    }
}
namespace sg14 {
    template<int IntegerDigits, int FractionalDigits = 0, class Narrowest = signed>
    using make_fixed = fixed_point<
            set_digits_t<Narrowest, IntegerDigits+FractionalDigits>,
            -FractionalDigits>;
    template<int IntegerDigits, int FractionalDigits = 0, class Narrowest = unsigned>
    using make_ufixed = make_fixed<
            IntegerDigits,
            FractionalDigits,
            typename make_unsigned<Narrowest>::type>;
}
namespace sg14 {
    namespace _impl {
        namespace fp {
            namespace arithmetic {
                struct raw_tag;
                struct lean_tag;
                struct wide_tag;
                using ::sg14::fixed_point;
                template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
                struct binary_pair_base {
                    using lhs_type = fixed_point<LhsRep, LhsExponent>;
                    using rhs_type = fixed_point<RhsRep, RhsExponent>;
                };
                template<class Lhs, class Rhs>
                struct binary_pair;
                template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
                struct binary_pair<fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>>
                        : binary_pair_base<LhsRep, LhsExponent, RhsRep, RhsExponent> {
                };
                template<class LhsRep, int LhsExponent, class Rhs>
                struct binary_pair<fixed_point<LhsRep, LhsExponent>, Rhs>
                        : binary_pair_base<LhsRep, LhsExponent, Rhs, 0> {
                    static_assert(std::numeric_limits<Rhs>::is_integer,
                            "named arithmetic functions take only fixed_point and integral types");
                };
                template<class Lhs, class RhsRep, int RhsExponent>
                struct binary_pair<Lhs, fixed_point<RhsRep, RhsExponent>>
                        : binary_pair_base<Lhs, 0, RhsRep, RhsExponent> {
                    static_assert(std::numeric_limits<Lhs>::is_integer,
                            "named arithmetic functions take only fixed_point and integral types");
                };
                template<class OperationTag, class ... Operands>
                struct rep_op_exponent;
                template<class Rhs>
                struct rep_op_exponent<_impl::plus_op, Rhs> : public std::integral_constant<int, Rhs::exponent> {
                };
                template<class Rhs>
                struct rep_op_exponent<_impl::minus_op, Rhs> : public std::integral_constant<int, Rhs::exponent> {
                };
                template<class Lhs, class Rhs>
                struct rep_op_exponent<_impl::add_op, Lhs, Rhs> : public std::integral_constant<int, _impl::min<int>(
                        Lhs::exponent,
                        Rhs::exponent)> {
                };
                template<class Lhs, class Rhs>
                struct rep_op_exponent<_impl::subtract_op, Lhs, Rhs>
                        : public std::integral_constant<int, _impl::min<int>(
                                Lhs::exponent,
                                Rhs::exponent)> {
                };
                template<class Lhs, class Rhs>
                struct rep_op_exponent<_impl::multiply_op, Lhs, Rhs> : public std::integral_constant<int,
                        Lhs::exponent+Rhs::exponent> {
                };
                template<class Lhs, class Rhs>
                struct rep_op_exponent<_impl::divide_op, Lhs, Rhs> : public std::integral_constant<int,
                        Lhs::exponent-Rhs::exponent> {
                };
                template<class PolicyTag, class OperationTag, class Lhs, class Rhs>
                struct result;
                template<class OperationTag, class Lhs, class Rhs>
                struct result<raw_tag, OperationTag, Lhs, Rhs> {
                    using lhs_rep = typename Lhs::rep;
                    using rhs_rep = typename Rhs::rep;
                    using rep_op_result = _impl::op_result<OperationTag, lhs_rep, rhs_rep>;
                    static constexpr int exponent = rep_op_exponent<OperationTag, Lhs, Rhs>::value;
                    using type = fixed_point<rep_op_result, exponent>;
                };
                template<class OperationTag, class Lhs, class Rhs>
                struct result<lean_tag, OperationTag, Lhs, Rhs> : result<raw_tag, OperationTag, Lhs, Rhs> {};
                template<class OperationTag, class Lhs, class Rhs>
                struct result<wide_tag, OperationTag, Lhs, Rhs> {
                    using lhs_rep = typename Lhs::rep;
                    using rhs_rep = typename Rhs::rep;
                    using rep_op_result = _impl::op_result<OperationTag, lhs_rep, rhs_rep>;
                    static constexpr int sufficient_sign_bits = std::is_signed<rep_op_result>::value;
                    static constexpr int sufficient_integer_digits = _impl::max(Lhs::integer_digits,
                            Rhs::integer_digits);
                    static constexpr int sufficient_fractional_digits = _impl::max(Lhs::fractional_digits,
                            Rhs::fractional_digits);
                    static constexpr _digits_type sufficient_digits = sufficient_integer_digits+sufficient_fractional_digits;
                    static constexpr int result_digits = _impl::max(sufficient_digits, digits<rep_op_result>::value);
                    using rep_type = set_digits_t<rep_op_result, result_digits>;
                    using type = fixed_point<rep_type, -sufficient_fractional_digits>;
                };
                template<class Lhs, class Rhs>
                struct result<wide_tag, _impl::multiply_op, Lhs, Rhs> {
                    using lhs_rep = typename Lhs::rep;
                    using rhs_rep = typename Rhs::rep;
                    using rep_op_result = _impl::op_result<_impl::multiply_op, lhs_rep, rhs_rep>;
                    static constexpr int digits = Lhs::digits+Rhs::digits;
                    static constexpr bool is_signed =
                            std::numeric_limits<lhs_rep>::is_signed || std::numeric_limits<rhs_rep>::is_signed;
                    using prewidened_result_rep = _impl::make_signed_t<rep_op_result, is_signed>;
                    using rep_type = set_digits_t<prewidened_result_rep, digits>;
                    static constexpr int rep_exponent = rep_op_exponent<_impl::multiply_op, Lhs, Rhs>::value;
                    using type = fixed_point<rep_type, rep_exponent>;
                };
                template<class Lhs, class Rhs>
                struct result<wide_tag, _impl::divide_op, Lhs, Rhs> {
                    using lhs_rep = typename Lhs::rep;
                    using rhs_rep = typename Rhs::rep;
                    using rep_op_result = _impl::op_result<_impl::multiply_op, lhs_rep, rhs_rep>;
                    static constexpr int integer_digits = Lhs::integer_digits+Rhs::fractional_digits;
                    static constexpr int fractional_digits = Lhs::fractional_digits+Rhs::integer_digits;
                    static constexpr int necessary_digits = integer_digits+fractional_digits;
                    static constexpr bool is_signed =
                            std::numeric_limits<lhs_rep>::is_signed || std::numeric_limits<rhs_rep>::is_signed;
                    static constexpr int promotion_digits = digits<rep_op_result>::value;
                    static constexpr int digits = _impl::max(necessary_digits, promotion_digits);
                    using prewidened_result_rep = _impl::make_signed_t<rep_op_result, is_signed>;
                    using rep_type = set_digits_t<prewidened_result_rep, digits>;
                    static constexpr int rep_exponent = -fractional_digits;
                    using type = fixed_point<rep_type, rep_exponent>;
                };
                template<class PolicyTag, class OperationTag, class Lhs, class Rhs>
                struct intermediate;
                template<class OperationTag, class Lhs, class Rhs>
                struct intermediate<lean_tag, OperationTag, Lhs, Rhs> {
                    using _result = result<lean_tag, OperationTag, Lhs, Rhs>;
                    using lhs_type = typename _result::type;
                    using rhs_type = lhs_type;
                };
                template<class Lhs, class Rhs>
                struct intermediate<lean_tag, _impl::multiply_op, Lhs, Rhs> {
                    using lhs_type = Lhs;
                    using rhs_type = Rhs;
                };
                template<class Lhs, class Rhs>
                struct intermediate<lean_tag, _impl::divide_op, Lhs, Rhs> {
                    using lhs_type = Lhs;
                    using rhs_type = Rhs;
                };
                template<class OperationTag, class Lhs, class Rhs>
                struct intermediate<wide_tag, OperationTag, Lhs, Rhs> {
                    using _result = result<wide_tag, OperationTag, Lhs, Rhs>;
                    using lhs_type = typename _result::type;
                    using rhs_type = lhs_type;
                };
                template<class Lhs, class Rhs>
                struct intermediate<wide_tag, _impl::multiply_op, Lhs, Rhs> {
                    using _result = result<wide_tag, _impl::multiply_op, Lhs, Rhs>;
                    using result_rep = typename _result::rep_type;
                    using prewidened_result_rep = typename _result::prewidened_result_rep;
                    using rep_type = typename std::conditional<
                            digits<prewidened_result_rep>::value>=_result::digits,
                            typename Lhs::rep, result_rep>::type;
                    using lhs_type = fixed_point<rep_type, Lhs::exponent>;
                    using rhs_type = Rhs;
                };
                template<class Lhs, class Rhs>
                struct intermediate<wide_tag, _impl::divide_op, Lhs, Rhs> {
                    using wide_result = result<wide_tag, _impl::divide_op, Lhs, Rhs>;
                    using rep_type = typename wide_result::rep_type;
                    static constexpr int exponent = Lhs::exponent-Rhs::digits;
                    using lhs_type = fixed_point<rep_type, exponent>;
                    using rhs_type = Rhs;
                };
                template<class PolicyTag, class OperationTag, class Lhs, class Rhs>
                struct operate_params {
                    using _binary_pair = binary_pair<Lhs, Rhs>;
                    using lhs_type = typename _binary_pair::lhs_type;
                    using rhs_type = typename _binary_pair::rhs_type;
                    using _intermediate = intermediate<PolicyTag, OperationTag, lhs_type, rhs_type>;
                    using intermediate_lhs = typename _intermediate::lhs_type;
                    using intermediate_rhs = typename _intermediate::rhs_type;
                    using _result = result<PolicyTag, OperationTag, lhs_type, rhs_type>;
                    using result_type = typename _result::type;
                };
            }
            using arithmetic_operator_tag = arithmetic::lean_tag;
            using division_arithmetic_operator_tag = arithmetic::wide_tag;
            using named_function_tag = arithmetic::wide_tag;
            using division_named_function_tag = arithmetic::lean_tag;
            template<class PolicyTag, class Operation, class Lhs, class Rhs>
            constexpr auto operate(const Lhs& lhs, const Rhs& rhs, Operation)
            -> typename arithmetic::operate_params<PolicyTag, Operation, Lhs, Rhs>::result_type
            {
                using params = arithmetic::operate_params<PolicyTag, Operation, Lhs, Rhs>;
                using intermediate_lhs = typename params::intermediate_lhs;
                using intermediate_rhs = typename params::intermediate_rhs;
                using result_type = typename params::result_type;
                using result_rep = typename result_type::rep;
                return result_type::from_data(
                        static_cast<result_rep>(
                                Operation()(
                                        static_cast<intermediate_lhs>(lhs).data(),
                                        static_cast<intermediate_rhs>(rhs).data())));
            };
        }
    }
}
namespace sg14 {
    template<class RhsRep, int RhsExponent>
    constexpr auto negate(const fixed_point<RhsRep, RhsExponent>& rhs)
    -> fixed_point<decltype(-rhs.data()), RhsExponent>
    {
        using result_type = fixed_point<decltype(-rhs.data()), RhsExponent>;
        return result_type::from_data(-rhs.data());
    }
    template<class Lhs, class Rhs>
    constexpr auto add(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::named_function_tag>(lhs, rhs, _impl::add_tag))
    {
        return _impl::fp::operate<_impl::fp::named_function_tag>(lhs, rhs, _impl::add_tag);
    }
    template<class Lhs, class Rhs>
    constexpr auto subtract(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::named_function_tag>(lhs, rhs, _impl::subtract_tag))
    {
        return _impl::fp::operate<_impl::fp::named_function_tag>(lhs, rhs, _impl::subtract_tag);
    }
    template<class Lhs, class Rhs>
    constexpr auto multiply(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::named_function_tag>(lhs, rhs, _impl::multiply_tag))
    {
        return _impl::fp::operate<_impl::fp::named_function_tag>(lhs, rhs, _impl::multiply_tag);
    }
    template<class Lhs, class Rhs>
    constexpr auto divide(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::division_named_function_tag>(lhs, rhs, _impl::divide_tag))
    {
        return _impl::fp::operate<_impl::fp::division_named_function_tag>(lhs, rhs, _impl::divide_tag);
    }
}
namespace sg14 {
    namespace _impl {
        namespace fp {
            namespace ct {
                template<class Lhs, class Rhs, class _Enable = void>
                struct common_type_mixed;
                template<class LhsRep, int LhsExponent, class RhsInteger>
                struct common_type_mixed<fixed_point
                        <LhsRep, LhsExponent>, RhsInteger, _impl::enable_if_t<std::numeric_limits<RhsInteger>::is_integer>> : std::common_type<
                        fixed_point<LhsRep, LhsExponent>, fixed_point<RhsInteger, 0>> {
                };
                template<class LhsRep, int LhsExponent, class Float>
                struct common_type_mixed<
                        fixed_point<LhsRep, LhsExponent>, Float,
                        _impl::enable_if_t<std::is_floating_point<Float>::value>>
                    : std::common_type<_impl::fp::float_of_same_size<LhsRep>, Float> {
                };
            }
        }
    }
}
namespace std {
    template<class Rep, int Exponent>
    struct common_type<sg14::fixed_point<Rep, Exponent>> {
        using type = sg14::fixed_point<
                typename std::common_type<Rep>::type,
                Exponent>;
    };
    template<class LhsRep, int LhsExponent, class Rhs>
    struct common_type<sg14::fixed_point<LhsRep, LhsExponent>, Rhs> {
        static_assert(!sg14::_impl::is_fixed_point<Rhs>::value, "fixed-point Rhs type");
        using type = typename sg14::_impl::fp::ct::common_type_mixed<sg14::fixed_point<LhsRep, LhsExponent>, Rhs>::type;
    };
    template<class Lhs, class RhsRep, int RhsExponent>
    struct common_type<Lhs, sg14::fixed_point<RhsRep, RhsExponent>> {
        static_assert(!sg14::_impl::is_fixed_point<Lhs>::value, "fixed-point Lhs type");
        using type = typename sg14::_impl::fp::ct::common_type_mixed<sg14::fixed_point<RhsRep, RhsExponent>, Lhs>::type;
    };
    template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
    struct common_type<sg14::fixed_point<LhsRep, LhsExponent>, sg14::fixed_point<RhsRep, RhsExponent>> {
        using type = sg14::fixed_point<sg14::_impl::common_type_t<LhsRep, RhsRep>, sg14::_impl::min(LhsExponent, RhsExponent)>;
    };
}
namespace sg14 {
    template<class RhsRep, int RhsExponent>
    constexpr auto operator-(const fixed_point<RhsRep, RhsExponent>& rhs)
    -> fixed_point<decltype(-rhs.data()), RhsExponent>
    {
        using result_type = fixed_point<decltype(-rhs.data()), RhsExponent>;
        return result_type::from_data(-rhs.data());
    }
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator+(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs, rhs, _impl::add_tag))
    {
        return _impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs, rhs, _impl::add_tag);
    }
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator-(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs, rhs, _impl::subtract_tag))
    {
        return _impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs, rhs, _impl::subtract_tag);
    }
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator*(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs, rhs, _impl::multiply_tag))
    {
        return _impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs, rhs, _impl::multiply_tag);
    }
    template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
    constexpr auto operator/(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::division_arithmetic_operator_tag>(lhs, rhs, _impl::divide_tag))
    {
        return _impl::fp::operate<_impl::fp::division_arithmetic_operator_tag>(lhs, rhs, _impl::divide_tag);
    }
    namespace _fixed_point_operators_impl {
        template<class Lhs, class Rhs>
        constexpr bool is_heterogeneous() {
            return (!std::is_same<Lhs, Rhs>::value) &&
                   (_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value);
        }
    }
    namespace _impl {
        template<
                class Operator, class Lhs, class Rhs,
                class = _impl::enable_if_t<Operator::is_comparison && sg14::_fixed_point_operators_impl::is_heterogeneous<Lhs, Rhs>()>>
        constexpr auto operate(const Lhs& lhs, const Rhs& rhs, Operator op)
        -> decltype(op(static_cast<_impl::common_type_t<Lhs, Rhs>>(lhs), static_cast<_impl::common_type_t<Lhs, Rhs>>(rhs)))
        {
            return op(static_cast<_impl::common_type_t<Lhs, Rhs>>(lhs), static_cast<_impl::common_type_t<Lhs, Rhs>>(rhs));
        };
        template<class Operator, class Rep, int Exponent, class = _impl::enable_if_t<Operator::is_comparison>>
        constexpr auto operate(const fixed_point<Rep, Exponent>& lhs, const fixed_point<Rep, Exponent>& rhs, Operator op)
        -> decltype(op(lhs.data(), rhs.data()))
        {
            return op(lhs.data(), rhs.data());
        };
    }
    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = _impl::enable_if_t<std::numeric_limits<RhsInteger>::is_integer>>
    constexpr auto operator+(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs + fixed_point<RhsInteger, 0>{rhs})
    {
        return lhs + fixed_point<RhsInteger, 0>{rhs};
    }
    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = _impl::enable_if_t<std::numeric_limits<RhsInteger>::is_integer>>
    constexpr auto operator-(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs - fixed_point<RhsInteger, 0>{rhs})
    {
        return lhs - fixed_point<RhsInteger, 0>{rhs};
    }
    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = _impl::enable_if_t<std::numeric_limits<RhsInteger>::is_integer>>
    constexpr auto operator*(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs*fixed_point<RhsInteger>(rhs))
    {
        return lhs*fixed_point<RhsInteger>(rhs);
    }
    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = _impl::enable_if_t<std::numeric_limits<RhsInteger>::is_integer>>
    constexpr auto operator/(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs/fixed_point<RhsInteger>{rhs})
    {
        return lhs/fixed_point<RhsInteger>{rhs};
    }
    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = _impl::enable_if_t<std::numeric_limits<LhsInteger>::is_integer>>
    constexpr auto operator+(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger, 0>{lhs} + rhs)
    {
        return fixed_point<LhsInteger, 0>{lhs} + rhs;
    }
    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = _impl::enable_if_t<std::numeric_limits<LhsInteger>::is_integer>>
    constexpr auto operator-(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}-rhs)
    {
        return fixed_point<LhsInteger>{lhs}-rhs;
    }
    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = _impl::enable_if_t<std::numeric_limits<LhsInteger>::is_integer>>
    constexpr auto operator*(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}*rhs)
    {
        return fixed_point<LhsInteger>{lhs}*rhs;
    }
    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = _impl::enable_if_t<std::numeric_limits<LhsInteger>::is_integer>>
    constexpr auto operator/(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}/rhs)
    {
        return fixed_point<LhsInteger>{lhs}/rhs;
    }
    template<class LhsRep, int LhsExponent, class RhsFloat, typename = _impl::enable_if_t<std::is_floating_point<RhsFloat>::value>>
    constexpr auto operator+(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsFloat& rhs)-> _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)+static_cast<result_type>(rhs);
    }
    template<class LhsRep, int LhsExponent, class RhsFloat, typename = _impl::enable_if_t <std::is_floating_point<RhsFloat>::value>>
    constexpr auto operator-(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsFloat& rhs)-> _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)-static_cast<result_type>(rhs);
    }
    template<class LhsRep, int LhsExponent, class RhsFloat>
    constexpr auto operator*(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const RhsFloat& rhs)
    -> _impl::common_type_t<
            fixed_point<LhsRep, LhsExponent>,
            _impl::enable_if_t<std::is_floating_point<RhsFloat>::value, RhsFloat>>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)*rhs;
    }
    template<class LhsRep, int LhsExponent, class RhsFloat>
    constexpr auto operator/(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const RhsFloat& rhs)
    -> _impl::common_type_t<
            fixed_point<LhsRep, LhsExponent>,
            _impl::enable_if_t<std::is_floating_point<RhsFloat>::value, RhsFloat>>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)/rhs;
    }
    template<class LhsFloat, class RhsRep, int RhsExponent, typename = _impl::enable_if_t <std::is_floating_point<LhsFloat>::value>>
    constexpr auto operator+(const LhsFloat& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)-> _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>;
        return static_cast<result_type>(lhs)+static_cast<result_type>(rhs);
    }
    template<class LhsFloat, class RhsRep, int RhsExponent, typename = _impl::enable_if_t <std::is_floating_point<LhsFloat>::value>>
    constexpr auto operator-(const LhsFloat& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)-> _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>;
        return static_cast<result_type>(lhs)-static_cast<result_type>(rhs);
    }
    template<class LhsFloat, class RhsRep, int RhsExponent>
    constexpr auto operator*(
            const LhsFloat& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> _impl::common_type_t <_impl::enable_if_t<std::is_floating_point<LhsFloat>::value, LhsFloat>, fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<fixed_point<RhsRep, RhsExponent>, LhsFloat>;
        return lhs*static_cast<result_type>(rhs);
    }
    template<class LhsFloat, class RhsRep, int RhsExponent>
    constexpr auto operator/(
            const LhsFloat& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> _impl::common_type_t <_impl::enable_if_t<std::is_floating_point<LhsFloat>::value, LhsFloat>, fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<fixed_point<RhsRep, RhsExponent>, LhsFloat>;
        return lhs/
                static_cast<result_type>(rhs);
    }
    template<class LhsRep, int LhsExponent, class Rhs>
    constexpr auto operator<<(const fixed_point<LhsRep, LhsExponent>& lhs, const Rhs& rhs)
    -> decltype(_impl::from_rep<fixed_point<decltype(lhs.data() << rhs), LhsExponent>>(lhs.data() << rhs))
    {
        return _impl::from_rep<fixed_point<decltype(lhs.data() << rhs), LhsExponent>>(lhs.data() << rhs);
    }
    template<class LhsRep, int LhsExponent, class Rhs>
    constexpr auto operator>>(const fixed_point<LhsRep, LhsExponent>& lhs, const Rhs& rhs)
    -> decltype(_impl::from_rep<fixed_point<decltype(lhs.data() >> rhs), LhsExponent>>(lhs.data() >> rhs))
    {
        return _impl::from_rep<fixed_point<decltype(lhs.data() >> rhs), LhsExponent>>(lhs.data() >> rhs);
    }
    template<class LhsRep, int LhsExponent, class RhsIntegral, RhsIntegral RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent+RhsValue>
    operator<<(const fixed_point<LhsRep, LhsExponent>& lhs, const_integer<RhsIntegral, RhsValue>)
    {
        return fixed_point<LhsRep, LhsExponent+RhsValue>::from_data(lhs.data());
    }
    template<class LhsRep, int LhsExponent, class RhsIntegral, RhsIntegral RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent-RhsValue>
    operator>>(const fixed_point<LhsRep, LhsExponent>& lhs, const_integer<RhsIntegral, RhsValue>)
    {
        return fixed_point<LhsRep, LhsExponent-RhsValue>::from_data(lhs.data());
    }
    template<class LhsRep, int LhsExponent, class RhsIntegral, RhsIntegral RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent+RhsValue>
    operator<<(const fixed_point<LhsRep, LhsExponent>& lhs, std::integral_constant<RhsIntegral, RhsValue>)
    {
        return fixed_point<LhsRep, LhsExponent+RhsValue>::from_data(lhs.data());
    }
    template<class LhsRep, int LhsExponent, class RhsIntegral, RhsIntegral RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent-RhsValue>
    operator>>(const fixed_point<LhsRep, LhsExponent>& lhs, std::integral_constant<RhsIntegral, RhsValue>)
    {
        return fixed_point<LhsRep, LhsExponent-RhsValue>::from_data(lhs.data());
    }
}
namespace sg14 {
    namespace _impl {
        template<class Rep, int Exponent>
        struct get_rep<fixed_point<Rep, Exponent>> {
            using type = Rep;
        };
        template<class OldRep, int Exponent, class NewRep>
        struct set_rep<fixed_point<OldRep, Exponent>, NewRep> {
            using type = fixed_point<NewRep, Exponent>;
        };
    }
    template<class Rep, int Exponent>
        struct digits<fixed_point<Rep, Exponent>> : digits<Rep> {
    };
    template<class Rep, int Exponent, _digits_type MinNumBits>
    struct set_digits<fixed_point<Rep, Exponent>, MinNumBits> {
        using type = fixed_point<set_digits_t<Rep, MinNumBits>, Exponent>;
    };
    template<class Rep, int Exponent, class Value>
    struct from_value<fixed_point<Rep, Exponent>, Value> {
        using type = fixed_point<Value>;
    };
    template<class Rep, int Exponent>
    constexpr auto abs(const fixed_point<Rep, Exponent>& x) noexcept
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
                    return (bit>n) ? sqrt_bit<Rep>(n, bit >> 2) : bit;
                }
                template<class Rep>
                constexpr Rep sqrt_bit(Rep n)
                {
                    return sqrt_bit<Rep>(n, Rep(1) << ((digits<Rep>::value + is_signed<Rep>::value) - 2));
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
                                            bit >> 2,
                                            static_cast<Rep>((result >> 1)+bit))
                             : sqrt_solve3<Rep>(n, bit >> 2, result >> 1)
                           : result;
                }
                template<class Rep>
                constexpr Rep sqrt_solve1(Rep n)
                {
                    return sqrt_solve3<Rep>(n, sqrt_bit<Rep>(n), Rep{0});
                }
            }
        }
    }
    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    sqrt(const fixed_point <Rep, Exponent>& x)
    {
        using widened_type = fixed_point<set_digits_t<Rep, digits<Rep>::value*2>, Exponent*2>;
        return
                (x<fixed_point<Rep, Exponent>(0))
                ? throw std::invalid_argument("cannot represent square root of negative value") :
                fixed_point<Rep, Exponent>::from_data(
                        static_cast<Rep>(_impl::fp::extras::sqrt_solve1(widened_type{x}.data())));
    }
    namespace _impl {
        namespace fp {
            namespace extras {
                template<class Rep, int Exponent, _impl::fp::float_of_same_size<Rep>(* F)(
                        _impl::fp::float_of_same_size<Rep>)>
                constexpr fixed_point <Rep, Exponent>
                crib(const fixed_point <Rep, Exponent>& x) noexcept
                {
                    using floating_point = _impl::fp::float_of_same_size<Rep>;
                    return static_cast<fixed_point<Rep, Exponent>>(F(static_cast<floating_point>(x)));
                }
            }
        }
    }
    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    sin(const fixed_point <Rep, Exponent>& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, std::sin>(x);
    }
    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    cos(const fixed_point <Rep, Exponent>& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, std::cos>(x);
    }
    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    exp(const fixed_point <Rep, Exponent>& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, std::exp>(x);
    }
    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    pow(const fixed_point <Rep, Exponent>& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, std::pow>(x);
    }
    template<class Rep, int Exponent>
    ::std::ostream& operator<<(::std::ostream& out, const fixed_point <Rep, Exponent>& fp)
    {
        return out << static_cast<long double>(fp);
    }
    template<class Rep, int Exponent>
    ::std::istream& operator>>(::std::istream& in, fixed_point <Rep, Exponent>& fp)
    {
        long double ld;
        in >> ld;
        fp = ld;
        return in;
    }
}
namespace std {
    template<class Rep, int Exponent>
    struct numeric_limits<sg14::fixed_point<Rep, Exponent>>
            : std::numeric_limits<sg14::_impl::number_base<sg14::fixed_point<Rep, Exponent>, Rep>> {
        using _value_type = sg14::fixed_point<Rep, Exponent>;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;
        static constexpr _value_type min() noexcept
        {
            return _value_type::from_data(_rep{1});
        }
        static constexpr _value_type max() noexcept
        {
            return _value_type::from_data(_rep_numeric_limits::max());
        }
        static constexpr _value_type lowest() noexcept
        {
            return _value_type::from_data(_rep_numeric_limits::lowest());
        }
        static constexpr bool is_integer = false;
        static constexpr _value_type epsilon() noexcept
        {
            return _value_type::from_data(_rep{1});
        }
        static constexpr _value_type round_error() noexcept
        {
            return _value_type::from_data(_rep{0});
        }
        static constexpr _value_type infinity() noexcept
        {
            return _value_type::from_data(_rep{0});
        }
        static constexpr _value_type quiet_NaN() noexcept
        {
            return _value_type::from_data(_rep{0});
        }
        static constexpr _value_type signaling_NaN() noexcept
        {
            return _value_type::from_data(_rep{0});
        }
        static constexpr _value_type denorm_min() noexcept
        {
            return _value_type::from_data(_rep{1});
        }
    };
}
namespace sg14 {
    template<int IntegerDigits, int FractionalDigits = 0, class Narrowest = signed>
    using elastic_fixed_point = fixed_point<elastic_integer<IntegerDigits+FractionalDigits, Narrowest>, -FractionalDigits>;
    template<
            typename Narrowest = int,
            typename Integral = int,
            Integral Value = 0>
    constexpr elastic_fixed_point<_impl::max(_impl::used_bits_symmetric(Value), 1), -trailing_bits(Value), Narrowest>
    make_elastic_fixed_point(const_integer<Integral, Value> = const_integer<Integral, Value>{})
    {
        return Value;
    }
    template<class Narrowest = int, class Integral = int>
    constexpr elastic_fixed_point<std::numeric_limits<Integral>::digits, 0, Narrowest>
    make_elastic_fixed_point(Integral value)
    {
        return {value};
    }
    namespace literals {
        template<char... Digits>
        constexpr auto operator "" _elastic()
        -> decltype(make_elastic_fixed_point<int, std::intmax_t, _const_integer_impl::digits_to_integral<Digits...>::value>()) {
            return make_elastic_fixed_point<int, std::intmax_t, _const_integer_impl::digits_to_integral<Digits...>::value>();
        }
    }
}
namespace sg14 {
    static constexpr struct native_overflow_tag {
    } native_overflow{};
    static constexpr struct throwing_overflow_tag {
    } throwing_overflow{};
    static constexpr struct saturated_overflow_tag {
    } saturated_overflow{};
    namespace _overflow_impl {
        template<class Result>
        constexpr Result return_if(bool condition, const Result& value, const char* )
        {
            return condition ? value : throw std::overflow_error("");
        }
        template<class T>
        struct positive_digits : public std::integral_constant<int, std::numeric_limits<T>::digits> {
        };
        template<class T>
        struct negative_digits
                : public std::integral_constant<int, std::is_signed<T>::value ? std::numeric_limits<T>::digits : 0> {
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
            return source>static_cast<Source>(std::numeric_limits<Destination>::max());
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
            return source<static_cast<Source>(std::numeric_limits<Destination>::lowest());
        }
        template<class OverflowTag, class Operator, class Enable = void>
        struct operate;
    }
    template<class Result, class Input>
    constexpr Result convert(native_overflow_tag, const Input& rhs)
    {
        return static_cast<Result>(rhs);
    }
    template<class Result, class Input>
    constexpr Result convert(throwing_overflow_tag, const Input& rhs)
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
    constexpr Result convert(saturated_overflow_tag, const Input& rhs)
    {
        using numeric_limits = std::numeric_limits<Result>;
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
        struct operate<native_overflow_tag, _impl::add_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const
            -> decltype(lhs+rhs)
            {
                return lhs+rhs;
            }
        };
        template<>
        struct operate<throwing_overflow_tag, _impl::add_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const
            -> decltype(lhs+rhs)
            {
                using result_type = decltype(lhs+rhs);
                using numeric_limits = std::numeric_limits<result_type>;
                return _overflow_impl::return_if(
                        !((rhs>=_impl::from_rep<Rhs>(0))
                          ? (lhs>numeric_limits::max()-rhs)
                          : (lhs<numeric_limits::lowest()-rhs)),
                        lhs+rhs,
                        "overflow in addition");
            }
        };
        template<>
        struct operate<saturated_overflow_tag, _impl::add_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const
            -> _impl::op_result<_impl::add_op, Lhs, Rhs>
            {
                using result_type = decltype(lhs+rhs);
                using numeric_limits = std::numeric_limits<result_type>;
                return (rhs>0)
                       ? (lhs>numeric_limits::max()-rhs) ? numeric_limits::max() : lhs+rhs
                       : (lhs<numeric_limits::lowest()-rhs) ? numeric_limits::lowest() : lhs+rhs;
            }
        };
    }
    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto add(OverflowTag, const Lhs& lhs, const Rhs& rhs)
    -> decltype(lhs+rhs)
    {
        return for_rep<decltype(lhs+rhs)>(_overflow_impl::operate<OverflowTag, _impl::add_op>(), lhs, rhs);
    }
    namespace _overflow_impl {
        template<>
        struct operate<native_overflow_tag, _impl::subtract_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const
            -> decltype(lhs-rhs)
            {
                return lhs-rhs;
            }
        };
        template<>
        struct operate<throwing_overflow_tag, _impl::subtract_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const
            -> decltype(lhs-rhs)
            {
                using result_type = decltype(lhs-rhs);
                using numeric_limits = std::numeric_limits<result_type>;
                return _overflow_impl::return_if(
                        (rhs<_impl::from_rep<Rhs>(0))
                        ? (lhs<=numeric_limits::max()+rhs)
                        : (lhs>=numeric_limits::lowest()+rhs),
                        lhs-rhs,
                        "positive overflow in subtraction");
            }
        };
        template<>
        struct operate<saturated_overflow_tag, _impl::subtract_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const
            -> _impl::op_result<_impl::subtract_op, Lhs, Rhs>
            {
                using result_type = decltype(lhs-rhs);
                using numeric_limits = std::numeric_limits<result_type>;
                return (rhs<0)
                       ? (lhs>numeric_limits::max()+rhs) ? numeric_limits::max() : lhs-rhs
                       : (lhs<numeric_limits::lowest()+rhs) ? numeric_limits::lowest() : lhs-rhs;
            }
        };
    }
    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto subtract(OverflowTag, const Lhs& lhs, const Rhs& rhs)
    -> decltype(lhs-rhs)
    {
        return for_rep<decltype(lhs-rhs)>(_overflow_impl::operate<OverflowTag, _impl::subtract_op>(), lhs, rhs);
    }
    namespace _overflow_impl {
        template<>
        struct operate<native_overflow_tag, _impl::multiply_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const
            -> decltype(lhs*rhs)
            {
                return lhs*rhs;
            }
        };
        template<class Lhs, class Rhs>
        constexpr bool is_multiply_overflow(const Lhs& lhs, const Rhs& rhs)
        {
            using result_nl = std::numeric_limits<decltype(lhs*rhs)>;
            return lhs && rhs && ((lhs>Lhs{})
                                  ? ((rhs>Rhs{}) ? (result_nl::max()/rhs) : (result_nl::lowest()/rhs))<lhs
                                  : ((rhs>Rhs{}) ? (result_nl::lowest()/rhs) : (result_nl::max()/rhs))>lhs);
        }
        template<>
        struct operate<throwing_overflow_tag, _impl::multiply_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const
            -> decltype(lhs*rhs)
            {
                return _overflow_impl::return_if(
                        !is_multiply_overflow(lhs, rhs),
                        lhs*rhs, "overflow in multiplication");
            }
        };
        template<>
        struct operate<saturated_overflow_tag, _impl::multiply_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const
            -> _impl::op_result<_impl::multiply_op, Lhs, Rhs>
            {
                using result_type = decltype(lhs*rhs);
                return is_multiply_overflow(lhs, rhs)
                       ? ((lhs>0) ^ (rhs>0))
                         ? std::numeric_limits<result_type>::lowest()
                         : std::numeric_limits<result_type>::max()
                       : lhs*rhs;
            }
        };
    }
    template<class OverflowTag, class Lhs, class Rhs>
    constexpr auto multiply(OverflowTag, const Lhs& lhs, const Rhs& rhs)
    -> decltype(lhs*rhs)
    {
        return for_rep<decltype(lhs*rhs)>(_overflow_impl::operate<OverflowTag, _impl::multiply_op>(), lhs, rhs);
    }
    namespace _overflow_impl {
        template<class OverflowTag>
        struct operate<OverflowTag, _impl::divide_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const
            -> decltype(lhs/rhs)
            {
                return lhs/rhs;
            }
        };
    }
    namespace _overflow_impl {
        template<class Operator>
        struct operate<native_overflow_tag, Operator,
                _impl::enable_if_t<Operator::is_comparison>> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const
        -> decltype(Operator()(lhs, rhs))
        {
            return Operator()(lhs, rhs);
        }
        };
        template<class Operator>
        struct operate<throwing_overflow_tag, Operator,
                _impl::enable_if_t<Operator::is_comparison>> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const
            -> _impl::op_result<Operator, Lhs, Rhs>
            {
                return Operator()(lhs, rhs);
            }
        };
        template<class Operator>
        struct operate<saturated_overflow_tag, Operator,
                _impl::enable_if_t<Operator::is_comparison>> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const
            -> _impl::op_result<Operator, Lhs, Rhs>
            {
                using converted = decltype(lhs | rhs);
                return Operator()(
                        convert<converted>(saturated_overflow, lhs),
                        convert<converted>(saturated_overflow, rhs));
            }
        };
    }
}
namespace sg14 {
    struct closest_rounding_tag {
        template<class To, class From>
        static constexpr To convert(const From& from)
        {
            return static_cast<To>(std::intmax_t(from+((from>=0) ? .5 : -.5)));
        }
    };
    template<class Rep = int, class RoundingTag = closest_rounding_tag>
    class precise_integer : public _impl::number_base<precise_integer<Rep, RoundingTag>, Rep> {
        using super = _impl::number_base<precise_integer<Rep, RoundingTag>, Rep>;
    public:
        using rounding = RoundingTag;
        constexpr precise_integer() = default;
        template<class T, _impl::enable_if_t<std::numeric_limits<T>::is_integer, int> Dummy = 0>
        constexpr precise_integer(const T& v)
                : super(static_cast<Rep>(v)) { }
        template<class T, _impl::enable_if_t<!std::numeric_limits<T>::is_integer, int> Dummy = 0>
        constexpr precise_integer(const T& v)
                : super(rounding::template convert<Rep>(v)) { }
        template<class T>
        constexpr explicit operator T() const
        {
            return static_cast<T>(super::data());
        }
    };
    template<class Rep, class RoundingTag>
    struct digits<precise_integer<Rep, RoundingTag>> : digits<Rep> {
    };
    template<class Rep, class RoundingTag, _digits_type MinNumBits>
    struct set_digits<precise_integer<Rep, RoundingTag>, MinNumBits> {
        using type = precise_integer<set_digits_t<Rep, MinNumBits>, RoundingTag>;
    };
    namespace _impl {
        template<class Rep, class RoundingTag>
        struct get_rep<precise_integer<Rep, RoundingTag>> {
            using type = Rep;
        };
        template<class OldRep, class RoundingTag, class NewRep>
        struct set_rep<precise_integer<OldRep, RoundingTag>, NewRep> {
            using type = precise_integer<NewRep, RoundingTag>;
        };
    }
    template<class Rep, class RoundingTag, class Value>
    struct from_value<precise_integer<Rep, RoundingTag>, Value> {
        using type = precise_integer<Value, RoundingTag>;
    };
    template<class Rep, class RoundingTag>
    struct scale<precise_integer<Rep, RoundingTag>>
    : scale<_impl::number_base<precise_integer<Rep, RoundingTag>, Rep>> {
    };
    namespace _precise_integer_impl {
        template<class T>
        struct is_precise_integer : std::false_type {
        };
        template<class Rep, class RoundingTag>
        struct is_precise_integer<precise_integer<Rep, RoundingTag>> : std::true_type {
        };
    }
    namespace _impl {
        template<class Operator, class RoundingTag, class LhsRep, class RhsRep, class = enable_if_t<Operator::is_arithmetic>>
        constexpr auto operate_common_tag(
                const precise_integer<LhsRep, RoundingTag>& lhs,
                const precise_integer<RhsRep, RoundingTag>& rhs)
        -> decltype(from_rep<precise_integer<op_result<Operator, LhsRep, RhsRep>, RoundingTag>>(Operator()(lhs.data(), rhs.data())))
        {
            using result_type = precise_integer<op_result<Operator, LhsRep, RhsRep>, RoundingTag>;
            return from_rep<result_type>(Operator()(lhs.data(), rhs.data()));
        }
        template<class Operator, class RoundingTag, class LhsRep, class RhsRep, enable_if_t<Operator::is_comparison, int> = 0>
        constexpr auto operate_common_tag(
                const precise_integer<LhsRep, RoundingTag>& lhs,
                const precise_integer<RhsRep, RoundingTag>& rhs)
        -> decltype(Operator()(lhs.data(), rhs.data()))
        {
            return Operator()(lhs.data(), rhs.data());
        }
        template<class Operator, class LhsRep, class LhsRoundingTag, class RhsRep, class RhsRoundingTag>
        constexpr auto operate(
                const precise_integer<LhsRep, LhsRoundingTag>& lhs,
                const precise_integer<RhsRep, RhsRoundingTag>& rhs,
                Operator)
        -> decltype(operate_common_tag<Operator, common_type_t<LhsRoundingTag, RhsRoundingTag>>(lhs, rhs))
        {
            return operate_common_tag<Operator, common_type_t<LhsRoundingTag, RhsRoundingTag>>(lhs, rhs);
        }
    }
    template<class LhsRep, class LhsRoundingTag, class RhsInteger>
    constexpr auto operator<<(
            const precise_integer<LhsRep, LhsRoundingTag>& lhs,
            const RhsInteger& rhs)
    -> decltype(from_rep<precise_integer<decltype(_impl::to_rep(lhs) << rhs), LhsRoundingTag>>(_impl::to_rep(lhs) << rhs))
    {
        return from_rep<precise_integer<
                decltype(_impl::to_rep(lhs) << rhs),
                LhsRoundingTag>>(_impl::to_rep(lhs) << rhs);
    }
}
namespace std {
    template<class Rep, class RoundingTag>
    struct numeric_limits<sg14::precise_integer<Rep, RoundingTag>>
            : numeric_limits<sg14::_impl::number_base<sg14::precise_integer<Rep, RoundingTag>, Rep>> {};
}
namespace sg14 {
    template<class Rep, class OverflowTag>
    class safe_integer;
    namespace _integer_impl {
        template<class T>
        struct is_safe_integer
                : std::false_type {
        };
        template<class Rep, class OverflowTag>
        struct is_safe_integer<safe_integer<Rep, OverflowTag>>
                : std::true_type {
        };
        template<class Lhs, class Rhs>
        struct are_integer_class_operands {
            static constexpr int integer_class = is_safe_integer<Lhs>::value + is_safe_integer<Rhs>::value;
            static constexpr int integer_or_float = _impl::is_integer_or_float<Lhs>::value + _impl::is_integer_or_float<Rhs>::value;
            static constexpr bool value = (integer_class >= 1) && (integer_or_float == 2);
        };
        template<class, class, class = void>
        struct common_type;
        template<class LhsRep, class RhsRep, class OverflowTag>
        struct common_type<
                safe_integer<LhsRep, OverflowTag>,
                safe_integer<RhsRep, OverflowTag>> {
            using type = safe_integer<
                    typename std::common_type<LhsRep, RhsRep>::type,
                    OverflowTag>;
        };
        template<class LhsRep, class LhsOverflowTag, class RhsInteger>
        struct common_type<
                safe_integer<LhsRep, LhsOverflowTag>, RhsInteger,
                _impl::enable_if_t<
                        !_integer_impl::is_safe_integer<RhsInteger>::value && std::is_integral<RhsInteger>::value>> {
            using type = typename sg14::safe_integer<typename std::common_type<LhsRep, RhsInteger>::type, LhsOverflowTag>;
        };
        template<class LhsRep, class LhsOverflowTag, class Float>
        struct common_type<
                safe_integer<LhsRep, LhsOverflowTag>, Float,
                _impl::enable_if_t<std::is_floating_point<Float>::value>> {
            using type = typename std::common_type<LhsRep, Float>::type;
        };
        template<class Lhs, class RhsRep, class RhsOverflowTag>
        struct common_type<Lhs, safe_integer<RhsRep, RhsOverflowTag>>
                : common_type<safe_integer<RhsRep, RhsOverflowTag>, Lhs> {
        };
    }
    template<class Rep = int, class OverflowTag = throwing_overflow_tag>
    class safe_integer : public _impl::number_base<safe_integer<Rep, OverflowTag>, Rep> {
        using _base = _impl::number_base<safe_integer<Rep, OverflowTag>, Rep>;
    public:
        using rep = Rep;
        using overflow_tag = OverflowTag;
        constexpr safe_integer() = delete;
        template<class RhsRep, class RhsOverflowTag>
        constexpr safe_integer(const safe_integer<RhsRep, RhsOverflowTag>& rhs)
                :safe_integer(rhs.data())
        {
        }
        template<class Rhs, _impl::enable_if_t<!_integer_impl::is_safe_integer<Rhs>::value, int> dummy = 0>
        constexpr safe_integer(const Rhs& rhs)
                :_base(convert<rep>(overflow_tag{}, rhs))
        {
        }
        template<class Integral, Integral Value, int Digits, int Exponent>
        constexpr safe_integer(const_integer<Integral, Value, Digits, Exponent>)
                : _base(static_cast<rep>(Value))
        {
            static_assert(Value <= std::numeric_limits<rep>::max(), "initialization by out-of-range value");
            static_assert(!std::numeric_limits<Integral>::is_signed || Value >= std::numeric_limits<rep>::lowest(), "initialization by out-of-range value");
        }
        template<class T>
        constexpr explicit operator T() const
        {
            return static_cast<T>(_base::data());
        }
    };
    namespace _impl {
        template<class Rep, class OverflowTag>
        struct get_rep<safe_integer<Rep, OverflowTag>> {
            using type = Rep;
        };
        template<class OldRep, class OverflowTag, class NewRep>
        struct set_rep<safe_integer<OldRep, OverflowTag>, NewRep> {
            using type = safe_integer<NewRep, OverflowTag>;
        };
    }
    template<class Rep, class OverflowTag>
    struct digits<safe_integer<Rep, OverflowTag>> : digits<Rep> {
    };
    template<class Rep, class OverflowTag, _digits_type MinNumBits>
    struct set_digits<safe_integer<Rep, OverflowTag>, MinNumBits> {
        using type = safe_integer<set_digits_t<Rep, MinNumBits>, OverflowTag>;
    };
    template<class Rep, class OverflowTag, class Value>
    struct from_value<safe_integer<Rep, OverflowTag>, Value> {
        using type = safe_integer<Value, OverflowTag>;
    };
    template<class Rep, class OverflowTag>
    struct scale<safe_integer<Rep, OverflowTag>> {
        using value_type = safe_integer<Rep, OverflowTag>;
        constexpr auto operator()(const value_type &i, int base, int exp) const
        -> decltype(_impl::to_rep(i) * _num_traits_impl::pow<value_type>(base, exp)) {
            return (exp < 0)
                   ? _impl::to_rep(i) / _num_traits_impl::pow<value_type>(base, -exp)
                   : _impl::to_rep(i) * _num_traits_impl::pow<value_type>(base, exp);
        }
    };
    template<class OverflowTag, class Rep>
    constexpr auto make_safe_integer(Rep const& value)
    -> safe_integer<Rep, OverflowTag>
    {
        return value;
    }
    namespace _impl {
        template<class OverflowTag, class OperatorTag, class LhsRep, class RhsRep, class = enable_if_t<OperatorTag::is_arithmetic>>
        constexpr auto operate_common_tag(
                OverflowTag,
                OperatorTag,
                const safe_integer<LhsRep, OverflowTag>& lhs,
                const safe_integer<RhsRep, OverflowTag>& rhs)
        -> decltype(make_safe_integer<OverflowTag>(_overflow_impl::operate<OverflowTag, OperatorTag>()(lhs.data(), rhs.data())))
        {
            return make_safe_integer<OverflowTag>(_overflow_impl::operate<OverflowTag, OperatorTag>()(lhs.data(), rhs.data()));
        }
        template<class OverflowTag, class OperatorTag, class LhsRep, class RhsRep, class = enable_if_t<OperatorTag::is_comparison>>
        constexpr auto operate_common_tag(
                OverflowTag,
                OperatorTag,
                const safe_integer<LhsRep, OverflowTag>& lhs,
                const safe_integer<RhsRep, OverflowTag>& rhs)
        -> decltype(_overflow_impl::operate<OverflowTag, OperatorTag>()(lhs.data(), rhs.data()))
        {
            return _overflow_impl::operate<OverflowTag, OperatorTag>()(lhs.data(), rhs.data());
        }
        template<class OperatorTag, class LhsRep, class LhsTag, class RhsRep, class RhsTag>
        constexpr auto operate(
                const safe_integer<LhsRep, LhsTag>& lhs,
                const safe_integer<RhsRep, RhsTag>& rhs,
                OperatorTag operator_tag)
        -> decltype(operate_common_tag(common_type_t<LhsTag, RhsTag>{}, operator_tag, lhs, rhs))
        {
            return operate_common_tag(common_type_t<LhsTag, RhsTag>{}, operator_tag, lhs, rhs);
        }
    }
    template <class LhsRep, class LhsOverflowTag, class RhsRep, class RhsOverflowTag> constexpr auto operator >> (const safe_integer<LhsRep, LhsOverflowTag>& lhs, const safe_integer<RhsRep, RhsOverflowTag>& rhs) -> safe_integer<LhsRep, LhsOverflowTag> { return lhs.data() >> rhs.data(); } template <class Lhs, class RhsRep, class RhsOverflowTag, _impl::enable_if_t<std::is_fundamental<Lhs>::value, int> dummy = 0> constexpr auto operator >> (const Lhs& lhs, const safe_integer<RhsRep, RhsOverflowTag>& rhs) -> Lhs { return lhs >> rhs.data(); } template <class LhsRep, class LhsOverflowTag, class Rhs, _impl::enable_if_t<std::is_fundamental<Rhs>::value, int> dummy = 0> constexpr auto operator >> (const safe_integer<LhsRep, LhsOverflowTag>& lhs, const Rhs& rhs) -> safe_integer<LhsRep, LhsOverflowTag> { return safe_integer<LhsRep, LhsOverflowTag>(lhs.data() >> rhs); };
    template <class LhsRep, class LhsOverflowTag, class RhsRep, class RhsOverflowTag> constexpr auto operator << (const safe_integer<LhsRep, LhsOverflowTag>& lhs, const safe_integer<RhsRep, RhsOverflowTag>& rhs) -> safe_integer<LhsRep, LhsOverflowTag> { return lhs.data() << rhs.data(); } template <class Lhs, class RhsRep, class RhsOverflowTag, _impl::enable_if_t<std::is_fundamental<Lhs>::value, int> dummy = 0> constexpr auto operator << (const Lhs& lhs, const safe_integer<RhsRep, RhsOverflowTag>& rhs) -> Lhs { return lhs << rhs.data(); } template <class LhsRep, class LhsOverflowTag, class Rhs, _impl::enable_if_t<std::is_fundamental<Rhs>::value, int> dummy = 0> constexpr auto operator << (const safe_integer<LhsRep, LhsOverflowTag>& lhs, const Rhs& rhs) -> safe_integer<LhsRep, LhsOverflowTag> { return safe_integer<LhsRep, LhsOverflowTag>(lhs.data() << rhs); };
}
namespace std {
    template<
            class Lhs,
            class RhsRep, class RhsOverflowTag>
    struct common_type<
            Lhs,
            sg14::safe_integer<RhsRep, RhsOverflowTag>>
            : sg14::_integer_impl::common_type<
                    Lhs,
                    sg14::safe_integer<RhsRep, RhsOverflowTag>> {
    };
    template<
            class LhsRep, class LhsOverflowTag,
            class Rhs>
    struct common_type<
            sg14::safe_integer<LhsRep, LhsOverflowTag>,
            Rhs>
            : sg14::_integer_impl::common_type<
                    sg14::safe_integer<LhsRep, LhsOverflowTag>,
                    Rhs> {
    };
    template<
            class LhsRep, class LhsOverflowTag,
            class RhsRep, int RhsExponent>
    struct common_type<
            sg14::safe_integer<LhsRep, LhsOverflowTag>,
            sg14::fixed_point<RhsRep, RhsExponent>>
            : std::common_type<
                    sg14::fixed_point<sg14::safe_integer<LhsRep, LhsOverflowTag>, 0>,
                    sg14::fixed_point<RhsRep, RhsExponent>> {
    };
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, class RhsOverflowTag>
    struct common_type<
            sg14::fixed_point<LhsRep, LhsExponent>,
            sg14::safe_integer<RhsRep, RhsOverflowTag>>
            : std::common_type<
                    sg14::fixed_point<LhsRep, LhsExponent>,
                    sg14::fixed_point<sg14::safe_integer<RhsRep, RhsOverflowTag>, 0>> {
    };
    template<
            class LhsRep, class LhsOverflowTag,
            class RhsRep, class RhsOverflowTag>
    struct common_type<
            sg14::safe_integer<LhsRep, LhsOverflowTag>,
            sg14::safe_integer<RhsRep, RhsOverflowTag>>
            : sg14::_integer_impl::common_type<
                    sg14::safe_integer<LhsRep, LhsOverflowTag>,
                    sg14::safe_integer<RhsRep, RhsOverflowTag>> {
    };
    template<class Rep, class OverflowTag>
    struct numeric_limits<sg14::safe_integer<Rep, OverflowTag>>
    : numeric_limits<sg14::_impl::number_base<sg14::safe_integer<Rep, OverflowTag>, Rep>> {};
}

#endif // SG14_FIXED_POINT_SINGLE_HEADER)
