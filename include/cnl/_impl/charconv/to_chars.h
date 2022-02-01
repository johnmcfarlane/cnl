
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TO_CHARS_H)
#define CNL_IMPL_TO_CHARS_H

#include "../../integer.h"
#include "../../number.h"
#include "../cnl_assert.h"
#include "../narrow_cast.h"
#include "../num_traits/digits.h"
#include "../num_traits/rounding.h"
#include "../num_traits/set_rounding.h"
#include "../numbers/signedness.h"
#include "constants.h"
#include "to_chars_capacity.h"

#include <array>
#include <charconv>
#include <limits>
#include <string_view>
#include <system_error>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // cnl::_impl::itoc
        [[nodiscard]] constexpr auto itoc(int value)
        {
            auto const c = (value < 10)
                                 ? zero_char + value
                                 : 'a' + value - 10;
            return static_cast<char>(c);
        }

        // cnl::_impl::to_chars_natural
        [[nodiscard]] constexpr auto to_chars_natural(char* ptr, char* last, auto const& value, int base = 10) -> char*
        {
            auto const quotient = value / base;

            auto const next_ptr = quotient ? to_chars_natural(ptr, last, quotient, base) : ptr;

            if (next_ptr == last || next_ptr == nullptr) {
                return nullptr;
            }

            // Note: linker may struggle with combination of clang, int128_t and sanitizer.
            // (See posix.cmake for details.)
            auto const remainder = value - (quotient * base);
            *next_ptr = itoc(static_cast<int>(remainder));

            return next_ptr + 1;
        }

        [[nodiscard]] constexpr auto
        to_chars_positive(char* const first, char* const last, integer auto const& value, int base)
        {
            auto const natural_last = to_chars_natural(first, last, value, base);
            return std::to_chars_result{
                    natural_last, natural_last ? std::errc{} : std::errc::value_too_large};
        }

        [[nodiscard]] constexpr auto
        to_chars_non_zero(char* const first, char* const last, number auto const& value, int base)
        {
            using number = std::remove_cvref_t<decltype(value)>;
            if constexpr (numbers::signedness_v<number>) {
                if (value < number{0}) {
                    auto const destination_length = std::distance(first, last);
                    if (destination_length < 2) {
                        return std::to_chars_result{last, std::errc::value_too_large};
                    }

                    // -ve
                    *first = '-';

                    // implementation does not support the most negative number
                    CNL_ASSERT(-std::numeric_limits<decltype(-value)>::max() <= value);

                    return to_chars_positive(first + 1, last, -value, base);
                }
            }

            return to_chars_positive(first, last, value, base);
        }
    }

    // partial implementation of std::to_chars overloaded on cnl::integer
    [[nodiscard]] constexpr auto to_chars(
            char* const first,
            char* const last,  // NOLINT(readability-non-const-parameter)
            integer auto const& value,
            int base = 10)
    {
        CNL_ASSERT(base >= 2);
        CNL_ASSERT(base <= 36);

        if (!value) {
            if (first == last) {
                // buffer too small to contain "0"
                return std::to_chars_result{last, std::errc::value_too_large};
            }

            // zero
            *first = _impl::zero_char;
            return std::to_chars_result{first + 1, std::errc{}};
        }

        using native_rounding_type = set_rounding_t<decltype(value), native_rounding_tag>;
        auto const& native_rounding_value = static_cast<native_rounding_type const&>(value);

        return _impl::to_chars_non_zero<native_rounding_type>(
                first, last, native_rounding_value, base);
    }

    template<int NumChars>
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
    struct to_chars_static_result {
        // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
        std::array<char, NumChars + 1> chars;

        // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
        int length;

        friend constexpr auto operator<=>(to_chars_static_result const&, to_chars_static_result const&) = default;

        // NOLINTNEXTLINE(google-explicit-constructor,hicpp-explicit-conversions)
        [[nodiscard]] constexpr operator std::string_view() const&
        {
            return std::string_view{
                    chars.data(),
                    _impl::narrow_cast<std::string_view::size_type>(length)};
        }

        // Taking a reference to a to_chars_static_result by rvalue is unsafe.
        [[nodiscard]] constexpr operator std::string_view() const&& = delete;
    };

    // variant of cnl::to_chars returning fixed-size array of chars
    // large enough to store any possible result for given input type
    template<int Base = 10>
    [[nodiscard]] constexpr auto to_chars_static(number auto const& value)
    {
        using number = std::remove_cvref_t<decltype(value)>;
        constexpr auto max_num_chars = _impl::to_chars_capacity<number>{}(Base);

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
        to_chars_static_result<max_num_chars> result;

        auto* const chars_begin{result.chars.data()};
        auto* const chars_end{chars_begin + max_num_chars};

        auto dynamic_result = to_chars(chars_begin, chars_end, value, Base);
        CNL_ASSERT(dynamic_result.ptr > chars_begin);
        CNL_ASSERT(dynamic_result.ptr <= chars_end);
        CNL_ASSERT(dynamic_result.ec == std::errc{});

        auto* const bytes_end{chars_begin + result.chars.size()};
        std::fill(dynamic_result.ptr, bytes_end, '\0');

        result.length = _impl::narrow_cast<int>(std::distance(chars_begin, dynamic_result.ptr));
        CNL_ASSERT(result.length >= 0);
        CNL_ASSERT(result.length <= max_num_chars);

        return result;
    }
}

#endif  // CNL_IMPL_TO_CHARS_H
