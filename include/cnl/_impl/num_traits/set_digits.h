
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
#include "digits.h"

#include <concepts>

namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_impl::set_digits

    /// \brief convert a type to a similar type with the given number of digits
    ///
    /// \tparam T the source type
    /// \tparam Digits the desired number of digits
    template<class T, int Digits>
    struct set_digits;

    template<std::signed_integral T, int Digits>
    struct set_digits<T, Digits> {
        static_assert(Digits <= digits_v<intmax_t>, "digits exceeds widest signed integer");
    };

    template<std::unsigned_integral T, int Digits>
    struct set_digits<T, Digits> {
        static_assert(Digits <= digits_v<uintmax_t>, "digits exceeds widest unsigned integer");
    };

    template<std::signed_integral T, int Digits>
    requires(Digits <= 7) struct set_digits<T, Digits> {
        using type = std::int8_t;
    };

    template<std::unsigned_integral T, int Digits>
    requires(Digits <= 8) struct set_digits<T, Digits> {
        using type = std::uint8_t;
    };

    template<std::signed_integral T, int Digits>
    requires(Digits > 7 && Digits <= 15) struct set_digits<T, Digits> {
        using type = std::int16_t;
    };

    template<std::unsigned_integral T, int Digits>
    requires(Digits > 8 && Digits <= 16) struct set_digits<T, Digits> {
        using type = std::uint16_t;
    };

    template<std::signed_integral T, int Digits>
    requires(Digits > 15 && Digits <= 31) struct set_digits<T, Digits> {
        using type = std::int32_t;
    };

    template<std::unsigned_integral T, int Digits>
    requires(Digits > 16 && Digits <= 32) struct set_digits<T, Digits> {
        using type = std::uint32_t;
    };

    template<std::signed_integral T, int Digits>
    requires(Digits > 31 && Digits <= 63) struct set_digits<T, Digits> {
        using type = std::int64_t;
    };

    template<std::unsigned_integral T, int Digits>
    requires(Digits > 32 && Digits <= 64) struct set_digits<T, Digits> {
        using type = std::uint64_t;
    };

#if defined(CNL_INT128_ENABLED)
    template<std::signed_integral T, int Digits>
    requires(Digits > 63 && Digits <= 127) struct set_digits<T, Digits> {
        using type = int128_t;
    };

    template<std::unsigned_integral T, int Digits>
    requires(Digits > 64 && Digits <= 128) struct set_digits<T, Digits> {
        using type = uint128_t;
    };
#endif

    /// \brief Alias to \ref set_digits.
    template<class T, int Digits>
    using set_digits_t = typename set_digits<T, Digits>::type;
}

#endif  // CNL_IMPL_NUM_TRAITS_SET_DIGITS_H
