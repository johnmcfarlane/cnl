
//          Copyright John McFarlane 2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_TO_CHARS_CAPACITY_H)
#define CNL_IMPL_SCALED_INTEGER_TO_CHARS_CAPACITY_H

#include "../charconv/to_chars_capacity.h"
#include "../scaled/is_scaled_tag.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl::_impl {
    constexpr auto num_digits_to_binary(int num_digits, int radix)
    {
        switch (radix) {
        case 2:
            return num_digits;
        case 8:
            return num_digits * 3;
        case 10:
            return (num_digits * 3322 + 678) / 1000;
        case 16:
            return num_digits * 4;
        default:
            return num_digits * used_digits(radix - 1);
        }
    }

    template<integer Rep, scaled_tag Scale>
    struct to_chars_capacity<scaled_integer<Rep, Scale>> {
        [[nodiscard]] constexpr auto operator()(int base = 10) const
        {
            CNL_ASSERT(base == 10);

            using type = scaled_integer<Rep, Scale>;
            constexpr auto exponent{Scale::exponent};
            constexpr auto radix{Scale::radix};

            // This number is a little pessemistic in the case that radix != 2.
            auto const fractional_digits = std::max(-exponent, 0);

            auto const sign_chars = static_cast<int>(cnl::numbers::signedness_v<type>);
            auto const num_significant_integer_bits{cnl::digits_v<type> - fractional_digits};
            auto const num_trailing_integer_bits{
                    num_digits_to_binary(std::max(0, exponent), radix)};
            auto const num_integer_bits{
                    num_significant_integer_bits + num_trailing_integer_bits};
            auto const integer_chars = num_digits_from_binary(num_integer_bits, base);
            auto const radix_chars = static_cast<int>(fractional_digits > 0);
            auto const fractional_chars = std::max(0, fractional_digits);

            return sign_chars + integer_chars + radix_chars + fractional_chars;
        }
    };
}

#endif  // CNL_IMPL_SCALED_INTEGER_TO_CHARS_CAPACITY_H
