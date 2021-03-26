
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TO_CHARS_H)
#define CNL_IMPL_TO_CHARS_H

#include "../integer.h"
#include "../numeric_limits.h"
#include "cnl_assert.h"
#include "num_traits/digits.h"
#include "num_traits/rounding.h"
#include "num_traits/set_rounding.h"
#include "numbers/signedness.h"

#include <array>
#include <charconv>
#include <string_view>
#include <system_error>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // cnl::_impl::max_to_chars_chars
        template<typename Scalar, int Base = 10>
        struct max_to_chars_chars {
            static constexpr auto _sign_chars = static_cast<int>(cnl::numbers::signedness_v<Scalar>);
            static constexpr auto _integer_chars = ((cnl::digits<Scalar> + 2) / 3);

            static constexpr auto value = _sign_chars + _integer_chars;
        };

        // cnl::_impl::itoc
        template<typename Scalar>
        [[nodiscard]] constexpr auto itoc(Scalar value)
        {
            static_assert(
                    std::is_same<typename rounding<Scalar>::type, native_rounding_tag>::value,
                    "wrong rounding type");
            auto c = '0' + static_cast<int>(value);
            return static_cast<char>(c);
        }

        // cnl::_impl::to_chars_natural
        template<class Integer>
        [[nodiscard]] constexpr auto to_chars_natural(char* ptr, char* last, Integer const& value) -> char*
        {
            auto const quotient = value / 10;

            auto const next_ptr = quotient ? to_chars_natural(ptr, last, quotient) : ptr;

            if (next_ptr == last || next_ptr == nullptr) {
                return nullptr;
            }

            // Note: linker may struggle with combination of clang, int128 and sanitizer.
            // See clang.cmake
            auto const remainder = value - (quotient * 10);
            *next_ptr = itoc(remainder);

            return next_ptr + 1;
        }

        template<integer Integer>
        auto to_chars_positive(
                char* const first,  // NOLINT(readability-avoid-const-params-in-decls)
                char* const last,  // NOLINT(readability-avoid-const-params-in-decls)
                Integer const& value) noexcept
        {
            auto const natural_last = to_chars_natural(first, last, value);
            return std::to_chars_result{
                    natural_last, natural_last ? std::errc{} : std::errc::value_too_large};
        }

        template<typename Number>
        [[nodiscard]] constexpr auto
        to_chars_non_zero(char* const first, char* const last, Number const& value)
        {
            if constexpr (numbers::signedness_v<Number>) {
                if (value < Number{}) {
                    auto const destination_length = std::distance(first, last);
                    if (destination_length < 2) {
                        return std::to_chars_result{last, std::errc::value_too_large};
                    }

                    // -ve
                    *first = '-';

                    // implementation does not support the most negative number
                    CNL_ASSERT(-numeric_limits<decltype(-value)>::max() <= value);

                    return to_chars_positive(first + 1, last, -value);
                }
            }

            return to_chars_positive(first, last, value);
        }
    }

    // partial implementation of std::to_chars overloaded on cnl::duplex_integer
    template<integer Integer>
    [[nodiscard]] constexpr auto to_chars(
            char* const first,  // NOLINT(readability-avoid-const-params-in-decls)
            char* const last,  // NOLINT(readability-avoid-const-params-in-decls,readability-non-const-parameter)
            Integer const& value)
    {
        if (!value) {
            if (first == last) {
                // buffer too small to contain "0"
                return std::to_chars_result{last, std::errc::value_too_large};
            }

            // zero
            *first = '0';
            return std::to_chars_result{first + 1, std::errc{}};
        }

        using native_rounding_type = set_rounding_t<decltype(value), native_rounding_tag>;
        auto const& native_rounding_value = static_cast<native_rounding_type>(value);

        return _impl::to_chars_non_zero<native_rounding_type>(
                first, last, native_rounding_value);
    }

    template<int NumChars>
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
    struct to_chars_static_result {
        std::array<char, NumChars + 1> chars;
        int length;
    };

    // variant of cnl::to_chars returning fixed-size array of chars
    // large enough to store any possible result for given input type
    template<typename Number>
    [[nodiscard]] constexpr auto to_chars_static(Number const& value)
    {
        constexpr auto max_num_chars = _impl::max_to_chars_chars<Number>::value;

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
        to_chars_static_result<max_num_chars> result;

        auto dynamic_result = to_chars(result.chars.data(), result.chars.data() + max_num_chars, value);
        CNL_ASSERT(dynamic_result.ptr > result.chars.data());
        CNL_ASSERT(dynamic_result.ptr <= result.chars.data() + max_num_chars);
        CNL_ASSERT(dynamic_result.ec == std::errc{});

        *dynamic_result.ptr = '\0';
        result.length = int(dynamic_result.ptr - result.chars.data());

        return result;
    }
}

#endif  // CNL_IMPL_TO_CHARS_H
