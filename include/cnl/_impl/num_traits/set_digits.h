
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/num_traits.h
/// \brief definitions supporting \ref cnl::set_digits

#if !defined(CNL_IMPL_NUM_TRAITS_SET_DIGITS_H)
#define CNL_IMPL_NUM_TRAITS_SET_DIGITS_H

#include "../config.h"
#include "../cstdint/types.h"
#include "../numbers/signedness.h"
#include "../type_traits/is_integral.h"
#include "digits.h"

namespace cnl {
    namespace _impl {
        template<typename T>
        concept signed_integral = integral<T> && numbers::signedness_v<T>;

        template<typename T>
        concept unsigned_integral = integral<T> && !numbers::signedness_v<T>;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_impl::set_digits

    /// \brief convert a type to a similar type with the given number of digits
    ///
    /// \tparam T the source type
    /// \tparam Digits the desired number of digits
    template<class T, int Digits>
    struct set_digits;

    template<_impl::signed_integral T, int Digits>
    struct set_digits<T, Digits> {
        static_assert(Digits <= digits_v<intmax>, "digits exceeds widest signed integer");
    };

    template<_impl::unsigned_integral T, int Digits>
    struct set_digits<T, Digits> {
        static_assert(Digits <= digits_v<uintmax>, "digits exceeds widest unsigned integer");
    };

    template<_impl::signed_integral T, int Digits>
    requires(Digits <= 7) struct set_digits<T, Digits> {
        using type = int8;
    };

    template<_impl::unsigned_integral T, int Digits>
    requires(Digits <= 8) struct set_digits<T, Digits> {
        using type = uint8;
    };

    template<_impl::signed_integral T, int Digits>
    requires(Digits > 7 && Digits <= 15) struct set_digits<T, Digits> {
        using type = int16;
    };

    template<_impl::unsigned_integral T, int Digits>
    requires(Digits > 8 && Digits <= 16) struct set_digits<T, Digits> {
        using type = uint16;
    };

    template<_impl::signed_integral T, int Digits>
    requires(Digits > 15 && Digits <= 31) struct set_digits<T, Digits> {
        using type = int32;
    };

    template<_impl::unsigned_integral T, int Digits>
    requires(Digits > 16 && Digits <= 32) struct set_digits<T, Digits> {
        using type = uint32;
    };

    template<_impl::signed_integral T, int Digits>
    requires(Digits > 31 && Digits <= 63) struct set_digits<T, Digits> {
        using type = int64;
    };

    template<_impl::unsigned_integral T, int Digits>
    requires(Digits > 32 && Digits <= 64) struct set_digits<T, Digits> {
        using type = uint64;
    };

#if defined(CNL_INT128_ENABLED)
    template<_impl::signed_integral T, int Digits>
    requires(Digits > 63 && Digits <= 127) struct set_digits<T, Digits> {
        using type = int128;
    };

    template<_impl::unsigned_integral T, int Digits>
    requires(Digits > 64 && Digits <= 128) struct set_digits<T, Digits> {
        using type = uint128;
    };
#endif

    /// \brief Alias to \ref set_digits.
    template<class T, int Digits>
    using set_digits_t = typename set_digits<T, Digits>::type;
}

#endif  // CNL_IMPL_NUM_TRAITS_SET_DIGITS_H
