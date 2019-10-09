
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_STATIC_INTEGER_H)
#define CNL_IMPL_STATIC_INTEGER_H

#include "../elastic_integer.h"
#include "../overflow_integer.h"
#include "../rounding_integer.h"
#include "../wide_integer.h"
#include "num_traits/digits.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<
                int Digits = digits<int>::value,
                class RoundingTag = nearest_rounding_tag,
                class OverflowTag = undefined_overflow_tag,
                class Narrowest = int>
        using static_integer =
                overflow_integer<
                        elastic_integer<
                                Digits,
                                rounding_integer<
                                        wide_integer<
                                                digits<Narrowest>::value,
                                                Narrowest>,
                                        RoundingTag>>,
                        OverflowTag>;

        template<
                class RoundingTag = nearest_rounding_tag,
                class OverflowTag = undefined_overflow_tag,
                class Narrowest = int,
                class Input = int>
        CNL_NODISCARD _impl::enable_if_t<!_impl::is_constant<Input>::value,
                static_integer<
                        numeric_limits<Input>::digits,
                        RoundingTag, OverflowTag,
                        Narrowest>>
        constexpr make_static_integer(Input const& input)
        {
            return input;
        }

        template<
                class RoundingTag = nearest_rounding_tag,
                class OverflowTag = undefined_overflow_tag,
                class Narrowest = int,
                CNL_IMPL_CONSTANT_VALUE_TYPE InputValue = 0>
        CNL_NODISCARD static_integer<
                used_digits(InputValue),
                RoundingTag, OverflowTag,
                Narrowest>
        constexpr make_static_integer(constant<InputValue>)
        {
            return InputValue;
        }
    }
}

#endif  // CNL_IMPL_STATIC_INTEGER_H
