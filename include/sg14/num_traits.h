
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions supporting generic treatment of numeric types

#if !defined(SG14_NUMERIC_TRAITS)
#define SG14_NUMERIC_TRAITS 1

#if !defined(SG14_GODBOLT_ORG)

#include "bits/limits.h"
#include "bits/type_traits.h"

#endif

#include <utility>

/// study group 14 of the C++ working group
namespace sg14 {

    using _digits_type = int;

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::is_composite (default specialization)

    template<class T, class Enable = void>
    struct is_composite : std::false_type {
        static_assert(!std::is_reference<T>::value, "T is a reference");
        static_assert(!std::is_const<T>::value, "T is const");
        static_assert(!std::is_volatile<T>::value, "T is volatile");
    };

#if (__cplusplus > 201402L)
    template<class T>
    constexpr auto is_composite_v = is_composite<T>::value;
#endif

    namespace _num_traits_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_num_traits_impl::are_composite

        template<class ... Args>
        struct are_composite;

        template<>
        struct are_composite<> : std::false_type {
        };

        template<class ArgHead, class ... ArgTail>
        struct are_composite<ArgHead, ArgTail...>
                : std::integral_constant<bool, is_composite<typename std::decay<ArgHead>::type>::value || are_composite<ArgTail...>::value> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_num_traits_impl::enable_for_range

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

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_num_traits_impl::set_digits_signed

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

#if defined(SG14_INT128_ENABLED)
        template<_digits_type MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, std::int64_t, SG14_INT128>> {
            using type = SG14_INT128;
        };
#endif

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_num_traits_impl::set_digits_unsigned

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

#if defined(SG14_INT128_ENABLED)
        template<_digits_type MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, std::uint64_t, SG14_UINT128>> {
            using type = SG14_UINT128;
        };
#endif

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_num_traits_impl::set_digits_integer

        template<class Integer, _digits_type MinNumDigits>
        using set_digits_integer = typename std::conditional<
                std::numeric_limits<Integer>::is_signed,
                set_digits_signed<MinNumDigits>,
                set_digits_unsigned<MinNumDigits>>::type;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // digits

    template<class T, class Enable = void>
    struct digits : std::integral_constant<_digits_type, std::numeric_limits<T>::digits> {
    };

#if (__cplusplus > 201402L)
    template<class T>
    constexpr _digits_type digits_v = digits<T>::value;
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // set_digits / set_digits_t

    template<class T, _digits_type Digits, class Enable = void>
    struct set_digits;

    template<class T, _digits_type Digits>
    struct set_digits<T, Digits, _impl::enable_if_t<std::is_integral<T>::value>>
            : _num_traits_impl::set_digits_integer<T, Digits> {
    };

#if defined(SG14_INT128_ENABLED)
    template<_digits_type Digits>
    struct set_digits<SG14_INT128, Digits>
            : _num_traits_impl::set_digits_integer<signed, Digits> {
    };

    template<_digits_type Digits>
    struct set_digits<SG14_UINT128, Digits>
            : _num_traits_impl::set_digits_integer<unsigned, Digits> {
    };
#endif

    template<class T, _digits_type Digits>
    using set_digits_t = typename set_digits<T, Digits>::type;

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::is_unsigned

    template<class T>
    struct is_signed : std::integral_constant<bool, std::numeric_limits<T>::is_signed> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_signed

    template<class, class = void>
    struct make_signed;

    template<class T>
    struct make_signed<T, _impl::enable_if_t<std::is_integral<T>::value>> : std::make_signed<T> {
    };

    template<class T>
    using make_signed_t = typename make_signed<T>::type;

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_unsigned

    template<class, class = void>
    struct make_unsigned;

    template<class T>
    struct make_unsigned<T, _impl::enable_if_t<std::is_integral<T>::value>> : std::make_unsigned<T> {
    };

#if defined(SG14_INT128_ENABLED)
    // TODO: sg14::is_integral
    template<>
    struct make_unsigned<SG14_INT128> {
        using type = SG14_UINT128;
    };
    template<>
    struct make_unsigned<SG14_UINT128> {
        using type = SG14_UINT128;
    };

    template<>
    struct make_signed<SG14_INT128> {
        using type = SG14_INT128;
    };
    template<>
    struct make_signed<SG14_UINT128> {
        using type = SG14_INT128;
    };
#endif

    template<class T>
    using make_unsigned_t = typename make_unsigned<T>::type;

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::make_signed - std::make_signed with IsSigned parameter

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

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::common_signedness

        template<class T1, class T2>
        struct common_signedness {
            static constexpr bool _are_signed = std::numeric_limits<T1>::is_signed | std::numeric_limits<T2>::is_signed;

            using type = typename std::common_type<make_signed_t<T1, _are_signed>,
                    make_signed_t<T2, _are_signed>>::type;
        };

        template<class T1, class T2>
        using common_signedness_t = typename common_signedness<T1, T2>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::encompasses

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

        // true if Encompassed can be cast to Encompasser without chance of overflow
        template<class Encompasser, class Encompassed>
        struct encompasses {
            static constexpr bool _lower = encompasses_lower<Encompasser, Encompassed>::value;
            static constexpr bool _upper =
                    static_cast<unsigned_or_float_t<Encompasser>>(std::numeric_limits<Encompasser>::max())
                    >= static_cast<unsigned_or_float_t<Encompassed>>(std::numeric_limits<Encompassed>::max());

            static constexpr bool value = _lower && _upper;
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::is_integer_or_float - trait to identify 'traditional' arithmetic concept

        template<class T>
        struct is_integer_or_float : std::integral_constant<
                bool,
                std::numeric_limits<T>::is_integer || std::numeric_limits<T>::is_iec559> {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::_impl::to_rep

    template<class Number, class Enable = void>
    struct to_rep {
        constexpr Number operator()(const Number &number) const {
            // by default, the rep type of a number type is the number type itself
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

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::from_rep

    template<class Number, class Enable = void>
    struct from_rep {
        template<class Rep>
        constexpr Number operator()(const Rep &rep) const {
            // by default, a number type's rep type is the number type itself
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

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::for_rep

    // invoke a given generic lambda on given parameters
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

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::from_value

    // if Number has Value for its Rep, what type would Number become?
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

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::scale

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

#endif    // SG14_NUMERIC_TRAITS
