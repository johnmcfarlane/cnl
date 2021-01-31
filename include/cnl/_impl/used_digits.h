
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_USED_DIGITS_H)
#define CNL_IMPL_USED_DIGITS_H

#include "../limits.h"

#include <type_traits>

namespace cnl {
    namespace _impl {

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::used_digits

        template<bool IsSigned>
        struct used_digits_signed;

        template<>
        struct used_digits_signed<false> {
            template<_impl::integer Integer>
            CNL_NODISCARD constexpr int operator()(Integer const& value, int radix) const
            {
                return (value > 0) ? 1 + used_digits_signed<false>{}(value / radix, radix) : 0;
            }
        };

        template<>
        struct used_digits_signed<true> {
            template<_impl::integer Integer>
            CNL_NODISCARD constexpr int operator()(Integer const& value, int radix) const
            {
                // Most negative number is not exploited;
                // thus negating the result or subtracting it from something else
                // will less likely result in overflow.
                return (value < 0) ? used_digits_signed<false>{}(Integer(-1) - value, radix)
                                   : used_digits_signed<false>{}(value, radix);
            }
        };

        template<_impl::integer Integer>
        CNL_NODISCARD constexpr int used_digits(
                Integer const& value, int radix = numeric_limits<Integer>::radix)
        {
            return used_digits_signed<std::is_signed_v<Integer>>{}(value, radix);
        }
    }
}

#endif  // CNL_IMPL_USED_DIGITS_H
