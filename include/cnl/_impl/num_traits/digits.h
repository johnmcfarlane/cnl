
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/num_traits.h
/// \brief definitions supporting \ref cnl::digits

#if !defined(CNL_IMPL_NUM_TRAITS_DIGITS_H)
#define CNL_IMPL_NUM_TRAITS_DIGITS_H

#include "../numbers/signedness.h"
#include "../type_traits/is_integral.h"
#include "../used_digits.h"
#include "is_composite.h"

#include <type_traits>

namespace cnl {
    /// \brief provide number of binary digits of the given type
    ///
    /// \tparam T the source type
    /// \tparam Digits the desired number of digits
    ///
    /// \note does not include a sign bit
    ///
    /// \sa cnl::set_digits
    template<typename T>
    inline constexpr int digits = 0;

    namespace _impl {
        template<typename Integer>
        inline constexpr auto fundamental_digits = CHAR_BIT * sizeof(Integer) - numbers::signedness_v<Integer>;
    }

    template<_impl::integral Integer>
    inline constexpr int digits<Integer> = _impl::fundamental_digits<Integer>;

    namespace _impl {
        // cnl::_impl::fractional_digits
        template<class T>
        inline constexpr int fractional_digits = 0;

        // cnl::_impl::integer_digits
        template<class T>
        inline constexpr auto integer_digits = digits<T> - fractional_digits<T>;
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_DIGITS_H
