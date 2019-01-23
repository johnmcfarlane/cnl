
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_FIXED_POINT_TO_CHARS_H
#define CNL_IMPL_FIXED_POINT_TO_CHARS_H

#include "../../rounding_integer.h"
#include "num_traits.h"
#include "type.h"
#include "../assert.h"
#include "../num_traits/fixed_width_scale.h"
#include "../to_chars.h"

#include <array>
#include <iterator>
#include <type_traits>
#include <utility>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Rep, int Exponent>
        struct max_to_chars_chars<fixed_point<Rep, Exponent>> {
            using _scalar = cnl::fixed_point<Rep, Exponent>;
            static constexpr auto _fractional_digits = cnl::_impl::fractional_digits<_scalar>::value;

            static constexpr auto _sign_chars = static_cast<int>(cnl::is_signed<_scalar>::value);
            static constexpr auto _integer_chars = ((cnl::_impl::integer_digits<_scalar>::value + 2) / 3);
            static constexpr auto _radix_chars = static_cast<int>(_fractional_digits > 0);
            static constexpr auto _fractional_chars = max(0, _fractional_digits);

            static constexpr auto value = _sign_chars + _integer_chars + _radix_chars + _fractional_chars;
        };

        // cnl::_impl::split - return given non-negative value as separate integral and fractional components
        template<typename Rep, int Exponent, int Radix, bool Flushed = (Radix==2 && Exponent<=-digits<Rep>::value)>
        struct split;

        template<typename Rep, int Exponent, int Radix>
        struct split<Rep, Exponent, Radix, false> {
        private:
            using value_type = fixed_point<Rep, Exponent, Radix>;

            constexpr auto integral(value_type const& scalar) const
            -> decltype(scale<Exponent, Radix>(to_rep(scalar)))
            {
                return scale<Exponent, Radix>(to_rep(scalar));
            }

            template<typename Integral>
            static auto from_integral_and_value(Integral const& integral, value_type const& value)
            -> decltype(std::make_pair(integral, value-integral))
            {
                return std::make_pair(integral, value-integral);
            }

        public:
            constexpr auto operator()(value_type const& value) const
            -> decltype(from_integral_and_value(integral(value), value))
            {
                return from_integral_and_value(integral(value), value);
            }
        };

        template<typename Rep, int Exponent, int Radix>
        struct split<Rep, Exponent, Radix, true> {
            using value_type = fixed_point<Rep, Exponent, Radix>;

            constexpr auto operator()(value_type const& value) const
            -> decltype(std::make_pair(Rep{}, value))
            {
                return std::make_pair(Rep{}, value);
            }
        };

        // case where value has enough integer digits to hold range, [0..10)
        template<typename Rep, int Exponent, int Radix>
        auto to_chars_fractional_specialized(
                char* first,
                char const* const last,
                fixed_point<Rep, Exponent, Radix> value) noexcept
        -> enable_if_t<integer_digits<fixed_point<Rep, Exponent, Radix>>::value>=4, char*>
        {
            do {
                // to_chars only supports fixed_point types that can represent all decimal units.
                using fixed_point = fixed_point<Rep, Exponent, Radix>;
                CNL_ASSERT(value<=numeric_limits<fixed_point>::max()/Rep{10});

                value = from_rep<fixed_point>(
                        cnl::fixed_width_scale<1, 10, Rep>{}(to_rep(value)));

                auto const split = _impl::split<Rep, Exponent, Radix>{}(value);
                *first = itoc(split.first);
                ++first;

                value = split.second;
                if (!value) {
                    break;
                }
            }
            while (first!=last);

            return first;
        }

        // case where value doesn't have enough integer digits to hold range, [0..10)
        template<typename Rep, int Exponent, int Radix>
        auto to_chars_fractional_specialized(
                char* const first,
                char* last,
                fixed_point<Rep, Exponent, Radix> const& value) noexcept
        -> enable_if_t<integer_digits<fixed_point<Rep, Exponent, Radix>>::value<4, char*>
        {
            // zero-out all of the characters in the output string
            std::fill<char*>(first, last, '0');
            auto const digits = std::distance(first, last);

            // store fractional bit, 0.5, as a sequence of decimal digits
            std::array<char, (Exponent*-302LL)/100> bit{ };
            CNL_ASSERT(std::ptrdiff_t(bit.size())>=digits);

            // Initially, the sequence is { 5, 0, 0, 0, ... }.
            bit[0] = 5;

            for (auto mask = fixed_point<Rep, Exponent, Radix>{ .5 };;) {
                // At this point, bit is bytewise decimal representation of bitwise mask.

                // If this bit is present,
                if (value & mask) {
                    // add it to the output string.
                    auto carry = 0;
                    for (auto pos = digits-1; pos>=0; --pos) {
                        *(first+pos) = char(*(first+pos)+bit[pos]+carry);
                        if (*(first+pos)>'9') {
                            *(first+pos) = char(*(first+pos)-10);
                            carry = 1;
                        }
                        else {
                            carry = 0;
                        }
                    }
                    CNL_ASSERT(carry==0);
                }

                mask >>= 1;
                if (!mask) {
                    break;
                }

                for (auto digit = std::end(bit)-1; digit>std::begin(bit); --digit) {
                    auto const before = digit[-1];
                    digit[-1] = char(before >> 1);

                    if (before & 1) {
                        digit[0] = char(digit[0]+5);
                        CNL_ASSERT(digit[0]<10);
                    }
                }
            }

            return last;
        }

        template<typename Rep, int Exponent, int Radix>
        auto to_chars_fractional(
                char* first,
                char* last,
                fixed_point<Rep, Exponent, Radix> const& value) noexcept
        -> to_chars_result
        {
            auto const destination_length = std::distance(first, last);
            if (destination_length<2) {
                return to_chars_result{first, std::errc{}};
            }

            *first = '.';
            first++;

            last = to_chars_fractional_specialized(first, last, value);

            // clean up trailing zeros to the right
            for (;;) {
                auto const prev = last[-1];
                if (prev!='0') {
                    if (prev=='.') {
                        --last;
                    }
                    break;
                }
                --last;
            }

            return to_chars_result{last, std::errc{}};
        }

        template<typename Rep, int Exponent, int Radix>
        to_chars_result to_chars_positive(
                char* const first,
                char* const last,
                fixed_point<Rep, Exponent, Radix> const& value) noexcept
        {
            auto const split = _impl::split<Rep, Exponent, Radix>{}(value);
            auto const natural_last = to_chars_natural(first, last, split.first);
            if (!natural_last) {
                return to_chars_result{last, std::errc::value_too_large};
            }

            if (!split.second) {
                return to_chars_result{natural_last, std::errc{}};
            }

            return to_chars_fractional(natural_last, last, split.second);
        }
    }

    // partial implementation of std::to_chars overloaded on cnl::fixed_point
    template<typename Rep, int Exponent, int Radix>
    to_chars_result to_chars(
            char* const first,
            char* const last,
            cnl::fixed_point<Rep, Exponent, Radix> const& value)
    {
        if (!value) {
            if (first==last) {
                // buffer too small to contain "0"
                return to_chars_result{last, std::errc::value_too_large};
            }

            // zero
            *first = '0';
            return to_chars_result{first+1, std::errc{}};
        }

        using native_rounding_type = set_rounding_t<decltype(value), _impl::native_rounding_tag>;
        auto const& native_rounding_value = static_cast<native_rounding_type>(value);

        return _impl::to_chars_non_zero<native_rounding_type>{}(
                first, last, native_rounding_value);
    }
}

#endif  // CNL_IMPL_FIXED_POINT_TO_CHARS_H
