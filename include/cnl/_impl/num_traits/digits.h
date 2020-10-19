
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/num_traits.h
/// \brief definitions supporting \ref cnl::digits

#if !defined(CNL_IMPL_NUM_TRAITS_DIGITS)
#define CNL_IMPL_NUM_TRAITS_DIGITS

#include "../../constant.h"
#include "../../cstdint.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/is_signed.h"
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
    template<typename T, class Enable = void>
    struct digits;

    namespace _impl {
        template<typename Integer>
        struct fundamental_digits
                : std::integral_constant<int, CHAR_BIT*sizeof(Integer)-is_signed<Integer>::value> {
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

#if defined(CNL_INT128_ENABLED)
    template<>
    struct digits<int128> : _impl::fundamental_digits<int128> {
    };
    template<>
    struct digits<uint128> : _impl::fundamental_digits<uint128> {
    };
#endif

    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct digits<constant<Value>> : std::integral_constant<
            int,
            _impl::used_digits((Value<0) ? -Value : Value)> {
    };

    template<class T>
    constexpr int digits_v = digits<T>::value;

    namespace _impl {
        // cnl::_impl::fractional_digits
        template <class T>
        struct fractional_digits : std::integral_constant<int, 0> {
        };

        // cnl::_impl::integer_digits
        template <class T>
        struct integer_digits : std::integral_constant<int, digits<T>::value - fractional_digits<T>::value> {
        };
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_DIGITS
