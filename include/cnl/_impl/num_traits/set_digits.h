
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/num_traits.h
/// \brief definitions supporting \ref cnl::set_digits

#ifndef CNL_IMPL_NUM_TRAITS_SET_DIGITS
#define CNL_IMPL_NUM_TRAITS_SET_DIGITS

#include "../type_traits/enable_if.h"
#include "../type_traits/is_integral.h"

namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // error: [un]signed_integer_cannot_have<Digits>

    // These types are declared for error reporting purposes only. They arise in
    // error message when an attempt it made to use set_digits or set_digits_t
    // to produce an integer that is wider than the widest fundamental integer
    // types, cnl::intmax and cnl::uintmax.

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
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::enable_for_range

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

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::set_digits_signed

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

#if defined(CNL_INT128_ENABLED)
        template<int MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, int64, int128>> {
            using type = int128;
        };
#endif

        template<int MinNumDigits>
        struct set_digits_signed<MinNumDigits, enable_for_range_t<MinNumDigits, intmax, void>>
                : signed_integer_cannot_have<MinNumDigits>::template digits_because_maximum_is<numeric_limits<intmax>::digits> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::set_digits_unsigned

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

#if defined(CNL_INT128_ENABLED)
        template<int MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, uint64, uint128>> {
            using type = uint128;
        };
#endif

        template<int MinNumDigits>
        struct set_digits_unsigned<MinNumDigits, enable_for_range_t<MinNumDigits, uintmax, void>>
                : unsigned_integer_cannot_have<MinNumDigits>::template digits_because_maximum_is<numeric_limits<uintmax>::digits> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::set_digits_integer

        template<class Integer, int MinNumDigits>
        using set_digits_integer = typename std::conditional<
                numeric_limits<Integer>::is_signed,
                set_digits_signed<MinNumDigits>,
                set_digits_unsigned<MinNumDigits>>::type;
    }

    /// \brief convert a type to a similar type with the given number of digits
    ///
    /// \tparam T the source type
    /// \tparam Digits the desired number of digits
    template<class T, int Digits, class _Enable = void>
    struct set_digits;

    template<class T, int Digits>
    struct set_digits<T, Digits, _impl::enable_if_t<_impl::is_integral<T>::value>>
            : _impl::set_digits_integer<T, Digits> {
    };

#if defined(CNL_INT128_ENABLED)
    template<int Digits>
    struct set_digits<int128, Digits>
            : _impl::set_digits_integer<signed, Digits> {
    };

    template<int Digits>
    struct set_digits<uint128, Digits>
            : _impl::set_digits_integer<unsigned, Digits> {
    };
#endif

    /// \brief Alias to \ref cnl::set_digits.
    template<class T, int Digits>
    using set_digits_t = typename set_digits<T, Digits>::type;
}

#endif  // CNL_IMPL_NUM_TRAITS_SET_DIGITS
