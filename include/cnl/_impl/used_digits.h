
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_USED_DIGITS_H
#define CNL_IMPL_USED_DIGITS_H

#include <type_traits>

namespace cnl {
    namespace _impl {

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::used_digits

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

                // Most negative number is not exploited;
                // thus negating the result or subtracting it from something else
                // will less likely result in overflow.
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

#endif  // CNL_IMPL_USED_DIGITS_H
